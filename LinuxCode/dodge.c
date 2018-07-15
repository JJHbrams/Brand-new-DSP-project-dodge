/*
                          <<<Brand-new DSP project>>>
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "Project dodge" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Start Date 	: 	2018.05~
Maker 			: 	201502273 Kim, Hyeonjong
								201502293 Jang, Jinhyeok
Build 2.2.7
*/
#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#include "tftlcd.h"
#include "random_table.h"

#define FROM_RAM 1
#define FROM_ROM 0

#define MAX_ENEMY 50//최대 적의 수
#define delta_t 0.3//position update time gap

#define recvSUCCESS 1
#define recvWAIT    2
#define recvFAIL    3
#define recvCONNECT 4
#define recvDETECT  5

#define TIMEOUT 60000

#define IO_GPIO112 (1<<16)

#define ADDR_OF_7SEG_DATA   0x14801000
#define ADDR_OF_7SEG_GRID   0x14800000 //7세그먼트 주소 지정

char bt_receive[100];//수신값 임시 저장 변수
char bt_send[30];//송신 값 임시 저장 변수
int recvSTATUS;
char other_btaddress[12];
unsigned char * seg_addr_grid, * seg_addr_data; //7세그먼트 주소 지정 번수

int randomfactor = 0; // time seed와 같은 역할을 수행하는 인자

/****************************About bluetooth***********************************/
void bluetooth_init(void)
{
  //블루투스 연결 초기화
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
	printf(" [p] Pairing\n");
    	printf(" [Q] quit\n");
    	printf(" Press any key to excute menu \n");
    	printf("###############################################\n");
	//printf("%d\n", -50);
}

void clear_receivebuf()
{
    //수신 버퍼 비움
    int i;
    for(i=0;i<100;i++)bt_receive[i]=0;
}

void clear_sendbuf()
{
    //송신버퍼 비움
    int i;
    for(i=0;i<30;i++)bt_send[i]=0;
}

char *StrStr(char * s, char * find)
{
    // 해당 문자열에 원하는 문자열이 존재하는지 판단
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
    // At command : AT
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
	BTPutChar(bt_send[i]); // AT 송신

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
    // OK가 수신된 경우
    if(StrStr(bt_receive,"OK")!=NULL) {
	     printf("at OK received!!!\n");
	      return 1;
    }
    // OK가 수신되지 못한 경우
    else{
	     printf("at+btcancel retry\n");
	      goto retry_btat;
    }

}

