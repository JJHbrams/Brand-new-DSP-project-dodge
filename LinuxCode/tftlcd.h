/***********************************************
 * PXA27x LCD Controller Register Define
 *
 * Create : 2006-06-23 by Lee Byung Kwon
 ***********************************************/
#ifndef __PXA2XX_LCD_H__
#define __PXA2XX_LCD_H__

// struct type for dbjects(enemy, player)
typedef struct{
	signed int x,y;
	signed int vx,vy;
  	int hit_box;
}object;

void tlcd_on(void);
void tlcd_off(void);
int tlcd_init(void);
int tlcd_test(void);
void Background_Display_Init(void);
void Glcd_Drew_FBox(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor);
void Glcd_Drew_Box(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor);
void Color_Bar_Display(void);
void enem_Display(object * obj);
void object_Display(object * obj);
void countdown_display(unsigned char num);


#define LMS350GF    1   // 320 x 240
#define LTP400WQ    2   // 480 x 272
#define LB064V02    3   // 640 x 480
#define LTP700WV    4   // 800 x 480
#define VGA_640_480 5   // 640 x 480

#define LCD_MODULE_TYPE  LMS350GF
#define SM2_CIS_SIZE 0

#if (SM2_CIS_SIZE == 0)
#define CIS_IMAGE_WIDTH		320
#define CIS_IMAGE_HEIGHT	240
#define CIS_VAL_SET		0x00
#elif (SM2_CIS_SIZE == 1)
#define CIS_IMAGE_WIDTH		160
#define CIS_IMAGE_HEIGHT	120
#define CIS_VAL_SET		0x01
#endif

#if (LCD_MODULE_TYPE == LMS350GF)
#define PP_ROW  320
#define PP_COL  240
#define BMP_ROW 320
#define BMP_COL 240
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#elif (LCD_MODULE_TYPE == LTP400WQ)
#define PP_ROW  480
#define PP_COL  272
#define BMP_ROW 480
#define BMP_COL 272
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#elif (LCD_MODULE_TYPE == LB064V02) || (LCD_MODULE_TYPE == VGA_640_480)
#define PP_ROW  640
#define PP_COL  480
#define BMP_ROW 640
#define BMP_COL 480
#define LCD_WIDTH 640
#define LCD_HEIGHT 480
#elif (LCD_MODULE_TYPE == LTP700WV)
#define PP_ROW  800
#define PP_COL  480
#define BMP_ROW 800
#define BMP_COL 480
#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#endif

#define PDB_ADD	0x80600000
#define PDB(x)	(*(volatile unsigned long *)(PDB_ADD + x))	/* Palette Des Buffer */
#define FDB_ADD	0x80600010
#define FDB(x)	(*(volatile unsigned long *)(FDB_ADD + x))	/* Frame Des Buffer */
#define PLB_ADD	0x80600020
#define PLB(x)	(*(volatile unsigned long *)(PLB_ADD + x)) 	/* Palette Buffer */
#define FLB_ADD	0x80600400
#define FLB(x)	(*(volatile unsigned long*)(FLB_ADD + x)) 	/* Frame Buffer */

#define BMP_RAM_BASE    0x80600000
#define BMP_LENGTH         0x00100000  //640*480*3
#define BMP_ROM_BASE    (0x04000000-BMP_LENGTH)



// LCD Controller Register summary
#define LCCR0	0x44000000
#define LCCR1	0x44000004
#define LCCR2	0x44000008
#define LCCR3	0x4400000C
#define LCCR4	0x44000010
#define LCCR5	0x44000014
/* DMA Channel x Frame Branch register */
#define FBR00x44000020
#define FBR1	0x44000024
#define FBR2	0x44000028
#define FBR3	0x4400002C
#define FBR4	0x44000030
#define FBR5	0x44000110
#define FBR6	0x44000114
/* LCD Controller Status register */
#define LCSR1	0x44000034
#define LCSR0	0x44000038
/* LCD Controller Interrupt ID register */
#define LIIDR	0x4400003C

