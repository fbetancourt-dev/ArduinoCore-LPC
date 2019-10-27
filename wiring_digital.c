#include "wiring_digital.h"

#include <string.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define PULL_UP_ENABLED 0
#define PULL_DOWN_ENABLED 3
#define NO_PULL_UP_DOWN 2

void lpc_gpio_cfg(uint32_t portPin, uint8_t direction, uint8_t cfg);
void lpc_gpio_set_state(uint32_t pin, int state);
int lpc_gpio_get_state(uint32_t pin);

void pinMode(uint32_t pin, uint8_t mode) {
  if (mode == OUTPUT) {
    lpc_gpio_cfg(pin, OUTPUT, NO_PULL_UP_DOWN);
  } else if (mode == INPUT) {
    lpc_gpio_cfg(pin, INPUT, NO_PULL_UP_DOWN);
  } else if (mode == INPUT_PULLDOWN) {
    lpc_gpio_cfg(pin, INPUT, PULL_DOWN_ENABLED);
  } else if (mode == INPUT_PULLUP) {
    lpc_gpio_cfg(pin, INPUT, PULL_UP_ENABLED);
  } else if (mode == HIGH_Z) {
    //#error "pinMode: HIGH_Z not implemented"
  } else {
    //#error "pinMode: mode not defined"
  }
}

void digitalWrite(uint32_t pin, uint8_t val) {
  lpc_gpio_set_state(pin, val);
}

int digitalRead(uint32_t pin) {
  return lpc_gpio_get_state(pin);
}