int ATBTINQ()
{
    // AT command : ATBTINQ
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
    // AT command ATZ
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
    // AT command
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
  // AT command : ATBTCANCEL
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

int Pairing()
{
    // Pairing할 준비
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
    for(i=0;i<10;i++) BTPutChar(bt_send[i]);
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
void enemy_init(object * Enem, unsigned char init, int index)
{
  // Initiate enemy objects
  // *Enem  : 대상 오브젝트
  // init   : 최초 초기화인지(init=1) 베젤에 나가서 다시 지정하는 것인지(init=0) 판별
  // index  : 대상 오브젝트의 인데스, 몇번째 적인지 지시
	int i;

	//set hit_box size
	(Enem->hit_box)=5;

	//initial 위치 지정
	if(init)
	{
    (Enem->x)=table[index]%PP_ROW;
  	if(((Enem->x)==0) || ((Enem->x)==PP_ROW-1)) (Enem->y)=table[index]%PP_COL;//x가 베젤 양끝으로 배정된 경우
  	else  (Enem->y)=(table[index]%2)*(PP_COL-1);//그 외의 경우
		 //속도지정(총 60단계,속도범위(0~59))
  	(Enem->vx)=table[index]%60;
  	(Enem->vy)=table[300-index]%60;
	}

	//post position
	else
	{
		i=(index + randomfactor)%300; //srand()의 time seed역할을 대신해줌, randomfactor는 플레이어의 속도에 관련있음
		randomfactor+=3;
		(Enem->x)=table[i]%PP_ROW; //x축 위치 재지정
  	if((Enem->x)==0 || (Enem->x)==PP_ROW-1) (Enem->y)=table[i]%PP_COL;//x가 베젤 양끝으로 배정된 경우
  	else  (Enem->y)=(table[i]%2)*(PP_COL-1);//그 외의 경우
		//속도지정
  	(Enem->vx)=table[i]%60;
  	(Enem->vy)=table[300-i]%60;
	}
}

void enemy_update(object * Enem, int index)
{
  //적의 위치 업데이트
	unsigned char x_dir=0;
	unsigned char y_dir=0;//0:positive, 1:negative

  //x=x0+vt
  //최고속도기 60이므로 중간값인 30을 기준으로 30보다 크면 양의 속도로 판단 : 0~30
  //                                             작으면 음의속도로 판단 : -30~0
	//vx direction
	if(Enem->vx < 30) x_dir=1;
	if(x_dir)	(Enem->x)-=(int)((30 - (Enem->vx))*delta_t);
	else		(Enem->x)+=(int)(((Enem->vx)-30)*delta_t);
	//vy direction
	if(Enem->vy < 30) y_dir=1;
	if(y_dir)	(Enem->y)-=(int)((30 - (Enem->vy))*delta_t);
	else		(Enem->y)+=(int)(((Enem->vy)-30)*delta_t);

  //다시 베젤로 돌아간 경우
  if((Enem->x)<=0||(Enem->x)>=(PP_ROW-1)||(Enem->y)<=0||(Enem->y)>=(PP_COL-1)) enemy_init(Enem,0,index);
}

void player_init(object * player)
{
  //플레이어 위치 초기화, 화면의 정중앙에 배치
	//set hir_box size
	(player->hit_box)=15;//5x5 hit box
	//위치 지정 : initioal position = center
	(player->x)=LCD_WIDTH/2 - (player->hit_box)/2;
	(player->y)=LCD_HEIGHT/2 - (player->hit_box)/2;
  //속도지정 : initila velocity =(0,0)
	(player->vx)=0;
  (player->vy)=0;
}

void player_update(object * player)
{
  int i=0;
  unsigned char flag=0;
	unsigned char x_dir=0;
	unsigned char y_dir=0;//0:positive, 1:negative
	unsigned char val=0;
	unsigned char delaycnt=0;
  //Update velocity data
  //data format : @#x00y00b&*, 11자리
  clear_receivebuf();
	while(BTGetChar(&val)==1)
  {
    //블루투스로부터 값이 전송된 경우
    delaycnt=0;
		i++;
		//printf("%c",val);
    if(val=='@')
    {
      //전송된 값이 패킷의 처음값인 @인 경우 인덱스를 1로 바꾸고 flag를 set
      i=1;
  		flag=1;
		}
		else if(val=='#')
		{
      //만약을 대비하여 패킷의 두번째 값인 #인 경우 인덱스를 2로 바꾸고 flag를 set
  		i=2;
  		flag=1;
		}
		if(flag)
		{
      //flag가 set된 경우 전송된 값을 임시 변수에 저장하고 패킷의 끝인 *가 전송되면 루프 탈출
  		bt_receive[i]=val;
  		if((i==11)&(val=='*'))  break;
		}
  }
  //x=x0+vt
	if(flag == 1){
    //블루투스로부터 유의미한 값이 전송된 경우
    //ASCII코드이므로 '0'에 해당하는 48을 빼줌
    //패킷의 구성상 @#x00y00b&* 의 4번째 데이터는 vy의 십의자리, 5번쨰 데이터는 vy의 일의자리를 나타냄
    //                           7번째 데이터는 vx의 십의자리, 8번쨰 데이터는 vx의 일의 자리를 나타냄
    player->vy = 10*(bt_receive[4]-48)+(bt_receive[5]-48);
  	player->vx = 10*(bt_receive[7]-48)+(bt_receive[8]-48);
		randomfactor++;// 랜덤팩터를 증가시킴
	}
	else{
    player->vx = 50;
  	player->vy = 50;
	}

	//vx direction
  // 플레이터의 속도는 0~99의 데이터로 이뤄짐
  // 50보다 큰 값이면 양의 속도로 판단 : 0~50
  // 50보다 작은 값이면 음의 속도로 판단 : -50~0
	if(player->vx < 50) x_dir=1;
	if(x_dir)	(player->x)-=(int)((50 - (player->vx))*delta_t);
	else		(player->x)+=(int)(((player->vx)-50)*delta_t);
	//vy direction
	if(player->vy < 50) y_dir=1;
	if(y_dir)	(player->y)-=(int)((50 - (player->vy))*delta_t);
	else		(player->y)+=(int)(((player->vy)-50)*delta_t);
	//position boundary
	if((player->x) < 0) player->x = 0;
	if((player)->y < 0) player->y = 0;
	if((player->x+ player->hit_box) >= PP_ROW) (player->x) = PP_ROW-1-player->hit_box;
	if((player->y+ player->hit_box) >= PP_COL) (player->y) = PP_COL-1-player->hit_box;
}


/*****************************About Gamover************************************/
void impact(unsigned char * flag, int num, object * enem, object *player)
{
  //충격 판단
	int i,j,k;
  int impact_data=0;
	int px = player->x;
	int py = player->y;
	int pvx = player->vx;
	int pvy = player->vy;
	int hb = player->hit_box;
	int pdirx = 0, pdiry = 0, edirx = 0, ediry = 0;
	int ipx, ipy, iex, iey;
	char ext = 3;//히트박스의 크기를 약간 확장

	for(i = 0; i < num; i++){//적의 인덱스를 늘려가며 충돌 판단
		if((px-ext <= enem[i].x) && (enem[i].x+ext) <= ((px) + hb)){
			if((py-ext) <= (enem[i].y) && (enem[i].y+ext) <= ((py) + hb)){
        //플레이어의 x위치와 적의 x위치를 비교하여 서로 겹친 경우를 충돌이라고 판단, flag set
				*flag = 1;
				/*Player*/
        //앞선 위치 업데이트와 같이 플레이어의 속도가 50보다 크면 양의 속도로판단
        //50보다 작으면 음의 속도로 판단
				if(pvx < 50) pdirx = 1;
				if(pdirx) ipx = 50 - pvx;
				else ipx = pvx - 50;

				if(pvy < 50) pdiry = 1;
				if(pdiry) ipy = 50 - pvx;
				else ipy = pvy - 50;

				/*enemy*/
        //앞선 위치 업데이트와 같이 적의 속도가 30보다 크면 양의 속도로판단
        //30보다 작으면 음의 속도로 판단
				if(enem[i].vx < 30) edirx = 1;
				if(edirx) iex = 30 - enem[i].vx;
				else iex = enem[i].vx - 30;

				if(enem[i].vy < 30) ediry = 1;
				if(ediry) iey = 30 - enem[i].vy;
				else iey = enem[i].vy - 30;

				/*Impact!*/
				impact_data = (iex+ipx) + (iey+ipy);//임팩트 계산
				impact_data /= 22;
				impact_data %= 10;//임팩트가 1~9가 되도록 조정
			}
		}
	}

	if(*flag)
	{
    // 충격량 전송
		// @#i(impact_data)b&*
		clear_sendbuf();
		bt_send[0]='@';
		bt_send[1]='#';
		bt_send[2]='i';
		bt_send[3]=impact_data + 48;
		bt_send[4]='b';
		bt_send[5]='&';
		bt_send[6]='*';
		bt_send[7]='\n';
		bt_send[8]='\r';
		bt_send[9]='\0';
		recvSTATUS = recvWAIT;
		BTPutString(bt_send);
	}
}

/******************************************************************************/
/*******************************7segment***************************************/
int Getsegcode(int x)
{
  //7세그먼트 데이터 반환
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

void display_7seg(int count)
{
  //7 세그먼트를 켜는 함수
	int i,j;
	unsigned int temp1,temp2;
	char data[6];
	unsigned int digit[6] ={0x20,0x10,0x08,0x04,0x02,0x01};

	data[5]=Getsegcode(count/100000);//십만의 자리
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
			Wait(100);    // 50us delay
		}
	}
}
/*****************************************************************************/

void c_main(void)
{
  int i,j,k;
	int score, init_score, old_score;//Score is play-time in 'seconds'
	int E_num;//Number of enemy, spawned
	unsigned char GAME_OVER=0;//game-over flag : game-over=1
	unsigned char connect_flag=0;//Blutooth connection flag : connected=1
  char ch;
	unsigned char val;

  object Enemy[MAX_ENEMY];
  object Player;

/*****************************Bluetooth settings*******************************/
	bluetooth_init();//initiate bluetooth
	bluetooth_message();

	//About bluetooth commands
	//bluetooth_message();
	while(!connect_flag)
	{
    // ATBTSCAN이 정상적으로 작동한 경우 루프를 탈출 & 페어링할 준비를 마침
		if(GetChar(&ch) ==1)
		{
			printf("%c\n", ch);

			switch(ch) {
				case '1':
								AT();
								break;
				case '2':
								ATZ();
								break;
				case 'p':
								connect_flag=Pairing();
								printf("Ready to pair");
								break;
				case 'P':
								connect_flag=Pairing();
								printf("Ready to pair");
								break;

				default:
								break;
			}
			bluetooth_message();
		}
		if(GetChar(&ch) == 1) if (ch == 'Q' || ch == 'q') goto END_GAME;
	}
/******************************Game settings***********************************/
replay:
	// clear 7-segment
	seg_addr_grid = (unsigned char *)ADDR_OF_7SEG_GRID;
	seg_addr_data = (unsigned char *)ADDR_OF_7SEG_DATA;//7 세그먼트의 주소를 지정할 변수 지정
  	// clear score board
	*seg_addr_grid = 0x00;
	*seg_addr_data = 0x00;//7세그먼트 초기화

	//Enemy 생성 & Plyaer 생성
  for(i=0;i<MAX_ENEMY;i++)
    enemy_init(&Enemy[i],1,i);

	player_init(&Player);
  //점수 초기화
	score=0;
	old_score = score;
  // 적의 수 초기화
	E_num=10;

  // 초기화면 검은색
  Glcd_Drew_Box(0,0,LCD_HEIGHT-1,LCD_WIDTH-1,_BLACK);

  // 게임오버 flag 초기화
	GAME_OVER = 0;

	while(BTGetChar(&val) == 1);
	clear_receivebuf();//수신 버퍼 초기화
  // START 신호 감지
	while (1)
	{
		if(BTGetChar(&val) == 1){
      // 앱에서 START를 누르기 전까지는 게임 실행x
      if(val == '@') break;
		}
 		msWait(10);
	}

  // 카운트다운 이미지 출력 '3' -> '2' ->  '1'
  for(i=2;i>=0;i--){
	   for(j=0;j<60;j++){
	      countdown_display(i);
	     }
  }

  // 점수 보정을 위한 게임 시작전 보정 상수
	init_score = GetTime();

  while(!GAME_OVER)
  {
    // 배경이미지 초기화
	  Background_Display_Init();
    //display & update enemys
    for(i=0;i<E_num;i++)
    {
     enemy_update(&Enemy[i],i);
     enem_Display(&Enemy[i]);
    }
    //display & update player
    player_update(&Player);
    object_Display(&Player);

    //Display present time - Terminal
    score=GetTime() - init_score;//초기 시간 보정

    //Display present time(Score) - 7-segment
    display_7seg(score);

    //Determine impacted or not
    impact(&GAME_OVER,E_num,Enemy,&Player);

    //10초마다 적이 1개씩 늘어남
    if((((score%1)==0)&&(score != old_score))&&(!GAME_OVER)){
      //1초가 지났고 GAME OVER가 되지 않은 경우
      E_num++;
      old_score = score;
    }
    if(E_num>MAX_ENEMY)	E_num=MAX_ENEMY;

    if(GetChar(&ch) == 1) if (ch == 'Q' || ch == 'q') break;
  }

  //q입력이 아니라 Game-over로 나온 경우
  if(GAME_OVER)
  {
   printf("\n################ GAME OVER #############\n");
   msWait(1000);
   while(BTGetChar(&val) == 1);
   clear_receivebuf();
   while (1)
   {
     //최종 점수 7세그먼트에 출력
     display_7seg(score);
     if(BTGetChar(&val) == 1){
       if(val == '@'){
         //어픞리케이션에서 '재시작'을 눌렀을 경우
         goto replay;//해당 label로 이동 -> 게임 다시 실행
       }
     }
     if(GetChar(&ch) == 1) if (ch == 'Q' || ch == 'q') break;
   }
 }
 // clear sed (Score board off)
 *seg_addr_grid = 0x00;
 *seg_addr_data = 0x00;

END_GAME:
  return;
}