#define TRGBR	0x44000040	/* TMED RGB Seed register */
#define TCR	0x44000044 	/* TMED Control register */

/* Overlay 1 Contro register 1 */
#define OVL1C1	0x44000050
#define OVL1C2	0x44000060
#define OVL2C1	0x44000070
#define OVL2C2	0x44000080

#define CCR	0x44000090	/* Cursor Control register */
#define CMDCR	0x44000100	/* Command Control register */
#define PRSR	0x44000104	/* Panel Read Status register */

#define FDADR0	0x44000200	/* DMA Channel 0 Frame Descriptor Address register */
#define FSADR0	0x44000204	/* DMA Channel 0 Frame Source Address register */
#define FIDR0	0x44000208	/* DMA Channel 0 Frame ID register */
#define LDCMD0	0x4400020c	/* LCD DMA Channel 0 Command register */

#define FDADR1	0x44000210	/* DMA Channel 1 Frame Descriptor Address register */
#define FSADR1	0x44000214	/* DMA Channel 1 Frame Source Address register */
#define FIDR1	0x44000218	/* DMA Channel 1 Frame ID register */
#define LDCMD1	0x4400021c	/* LCD DMA Channel 1 Command register */

#define FDADR2	0x44000220	/* DMA Channel 2 Frame Descriptor Address register */
#define FSADR2	0x44000224	/* DMA Channel 2 Frame Source Address register */
#define FIDR2	0x44000228	/* DMA Channel 2 Frame ID register */
#define LDCMD2	0x4400022c	/* LCD DMA Channel 2 Command register */

#define FDADR3	0x44000230	/* DMA Channel 3 Frame Descriptor Address register */
#define FSADR3	0x44000234	/* DMA Channel 3 Frame Source Address register */
#define FIDR3	0x44000238	/* DMA Channel 3 Frame ID register */
#define LDCMD3	0x4400023c	/* LCD DMA Channel 3 Command register */

#define FDADR4	0x44000240	/* DMA Channel 4 Frame Descriptor Address register */
#define FSADR4	0x44000244	/* DMA Channel 4 Frame Source Address register */
#define FIDR4	0x44000248	/* DMA Channel 4 Frame ID register */
#define LDCMD4	0x4400024c	/* LCD DMA Channel 4 Command register */

#define FDADR5	0x44000250	/* DMA Channel 5 Frame Descriptor Address register */
#define FSADR5	0x44000254	/* DMA Channel 5 Frame Source Address register */
#define FIDR5	0x44000258	/* DMA Channel 5 Frame ID register */
#define LDCMD5	0x4400025c	/* LCD DMA Channel 5 Command register */

#define FDADR6	0x44000260	/* DMA Channel 1 Frame Descriptor Address register */
#define FSADR6	0x44000264	/* DMA Channel 1 Frame Source Address register */
#define FIDR6	0x44000268	/* DMA Channel 1 Frame ID register */
#define LDCMD6	0x4400026c	/* LCD DMA Channel 1 Command register */

#define LCDBSCNTR 0x48000054	/* LCD Buffer Strength Control register */

/* LCD Controller Register Bit Define */
/* LCCR0 Regiseter bit define */
#define LCCR3_1BPP  (0 << 24)
#define LCCR3_2BPP  (1 << 24)
#define LCCR3_4BPP  (2 << 24)
#define LCCR3_8BPP  (3 << 24)
#define LCCR3_16BPP (4 << 24)
#define LCCR3_18BPP (5 << 24)

#define LCCR0_ENB   (1 << 0)		/* LCD Controller enable */
#define LCCR0_CMS   (1 << 1)		/* Color/Monochrome Display Select */
#define LCCR0_Color (LCCR0_CMS*0)   /*  Color display                  */
#define LCCR0_Mono  (LCCR0_CMS*1)   /*  Monochrome display             */
#define LCCR0_SDS   (1 << 2)		/* Single/Dual Panel Display       */
                                    /* Select                          */
#define LCCR0_Sngl  (LCCR0_SDS*0)   /*  Single panel display           */
#define LCCR0_Dual  (LCCR0_SDS*1)   /*  Dual panel display             */

