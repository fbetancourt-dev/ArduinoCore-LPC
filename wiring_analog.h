#ifndef __WIRING_ANALOG_H_
#define __WIRING_ANALOG_H_

#include <cross_studio_io.h>
#include <ctl_api.h>
#include <targets/LPC2000.h>
#include <targets/liblpc2000.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

//#include "Arduino.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

void analogReference(uint8_t mode);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

#ifdef __cplusplus
}
#endif

#endif // whole file