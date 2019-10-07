#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"

#define SERIAL_TX_BUFFER_SIZE 1024
#define SERIAL_RX_BUFFER_SIZE 1024

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream {
protected:
  uint8_t *_rx_buffer;
  volatile int _rx_buffer_tail;
  volatile int _rx_buffer_head;

  volatile int uart_rxqueue_count;
  volatile int _tot_chars_rx;

  volatile int uart_rx_event_bit;

  uint8_t *_tx_buffer;
  volatile int _tx_buffer_head;
  volatile int _tx_buffer_tail;

  volatile int uart_txqueue_count;
  volatile int uart_tot_chars_tx;

  volatile int uart_framing_error_count;
  volatile int uart_parity_error_count;
  volatile int uart_overrun_error_count;

  volatile unsigned long _ulBytesWrittenToTHR;
  volatile int _iCharsLost;
  volatile unsigned char _last_putchar;

  volatile int _uart_flags;

  volatile uint32_t _rxPin;
  volatile uint32_t _txPin;
  volatile uint32_t _rtsPin;
  volatile uint32_t _ctsPin;

  // UART Register Map

  volatile unsigned char *_UxLSR; // Line Status Register (8 bit Data) - Read Only
                                  // [RX FIFO][TEMT][THRE][BI][FE][PE][OE][DR]

  volatile unsigned char *_UxSCR; // Scratch Pad Register (8 bit Data) - R/W

  volatile unsigned long *_UxACR; // Auto-baud Control Register (32 bit Data) - R/W
                                  // [Reserved 31-10][ABTO IntClr][ABEO IntClr][Reserved 7-3][Auto Reset][Mode]

  volatile unsigned long *_UxICR; // IrDA Control Register (8 bit Data) - R/W
                                  // [Reserved][PulseDiv][FixPulse En][IrDAInv][IrDAEn]

  volatile unsigned long *_UxFDR; // Fractional Divider Register ( 8 bit Data) - R/W
                                  // [MulVal][DivAddVal]

  volatile unsigned char *_UxTER; // Transmit Enable Register (8 bit Data) - R/W

  volatile unsigned char *_UxRBR; // Receiver Buffer Register

  volatile unsigned char *_UxTHR; // Transmit Holding Register

  volatile unsigned char *_UxDLL; // Divisor Latch LSB - R/W

  volatile unsigned char *_UxDLM; // Divisor Latch MSB - R/W

  volatile unsigned long *_UxIER; // Interrupt Enable Register

  volatile unsigned long *_UxIIR; // Interrupt ID Register

  volatile unsigned char *_UxFCR; // FIFO Control Register - Write Only

  volatile unsigned char *_UxLCR; // Line Control Register - R/W
                                  // [DLAB][Set Break][Stick Parity][Even Parity Select][Parity Ebable][Number of Stop Bits][Word Length Select 1-0]

  volatile unsigned char *_UxMSR;

  int _PCONP_PCUARTx;
  uint8_t _UARTx_RX_EVENT_ID;
  uint8_t _UARTx_INT;

  volatile uint8_t *_UARTxRX_EVENT_ID;
  volatile uint8_t *_UARTxRX_INT;

  void (*_UARTx_ISR)(void);

  // Has any byte been written to the UART since begin()
  bool _written;

public:
  inline HardwareSerial(
      volatile unsigned char *LSR, volatile unsigned char *SCR,
      volatile unsigned long *ACR, volatile unsigned long *ICR,
      volatile unsigned long *FDR, volatile unsigned char *TER,
      volatile unsigned char *RBR, volatile unsigned char *THR,
      volatile unsigned char *DLL, volatile unsigned char *DLM,
      volatile unsigned long *IER, volatile unsigned long *IIR,
      volatile unsigned char *FCR, volatile unsigned char *LCR,
      volatile unsigned char *MSR, int PCONP_PCUARTX,
      uint8_t UARTx_RX_EVENT_ID, uint8_t UARTx_INT,
      void (*_UARTx_ISR)(void),
      uint32_t uart_rx_buffer_addr, uint32_t uart_tx_buffer_addr,
      uint32_t rxPin, uint32_t txPin, uint32_t rtsPin, uint32_t ctsPin);

  void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
  void begin(unsigned long, uint8_t);
  void begin(unsigned long baud, uint8_t config, uint32_t rtsPin, uint32_t ctsPin);
  void end();

  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual int availableForWrite(void);
  virtual void flush(void);
  
  virtual size_t write(uint8_t);

  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
  using Print::write; // pull in write(str) and write(buf, size) from Print
  operator bool() { return true; }

  void setBaudRate(unsigned long baudrate);

  int queue_uart_nowait(unsigned char *buf, int len);
  int queue_uart(unsigned char *buf, int len, uint32_t *pu32TaskActivity);
  int dequeue_uart(unsigned char *buf, int len, long timeout);

  int tx_queue_count();
  int uartReadChar(unsigned char *cptr);
  int UartBumpTxAlong();


  int __putchar(int b1, __printf_tag_ptr tag);
  int __getchar();

  void raw_putchar(char c);
  void raw_puts(const char *c);

  void printf(const char *format, ...);

  // Interrupt handlers - Not intended to be called externally
  inline void _handle_usart_irq(void);
};

extern HardwareSerial Serial;
#define HAVE_HWSERIAL0

extern HardwareSerial Serial1;
#define HAVE_HWSERIAL1

extern HardwareSerial Serial2;
#define HAVE_HWSERIAL2

extern HardwareSerial Serial3;
#define HAVE_HWSERIAL3

extern void serialEventRun(void) __attribute__((weak));

#endif // whole file