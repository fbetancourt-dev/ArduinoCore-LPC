#include "HardwareSerial.h"

#include "Arduino.h"
#include "HardwareSerial_private.h"

// this next line disables the entire HardwareSerial.cpp
// this is so I can support any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)
// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
#if defined(HAVE_HWSERIAL0)
void serialEvent() __attribute__((weak));
bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
void serialEvent1() __attribute__((weak));
bool Serial1_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL2)
void serialEvent2() __attribute__((weak));
bool Serial2_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL3)
void serialEvent3() __attribute__((weak));
bool Serial3_available() __attribute__((weak));
#endif

void serialEventRun(void) {
#if defined(HAVE_HWSERIAL0)
  //if (Serial0_available && serialEvent && Serial0_available())
    serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
  if (Serial1_available && serialEvent1 && Serial1_available())
    serialEvent1();
#endif
#if defined(HAVE_HWSERIAL2)
  if (Serial2_available && serialEvent2 && Serial2_available())
    serialEvent2();
#endif
#if defined(HAVE_HWSERIAL3)
  if (Serial3_available && serialEvent3 && Serial3_available())
    serialEvent3();
#endif
}

// Public Methods //////////////////////////////////////////////////////////////
void HardwareSerial::begin(unsigned long baud, byte config) {
  this->_rx_buffer_tail = 0;
  this->_rx_buffer_head = 0;
  this->_tot_chars_rx = 0;
  this->uart_txqueue_count = 0;
  this->uart_rxqueue_count = 0;
  this->uart_framing_error_count = 0;
  this->uart_parity_error_count = 0;
  this->_tx_buffer_tail = 0;
  this->_tx_buffer_head = 0;
  this->uart_tot_chars_tx = 0;
  this->_ulBytesWrittenToTHR = 0;

  if (this->_rxPin == P0_11 && this->_txPin == P0_10) {
    // connect the TX and RX to the external pins
    // NOTE: using pins P0.10 and P0.11, other pins are possible
    PINSEL0 = (PINSEL0 & 0xFF0FFFFF) | 0x00500000;
  } else if (this->_rxPin == P0_3 && this->_txPin == P0_2) {
#if (PROCESSOR_MODEL >= 2300 && PROCESSOR_MODEL <= 2388)
    // connect the TX and RX to the external pins - P02 and P03
    PINSEL0 = (PINSEL0 & 0xFFFFFF0F) | 0x00000050;
#else
    // connect the TX and RX to the external pins
    PINSEL0 = (PINSEL0 & 0xFFFFFFF0) | 0x00000005;
#endif
  } else if (this->_rxPin == P0_16 && this->_txPin == P0_15) {
#if (PROCESSOR_MODEL >= 2300 && PROCESSOR_MODEL <= 2388)
    // connect the TX and RX to the external pins  P015 and P016
    PINSEL0 = (PINSEL0 & 0x3FFFFFFF) | 0x40000000;
    PINSEL1 = (PINSEL1 & 0xFFFFFFFC) | 0x00000001;
    // connect the CTS and RTS to the external pins
    // PINSEL1 = (PINSEL1 & 0xFFFFCFF3) | 0x00001004;
#else
    // connect the TX and RX to the external pins
    PINSEL0 = (PINSEL0 & 0xFFF0FFFF) | 0x00050000;
    // connect the CTS and RTS to the external pins
    // PINSEL0 = (PINSEL0 & 0xFF0FFFFF) | 0x00500000;
#endif
  } else if (this->_rxPin == P0_1 && this->_txPin == P0_0) {
    // connect the TX and RX to the external pins
    // NOTE: using pins P0.0 and P0.1, other pins are possible
    PINSEL0 = (PINSEL0 & 0xFFFFFFF0) | 0x0000000A;
  }

  // enable UARTx in the power control register
  PCONP |= this->_PCONP_PCUARTx;

  // setup the word format to 8-N-1
  *this->_UxLCR = (volatile unsigned char)0x03;
  *this->_UxFCR = (volatile unsigned char)0x87; // interrupt on 8 characters
  // enable UART Receive Data Available Interrupt and the Transmit
  // Holding Register Empty Interrupt
  *this->_UxIER = (volatile unsigned char)(U0IER_RBR_Interrupt_Enable | U0IER_THRE_Interrupt_Enable);

  // set the baudrate
  this->setBaudRate(baud);

  // attach interrupt
  ctl_set_isr(this->_UARTx_INT, 0, CTL_ISR_TRIGGER_FIXED, this->_UARTx_ISR, 0);
  ctl_unmask_isr(this->_UARTx_INT);
  *this->_UxTER = 0x080; // turn transmitter on

  //debug_printfX(debug_prog_flow, "UART0 installed at %d baud\r\n", baudrate);
}

