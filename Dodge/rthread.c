#include "chat.h"
extern int recvOK;
extern int recvSTATUS;
extern char recvbuf[255];
extern char lbtinfo[50];
extern char obtinfo[10][50];
extern char howmanybt;
extern char btcommand;
extern char connectionOK;
extern int dev_state;

int g_recvnic;
int g_quit;

void *receive_thread(void *arg)
{
	int recv_fd;
	int *arg_fd;
	fd_set readfs;
	int sel;
	struct timeval timeout;
	int len;
	int i;
#if debug == 1
	printf("thread started!!!\n");
#endif
	arg_fd = ( int * )arg;
	recv_fd = (int)arg_fd;
#if debug == 1
	printf("recv_fd = %d\n",recv_fd);
#endif
	FD_ZERO(&readfs);

	timeout.tv_sec = 0;
	timeout.tv_usec = 5000;

	while(recvOK==TRUE){
		FD_SET(recv_fd, &readfs);
		if ((sel = select(recv_fd + 1, &readfs, NULL, NULL, &timeout)) > 0) {

			if (FD_ISSET(recv_fd,&readfs)) {
				memset(recvbuf, 0, sizeof(recvbuf));
				len = read(recv_fd, recvbuf, sizeof(recvbuf));
#if debug == 1
				for(i=0;i<len;i++){
					printf("0x%2x, ",recvbuf[i]);
					if(i%12 ==0)printf("\n");
				}
				printf("\n");
				printf("%dbyte receive!!\n",len);
				recvbuf[len]='\0';
				if(len>0)printf("%s",recvbuf);
#endif
				if((btcommand==ATBTINFO)&&(len>4))memcpy(lbtinfo,recvbuf,len);
				else if((btcommand==ATBTINQ)&&(len>4)){
					memcpy(&obtinfo[howmanybt][1],recvbuf,len);
					obtinfo[howmanybt][0]=len;
					obtinfo[howmanybt][len]='\0';
					howmanybt++;
					recvSTATUS = recvDETECT;
				}

				if(strstr(recvbuf, "OK") != NULL)recvSTATUS = recvSUCCESS;
				if(strstr(recvbuf, "ERROR") != NULL)recvSTATUS = recvFAIL;
				if(strstr(recvbuf, "CONNECT") != NULL)recvSTATUS = recvCONNECT;
				if(connectionOK==TRUE){
					recvbuf[len]='\0';
					if(g_recvnic);
						printf("%s\n",recvbuf);
				}
				g_recvnic = 1;
			}
		}
		timeout.tv_sec = 0;
		timeout.tv_usec = 5000;
	}
#if debug == 1
	printf("thread ended!!!\n");
	sleep(1);
#endif
}

void ch_channel(void)
{
        int fd;
        void *baseaddr;
        unsigned long *gpdr3;
        unsigned long *gpsr3;
        unsigned long *gpcr3;

        fd = open( "/dev/mem", O_RDWR|O_SYNC );
        if( fd < 0 )
        {
                perror( "/dev/mem open error" );
                exit(1);
        }

        baseaddr = mmap( 0,IO_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED,fd,IO_BASE_ADDR );

        if( baseaddr != NULL ) {
                gpdr3 = (unsigned long *) ( (unsigned char *)baseaddr + IO_GPDR3_OFFSET );
                gpsr3 = (unsigned long *) ( (unsigned char *)baseaddr + IO_GPSR3_OFFSET );
                gpcr3 = (unsigned long *) ( (unsigned char *)baseaddr + IO_GPCR3_OFFSET );
        } else {
		printf("mmap error \n");
	}
                *gpdr3 |= IO_GPIO112;
                *gpcr3 |= IO_GPIO112;

        munmap(baseaddr, IO_SIZE);
        close(fd);
}
