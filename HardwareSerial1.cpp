#include "HardwareSerial.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#if defined(HAVE_HWSERIAL1)

#if defined(HAVE_HWSERIAL1)
static void USART1_ISR(void)
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
  Serial1._handle_usart_irq();
}

#if defined(U1DLL) && defined(U1DLM)
HardwareSerial Serial1(&U1LSR, &U1SCR, &U1ACR, NULL, &U1FDR, &U1TER, &U1RBR, &U1THR, &U1DLL, &U1DLM, &U1IER, &U1IIR, &U1FCR, &U1LCR, NULL, PCONP_PCUART1, UART1_RX_EVENT_ID, UART1_INT, USART1_ISR, 0x7fd00400, 0x7fd01400, P0_16, P0_15, NULL, NULL);
#endif

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial1_available() {
  return 0; // Serial.available();
}

#endif // HAVE_HWSERIAL1