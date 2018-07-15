#include <config.h>
#include <reg.h>
#include <serial.h>
#include <types.h>
#include <lib.h>
#include <time.h>

#define ENOERROR	0	/* no error at all */
#define EINVAL		1	/* invalid argument */
#define ENOPARAMS	2	/* not enough parameters */
#define EMAGIC		3	/* magic value failed */
#define ECOMMAND	4	/* invalid command */
#define ENAN		5	/* not a number */
#define EALIGN		6	/* addres not aligned */
#define ERANGE		7	/* out of range */
#define ETIMEOUT	8	/* timeout exceeded */
#define ETOOSHORT	9	/* short file */
#define ETOOLONG	10	/* long file */
#define EAMBIGCMD	11	/* ambiguous command */
#define EFLASHERASE	12	/* can't erase flash block */
#define EFLASHPGM	13	/* flash program error */
#define ESERIAL		14	/* serial port error */

#define INT_BUF_SIZE 1024
#define	MAX_RETRIES	 10

#define TEST_MAX_RETRIES do { 				                   \
	if(retries++ > MAX_RETRIES) {	 		                   \
		return -ETIMEOUT;				           \
	}						                   \
} while(0)

#define	DEC(c)	(((c) - ' ') & 077)		/* single character decode */
#define	IS_DEC(c) ( (((c) - ' ') >= 0) &&  (((c) - ' ') <= 077 + 1) )
/* #define IS_DEC(c) (1) */

#define OUT_OF_RANGE do {	\
	PutChar('\n');	\
        PutString(buf);	\
	PutChar('\n');	\
        return -ERANGE; \
} while(0)

#define PUT_CHAR(x) do {	             \
	if(bytesWritten < bufLen)	     \
	        bufBase[bytesWritten++] = x; \
} while(0)

void SerialInit(void)
{
   __REG(FFUART_BASE + FFLCR) = 0x00000003; // 8-bit, 1 stop, no parity ¼¼ÆÃ.
   __REG(FFUART_BASE + FFFCR) = 0x00000007; // Reset tx, rx FIFO. clear. FIFO enable
   __REG(FFUART_BASE + FFIER) = 0x00000040; // UART Enable Interrupt
   __REG(FFUART_BASE + FFLCR) |= 0x00000080; // DLAB set=latch registers, DLAB clear=ÀÏ¹Ý Æ÷Æ®.
   __REG(FFUART_BASE + FFDLL) = SERIAL_SPEED_RATE; // baud rate ¼³Á¤.
   __REG(FFUART_BASE + FFLCR) &= 0xFFFFFF7F; // DLAB clear, ÀÏ¹Ý Æ÷Æ®·Î ÀüÈ¯.

   // Transmit Shift Register, Transmit Holding Register, FIFO¿¡
   // µ¥ÀÌÅ¸°¡ ¾øÀ»¶§±îÁö ±â´Ù¸°´Ù.
   while(! __REG(FFUART_BASE + FFLSR) & 0x00000040 );
   
   __REG(0x40100020) = 0x00; // 
   __REG(0x40200020) = 0x00; //
   __REG(STIER) = 0x40; // UART Unit Enable
   __REG(STFCR) = 0x07; // RESETTF, RESETRF, TRFIFOE
   __REG(STLCR) = 0x03; // i stop, 8bit data, no parity
   //__REG(STISR) = 0x04; // XMODE, RCVEIR
   return;
}

int SerialInputString(char *s, const int len, const int timeout)
{
	u32 startTime, currentTime;
	char c;
	int i;
	int numRead;
	int skipNewline = 1;
	int maxRead = len - 1;

	startTime = GetTime();

	for(numRead = 0, i = 0; numRead < maxRead;) {
		/* try to get a byte from the serial port */
		while((__REG(FFUART_BASE  + FFLSR) & 0x00000001)==0) {
			currentTime = GetTime();

			/* check timeout value */
			if((currentTime - startTime) >
			    (timeout)) {
			   //(timeout * TICKS_PER_SECOND)) {
				/* timeout */
				s[i++] = '\0';
				return(numRead);
			}
		}
		GetChar(&c);
		/* check for errors */
		//if(c < 0) {
		//	s[i++] = '\0';
		//	return c;
		//}

		/* eat newline characters at start of string */
		if((skipNewline == 1) && (c != '\r') && (c != '\n'))
			skipNewline = 0;

		if(skipNewline == 0) {
			if((c == '\r') || (c == '\n')) {
				s[i++] = '\0';
				return(numRead);
			} else {
				s[i++] = (char)c;
				numRead++;
			}
		}
	}
	return(numRead);
}

void PutChar(const char ch)
{
	// FIFO¿¡ µ¥ÀÌÅ¸°¡ ¾øÀ»¶§±îÁö ±â´Ù¸°´Ù.
	while ((__REG(FFUART_BASE + FFLSR) & 0x00000020) == 0 );

	__REG(FFUART_BASE + FFTHR) = ((u32)ch & 0xFF);

	if (ch=='\n') PutChar('\r'); // c=='\n'ÀÌ¸é, ½ÇÁ¦·Î´Â "\n\r"À» Ãâ·Â.
}

void PutString(const char *ch) //¹®Àß¿­À» Ãâ·ÂÇÏ±â À§ÇØ ±¸ÇöÇØ ³õ¾ÒÁö¸¸ printf¹®ÀÌ ÀÖ¾î °ÅÀÇ »ç¿ëÇÏÁö ¾Ê´Â´Ù.
{
	while(*ch != '\0')
		PutChar(*ch++);
}

