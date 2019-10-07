#ifndef __WIRING_DIGITAL_H_
#define __WIRING_DIGITAL_H_

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

#define HIGH 0x1
#define LOW 0x0
#define HIGH_Z 0x2

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

void pinMode(uint32_t pin, uint8_t mode);
void digitalWrite(uint32_t pin, uint8_t val);
int digitalRead(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif // whole file