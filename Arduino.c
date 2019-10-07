#include "Arduino.h"

/*
int  DecimalToBCD (int Decimal)
{
   return (((Decimal/10) << 4) | (Decimal % 10));
}

int BCDToDecimal(int BCD)
{
   return (((BCD>>4)*10) + (BCD & 0xF));
}
*/

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned long getPeripheralClock() {
  unsigned long pclock = OSCILLATOR_CLOCK_FREQUENCY; // oscillator freq
  pclock *= 6;                                  // clock multiplier
  pclock /= 4;                                  // peripheral divisor
  return pclock;
}
