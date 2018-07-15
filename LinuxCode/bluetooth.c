#include <serial.h>
#include <lib.h>
#include <reg.h>
#include <time.h>

#define recvSUCCESS 1
#define recvWAIT    2
#define recvFAIL    3
#define recvCONNECT 4
#define recvDETECT  5

#define TIMEOUT 60000

void clear_receivebuf();
void clear_sendbuf();
char * StrStr (char * s, char * find);

int AT();
int ATBTCANCEL();
int ATBTINQ();
int ATZ();
int ATBTCSD();


char bt_receive[100];
char bt_send[30];
int recvSTATUS;
char other_btaddress[12];


#define IO_GPIO112 (1<<16)

void bluetooth_init(void)
{

    SerialInit_BT9600();

    // Bluetooth Mode Settings
	*(unsigned int *)GPIO112_MFPR = 0xC800;
    __REG(GPIO_BASE + GPDR3) |= IO_GPIO112;
    printf("Bluetooth Module Communication Mode **\n");
    __REG(GPIO_BASE + GPCR3) |= IO_GPIO112;

}


void bluetooth_message(void)
{
    printf("\n################ BLUETOOTH TEST MENU #############\n");
    printf(" [1] AT Command \n");
    printf(" [2] ATZ Command\n");
    printf(" [Q] quit\n");
    printf(" Press any key to excute menu \n");
    printf("###############################################\n");
}


void c_main(void)
{
	char ch;
	bluetooth_init();
	bluetooth_message();

	while(1) {
		
		if(GetChar(&ch) ==1) 
		{
			switch(ch) {
				case '1':
					AT();
					break;
				case '2':
					ATZ();
					break;
				default:
					break;
			}
			bluetooth_message();			
		}

		if (ch == 'Q') break;
		if (ch == 'q') break;

	}
}



void clear_receivebuf()
{
    int i;
    for(i=0;i<100;i++)bt_receive[i]=0;
}

void clear_sendbuf()
{
    int i;
    for(i=0;i<30;i++)bt_send[i]=0;
}

char *StrStr(char * s, char * find)
{
    char c, sc;
    int len;

    if ((c = * find ++) != 0)
    {
	len = StrLen (find);
	do
	{
	    do
	    {
		if ((sc = * s ++) == 0) return (NULL);
	    }
	    while (sc != c);
	}
	while (StrNCmp (s, find, len) != 0);
	s --;
    }
    return ((char *) s);
}

int AT()
{
    int i,delaycnt,delay;
    char val;

    printf("AT Command Test\n");
    clear_sendbuf();
retry_btat:

    bt_send[0]='A';
    bt_send[1]='T';
    bt_send[2]=0x0d;
    bt_send[3]='\0';

    recvSTATUS = recvWAIT;
	
    for(i=0;i<3;i++)
	BTPutChar(bt_send[i]);
	
    printf("at command sent!!!\n");
	
    i=0;
    delaycnt=0;
    clear_receivebuf();
	
    while(1) {
		if(BTGetChar(&val)==1) 
		{			
		  bt_receive[i++]=val;	
		  if((i>3)&(val==0x0a)) 	break;
		  delaycnt=0;
		}

		for(delay=0;delay<100;delay++);
		delaycnt++;
		if(delaycnt>TIMEOUT)return 0;
    }
	
    if(StrStr(bt_receive,"OK")!=NULL) {
	printf("at OK received!!!\n");
	return 1;
    }
    else{
	printf("at+btcancel retry\n");
	goto retry_btat;
    }
	
}


int ATBTINQ()
{
    int i,delaycnt;
    char val;

    clear_sendbuf();
    __REG(BTUART_BASE + BTFCR) |= 0x00000002; // Reset tx, rx FIFO. clear. FIFO enable

    bt_send[0]='A';
    bt_send[1]='T';
    bt_send[2]='+';
    bt_send[3]='B';
    bt_send[4]='T';
    bt_send[5]='I';
    bt_send[6]='N';
    bt_send[7]='Q';
    bt_send[8]='?';
    bt_send[9]=0x0d;
    bt_send[10]='\0';
    recvSTATUS = recvWAIT;
    for(i=0;i<10;i++)
	BTPutChar(bt_send[i]);
    i=0;
    delaycnt=0;
    msWait(10);
    clear_receivebuf();
    while(1) {
	if(BTGetChar(&val)==1) {
	    delaycnt=0;
	    bt_receive[i++]=val;
	    if((i>3)&(val==0x0a))break;
	}
	msWait(1);

    }
    if(StrStr(bt_receive,"OK") != NULL) {
	printf("at+btinq OK received!!!\n");
	return 1;
    }
    else{
	printf("at+btinq retry\n");
    }
    for(i=0;i<12;i++)other_btaddress[i]=bt_receive[i+2];
    if(ATBTCANCEL()==1) {
	printf("BTINQ OK!!\n");
	return 1;
    }
    else return 0;
}



