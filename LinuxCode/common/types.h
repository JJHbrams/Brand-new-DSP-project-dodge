#ifndef __TYPES_H__
#define __TYPES_H__

#define	true	1
#define	false	0
#define	NULL	(void *)0
#define	va_start(ap, p)	    (ap = (char *) (&(p)+1))
#define	va_arg(ap, type)    ((type *) (ap += sizeof(type)))[-1]
#define	va_end(ap)
#define	HZ  (1)

typedef	unsigned long		u32;
typedef	unsigned long		UINT32;
typedef unsigned long		DWORD;

//typedef DWORD			__unaligned;


typedef	unsigned short	u16;
typedef	unsigned short	UINT16;
typedef unsigned short		WORD;

typedef	unsigned char		u8;
typedef unsigned char		UINT8;
typedef unsigned char		BYTE;
typedef BYTE				*PBYTE;
typedef BYTE				*PUINT8;

typedef	int				bool;
typedef int				BOOL;
typedef	char 			*va_list;
typedef volatile unsigned long vulong;
typedef void				*PVOID;

typedef const char *		PCSTR;

#if defined(HBE_XM_Bulverde) || (HBE_SM2)
typedef unsigned long		ulong;
typedef unsigned short		ushort;
typedef unsigned char		uchar;
typedef unsigned int		uint;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long		uint32;
typedef signed char		sint8;
typedef signed short		sint16;
typedef signed long		sint32;

typedef unsigned long		time_t;
typedef unsigned long		clock_t;
#endif



#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)


#endif
