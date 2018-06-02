#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

#include "tftlcd.h"

#define FROM_RAM 1
#define FROM_ROM 0


void c_main(void)
{

	printf("\nDrawing Test Pattern \n");
	Color_Bar_Display();

}