#define LCCR0_LDM   (1 << 3)		/* LCD Disable Done Mask */
#define LCCR0_SFM   (1 << 4)		/* Start of frame mask */
#define LCCR0_IUM   (1 << 5)		/* Input FIFO underrun mask */
#define LCCR0_EFM   (1 << 6)		/* End of Frame mask */
#define LCCR0_PAS   (1 << 7)		/* Passive/Active display Select   */
#define LCCR0_Pas   (LCCR0_PAS*0)   /*  Passive display (STN)          */
#define LCCR0_Act   (LCCR0_PAS*1)   /*  Active display (TFT)           */
#define LCCR0_DPD   (1 << 9)		/* Double Pixel Data (monochrome   */
                                    /* display mode)                   */
#define LCCR0_4PixMono  (LCCR0_DPD*0)   /*  4-Pixel/clock Monochrome       */
                                        /*  display                        */
#define LCCR0_8PixMono  (LCCR0_DPD*1)  	/*  8-Pixel/clock Monochrome       */
                                       	/*  display                        */
#define LCCR0_DIS	(1 << 10)			/* LCD Disable */
#define LCCR0_QDM	(1 << 11)			/* LCD Quick Disable mask */
#define LCCR0_PDD(x)	((x * 0xfff) << 12)	/* Palette DMA request delay */
#define LCCR0_BM	(1 << 20) 			/* Branch mask */
#define LCCR0_OUM	(1 << 21)			/* Output FIFO underrun mask */

#define LCCR1_PPL(x)   	((x & 0x3ff)<<0)  	/* Pixels Per Line - 1 */
#define LCCR1_HSW(x)   	((x & 0x3f) << 10)	/* Horizontal Synchronization     */
#define LCCR1_ELW(x)  	((x & 0xff) << 16)	/* End-of-Line pixel clock Wait    */
#define LCCR1_BLW(x)	((x & 0xff) << 24)	/* Beginning-of-Line pixel clock   */

#define LCCR2_LPP(x)	((x & 0x3ff) << 0)	/* Line Per Panel - 1              */
#define LCCR2_VSW(x)	((x & 0x3f) << 10)	/* Vertical Synchronization pulse  */
#define LCCR2_EFW(x)	((x & 0xff) << 16)	/* End-of-Frame line clock Wait    */
#define LCCR2_BFW(x)	((x & 0xff) << 24)	/* Beginning-of-Frame line clock   */

#define LCCR3_PCD(x)	((x & 0xff) << 0)	/* Pixel clock divisor */
#define LCCR3_ACB(x)	((x & 0xff) << 8)	/* AC Bias pin frequency */
#define LCCR3_API(x)	((x & 0xf) << 16)	/* AC Bias pin trasitions per interrupt */
#define LCCR3_VSP	(1 << 20)			/* vertical sync polarity */
#define LCCR3_HSP	(1 << 21)			/* horizontal sync polarity */
#define LCCR3_PCP	(1 << 22)			/* Pixel Clock Polarity (L_PCLK)   */
#define LCCR3_PixRsEdg 	(LCCR3_PCP*0)   /*  Pixel clock Rising-Edge        */
#define LCCR3_PixFlEdg 	(LCCR3_PCP*1)   /*  Pixel clock Falling-Edge       */
#define LCCR3_OEP      	(1 << 23)       /* Output Enable Polarity (L_BIAS, */
                                        /* active display mode)            */
#define LCCR3_OutEnH   	(LCCR3_OEP*0)   /*  Output Enable active High      */
#define LCCR3_OutEnL   	(LCCR3_OEP*1)   /*  Output Enable active Low       */

#define LCCR3_BPP16	(0x04 << 24)	/* 16bits per pixel */
#define LCCR3_BPP8	(0x03 << 24)	/* 8bits per pixel */
#define LCCR3_BPP4	(0x02 << 24)	/* 4bits per pixel */
#define LCCR3_BPP2	(0x01 << 24)	/* 2bits per pixel */
#define LCCR3_BPP1	(0x00 << 24)	/* 1bits per pixel */
#define LCCR3_DPC	(1 << 27)		/* double pixel clock mode */
#define LCCR3_PDFOR(x)	((x & 0x3) << 30) /* Pixel Data format */

