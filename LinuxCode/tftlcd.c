/**************************************
 *  PXA2xx LCD Cotroller Initial
 *
 **************************************/

#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#include "tftlcd.h"
#include "count_number.h"
//#include "count_number_old.h"

/* global */
int color_depth = 16;
int fg_color;
int bg_color;
int col;
int row;

unsigned long fb_offset;
volatile unsigned short * pfbdata;

#define BG_DEFAULT	1
#define FG_DEFAULT	0x00e000e0

#define FB_SIZE(x)	(((PP_COL * PP_ROW) * x) / 8)
#define ROW_SIZE(x)	((PP_ROW * x) / 8)


const unsigned short color_pattern[] = {
    _RED,
    _GREEN,
    _BLUE,
    _CYAN,
    _MAGENTA,
    _YELLOW,
    _GRAY,
    _WHITE,
    _BLACK,
};

//배경초기화
void Background_Display_Init(void)
{
    //배경 초기화
    int i;

    fg_color = FG_DEFAULT;
    bg_color = BG_DEFAULT;
    row = 0;
    col = 0;
    fb_offset = 0x00;

    for(i=0;i<(PP_COL * PP_ROW)*2; i+=2){
	     FLB(i) = bg_color;
    }
}
//원하는 위치에 이미지 띄우기
void Glcd_Drew_FBox(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor)
{
    //입력변수:처음높이, 처음 너비, 마지막 높이, 마지막 너비, 색상
    unsigned int i,j;

    pfbdata =(unsigned short*)FLB_ADD;//데이터 쓰기

    for(i=Gy_st; i<Gy_end; i++)				// ypoint = 0 to 480
    {
	fb_offset=i*PP_ROW;						// offset = y * (0~640)
	//		fb_offset=i*640;						// offset = y * (0~640)

	for(j=Gx_st; j<Gx_end; j++)			// xpoint ++
	{
	    *(pfbdata+fb_offset+j) = Fcolor;
	}
    }

}

void Glcd_Drew_Box(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor)
{
    //입력변수:처음높이, 처음 너비, 마지막 높이, 마지막 너비, 색상
    unsigned int j;

    pfbdata =(unsigned short*)FLB_ADD;//데이터 쓰기

    fb_offset=Gy_st*PP_ROW;
    for(j=Gx_st; j<Gx_end; j++)	*(pfbdata+fb_offset+j) = Fcolor;

    fb_offset=Gy_end*PP_ROW;
    for(j=Gx_st; j<Gx_end; j++)	*(pfbdata+fb_offset+j) = Fcolor;

    for(j=Gy_st; j<Gy_end; j++)
    {
	*(pfbdata+j*PP_ROW+Gx_st) = Fcolor;
	*(pfbdata+j*PP_ROW+Gx_end) = Fcolor;

    }
}

void enem_Display(object * obj)
{
    //obj : enemy or player
    //plot objects
    Glcd_Drew_FBox((obj->y),(obj->x),(obj->y)+(obj->hit_box),(obj->x)+(obj->hit_box),_MAGENTA);
}

void object_Display(object * obj)
{
    //obj : enemy or player
    //plot objects
    Glcd_Drew_FBox((obj->y),(obj->x),(obj->y)+(obj->hit_box),(obj->x)+(obj->hit_box),_WHITE);
}

void countdown_display(unsigned char num)
{
  // 카운트다운 이미지 출력
  //num : 출력할 숫자
  int i,j,k;
  pfbdata =(unsigned short*)FLB_ADD;//데이터 쓰기

  for(i=0;i<PP_COL;i++) {
          for(j=0;j<PP_ROW;j++) {
                  k = i *PP_ROW +j;
                  *(pfbdata+(i*PP_ROW)+j) = number[num][k];//MATLAB으로 만든 High color RGB 이미지 데이터값 대입
          }
  }
}
