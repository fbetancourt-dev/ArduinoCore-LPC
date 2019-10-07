#ifndef __WIRING_H
#define __WIRING_H

#include <cross_studio_io.h>
#include <ctl_api.h>
#include <targets/LPC2000.h>
#include <targets/liblpc2000.h>

#include <inttypes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PROCESSOR_MODEL
#define PROCESSOR_MODEL 2378
#endif

void setupExternalRam(void);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

#ifdef __cplusplus
}
#endif

#endif