void HardwareSerial::begin(unsigned long baud, byte config, uint32_t rtsPin, uint32_t ctsPin) {
  this->_uart_flags |= UART_USE_RTS_CTS;
  if (this->_ctsPin == NULL || this->_rtsPin == NULL) // only use CTS/RTS if pins supplied
  {
    this->_uart_flags &= (~UART_USE_RTS_CTS);
  }
  begin(baud, config);
}

void HardwareSerial::end() {
}

int HardwareSerial::available(void) {
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
  //return this->_tot_chars_rx;
}

int HardwareSerial::peek(void) {
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
}

int HardwareSerial::read(void) {
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
    _rx_buffer_tail = (volatile int)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

int HardwareSerial::availableForWrite(void) {
  volatile int head;
  volatile int tail;

  //TX_BUFFER_ATOMIC {
  int en = ctl_global_interrupts_set(0); // disable
  head = _tx_buffer_head;
  tail = _tx_buffer_tail;
  ctl_global_interrupts_set(en); // set to previous state
  //}

  if (head >= tail)
    return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  return tail - head - 1;
}

void HardwareSerial::flush() {
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written)
    return;
  //ENTER_CRITICAL
  int en = ctl_global_interrupts_set(0); // disable

  this->uart_rxqueue_count = 0;
  this->_rx_buffer_head = 0;
  this->_rx_buffer_tail = 0;

  // Interrupts are globally disabled, but the DR empty
  // interrupt should be enabled, so poll the DR empty flag to
  // prevent deadlock
  /*
** Clear the 'data waiting' event flag for this uart.
*/
  ctl_events_set_clear(&event_set_isr, 0, this->uart_rx_event_bit);
  //EXIT_CRITICAL
  ctl_global_interrupts_set(en); // set to previous state

  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished tranmission (TXC is set).
}

