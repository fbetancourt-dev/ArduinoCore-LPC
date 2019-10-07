#include "wiring_analog.h"
#include "Arduino.h"

#include "pins_arduino.h"

#include <string.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode) {
  // can't actually set the register here because the default setting
  // will connect AVCC and the AREF pin, which would cause a short if
  // there's something connected to AREF.
  analog_reference = mode;
}

int analogRead(uint8_t pin) {
  uint8_t low, high;
  // combine the two bytes
  return (high << 8) | low;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, int val) {
  // We need to make sure the PWM output is enabled for those pins
  // that support it, as we turn it off when digitally reading or
  // writing with them.  Also, make sure the pin is in output mode
  // for consistenty with Wiring, which doesn't require a pinMode
  // call for the analog output pins.
  pinMode(pin, OUTPUT);
  if (val == 0) {
    digitalWrite(pin, LOW);
  } else if (val == 255) {
    digitalWrite(pin, HIGH);
  }
}