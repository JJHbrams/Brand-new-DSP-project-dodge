#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1
#define FALSE 0
#define TRUE 1
#define recvSUCCESS		1
#define recvWAIT		2
#define recvFAIL		3
#define recvCONNECT	        4
#define recvDETECT		5

#define AT			1
#define ATZ			2
#define ATF			3
#define ATBTINQ		        4
#define ATD			5
#define ATBTSCAN		6
#define ATBTCANCEL	        7
#define ATPLUS			8
#define ATO			9
#define ATH			10
#define ATBTAUTH		11
#define ATBTSEC		        12
#define ATBTLAST		13
#define ATBTMODE		14
#define ATBTNAME		15
#define ATBTKEY		        16
#define ATBTINFO		17
#define ATBTLPM		        18
#define ATV			19
#define ATBTSD			20
#define ATBTCSD		        21
#define ATBTFP			22

typedef struct{
	char btaddress[13];
	char btfname[33];
	char btcod[7];
}btdevice;

#define IO_BASE_ADDR 0x40E00000
#define IO_SIZE 0x00000050

#define IO_GPDR3_ADDR 0x40E0010C
#define IO_GPSR3_ADDR 0x40E00118
#define IO_GPCR3_ADDR 0x40E00124

#define IO_GPDR3_OFFSET ( IO_GPDR3_ADDR - IO_BASE_ADDR )
#define IO_GPSR3_OFFSET ( IO_GPSR3_ADDR - IO_BASE_ADDR )
#define IO_GPCR3_OFFSET ( IO_GPCR3_ADDR - IO_BASE_ADDR )

#define IO_GPIO112 (1<<16)

void *receive_thread(void *arg);
