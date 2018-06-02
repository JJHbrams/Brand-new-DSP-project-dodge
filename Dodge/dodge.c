/*
                          <<<Brand-new DSP project>>>
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "Project dodge" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Start Date 	: 	2018.05~
Maker 			: 	201502273 Kim, Hyeonjong
								201502293 Jang, Jinhyeok
Build 1.1.0

*/
#include <lib.h>
#include <stlib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#include "tftlcd.h"
#include "random_table.h"

#define FROM_RAM 1
#define FROM_ROM 0

#define MAX_ENEMY 50
#define delta_t 0.01//position update time gap

#define recvSUCCESS 1
#define recvWAIT    2
#define recvFAIL    3
#define recvCONNECT 4
#define recvDETECT  5

#define TIMEOUT 60000

#define IO_GPIO112 (1<<16)

#define ADDR_OF_7SEG_DATA   0x14801000
#define ADDR_OF_7SEG_GRID   0x14800000

char bt_receive[100];
char bt_send[30];
int recvSTATUS;
char other_btaddress[12];
/****************************About bluetooth***********************************/
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
    printf("\n################ BLUETOOTH MENU #############\n");
    printf(" [1] AT Command \n");
    printf(" [2] ATZ Command\n");
		printf(" [3] Pairing\n");
		printf(" [4] Halt connection\n");
    printf(" [Q] quit\n");
    printf(" Press any key to excute menu \n");
    printf("###############################################\n");
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

int Pairing()
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
    bt_send[5]='S';
    bt_send[6]='C';
    bt_send[7]='A';
    bt_send[8]='N';
    bt_send[9]=0x0d;
    bt_send[10]='\0';//AT command : ATBTSCAN (대상이 스캔할 수 있도록 허용)
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
			    if((i>3)&(val==0x0a))	break;
			}
			msWait(1);
    }
    if(StrStr(bt_receive,"OK") != NULL) {
			printf("ATBTSCAN OK received!!!\n");
			return 1;
    }
    else{
			printf("ATBTSCAN retry\n");
    }
    for(i=0;i<12;i++)	other_btaddress[i]=bt_receive[i+2];
    if(ATBTCANCEL()==1) {
			printf("SCAN OK!!\n");
			return 1;
    }
    else return 0;
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
/******************************************************************************/
/***************************Objrcts initiation & update************************/
void enemy_init(object * Enem)
{
	//set hir_box size
	(Enem->hit_box)=1;
	//위치 지정
  (Enem->x)=rand()%PP_ROW;
  if((Enem->x)==0 || PP_ROW-1) (Enem->y)=rand()%PP_COL;//x가 베젤 양끝으로 배정된 경우
  else  (Enem->y)=(rand()%2)*(PP_COL-1);//그 외의 경우
  //속도지정(총 100단계,속도범위(-50~50))
  (Enem->vx)=table[GetTime()%300];
  (Enem->vy)=table[GetTime()%300];//time seed...(1초간격으로 시간 출력)
}

void enemy_update(object * Enem)
{
  //x=x0+vt
  (Enem->x)+=(Enem->vx)*delta_t;
  (Enem->y)+=(Enem->vy)*delta_t;
  //다시 베젤로 돌아간 경우
  if((Enem->x)==0||(Enem->x)==(PP_ROW-1)||(Enem->y)==0||(Enem->y)==(PP_COL-1)) enemy_init(Enem);
}

void player_init(object * player)
{
	//set hir_box size
	(player->hit_box)=5;
	//위치 지정 : initioal position = center
	(player->x)=LCD_WIDTH/2 - (player->hit_box)/2;
	(player->y)=LCD_HEIGHT/2 - (player->hit_box)/2;
  //속도지정 : initila velocity =(0,0)
	(player->vx)=0;
  (player->vy)=0;
}

void player_update(object * player)
{
  //x=x0+vt
  (player->x)+=(player->vx)*delta_t;
  (player->y)+=(player->vy)*delta_t;
}

/*******************************7segment***************************************/
int Getsegcode(int x)
{
	char code;
	switch (x) {
		case 0 : code = 0x3f; break;//0
		case 1 : code = 0x06; break;//1
		case 2 : code = 0x5b; break;//2
		case 3 : code = 0x4f; break;//3
		case 4 : code = 0x66; break;//4
		case 5 : code = 0x6d; break;//5
		case 6 : code = 0x7d; break;//6
		case 7 : code = 0x07; break;//7
		case 8 : code = 0x7f; break;//8
		case 9 : code = 0x6f; break;//9
		default  : code = 0;    break;
	}
	return code;
}

void display_7seg(int score)
{
	int i,j;

	data[5]=Getsegcode(score/100000);//십만의 자리
	temp1=count%100000;
	data[4]=Getsegcode(temp1/10000);//만의 자리
	temp2=temp1%10000;
	data[3]=Getsegcode(temp2/1000);//천의 자리
	temp1=temp2%1000;
	data[2]=Getsegcode(temp1/100);//백의 자리
	temp2=temp1%100;
	data[1]=Getsegcode(temp2/10);//십의 자리
	data[0]=Getsegcode(temp2%10);//일의 자리
	for(j=0;j<100;j++)
	{
		for(i=0;i<6;i++)
		{
			*seg_addr_grid = ~digit[i];//핀 활성화
			*seg_addr_data = data[i];//데이터 출력
			Wait(50);    // 50us delay
		}
	}
}
/*****************************************************************************/

void c_main(void)
{
  int i,j,k;
	int score=0;
  char ch;
	unsigned int temp1,temp2;
	char data[6];
	unsigned int digit[6] ={0x20,0x10,0x08,0x04,0x02,0x01};
	unsigned char * seg_addr_grid, * seg_addr_data;

  object Enemy[MAX_ENEMY];
  object Player;

	// clear sed
	seg_addr_grid = (unsigned char *)ADDR_OF_7SEG_GRID;
	seg_addr_data = (unsigned char *)ADDR_OF_7SEG_DATA;

	*seg_addr_grid = 0x00;
	*seg_addr_data = 0x00;

	InitTimer();
	bluetooth_init();
	bluetooth_message();

  //printf("\nDrawing ENEMY \n");
  //Enemy 생성 (10개만) & Plyaer 생성
  for(i=0;i<10;i++) enemy_init(&Enemy[i]);
	player_init(&Player);

  while(1)
  {
		//display enemys
		for(i=0;i<10;i++)
    {
      object_Display(&Enemy[i]);
      enemy_update(&Enemy[i]);
    }
		//display player
		object_Display(&Player);
		//player_update(&Enemy[i]);

		//About bluetooth commands
		if(GetChar(&ch) ==1)
		{
			switch(ch) {
				case '1':
								AT();
								break;
				case '2':
								ATZ();
								break;
				case '3':
								Pairing();
								break;
				case '4':
								clear_sendbuf();
								BTPutChar("@#x y b&*");
								break;
				default:
								break;
			}
			bluetooth_message();
		}
		//Display present time - Terminal
		score=GetTime();
		printf("%d\n", score);
		//Display present time - 7-segment
		display_7seg(score);

    if(GetChar(&ch) == 1) if (ch == 'Q' || ch == 'q') break;
  }
	// clear sed
	*seg_addr_grid = 0x00;
	*seg_addr_data = 0x00;
  return;
}
