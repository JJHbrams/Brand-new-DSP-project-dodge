#ifndef __SERIAL_H__
#define __SERIAL_H__

void SerialInit(void);
void PutChar(const char ch);
void PutString(const char *ch);
int GetChar(char * ch);
int uart_getc(void);

void SerialInit_BT9600(void);
void BTPutChar(const char ch);
void BTPutString(const char *ch);
int BTGetChar(char * ch);

#define UART_RBR    (0x00)
#define UART_THR    (0x00)
#define UART_IER    (0x04)
#define UART_IIR    (0x08)
#define UART_FCR    (0x08)
#define UART_LCR    (0x0C)
#define UART_MCR    (0x10)
#define UART_LSR    (0x14)
#define UART_MSR    (0x18)
#define UART_SPR    (0x1C)
#define UART_ISR    (0x20)
#define UART_DLL    (0x00)
#define UART_DLH    (0x04)

#define UART_IER_DMAE	(0x80)
#define UART_IER_UUE	(0x40)
#define UART_IER_NRZE	(0x20)
#define UART_IER_RTOIE	(0x10)
#define UART_IER_MIE	(0x08)
#define UART_IER_RLSE	(0x04)
#define UART_IER_TIE	(0x02)
#define UART_IER_RAVIE	(0x01)

#define UART_IIR_FIFOES_MASK	    (0x0C)
#define UART_IIR_FIFOES_NOT_FIFO    (0x00)
#define UART_IIR_FIFOES_FIFO	    (0x0C)
#define UART_IIR_TOD		    (0x08)
#define UART_IIR_IID_MASK	    (0x06)
#define UART_IIR_IID_MODEM_STATUS   (0x00)
#define UART_IIR_IID_TRD	    (0x02)
#define UART_IIR_IID_RDA	    (0x04)
#define UART_IIR_IID_RERR	    (0x06)
#define UART_IIR_IP		    (0x01)

#define UART_LSR_FIFOE	(0x80)
#define UART_LSR_TEMT	(0x40)
#define UART_LSR_TDRQ	(0x20)
#define UART_LSR_BI	(0x10)
#define UART_LSR_FE	(0x08)
#define UART_LSR_PE	(0x04)
#define UART_LSR_OE	(0x02)
#define UART_LSR_DR	(0x01)

int bt_uart_putc(int ch);                   // put char : \n -> \r\n
int bt_uart_getc(void);                             // get char : \r -> \n
int bt_uart_puts(const char *s);

int bt_uart_putb(int ch);                   // put byte
int bt_uart_getb(void);                             // get byte
int bt_uart_write(const void *buff, int count);

#endif
