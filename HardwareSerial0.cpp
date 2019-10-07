#include "HardwareSerial.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#if defined(HAVE_HWSERIAL0)

#if defined(HAVE_HWSERIAL0)
static void USART0_ISR(void)
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
  Serial._handle_usart_irq();
}

#if defined(U0DLL) && defined(U0DLM)
HardwareSerial Serial(&U0LSR, &U0SCR, &U0ACR, NULL, &U0FDR, &U0TER, &U0RBR, &U0THR, &U0DLL, &U0DLM, &U0IER, &U0IIR, &U0FCR, &U0LCR, NULL, PCONP_PCUART0, UART0_RX_EVENT_ID, UART0_INT, USART0_ISR, 0x7fd00000, 0x7fd01000, P0_3, P0_2, NULL, NULL);
#endif

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial0_available() {
  return 0; // Serial.available();
}

#endif // HAVE_HWSERIAL0