
CC = arm-linux-gcc
OBJCOPY = arm-linux-objcopy

INCLUDE = -I./common
CFLAGS = -O2 -Wall -g -fomit-frame-pointer -mapcs-frame -nostdinc -ffreestanding -msoft-float $(INCLUDE)
OCFLAGS = -O binary -R .note -R .comment -S
LDFLAGS = -static -nostdlib -Wl,-T,./common/ld-xscale

COMPILE = $(CC) $(CFLAGS)

OBJECT = common/start.o common/time.o common/lib.o common/serial.o common/types.o common/raise.o
OBJECT += tftlcd.o dodge.o

OBJECT_CP = start.o time.o lib.o serial.o types.o raise.o
OBJECT_CP += tftlcd.o dodge.o

TARGET_IMAGE = dodge

all : $(OBJECT)
	$(CC) $(INCLUDE) $(CFLAGS) $(LDFLAGS) -o $(TARGET_IMAGE)-elf32 $(OBJECT_CP) -lgcc
	$(OBJCOPY) $(OCFLAGS) $(TARGET_IMAGE)-elf32 $(TARGET_IMAGE)
.S.o :
	$(COMPILE) -c $<

.c.o :
	$(COMPILE) -c $<

clean :
	rm -rf *.o
	rm -rf $(TARGET_IMAGE) $(TARGET_IMAGE)-elf32
