#include "Display_ST7789.h"
#include "Gyro_QMI8658.h"
#include "LVGL_Driver.h"
#include "PWR_Key.h"
#include "BAT_Driver.h"
#include "ui.h"
#include <math.h>

// --- Global variables and forward declarations (no changes needed) ---
float g_roll_calibration_offset = -4.5f;
float g_pitch_calibration_offset = 4.5f;
const float RECOIL_Z_AXIS_THRESHOLD_NEGATIVE = -1.0f;
const unsigned long RECOIL_COOLDOWN_MS = 1500;
unsigned long last_recoil_detection_time = 0;

float get_roll_from_sensor_impl();
float get_pitch_from_sensor_impl();
bool checkForRecoil();
extern "C" float get_qmi8658_roll_degrees() { return get_roll_from_sensor_impl(); }
extern "C" float get_qmi8658_pitch_degrees() { return get_pitch_from_sensor_impl(); }


// --- Dedicated Task for Hardware Polling ---
void DriverTask(void* parameter) {
  while(1) {
    // This task now handles all direct hardware communication
    PWR_Loop();
    QMI8658_Loop();
    BAT_Get_Volts(); // This updates the global BAT_analogVolts variable

    // Delay the task to yield CPU time to other processes
    vTaskDelay(pdMS_TO_TICKS(20)); // Poll hardware every 20ms
  }
}


void setup()
{
  PWR_Init();
  Serial.begin(115200);
  Serial.println("Setup: Starting system initialization...");

  I2C_Init();
  Backlight_Init();
  LCD_Init();
  BAT_Init();
  Lvgl_Init();
  QMI8658_Init();
  setAccScale(ACC_RANGE_4G);
  ui_init();

  Serial.println("Setup: Creating DriverTask for hardware polling...");
  // Create and run the driver task on Core 0
  xTaskCreatePinnedToCore(
    DriverTask,
    "DriverTask",
    4096, // Stack size
    NULL,
    1,    // Priority
    NULL,
    0     // Run on Core 0
  );

  Serial.println("Setup: System initialization complete.");
}

// C++ sensor implementation functions (no changes needed)
float get_roll_from_sensor_impl() {
    float acc_y = getAccY();
    float acc_x = getAccX();
    float raw_roll_radians = atan2f(-acc_y, -acc_x);
    float raw_roll_degrees = raw_roll_radians * (180.0f / M_PI);
    return raw_roll_degrees - g_roll_calibration_offset;
}

float get_pitch_from_sensor_impl() {
    float acc_x = getAccX();
    float acc_y = getAccY();
    float acc_z = getAccZ();
    float raw_pitch_radians = atan2f(acc_z, sqrtf(acc_x * acc_x + acc_y * acc_y));
    float raw_pitch_degrees = raw_pitch_radians * (180.0f / M_PI);
    return raw_pitch_degrees - g_pitch_calibration_offset;
}

bool checkForRecoil() {
    float az = getAccZ();
    if (az < RECOIL_Z_AXIS_THRESHOLD_NEGATIVE) {
        return true;
    }
    return false;
}


// --- Main Loop: Handles UI and Application Logic ---
void loop()
{
  // --- Recoil Detection Logic ---
  unsigned long current_time = millis();
  if (current_time - last_recoil_detection_time > RECOIL_COOLDOWN_MS) {
      if (checkForRecoil()) {
          Serial.println("Main.ino: Physical recoil DETECTED!");
          ui_competition_page_handle_recoil_event();
          last_recoil_detection_time = current_time;
      }
  }
  // --- End of Recoil Logic ---

  Lvgl_Loop(); // LVGL's main loop handler
  delay(5);    // Keep the loop responsive
}