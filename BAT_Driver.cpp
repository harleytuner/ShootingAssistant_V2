#include "BAT_Driver.h"
#include "Arduino.h"

float BAT_analogVolts = 0;

void BAT_Init(void)
{
  // Set the ADC resolution to 12 bits (0-4095)
  analogReadResolution(12);

  // Configure the ADC for the correct voltage range
  analogSetPinAttenuation(BAT_ADC_PIN, ADC_11db);
}

float BAT_Get_Volts(void)
{
  // Directly read the ADC pin without any Wi-Fi checks
  int millivolts = analogReadMilliVolts(BAT_ADC_PIN);

  // Calculate the actual battery voltage
  BAT_analogVolts = (float)(millivolts * 3.0 / 1000.0) / Measurement_offset;

  return BAT_analogVolts;
}