void lpc_gpio_cfg(uint32_t pin, uint8_t direction, uint8_t cfg) {
  uint32_t baseHardwareAddress = 0;
  uint32_t pinSelectOffset = 0;
  uint32_t pinModeOffset = 0;
  uint32_t port = 0;
  uint32_t pullupmask = 0;
  uint32_t *RisingIntReg = 0;
  uint32_t *FallingIntReg = 0;

  uint32_t *IOxPIN = 0;
  uint32_t *IOxDIR = 0;
  //uint32_t *IOxSET;
  //uint32_t *IOxCLR;
  uint32_t *IOxMASK = 0;
  uint32_t *PINSELx = 0;
  uint32_t *PINMODEx = 0;
  uint32_t pinMask = 0;
  uint32_t shiftCount = 0;
  uint32_t portPin = 0;

  portPin = pin;

  shiftCount = portPin & 0x1F;  // pin number in bits 4:0
  port = (portPin >> 16) & 0x7; // port number in bits 18:16
  pinMask = 1 << shiftCount;

  // baseHardwareAddress = GPIO_BASE;
  baseHardwareAddress = FIO_BASE;
  switch (port) {
  case 0:
    pinSelectOffset = ((pinMask < 0x00010000) ? PINSEL0_OFFSET : PINSEL1_OFFSET);
    pinModeOffset = ((pinMask < 0x00010000) ? PINMODE0_OFFSET : PINMODE1_OFFSET);
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO0PIN_OFFSET);
    IOxMASK = (uint32_t *)(baseHardwareAddress + FIO0MASK_OFFSET);
    IOxDIR = (uint32_t *)(baseHardwareAddress + FIO0DIR_OFFSET);
    //IOxSET = (uint32_t *)(baseHardwareAddress + FIO0SET_OFFSET);
    //IOxCLR = (uint32_t *)(baseHardwareAddress + FIO0CLR_OFFSET);
    PINSELx = (uint32_t *)(PCB_BASE + pinSelectOffset);
    PINMODEx = (uint32_t *)(PCB_BASE + pinModeOffset);
    break;
  case 1:
    pinSelectOffset = ((pinMask < 0x00010000) ? PINSEL2_OFFSET : PINSEL3_OFFSET);
    pinModeOffset = ((pinMask < 0x00010000) ? PINMODE2_OFFSET : PINMODE3_OFFSET);
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO1PIN_OFFSET);
    IOxMASK = (uint32_t *)(baseHardwareAddress + FIO1MASK_OFFSET);
    IOxDIR = (uint32_t *)(baseHardwareAddress + FIO1DIR_OFFSET);
    //IOxSET = (uint32_t *)(baseHardwareAddress + FIO1SET_OFFSET);
    //IOxCLR = (uint32_t *)(baseHardwareAddress + FIO1CLR_OFFSET);
    PINSELx = (uint32_t *)(PCB_BASE + pinSelectOffset);
    PINMODEx = (uint32_t *)(PCB_BASE + pinModeOffset);
    break;
  case 2:
    pinSelectOffset = ((pinMask < 0x00010000) ? PINSEL4_OFFSET : PINSEL5_OFFSET);
    pinModeOffset = ((pinMask < 0x00010000) ? PINMODE4_OFFSET : PINMODE5_OFFSET);
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO2PIN_OFFSET);
    IOxMASK = (uint32_t *)(baseHardwareAddress + FIO2MASK_OFFSET);
    IOxDIR = (uint32_t *)(baseHardwareAddress + FIO2DIR_OFFSET);
    //IOxSET = (uint32_t *)(baseHardwareAddress + FIO2SET_OFFSET);
    //IOxCLR = (uint32_t *)(baseHardwareAddress + FIO2CLR_OFFSET);
    PINSELx = (uint32_t *)(PCB_BASE + pinSelectOffset);
    PINMODEx = (uint32_t *)(PCB_BASE + pinModeOffset);
    break;
  case 3:
    pinSelectOffset = ((pinMask < 0x00010000) ? PINSEL6_OFFSET : PINSEL7_OFFSET);
    pinModeOffset = ((pinMask < 0x00010000) ? PINMODE6_OFFSET : PINMODE7_OFFSET);
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO3PIN_OFFSET);
    IOxMASK = (uint32_t *)(baseHardwareAddress + FIO3MASK_OFFSET);
    IOxDIR = (uint32_t *)(baseHardwareAddress + FIO3DIR_OFFSET);
    //IOxSET = (uint32_t *)(baseHardwareAddress + FIO3SET_OFFSET);
    //IOxCLR = (uint32_t *)(baseHardwareAddress + FIO3CLR_OFFSET);
    PINSELx = (uint32_t *)(PCB_BASE + pinSelectOffset);
    PINMODEx = (uint32_t *)(PCB_BASE + pinModeOffset);
    break;
  case 4:
    pinSelectOffset = ((pinMask < 0x00010000) ? PINSEL8_OFFSET : PINSEL9_OFFSET);
    pinModeOffset = ((pinMask < 0x00010000) ? PINMODE8_OFFSET : PINMODE9_OFFSET);
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO4PIN_OFFSET);
    IOxMASK = (uint32_t *)(baseHardwareAddress + FIO4MASK_OFFSET);
    IOxDIR = (uint32_t *)(baseHardwareAddress + FIO4DIR_OFFSET);
    //IOxSET = (uint32_t *)(baseHardwareAddress + FIO4SET_OFFSET);
    //IOxCLR = (uint32_t *)(baseHardwareAddress + FIO4CLR_OFFSET);
    PINSELx = (uint32_t *)(PCB_BASE + pinSelectOffset);
    PINMODEx = (uint32_t *)(PCB_BASE + pinModeOffset);
    break;
  }

  int en = ctl_global_interrupts_set(0); // disable

  if (direction == OUTPUT) {
    // for outputs we want to set the initial state in case this
    // this has already been configured as a GPIO so that we make
    // sure that the first state we drive is the correct one
    *IOxPIN = (*IOxPIN & ~pinMask) | (cfg << shiftCount);

    // set the direction of the GPIO as output
    *IOxDIR |= pinMask;
  } else //INPUT
  {
    // make the GPIO an input
    *IOxDIR &= ~pinMask;
    pullupmask = (0x00000003 << ((shiftCount & 15) * 2)); // pull-down
    *PINMODEx &= ~pullupmask;                             // clear the bits.  This is pull-up enabled.
    *PINMODEx |= (cfg << ((shiftCount & 15) * 2));        // 11 = pull-down, 0=pull-up,10=none
    if ((port == 0) || (port == 2))                       // interrupts available for ports 0 and 2
    {
      RisingIntReg = (uint32_t *)(GPIO_BASE + ((port == 0) ? IO0IntEnR_OFFSET : IO2IntEnR_OFFSET));
      *RisingIntReg |= (1 << shiftCount);
      FallingIntReg = (uint32_t *)(GPIO_BASE + ((port == 0) ? IO0IntEnF_OFFSET : IO2IntEnF_OFFSET));
      *FallingIntReg |= (1 << shiftCount);
    }
  }

  *IOxMASK &= ~pinMask; // set mask bit to 0 to enable reads/writes
  // the last step is to connect the pin to this function, this step is
  // last so that outputs will always be driven to the correct state

  // setup the pin usage in the PINSEL block to configure this pin as a GPIO
  *PINSELx &= ~(0x00000003 << ((shiftCount & 15) * 2));

  ctl_global_interrupts_set(en); // set to previous state
}

