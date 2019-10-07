#ifndef EONLog_h
#define EONLog_h

#include "Arduino.h"
#include "HardwareSerial.h"

#include "Arduino.h"

// #define DISABLE_EON_LOGGING

typedef enum
{
    debug_xbee_rx=1,
    debug_xbee_tx=2,
    debug_dex=4,
    debug_gsm=8,
    debug_hardware=16,
    debug_prog_flow=32,
    debug_errors=64,
    debug_memory=128,
    debug_force=256,
    debug_xml=512
} DEBUGFLAGBITS;

class EONLog {
protected:

HardwareSerial *_serialPort;
char cTimeStampBuf[20];
bool g_bUartsInitialized;
unsigned short DebugFlags;
char *PrintBuffer = NULL;
char *bitbuffer;
int bitbufferindex;
bool g_forceOutputToPortA;
bool bTestModeRunning;

// The following vars keeep track of the queued debug output, i.e., debug
// output that is queued because the current port in in use.
char* pDebugLog;
char* pDebugLogPos;
int iDebugLogSize;
bool bDebugLogActive;

public:
  inline EONLog();
  void begin(HardwareSerial *serialport);
  void printfX(unsigned short iDebugLevel, const char *format, ...);
  unsigned short GetDebugFlags(void);
  void AddToDebugLog(char* pDebugInfo);
  void ActivateDebugLog(void);
  void DeActivateDebugLog(void);
};

extern EONLog Debug;
#define HAVE_DEBUG

#endif // whole file
