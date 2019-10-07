#include "HardwareSerial.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#if defined(HAVE_HWSERIAL2)

#if defined(HAVE_HWSERIAL2)
static void USART2_ISR(void)
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
  Serial2._handle_usart_irq();
}

#if defined(U2DLL) && defined(U2DLM)
HardwareSerial Serial2(&U2LSR, &U2SCR, &U2ACR, NULL, &U2FDR, &U2TER, &U2RBR, &U2THR, &U2DLL, &U2DLM, &U2IER, &U2IIR, &U2FCR, &U2LCR, NULL, PCONP_PCUART2, UART2_RX_EVENT_ID, UART2_INT, USART2_ISR, 0x7fd00800, 0x7fd01800, P0_11, P0_10, NULL, NULL);
#endif

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial2_available() {
  return 0; // Serial.available();
}

#endif // HAVE_HWSERIAL2