void lpc_gpio_set_state(uint32_t pin, int state) {
  uint32_t baseHardwareAddress = 0;
  uint32_t port = 0;

  uint32_t *IOxSET = 0;
  uint32_t *IOxCLR = 0;
  uint32_t pinMask = 0;
  uint32_t shiftCount = 0;

  shiftCount = pin & 0x1F;  // pin number in bits 4:0
  port = (pin >> 16) & 0x7; // port number in bits 18:16
  pinMask = 1 << shiftCount;

  // baseHardwareAddress = GPIO_BASE;
  baseHardwareAddress = FIO_BASE;
  switch (port) {
  case 0:
    IOxSET = (uint32_t *)(baseHardwareAddress + FIO0SET_OFFSET);
    IOxCLR = (uint32_t *)(baseHardwareAddress + FIO0CLR_OFFSET);
    break;
  case 1:
    IOxSET = (uint32_t *)(baseHardwareAddress + FIO1SET_OFFSET);
    IOxCLR = (uint32_t *)(baseHardwareAddress + FIO1CLR_OFFSET);
    break;
  case 2:
    IOxSET = (uint32_t *)(baseHardwareAddress + FIO2SET_OFFSET);
    IOxCLR = (uint32_t *)(baseHardwareAddress + FIO2CLR_OFFSET);
    break;
  case 3:
    IOxSET = (uint32_t *)(baseHardwareAddress + FIO3SET_OFFSET);
    IOxCLR = (uint32_t *)(baseHardwareAddress + FIO3CLR_OFFSET);
    break;
  case 4:
    IOxSET = (uint32_t *)(baseHardwareAddress + FIO4SET_OFFSET);
    IOxCLR = (uint32_t *)(baseHardwareAddress + FIO4CLR_OFFSET);
    break;
  }

  if (state) {
    *IOxSET = pinMask;
  } else {
    *IOxCLR = pinMask;
  }
}

int lpc_gpio_get_state(uint32_t pin) {
  int retval = 0;

  uint32_t baseHardwareAddress = 0;
  uint32_t port = 0;

  uint32_t *IOxPIN = 0;
  uint32_t pinMask = 0;
  uint32_t shiftCount = 0;

  shiftCount = pin & 0x1F;  // pin number in bits 4:0
  port = (pin >> 16) & 0x7; // port number in bits 18:16
  pinMask = 1 << shiftCount;

  // baseHardwareAddress = GPIO_BASE;
  baseHardwareAddress = FIO_BASE;
  switch (port) {
  case 0:
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO0PIN_OFFSET);
    break;
  case 1:
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO1PIN_OFFSET);
    break;
  case 2:
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO2PIN_OFFSET);
    break;
  case 3:
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO3PIN_OFFSET);
    break;
  case 4:
    IOxPIN = (uint32_t *)(baseHardwareAddress + FIO4PIN_OFFSET);
    break;
  }

  retval = ((*(IOxPIN)&pinMask) ? 1 : 0);
  return retval;
}