int GetChar(char * ch) //½Ã¸®¾ó·Î ÀÔ·ÂµÈ µ¥ÀÌÅ¸¸¦ °¡Á® ¿Â´Ù.
{
	if((__REG(FFUART_BASE + FFLSR) & 0x00000001)==0){ // FIFO¿¡ µ¥ÀÌÅ¸°¡ ÀÖÀ»¶§.
		return 0;
	} else {

		*(volatile char *)ch = __REG(FFUART_BASE + FFRBR);
		return 1;
	}
}

int uart_getc(void)
{
	int ch = -1;
	int cnt = 0;
	
	while(!((__REG(FFUART_BASE + FFLSR)) & 0x00000001))
	{
		cnt++;
	}
	ch =  __REG(FFUART_BASE + FFRBR);

	if (ch == '\r') ch = '\n';
	return ch;
}

int uart_getc2(void){
	int ch = -1;
	if ((__REG(FFUART_BASE + FFLSR)) & 0x00000001)
		ch =  __REG(FFUART_BASE + FFRBR);
	if (ch == '\r') ch = '\n';
	return ch;
}


static const unsigned long bt_base = 0x40200000;
#define readb(c)		(*(volatile u8  *)(c))
#define writeb(v, c)		(*(volatile u8  *)(c) = (v))
#define bt_uart_put(v, p)	writeb(v, bt_base + (p))
#define bt_uart_get(p)		readb(bt_base + (p))

void SerialInit_BT9600(void)
{
	__REG(BTUART_BASE + BTLCR) = 0x00000003; // 8-bit, 1 stop, no parity ¼¼Æ.
	__REG(BTUART_BASE + BTFCR) = 0x00000007; // Reset tx, rx FIFO. clear. FIFO enable
	__REG(BTUART_BASE + BTIER) = 0x00000040; // UART Enable Interrupt
	__REG(BTUART_BASE + BTLCR) |= 0x00000080; // DLAB set=latch registers, DLAB clear=À¹ÝÆ¿.
	__REG(BTUART_BASE + BTDLL) = RATE_9600; // baud rate ¼³d.
	__REG(BTUART_BASE + BTLCR) &= 0xFFFFFF7F; // DLAB clear, À¹ÝÆ¿·ÎÀ¿.
	// Transmit Shift Register, Transmit Holding Register, FIFO¿¡
	// µ¥À¿°¡ ¾ø±î ±â¸°´Ù
	while(! __REG(BTUART_BASE + BTLSR) & 0x00000040 );

	__REG(0x40200020) = 0x00; //
	__REG(STIER) = 0x40; // UART Unit Enable
	__REG(STFCR) = 0x07; // RESETTF, RESETRF, TRFIFOE
	__REG(STLCR) = 0x03; // i stop, 8bit data, no parity
	//__REG(STISR) = 0x04; // XMODE, RCVEIR
	return;
}

void BTPutChar(const char ch)
{
	// FIFO¿¡ µ¥À¿°¡ ¾ø±î ±â¸°´Ù
	while ((__REG(BTUART_BASE + BTLSR) & 0x00000020) == 0 );

	__REG(BTUART_BASE + BTTHR) = ((u32)ch & 0xFF);

	if (ch=='\n') BTPutChar('\r'); // c=='\n'À¸é½Ç¦·¿Â"\n\r"; Ã·Â
}

void BTPutString(const char *ch) //¹®À¿­; Ã·Â¿â§Ç ±¸ÇÇ ³õöprintf¹®À À¾îÅÇ»çÇÁ ¾¿´Ù
{
	while(*ch != '\0')
		BTPutChar(*ch++);
}

int BTGetChar(char * ch) //½ø®¾óÀ·µÈµ¥À¿¸¦ °¡n ¿´Ù
{
	if((__REG(BTUART_BASE + BTLSR) & 0x00000001)==0){ // FIFO¿¡ µ¥À¿°¡ À;¶§.
		return 0;
	} else {

		*(volatile char *)ch = __REG(BTUART_BASE + BTRBR);
		return 1;
	}
}

int bt_uart_putc(int ch){
	if (ch == '\n') bt_uart_putc('\r');
	while (!(bt_uart_get(UART_LSR) & LSR_TDRQ));
	bt_uart_put(ch, UART_THR);
	return ch;
}

int bt_uart_getc(void){
	int ch = -1;
	if (bt_uart_get(UART_LSR) & LSR_DR)
		ch = bt_uart_get(UART_RBR);
	if (ch == '\r') ch = '\n';
	return ch;
}

int bt_uart_puts(const char *s){
	const char *t = s;
	while (*t) bt_uart_putc(*t++);
	return t - s;
}

int bt_uart_putb(int ch){
	while (!(bt_uart_get(UART_LSR) & LSR_TDRQ));
	bt_uart_put(ch, UART_THR);
	return ch;
}

int bt_uart_getb(void){
	int ch = -1;
	if (bt_uart_get(UART_LSR) & LSR_DR)
		ch = bt_uart_get(UART_RBR);
	return ch;
}

int bt_uart_write(const void *buff, int count){
	const char *s = buff;
	while (count-- > 0){
		while (!(bt_uart_get(UART_LSR) & LSR_TDRQ));
		bt_uart_put(*s++, UART_THR);
	}
	return count;
}

