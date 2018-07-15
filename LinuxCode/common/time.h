#ifndef __TIME_H__
#define __TIME_H__

#include <types.h>

#if defined(HBE_XM_DVK)
void InitTimer();
#endif

u32  GetTime();
void msleep(unsigned int msec);

void Wait(unsigned int microSeconds);
void msWait(UINT32 msVal) ;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;


#if defined(HBE_XM_Bulverde) || (HBE_SM2)
#define CLOCKS_PER_SEC			(3686400)	// for clock()

extern bool time_init(void);
extern time_t time(time_t *t);
extern clock_t clock(void);
bool InitTimer();
#endif

#endif
