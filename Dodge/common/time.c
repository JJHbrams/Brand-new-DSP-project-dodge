////////////////////////////////////////////////////////////////////////////////////////////////////
// 설명 : 아직 타이머에 관한 본 파일 함수들이 완전하게 구현되어 있지 않다.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <config.h>
#include <reg.h>
#include <time.h>
#include <types.h>

#if defined(HBE_XM_DVK)
void InitTimer(void)
{
//	unsigned long old_rcnr = RCNR;

	/* Force 32-kHz oscillator on */
//	PCFR |= PCFR_FO;

	/* disable timer interrupts and clear the AL and HZ bits*/
//	RTSR = (RTSR_AL | RTSR_HZ);

	/* put the counter to 0 */
	/* strange enough, this doesn't seem to work -- Erik */
	/* RCNR = 0x0; */
	/* RCNR writes may be delayed by a 32-kHz clock cycle */

//	RCNR = 0x0;
//	while (RCNR > old_rcnr)
//	;
}
#endif

#if defined(HBE_XM_Bulverde) || (HBE_SM2)
static unsigned long msecond_boundary;
static int udelay_init(void);

bool time_init(void)
{
	udelay_init();
	RCNR = 0;
	OSCR = 0;
	return true;
}

bool InitTimer(void)
{
	udelay_init();
	RCNR = 0;
	OSCR = 0;
	return true;
}

time_t time(time_t *t){
	*t = RCNR;		// 1 Hz
	return *t;
}

clock_t clock(void){
	return OSCR;	// 3.6864 MHz
}

/*
 * 100000 : a / 3686400 s = x : 1 / 1000 s
 * x = 3686400 * 100000 / (1000 * a)
 * x = 368640000 / a
 */
static int udelay_init(void){
	unsigned long i, diff;

	OSCR = 0;
	for (i=0; i < 100000; i++);
	diff = OSCR;
	msecond_boundary = 368640000 / diff;
	//printf("udelay : msecond_boundary : %d [0x%08lx]\n", msecond_boundary, msecond_boundary);
	return 0;
}
#endif

u32 GetTime()
{
	return((u32) RCNR);
}

void msleep(unsigned int msec)
{

	unsigned long startTime=0, currentTime=0;
	startTime = GetTime();
	while(1) {
		currentTime = GetTime();
	
		if((currentTime - startTime) > msec) {
			break;
		}
	}

}

// uS Delay
void Wait(unsigned int microSeconds)
{
    UINT32 Value, Time;

    Time   = OSCR;
    Value = Time + (microSeconds * 4);
    if (Value < Time)
    {  // test for wrap.
        while (Time < OSCR);
    }
    while (OSCR <= Value);
}

// mS delay
void msWait(UINT32 msVal) 
{
    Wait(msVal*1000);
}


