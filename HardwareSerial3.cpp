#include "HardwareSerial.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#if defined(HAVE_HWSERIAL3)

#if defined(HAVE_HWSERIAL3)
static void USART3_ISR(void)
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
  Serial3._handle_usart_irq();
}

#if defined(U3DLL) && defined(U3DLM)
HardwareSerial Serial3(&U3LSR, &U3SCR, &U3ACR, NULL, &U3FDR, &U3TER, &U3RBR, &U3THR, &U3DLL, &U3DLM, &U3IER, &U3IIR, &U3FCR, &U3LCR, NULL, PCONP_PCUART3, UART3_RX_EVENT_ID, UART3_INT, USART3_ISR, 0x7fd00c00, 0x7fd01c00, P0_1, P0_0, NULL, NULL);
#endif

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial3_available() {
  return 0; // Serial.available();
}

#endif // HAVE_HWSERIAL3