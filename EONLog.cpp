#include "EONLog.h"

CTL_MUTEX_t mutexDebugOutput;

EONLog::EONLog() : g_forceOutputToPortA(false), g_bUartsInitialized(false), bTestModeRunning(false)
{

}

void EONLog::begin(HardwareSerial * serialport)
{
  this->_serialPort = serialport;
  this->g_bUartsInitialized = true;
}

void EONLog::printfX(unsigned short iDebugLevel, const char *format, ...)
{
    if (!g_bUartsInitialized) return;

    unsigned uLock;
    uint32_t curtime;
    uint32_t hr;
    uint32_t minute;
    uint32_t second;
    uLock = ctl_mutex_lock(&mutexDebugOutput, CTL_TIMEOUT_NONE, 0);

    va_list args;
    va_start(args, format);
    //if (DebugFlags & iDebugLevel) {
      if (g_bUartsInitialized && (PrintBuffer != NULL)) {
        vsprintf(PrintBuffer, format, args);
        // Now, throw this out the debug port..
        // Add a timestamp..
        curtime=millis();
        hr=curtime/3600000;
        minute=(curtime%3600000) / 60000;
        second=(curtime%60000)/1000;
        sprintf(cTimeStampBuf, "%u:%02u:%02u.%03u: ", hr,minute,second,curtime%1000);
#ifdef BOOT_LOADER_CODE
        if (g_forceOutputToPortA || g_bUpdateRequestedByUser || bTestModeRunning) {
          if (bDebugLogActive) {
              AddToDebugLog(cTimeStampBuf);
              AddToDebugLog(PrintBuffer);
          } else {
              this->begin(&Serial2);
              this->_serialPort->queue_uart(cTimeStampBuf, strlen(cTimeStampBuf), NULL);
              this->_serialPort->queue_uart(PrintBuffer, strlen(PrintBuffer), NULL);
          }
#else
        if (g_forceOutputToPortA || bTestModeRunning) {
          if (bDebugLogActive) {
              AddToDebugLog(cTimeStampBuf);
              AddToDebugLog(PrintBuffer);
          } else {
              this->begin(&Serial2);
              this->_serialPort->queue_uart((unsigned char*)cTimeStampBuf, strlen(cTimeStampBuf), NULL);
              this->_serialPort->queue_uart((unsigned char*)PrintBuffer, strlen(PrintBuffer), NULL);

          }
#endif
        } else {
          #if DEBUG && 0
            debug_puts(PrintBuffer);
          #else
            this->begin(&Serial3);
            this->_serialPort->queue_uart((unsigned char*)cTimeStampBuf, strlen(cTimeStampBuf), NULL);
            this->_serialPort->queue_uart((unsigned char*)PrintBuffer, strlen(PrintBuffer), NULL);
          #endif
        }
      }
    //}
    va_end(args);

    ctl_mutex_unlock(&mutexDebugOutput);
}

void EONLog::AddToDebugLog(char* pDebugInfo)
{
    int iLen = strlen(pDebugInfo);

    // Make sure we don't write past the end of our allotted space..
    if ((iDebugLogSize + iLen) >= COMPBUFSIZE) {
        memset (bitbuffer, 0, COMPBUFSIZE);
        pDebugLog = bitbuffer;
        pDebugLogPos = bitbuffer;
        iDebugLogSize = 0;
    }

    strcpy(pDebugLogPos, pDebugInfo);
    pDebugLogPos += iLen;
    iDebugLogSize += iLen;
}

void EONLog::ActivateDebugLog(void)
{
    unsigned uLock;

    uLock = ctl_mutex_lock(&mutexDebugOutput, CTL_TIMEOUT_NONE, 0);

    memset (bitbuffer, 0, COMPBUFSIZE);
    pDebugLog = bitbuffer;
    pDebugLogPos = bitbuffer;
    iDebugLogSize = 0;
    bDebugLogActive = true;
    
    ctl_mutex_unlock(&mutexDebugOutput);
}

void EONLog::DeActivateDebugLog(void)
{
    unsigned uLock;

    uLock = ctl_mutex_lock(&mutexDebugOutput, CTL_TIMEOUT_NONE, 0);

    bDebugLogActive = false;
    
    ctl_mutex_unlock(&mutexDebugOutput);
}

EONLog Debug;