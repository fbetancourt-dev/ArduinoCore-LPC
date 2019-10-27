#ifndef bsp_h
#define bsp_h

#include "Arduino.h"

#define GPIO_port_a                 P0_6  // Port A, PIN_113, PORTA_SEL
#define GPIO_port_b                 P0_5  // Port B, PIN_115, PORTB_SEL

#define GPIO_gsmreset               P0_31 // GSM RESET, PIN_36, MODEM_RESET
#define GPIO_onoff                  P0_13 // GSM ON/OFF, PIN_32, ON_OFF
#define GPIO_pwrmon                 P0_18 // GSM PWRMON, PIN_86, PWRMON
#define GPIO_gsmservice             P0_19 // GSM SERVICE, PIN_85, SERVICE
#define GPIO_gsmCTS                 P0_17 // GSM CTS, PIN_87, CTS1
#define GPIO_gsmRTS                 P0_22 // GSM RTS, PIN_80, RTS1

#define GPIO_gsmreset               P0_31 // GSM RESET, PIN_36, MODEM_RESET
#define GPIO_onoff                  P0_13 // GSM ON/OFF, PIN_32, ON_OFF
#define GPIO_pwrmon                 P0_18 // GSM PWRMON, PIN_86, PWRMON
#define GPIO_gsmservice             P0_19 // GSM SERVICE, PIN_85, SERVICE
#define GPIO_gsmCTS                 P0_17 // GSM CTS, PIN_87, CTS1
#define GPIO_gsmRTS                 P0_22 // GSM RTS, PIN_80, RTS1


#define GPIO_xbeereset              P0_27 // XBEE RESET, PIN_35, ~RESET_XBEE
#define GPIO_xbeeCTS                P0_26 // XBEE CTS, PIN_8, XBEE_CTS
#define GPIO_xbeeRTS                P0_25 // XBEE RTS, PIN_10, XBEE_RTS
#define GPIO_xbeeTXD                P0_2  // XBEE TXD, PIN_141, XBEE_TXD
#define GPIO_xbeeRXD                P0_3  // XBEE RXD, PIN_142, XBEE_RXD

#define GPIO_PowerFailure           P0_24 // Power Failure, PIN_11, POWER_FAIL

#define GPIO_VmDoorOpen             P1_19 // JAN_IO7, PIN_47, JAN_IO7

#define GPIO_ForceVmDoorOpen        P1_20 // JAN_IO6, PIN_49, JAN_IO6
#define GPIO_VmDoor2Open            P1_21 // JAN_IO5, PIN_50, JAN_IO5
#define GPIO_ForceVmDoor2Open       P1_22 // JAN_IO4, PIN_51, JAN_IO4

#define GPIO_button1                P0_28 // Push button 1, PIN_34, ~DEX_RD
#define GPIO_button2                P1_23 // Push button 2, PIN_53, JAN_IO3

#define GPIO_error                  P0_8  // ERROR LED, PIN_111, ERROR_LED
#define GPIO_busy                   P0_7  // BUSY LED, PIN_112, ARM_LED

#define GPIO_battery_voltage        P0_12 // Battery Voltage, PIN_29, Batt_Volt

#define GPIO_temperature            
#define GPIO_temperature2           

#define GPIO_UL_GL                  P1_25 // UL/GL determination, PIN_56, JAN_IO1
#define GPIO_HL                     P1_22 // HL, PIN_51, JAN_IO4

#define GPIO_dummy                  P1_27 // X, PIN_61, X
#define GPIO_testCTS                P2_0  // X, PIN_107, X
#define GPIO_testRTS                P2_1  // X, PIN_106, X 

#define GPIO_A16                    P1_28 // ext mem a16, PIN_63, EXT_MEM_A16

#define GPIO_WatchdogFeed           P0_20 // Watchdog circuit feed, PIN_83, ~WATCH_DOG
#define GPIO_WatchdogEnable         P0_4  // Watchdog chip enable (active low), PIN 116, ~WATCHDOG_EN

#define SPI_MISO                    P1_23 // MISO, PIN_53, 
#define SPI_MOSI                    P1_24 // MOSI, PIN_54, 
#define SPI_CS                      P1_21 // CS, PIN_50, 
#define SPI_SCK                     P1_20 // SCK, PIN_49,

#define TXD0                        P0_2 // 
#define RXD0                        P0_3 // 

#define TXD1                        P0_15 // 
#define RXD1                        P0_16 // 

#define TXD2                        P0_10 // 
#define RXD2                        P0_11 // 

#define TXD3                        P0_0  // 
#define RXD3                        P0_1  // 

#endif // whole file