#define LCSR_LDD	(1 << 0)	/* LCD Disable Done */
#define LCSR_SOF	(1 << 1)	/* Start of frame */
#define LCSR_BER	(1 << 2)	/* Bus error */
#define LCSR_ABC	(1 << 3)	/* AC Bias count */
#define LCSR_IUL	(1 << 4)	/* input FIFO underrun Lower panel */
#define LCSR_IUU	(1 << 5)	/* input FIFO underrun Upper panel */
#define LCSR_OU		(1 << 6)	/* output FIFO underrun */
#define LCSR_QD		(1 << 7)	/* quick disable */
#define LCSR_EOF	(1 << 8)	/* end of frame */
#define LCSR_BS		(1 << 9)	/* branch status */
#define LCSR_SINT	(1 << 10)	/* subsequent interrupt */

#define LDCMD_PAL	(1 << 26)	/* instructs DMA to load palette buffer */

#define LCSR_LDD	(1 << 0)	/* LCD Disable Done */
#define LCSR_SOF	(1 << 1)	/* Start of frame */
#define LCSR_BER	(1 << 2)	/* Bus error */
#define LCSR_ABC	(1 << 3)	/* AC Bias count */
#define LCSR_IUL	(1 << 4)	/* input FIFO underrun Lower panel */
#define LCSR_IUU	(1 << 5)	/* input FIFO underrun Upper panel */
#define LCSR_OU		(1 << 6)	/* output FIFO underrun */
#define LCSR_QD		(1 << 7)	/* quick disable */
#define LCSR_EOF	(1 << 8)	/* end of frame */
#define LCSR_BS		(1 << 9)	/* branch status */
#define LCSR_SINT	(1 << 10)	/* subsequent interrupt */

#define LDCMD_PAL	(1 << 26)	/* instructs DMA to load palette buffer */


#define RED_PIXEL(x)	(x & 0x1f) << 11  //(x & 0xff)<<11
#define GREEN_PIXEL(x)	(x & 0x3f) << 5    //(x & 0xff)<<5
#define BLUE_PIXEL(x)	(x & 0x1f) << 0    //(x & 0xff)<<0

#define	_RED	    RED_PIXEL(0xff)|GREEN_PIXEL(0x00)|BLUE_PIXEL(0x00)
#define	_GREEN	    RED_PIXEL(0x00)|GREEN_PIXEL(0xff)|BLUE_PIXEL(0x00)
#define	_BLUE	    RED_PIXEL(0x00)|GREEN_PIXEL(0x00)|BLUE_PIXEL(0xff)
#define	_BLACK	    RED_PIXEL(0x00)|GREEN_PIXEL(0x00)|BLUE_PIXEL(0x00)
#define	_CYAN	    RED_PIXEL(0x00)|GREEN_PIXEL(0xff)|BLUE_PIXEL(0xff)
#define	_MAGENTA    RED_PIXEL(0xff)|GREEN_PIXEL(0x00)|BLUE_PIXEL(0xff)
#define	_YELLOW	    RED_PIXEL(0xff)|GREEN_PIXEL(0xff)|BLUE_PIXEL(0x00)
#define	_GRAY	    RED_PIXEL(0xbe)|GREEN_PIXEL(0xbe)|BLUE_PIXEL(0xbe)
#define	_WHITE	    RED_PIXEL(0xff)|GREEN_PIXEL(0xff)|BLUE_PIXEL(0xff)
#endif


void menu_print(void);
void Background_Display_Init(void);
void Glcd_Drew_FBox(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor);
void Glcd_Drew_Box(unsigned int Gy_st,unsigned int Gx_st,unsigned int Gy_end,unsigned int Gx_end,unsigned short Fcolor);
void draw_bmp(int dx_st,int dy_st,int dw_x, int dw_y, int from_xxx);
void Color_Bar_Display(void);