size_t HardwareSerial::write(uint8_t c) {
  _written = true;
  queue_uart_nowait((unsigned char *)&c, 1);
  return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void HardwareSerial::setBaudRate(unsigned long baudrate) {
  // The formula that calculates the baud rate has three unknowns in it. Thus there
  // is no straight forward way to calculate all of the necessary registers required
  // to setup the UART. The only generic and viable approach to compute these parameters
  // is to iterate through all of the permutations of two of the three unknowns.
  // This is an iterative and lengthy approach but in general the baudrate of the
  // UART does not frequently change making this a one time hit.

  uint32_t prescalerMultiplier = 1;
  uint32_t prescalerDivider = 0;

  uint32_t bestPrescalerMultiplier = 1;
  uint32_t bestPrescalerDivider = 0;
  uint32_t bestDivisorLatch = 1;
  uint32_t bestError = 0xFFFFFFFF;

  uint32_t peripheralClock = getPeripheralClock();

#if !(PROCESSOR_MODEL >= 2104 && PROCESSOR_MODEL <= 2106)

  // loop through all of the prescaler multiplier and dividor values until
  // we get to one that produces a very accurate baudrate
  for (prescalerMultiplier = 1; prescalerMultiplier < 16 && bestError > 1; prescalerMultiplier++) {
    for (prescalerDivider = 0; prescalerDivider < 16 && bestError > 1; prescalerDivider++) {

#endif
      uint32_t candidateDivisorLatch;
      uint32_t candidateBaudrate;
      uint32_t candidateError;

      // calculate the candidate divisor latch
      candidateDivisorLatch = (peripheralClock * prescalerMultiplier) / (16 * baudrate * (prescalerMultiplier + prescalerDivider));

      // The calculation of the candidate divisor latch was all done as
      // integer math. It is possible that a value 1 higher actually
      // produces a more accurate baudrate.
      uint32_t i;
      for (i = 0; i < 2; i++) {
        // determine the baudrate based on the candidate divisor latch
        candidateBaudrate = (peripheralClock * prescalerMultiplier) / (16 * candidateDivisorLatch * (prescalerMultiplier + prescalerDivider));

        // determine the error for this candidate baudrate
        if (candidateBaudrate > baudrate) {
          candidateError = candidateBaudrate - baudrate;
        } else {
          candidateError = baudrate - candidateBaudrate;
        }

        // if we have a new best fit
        if (candidateError < bestError && candidateDivisorLatch < 0xFFFF) {
          bestError = candidateError;
          bestPrescalerMultiplier = prescalerMultiplier;
          bestPrescalerDivider = prescalerDivider;
          bestDivisorLatch = candidateDivisorLatch;
        }

        // try the next highest divisor latch value
        candidateDivisorLatch++;
      }
#if !(PROCESSOR_MODEL >= 2104 && PROCESSOR_MODEL <= 2106)
    }
  }
#endif

  // grant access to the divisor latch
  *this->_UxLCR |= U1LCR_Divisor_Latch_Access_Bit;

#if !(PROCESSOR_MODEL >= 2104 && PROCESSOR_MODEL <= 2106)

  // set the multiplier/divider values
  *this->_UxFDR = (bestPrescalerMultiplier << U1FDR_MULVAL_BIT) |
                  (bestPrescalerDivider << U1FDR_DIVADDVAL_BIT);

#endif

  // setup the divisor latch
  *this->_UxDLM = (uint8_t)((bestDivisorLatch >> 8) & 0xFF);
  *this->_UxDLL = bestDivisorLatch & 0xFF;

  // activate the new baudrate
  *this->_UxLCR &= ~U1LCR_Divisor_Latch_Access_Bit;
}

int HardwareSerial::queue_uart_nowait(unsigned char *buf, int len) {
  int retval = 0;
  int idx1, idx2, k;
  int queue_count;
  unsigned char b;

  if (len == 0 || buf == NULL) {
    return len;
  }

  int en = ctl_global_interrupts_set(0); // disable
  queue_count = this->uart_txqueue_count;
  if (queue_count == 0) {
    this->_tx_buffer_head = this->_tx_buffer_tail = 0;
  }
  idx1 = this->_tx_buffer_head;
  idx2 = this->_tx_buffer_tail;
  ctl_global_interrupts_set(en); // set to previous state
                                 /*
** Figure out how many bytes we can safely copy.
*/
  if (queue_count == 0) {
    retval = SERIAL_TX_BUFFER_SIZE; // the whole fifo
  } else {
    retval = SERIAL_TX_BUFFER_SIZE - queue_count;
  }

  if (retval > 0) {
    if (retval > len) {
      retval = len;
    }
    if (idx1 >= idx2) // can fill to end of fifo
    {
      k = SERIAL_TX_BUFFER_SIZE - idx1;
      if (retval <= k) // whole thing fits
      {
        memcpy(&this->_tx_buffer[idx1], buf, retval);
        idx1 = (idx1 + retval) % SERIAL_TX_BUFFER_SIZE;
      } else // this copy will cause a wrap.
      {
        memcpy(&this->_tx_buffer[idx1], buf, k);
        memcpy(this->_tx_buffer, &buf[k], retval - k);
        idx1 = retval - k;
      }
    } else // will not reach end of fifo
    {
      k = idx2 - idx1; // can fill this many bytes
      if (retval > k) {
        retval = k;
      }
      memcpy(&this->_tx_buffer[idx1], buf, retval);
      idx1 += retval;
    }
    int en = ctl_global_interrupts_set(0); // disable
    this->_tx_buffer_head = idx1;
    this->uart_txqueue_count += retval;

    if (!(this->_uart_flags & UART_TRANSMITTING)) {
      this->_uart_flags |= UART_TRANSMITTING;
      b = this->_tx_buffer[this->_tx_buffer_tail++];
      this->_tx_buffer_tail %= SERIAL_TX_BUFFER_SIZE;
      this->uart_txqueue_count--;
      *this->_UxTHR = b;
      ++this->_ulBytesWrittenToTHR;
    }
    ctl_global_interrupts_set(en); // set to previous state
  }
  this->uart_tot_chars_tx += retval;
  return retval;
}

int HardwareSerial::queue_uart(unsigned char *buf, int len, uint32_t *pu32TaskActivity) {
  int iBytesSent;
  int iTotalBytesSent = 0;
  int iBytesToSend = len;
  if (buf == NULL || len <= 0) {
    return 0;
  }
  while (iBytesToSend > 0) {
    iBytesSent = queue_uart_nowait(&buf[iTotalBytesSent], iBytesToSend);
    iTotalBytesSent += iBytesSent;
    iBytesToSend -= iBytesSent;
    if (iBytesToSend > 0) {
      ctl_timeout_wait(millis() + 200L);
      if (pu32TaskActivity) {
        *pu32TaskActivity = millis();
      }
    }
  }
  return iTotalBytesSent;
}

int HardwareSerial::dequeue_uart(unsigned char *buf, int len, long timeout) {
  int retval = 0;
  int k;
  int queue_count, head, tail;

  if (len == 0 || buf == NULL) {
    return 0;
  }
  /*
** Disable interrupts and get local copies of count, head and tail.
** If count is 0, check if a timeout value is specified, if so,
** wait that long for new characters to come in.  Using the event structure,
** we will be woken up when new chars come in, or on timeout.
*/

  int en = ctl_global_interrupts_set(0); // disable

  queue_count = this->uart_rxqueue_count;
  head = this->_rx_buffer_head;
  tail = this->_rx_buffer_tail;

  ctl_global_interrupts_set(en); // set to previous state

  if (queue_count == 0 && timeout != 0) {
    /*
** Using the event queue, we will wait here until either we have data
** available, or the timeout occurs.
*/
    ctl_events_wait(CTL_EVENT_WAIT_ANY_EVENTS, &event_set_isr,
        this->uart_rx_event_bit, (CTL_TIMEOUT_t)1, ctl_get_current_time() + timeout);
    en = ctl_global_interrupts_set(0); // disable
    queue_count = this->uart_rxqueue_count;
    head = this->_rx_buffer_head;
    tail = this->_rx_buffer_tail;
    ctl_global_interrupts_set(en); // set to previous state
  }
  if (queue_count == 0) {
    /*
** Clear the 'data waiting' event flag for this uart.
*/
    ctl_events_set_clear(&event_set_isr, 0, this->uart_rx_event_bit);
    return 0;
  }
  if (queue_count < len) {
    len = queue_count; // we will pull this many bytes
  }
  // Changed to line below on 12/24..  Dave  orig : if(head < tail) // head wrapped, pull from tail to end of buffer
  if (head <= tail) // head wrapped, pull from tail to end of buffer
  {
    k = SERIAL_RX_BUFFER_SIZE - tail; // pull at most k bytes
    if (len < k)                      // fewer than k were requested
    {
      k = len; // so pull the amount requested
    }
    memcpy(buf, &this->_rx_buffer[tail], k);
    retval += k;
    len -= k;
    buf = &buf[k];
    tail = (tail + k) % SERIAL_RX_BUFFER_SIZE;
  }
  /*
** if we just wrapped the tail pointer, or if tail was already < head,
** we continue filling up the buffer
*/
  if ((head > tail) && (len > 0)) {
    k = head - tail;
    if (len < k) {
      k = len;
    }
    memcpy(buf, &this->_rx_buffer[tail], k);
    retval += k;
    len -= k;
    tail += k; // no chance of a wrap here, head was > tail
  }
  this->_tot_chars_rx += retval;
  en = ctl_global_interrupts_set(0); // disable
  this->uart_rxqueue_count -= retval;
  if (this->uart_rxqueue_count == 0) // did we hit 0 ?
  {
    /*
** 0 bytes left on queue, so clear the 'data waiting' event flag
*/
    ctl_events_set_clear(&event_set_isr, 0, this->uart_rx_event_bit);
  }
  this->_rx_buffer_tail = tail;
  if ((this->_uart_flags & (UART_USE_RTS_CTS | UART_RTS_ASSERTED)) == (UART_USE_RTS_CTS | UART_RTS_ASSERTED)) {
    if (this->uart_rxqueue_count < UART_RX_BUFFER_LOWATER) {
      // de-assert RTS so the module will start sending data again.
      this->_uart_flags &= ~UART_RTS_ASSERTED;
      digitalWrite(this->_rtsPin, LOW);
    }
  }
  ctl_global_interrupts_set(en); // set to previous state
  return retval;
}

int HardwareSerial::tx_queue_count() {
  int retval = 0;
  int en = ctl_global_interrupts_set(0); // disable
  retval = this->uart_txqueue_count;
  ctl_global_interrupts_set(en); // set to previous state
  return retval;
}

int HardwareSerial::uartReadChar(unsigned char *cptr) {
  int retval = 0;

  if (this->uart_rxqueue_count > 0) {
    retval = 1;
    int en = ctl_global_interrupts_set(0); // disable
    *cptr = this->_rx_buffer[this->_rx_buffer_tail++];
    this->_rx_buffer_tail %= SERIAL_RX_BUFFER_SIZE;
    this->uart_rxqueue_count--;
    if (this->uart_rxqueue_count == 0) // did we hit 0 ?
    {
      /*
** 0 bytes left on queue, so clear the 'data waiting' event flag
*/
      ctl_events_set_clear(&event_set_isr, 0, this->uart_rx_event_bit);
    }
    ctl_global_interrupts_set(en); // set to previous state
  }

  return retval;
}

int HardwareSerial::UartBumpTxAlong() {
  int retval = 0;
  unsigned char b;
  int en = ctl_global_interrupts_set(0); // disable
  if (this->uart_txqueue_count > 0) {
    if (!(this->_uart_flags & UART_TRANSMITTING)) {
      this->_uart_flags |= UART_TRANSMITTING;
      b = this->_tx_buffer[this->_tx_buffer_tail++];
      this->_tx_buffer_tail %= SERIAL_TX_BUFFER_SIZE;
      this->uart_txqueue_count--;
      *this->_UxTHR = b;
      this->_ulBytesWrittenToTHR++;
      this->uart_tot_chars_tx++;
    }
  }
  ctl_global_interrupts_set(en); // set to previous state
  return retval;
}

int HardwareSerial::__putchar(int b1, __printf_tag_ptr tag) {
  int retval = 0x0ff & b1;
  unsigned char b[2];
  if ((b1 == '\n') && (this->_last_putchar != '\r')) {
    b[0] = '\r';
    b[1] = '\n';
    queue_uart_nowait(b, 2);
    //queue_uart_nowait(b, 2, NULL);
  } else {
    b[0] = 0x0ff & b1;
    queue_uart_nowait(b, 1);
    //queue_uart(b, 1, NULL);
  }
  this->_last_putchar = 0x0ff & b1;
  return retval;
}

int HardwareSerial::__getchar() {
  int retval = EOF;
  unsigned char b;
  while (dequeue_uart(&b, 1, 200L) == 0)
    ;
  retval = 0x0ff & b;
  if (retval == 13) {
    retval = 10;
  }
  __putchar(retval, 0);
  return retval;
}

void HardwareSerial::raw_putchar(char c) {
  queue_uart((unsigned char *)&c, 1, NULL);
}

void HardwareSerial::raw_puts(const char *c) {
  queue_uart((unsigned char *)c, strlen(c), NULL);
}

void HardwareSerial::printf(const char *fmt, ...) {
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, 128, fmt, args);
  va_end(args);
  unsigned long len = strlen(tmp);
  queue_uart((unsigned char *)tmp, len, NULL);
}

#endif // whole file