/* Arduino SPIMemory Library v.3.4.0
 * Copyright (C) 2017 by Prajwal Bhattaram
 * Created by Prajwal Bhattaram - 18/04/2018
 * Modified by Prajwal Bhattaram - 03/06/2019
 *
 * This file is part of the Arduino SPIMemory Library. This library is for
 * Flash and FRAM memory modules. In its current form it enables reading,
 * writing and erasing data from and to various locations;
 * suspending and resuming programming/erase and powering down for low power operation.
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License v3.0
 * along with the Arduino SPIMemory Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef SPIMEMORY_H
#define SPIMEMORY_H
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//    Uncomment the code below to enable SFDP discovery - especially  //
//                    if using an unsupported chip                    //
//                                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//#define USES_SFDP                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//     Uncomment the code below to run a diagnostic if your flash 	  //
//                         does not respond                           //
//                                                                    //
//      Error codes will be generated and returned on functions       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//#define RUNDIAGNOSTIC                                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   Uncomment the code below to increase the speed of the library    //
//                  by disabling _notPrevWritten()                    //
//                                                                    //
// Make sure the sectors being written to have been erased beforehand //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//#define HIGHSPEED                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   Uncomment the code below to disable overflow and force data      //
//   to only be written to the last address of the flash memory       //
//    and not rollover to address 0x00 when the end is reached        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//#define DISABLEOVERFLOW                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   Comment out the code below to disable DMA mode on SAMD based     //
//                        platforms (In ALPHA)                        //
//                                                                    //
//   Change the ZERO_SPISERCOM define below to use other SPI ports    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//#define ENABLEZERODMA                                               //
//#define ZERO_SPISERCOM SERCOM4                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  #include <Arduino.h>
  #include <SPI.h>
  #include "defines.h"
  #include "SPIFlash.h"
  #include "SPIFram.h"
  #include "diagnostics.h"

#include "Arduino.h"
#if defined (ARDUINO_ARCH_SAM)
  #include <malloc.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include "DMASAM.h"
#endif

#if defined (ARDUINO_ARCH_SAMD)
  #if defined (ENABLEZERODMA)
    #include "DMASAMD.h"
  #endif
#endif

#if defined (BOARD_RTL8195A)
  #ifdef __cplusplus
    extern "C" {
  #endif

  #include "gpio_api.h"
  #include "PinNames.h"

  #ifdef __cplusplus
    }
  #endif
#endif


#define SPIFLASH_LIBVER 3
#define SPIFLASH_LIBSUBVER 4
#define SPIFLASH_REVVER 0

#define SPIFRAM_LIBVER 0
#define SPIFRAM_LIBSUBVER 0
#define SPIFRAM_REVVER 1

class SPIMemory {
public:
  //------------------------------------ Constructor ------------------------------------//
  SPIMemory(void) {};
  ~SPIMemory(void) {};
  //------------------------------- Public functions -----------------------------------//
  //------------------------------- Public variables -----------------------------------//


};

extern SPIMemory SPIMem; //default SPIMemory instance;

#endif // _SPIMEMORY_H_
