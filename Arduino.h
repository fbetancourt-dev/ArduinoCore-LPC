#ifndef Arduino_h
#define Arduino_h

#include <cross_studio_io.h>
#include <ctl_api.h>

//#include <targets/LPC2000.h>
//#include <targets/liblpc2000.h>
#include <targets/LPC2378.h>

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>
#include <stdint.h>

#include <ctype.h>
#include <string.h>

#include <stdarg.h>
#include <stdio.h>

#include "WMath.h"
#include "binary.h"

#include "wiring.h"
#include "wiring_digital.h"

#include "bsp.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL 0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define interrupts() sei()
#define noInterrupts() cli()

unsigned long getPeripheralClock();
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitSet(value, bit) ((value) |= (1ULL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1ULL << (bit)))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define REPEAT8(x) x, x, x, x, x, x, x, x
#define BV0TO7 _BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7)
#define BV7TO0 _BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0)

#define isascii(c) (((unsigned char)(c)) <= 0x7f)
#define toascii(c) (((unsigned char)(c)) & 0x7f)

/*
int DecimalToBCD(int Decimal);
int BCDToDecimal(int BCD);
*/

/*
#include "boards.h"
#include "bsp.h"

#include "wiring.h"
#include "wiring_digital.h"

#include "WInterrupts.h"
*/

#include "pins_arduino.h"
#include "wiring.h"
#include "wiring_digital.h"
//#include "wiring_analog.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef bool boolean;
typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "HardwareSerial.h"
#include "WString.h"
#endif

//#define ENTER_CRITICAL  ctl_global_interrupts_set(0); // disable
//#define EXIT_CRITICAL   ctl_global_interrupts_set(1); // set to previous state



#endif // whole file