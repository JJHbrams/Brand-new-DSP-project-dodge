#include <stdio.h>
/*
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
*/
//**************************************************
//				컴파일 단축키 : shift + F5 (notepad++)
//				컴파일 단축키 : F5 (Atom)
//**************************************************

typedef struct {int x, y, vx, vy, atk} Enemy;

typedef struct {int x, y, vx, vy, health} Player;

typedef struct {int x, y, vx, vy} Star;

void main()
{
	char *data[]={"@#x y b&*@#x!y$b&*@#x#y$b&*@#x y!b&*"};// 실제로는 계속 한 글자씩 갱신되는 데이터
	char *p=data[0];
	char x_flag=0;
	char y_flag=0;
	char x,y,vx,vy;
	int i,j;

	//Data processing
	for(i=0;i<36;i++)
	{
		if(*p=='@')	printf("Start	");
		else if(*p=='x')
		{
			printf("X : 	");
			x_flag=1;
		}
		else if(*p=='y')
		{
			printf("Y : 	");
			x_flag=0;
			y_flag=1;
		}
		else if(*p=='b')
		{
			printf("	END \n\n");
			y_flag=0;
		}
		else
		{
			if(x_flag&&!y_flag)
			{
				vx=*p;
				printf("%c	  (number : %d)			",*p,(int)(*p));
			}
			if(y_flag)
			{
				vy=*p;
				printf("%c	  (number : %d) 		",*p,(int)(*p));
			}
		}
		p++;
	}



}
