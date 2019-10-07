#include "wiring.h"

#include "wiring_digital.h"

void setupExternalRam(void) {
  /**
  * Setup the control lines for the external RAM Bank 0.
  * We have 128K of external RAM, but set up on only one chip, so
  * only CS0 is connected.  Don't need CS1.
  *
  * Only 64K addressable at any time, so we have 64K to use normally,
  * but another 64K bank that can be used for OTA updates or for
  * temporary storage, but only one of the 64K banks can be accessed
  * at any given time.
  *
  * Switch between banks by changing the state of P1.28, which is
  * attached to address line 16 of the external RAM.
  */
  //only for (PROCESSOR_MODEL == 2378) && defined(SETUP_EON_EXTERNAL_RAM)
  // Enable EMC pins D0 - D7
  PINSEL6 = (PINSEL6 & 0xFFFF0000) | 0x00005555;
  PINMODE6 = (PINMODE6 & 0xFFFF0000) | 0x0000AAAA;

  // Enable EMC pins A0 - A15
  PINSEL8 = 0x55555555;
  PINMODE8 = 0xAAAAAAAA;

  // Enable EMC pins OE#, BLS0#, CS0#
  PINSEL9 = (PINSEL9 & 0xCFF0FFFF) | 0x10090000;
  PINMODE9 = (PINMODE9 & 0xCFF0FFFF) | 0x200A0000;
  // use fast I/O
  SCS |= SCS_GPIOM;

  FIO1MASK = 0x00000000;   // enable it for writing
  PINMODE3 &= ~0x03000000; // set P1.28 to no pullup/pulldown
  PINMODE3 |= 0x02000000;  //

  pinMode(P1_28, OUTPUT); // ext mem a16
  digitalWrite(P1_28, LOW);

  // Enable the memory controller with normal memory map
  EMCControl = 1;
}

void delay(unsigned long ms) {
  ctl_timeout_wait(ctl_get_current_time() + ms);
}

unsigned long millis() {
  unsigned long m;
  //uint8_t oldSREG = SREG;

  // disable interrupts while we read timer0_millis or we might get an
  // inconsistent value (e.g. in the middle of a write to timer0_millis)
  //cli();
  m = ctl_get_current_time();
  //SREG = oldSREG;

  return m;
}