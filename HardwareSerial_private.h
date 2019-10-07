#include "HardwareSerial.h"

#include "wiring_digital.h"
#include "wiring_private.h"

extern CTL_EVENT_SET_t event_set_isr; // use these events to trigger tasks from ISRs

/*
** How many bytes can we stuff into the FIFO when we
** get a THRE interrupt ?
** Max is 15.  Min is 1.
** Higher values mean less interrupt overhead.
*/

#define TX_HARDWARE_FIFO_SIZE 14

#define UART_RX_BUFFER_HIWATER 896
#define UART_RX_BUFFER_LOWATER 128

#define UART_USE_RTS_CTS 1
#define UART_RTS_ASSERTED 2
#define UART_CTS_ASSERTED 4
#define UART_TRANSMITTING 8

#define UART0_INT 6
#define UART1_INT 7
#define UART2_INT 28
#define UART3_INT 29

#define UART0_RX_EVENT_ID 8
#define UART1_RX_EVENT_ID 16
#define UART2_RX_EVENT_ID 32
#define UART3_RX_EVENT_ID 64

#define UxIIR_Interrupt_Pending_MASK 1

typedef enum {
  BAUDRATE_1200 = 1200,
  BAUDRATE_2400 = 2400,
  BAUDRATE_4800 = 4800,
  BAUDRATE_9600 = 9600,
  BAUDRATE_19200 = 19200,
  BAUDRATE_38400 = 38400,
  BAUDRATE_57600 = 57600,
  BAUDRATE_115200 = 115200
} BaudrateType;

// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(
    volatile unsigned char *LSR,
    volatile unsigned char *SCR,
    volatile unsigned long *ACR,
    volatile unsigned long *ICR,
    volatile unsigned long *FDR,
    volatile unsigned char *TER,
    volatile unsigned char *RBR,
    volatile unsigned char *THR,
    volatile unsigned char *DLL,
    volatile unsigned char *DLM,
    volatile unsigned long *IER,
    volatile unsigned long *IIR,
    volatile unsigned char *FCR,
    volatile unsigned char *LCR,
    volatile unsigned char *MSR,
    int PCONP_PCUARTx,
    uint8_t UARTx_RX_EVENT_ID,
    uint8_t UARTx_INT,
    void (*_UARTx_ISR)(void),
    uint32_t uart_rx_buffer_addr,
    uint32_t uart_tx_buffer_addr,
    uint32_t rxPin,
    uint32_t txPin,
    uint32_t rtsPin,
    uint32_t ctsPin) : _UxLSR(LSR), _UxSCR(SCR), _UxACR(ACR), _UxICR(ICR),
                       _UxFDR(FDR), _UxTER(TER), _UxRBR(RBR), _UxTHR(THR),
                       _UxDLL(DLL), _UxDLM(DLM), _UxIER(IER), _UxIIR(IIR),
                       _UxFCR(FCR), _UxLCR(LCR), _UxMSR(MSR), _PCONP_PCUARTx(PCONP_PCUARTx),
                       _UARTx_RX_EVENT_ID(UARTx_RX_EVENT_ID), _UARTx_INT(UARTx_INT),
                       _UARTx_ISR(_UARTx_ISR),
                       _rx_buffer((unsigned char *)uart_rx_buffer_addr),
                       _tx_buffer((unsigned char *)uart_tx_buffer_addr),
                       _rxPin(rxPin), _txPin(txPin), _rtsPin(rtsPin), _ctsPin(ctsPin),
                       _iCharsLost(0), _last_putchar(0) {
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_handle_usart_irq(void) {
  /* Read IIR to clear interrupt and find out the cause */
  unsigned iir = *this->_UxIIR;
  uint16_t temp;
  unsigned char b;
  unsigned char ucLSR;

  /* Handle UART0 interrupt */
  while (!(iir & UxIIR_Interrupt_Pending_MASK)) {
    switch ((iir >> 1) & 0x7) {
    case 1:
      /* THRE interrupt */
      if (this->uart_txqueue_count > 0) {
        this->_uart_flags |= UART_TRANSMITTING;
        temp = TX_HARDWARE_FIFO_SIZE;
        if (temp > this->uart_txqueue_count) {
          temp = this->uart_txqueue_count;
        }
        this->uart_txqueue_count -= temp;
        while (temp > 0) {
          *this->_UxTHR = this->_tx_buffer[this->_tx_buffer_tail++];
          this->_tx_buffer_tail %= SERIAL_TX_BUFFER_SIZE;
          temp--;
          this->_ulBytesWrittenToTHR++;
        }
      } else {
        this->_uart_flags &= ~UART_TRANSMITTING;
        this->_tx_buffer_tail = this->_tx_buffer_head = 0;
      }
      break;
    case 2: /* RDA interrupt */
    case 6: // character timeout
    case 3: // line status register
#if 1
      while (1) {
        // Save the Line status register because reading
        // it clears error status and we want to preserve
        // that so we can log the errors.
        ucLSR = *this->_UxLSR;

        // Check for parity, framing, and overrun errors..
        if (ucLSR & (U1LSR_PE | U1LSR_FE | U1LSR_OE)) {
          // Check for parity errors..
          if (ucLSR & U1LSR_PE) {
            this->uart_parity_error_count++;
          }
          // Check for framing errors..
          if (ucLSR & U1LSR_FE) {
            this->uart_framing_error_count++;
          }
          // Check for overrun errors..
          if (ucLSR & U1LSR_OE) {
            this->uart_overrun_error_count++;
          }
        }

        if (ucLSR & U1LSR_RDR) {
          b = *this->_UxRBR;
          ucLSR = *this->_UxLSR;
          if (this->uart_rxqueue_count < SERIAL_RX_BUFFER_SIZE) {
            this->_rx_buffer[this->_rx_buffer_head++] = b;
            this->_rx_buffer_head %= SERIAL_RX_BUFFER_SIZE;
            this->uart_rxqueue_count++;
          } else {
            this->_iCharsLost++;
          }
        } else {
          break;
        }
      }
      if (this->_uart_flags & UART_USE_RTS_CTS) {
        if (this->uart_rxqueue_count > UART_RX_BUFFER_HIWATER) {
          digitalWrite(this->_rtsPin, HIGH); // assert RTS to stop transmitter
          this->_uart_flags |= UART_RTS_ASSERTED;
        }
      }
      if (this->uart_rxqueue_count > 0) {
        ctl_events_set_clear(&event_set_isr, this->uart_rx_event_bit, 0);
      }
#else
      while (*u->UxLSR & U1LSR_RDR) {
        b = *u->UxRBR;
        if (u->uart_rxqueue_count < UART_RX_BUFFER_SIZE) {
          u->uart_rx_buffer[u->uart_rx_insert_idx++] = b;
          u->uart_rx_insert_idx %= UART_RX_BUFFER_SIZE;
          u->uart_rxqueue_count++;
        } else {
          iCharsLost++;
        }
      }
      if (u->uart_flags & UART_USE_RTS_CTS) {
        if (u->uart_rxqueue_count > UART_RX_BUFFER_HIWATER) {
          set_GPIO_state(u->RTSpin, 1); // assert RTS to stop transmitter
          u->uart_flags |= UART_RTS_ASSERTED;
        }
      }
      if (u->uart_rxqueue_count > 0) {
        ctl_events_set_clear(&event_set1, u->uart_rx_event_bit, 0);
      }
      // Check for parity errors..
      if (*u->UxLSR & U1LSR_PE) {
        u->uart_parity_error_count++;
      }
      // Check for framing errors..
      if (*u->UxLSR & U1LSR_FE) {
        u->uart_framing_error_count++;
      }
      // Check for overrun errors..
      if (*u->UxLSR & U1LSR_OE) {
        u->uart_overrun_error_count++;
      }
#endif
      break;
    case 0: // modem status register change
      b = *this->_UxMSR;
      break;
    }
    iir = *this->_UxIIR;
  }
}