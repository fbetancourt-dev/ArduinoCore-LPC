#ifndef config_h
#define config_h

/*
** Build date is year-month-day
*/
#define FIRMWARE_BUILD_DATE "2015-10-13"
/*
** Version number is major.minor.buildnum
*/
#define FIRMWARE_VERSION "1.3.0.027"


/*
** The DEXBUFSIZE is to TOTAL memory allocated to
** DEX.  After reading dex on port A, whatever is left over
** in the dex buffer will be used to read on port B.
** Total size is limited to this value.
*/

#define COMPBUFSIZE	8192
#define DEXBUFSIZE	16384
#define XMITBUFSIZE	8192
#define WORKBUFSIZE	4096
#define ERRORBUFSIZE	1024
#define GPRSREPORTBUFSIZE 1024
#define TCP_CACHE_SIZE	1024
#define GSMRESPONSEBUFSIZE  4096
#define SERIALNUMSIZE	60
#define VA1SIZE		80
#define CA1SIZE		80
#define PASSWORDSIZE	15
#define PACKETCACHESIZE 512

#endif // whole file