int ATZ()
{
    int i,delaycnt;
    char val;

    __REG(BTUART_BASE + BTFCR) |= 0x00000002; // Reset tx, rx FIFO. clear. FIFO enable
    clear_sendbuf();

retry_atz:
    bt_send[0]='A';
    bt_send[1]='T';
    bt_send[2]='Z';
    bt_send[3]=0x0d;
    bt_send[4]='\0';
    recvSTATUS = recvWAIT;
    for(i=0;i<4;i++)
	BTPutChar(bt_send[i]);
    i=0;
    delaycnt=0;
    clear_receivebuf();
    msWait(1);
    while(1){
	if(BTGetChar(&val)==1){
	    delaycnt=0;
	    bt_receive[i++]=val;
	    if((i>3)&(val==0x0a))break;
	}
	msWait(1);
    }
    if(StrStr(bt_receive,"OK") != NULL) {
	printf("bt atz OK received!!");	
	return 1;
    }
    else{
	printf("atzl retry\n");
	goto retry_atz;
    }
}

int ATBTCSD()
{
    int i,delaycnt;
    char val;

    clear_sendbuf();
//    __REG(BTUART_BASE + BTFCR) |= 0x00000002; // Reset tx, rx FIFO. clear. FIFO enable

retry_btcsd:
    bt_send[0]='A';
    bt_send[1]='T';
    bt_send[2]='+';
    bt_send[3]='B';
    bt_send[4]='T';
    bt_send[5]='C';
    bt_send[6]='S';
    bt_send[7]='D';
    bt_send[8]=0x0d;
    bt_send[9]='\0';
    recvSTATUS = recvWAIT;
	
	for(i=0;i<9;i++)
		BTPutChar(bt_send[i]);

	i=0;
	delaycnt=0;
	msWait(1);
	clear_receivebuf();

	while(1){
		if(BTGetChar(&val)==1){
			delaycnt=0;
			bt_receive[i++]=val;
			printf("%c",val);

			if((i>3)&(val==0x0a))break;
		}

		msWait(1);
	}

	if(StrStr(bt_receive,"OK") != NULL) {
		printf("btcsd OK received!!");	
		return 1;
	}
	else{
		printf("btcsd retry\n");
		goto retry_btcsd;
    }
}



int ATBTCANCEL()
{
	int i,delaycnt;
	char val;

	clear_sendbuf();

retry_btcancel:
	bt_send[0]='A';
	bt_send[1]='T';
	bt_send[2]='+';
	bt_send[3]='B';
	bt_send[4]='T';
	bt_send[5]='C';
	bt_send[6]='A';
	bt_send[7]='N';
	bt_send[8]='C';
	bt_send[9]='E';
	bt_send[10]='L';
	bt_send[11]=0x0d;
	bt_send[12]='\0';
	recvSTATUS = recvWAIT;
	for(i=0;i<12;i++)
		BTPutChar(bt_send[i]);
	i=0;
	printf("at+btcancel sent!!!\n");
	delaycnt=0;
	clear_receivebuf();
	while(1) {
		if(BTGetChar(&val) == 1) {
			delaycnt=0;
			bt_receive[i++]=val;
			if((i>3)&(val==0x0a))break;
		}
		msWait(1);
	}

	if(StrStr(bt_receive,"OK") != NULL) {	
		printf("at+btcancel OK received!!!\n");
		return 1;
	} else {
		printf("at+btcancel retry\n");
		goto retry_btcancel;
	}
}


int checkwall;
/////////////////////////////////////////////
// Bluetooth Baud rate chage Command send...
// Module Original speed : 9600 to Command
//   "AT+UARTCONFIG,38400,N,1"
//
int BRM_rate_Change_(void)
{
    int promiOK;
    int connectOK;

    SerialInit_BT9600();
    checkwall = 0;
    connectOK=0;
    promiOK=0;

    msWait(1);

    if(AT()){
	promiOK=1;
    }

    ///////////////////////////
    //ATUARTCONFIG();
    //ATZ_NULL();

    return 0;
}

int response_read()
{
    char val,ch;

    printf("Press 'q' or 'Q' Key to exit...\n");

    while (1) {
	if (BTGetChar(&val)==1){
	    printf("%c",val);
	    return 0;
	}

	if (GetChar(&ch) ==1) {
	    if (ch == 'Q') break;
	    if (ch == 'q') break;
	}
    }
    return 0;
}


