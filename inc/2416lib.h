

//===================================================================
// File Name : 2440lib.h
// Function  : TQ2440 
// Version   : 1.0
//===================================================================

#ifndef __2440lib_h__
#define __2440lib_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"


#define DebugOut Uart_Printf

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))



#define MaxTimer	10

//#define ReceiveLen	30		// 考虑各种PLC

typedef enum
{
	TIMER_STOP = 1,
	TIMER_PAUSE,
	TIMER_START,
	//TIMER_RESUME,
	TIMER_READ
} Timer2_FUNC;


///////////////////////////

void Timer2_Init(void);
void Timer2_Start(void);
void Timer2_Stop(void);
void __irq Timer2_Proc(void);

void UartTimeOut_Start(void);


void Delay(int time);

void Port_Init(void);

void ChangeMPllValue(int m,int p,int s);
void ChangeClockDivider(int armdiv_val, int prediv_val, int hdivn_val,int pdivn_val);
//void ChangeUPllValue(int m,int p,int s);

void Uart_Init(int mclk,int baud);
void Uart_InitDownload(int pclk,int baud,int Flag);

void Uart_Select(int ch);

void Uart_TxEmpty(int ch);
void Unitel_SendByte(int data);


void Uart_SendByte(int data);
void Uart_Printf(char *fmt,...);
void Uart_SendString(char *pt);

void putch(int data);
void putStr(char *pt);

char getch(void);
char Uart_Getch(void);
char Uart_DownloadGetch(U8 * ErrFlag);
void __irq Timer0Done(void);


//void Buzzer_Freq_Set( U32 freq );
//void Buzzer_Stop( void );
//void Beep(U32 freq, U32 ms);
void Beep(int BeepStatus);

void * malloc(unsigned nbyte) ;
void free(void *pt);




//////////////   090721  下载程序到NandFlash  //////////////////////////
void Uart_GetString(char *string);
void SDRAM_test(unsigned int rambase,unsigned int ramend);
char Uart_Getch_DNW(void);
void Uart_Init_DNW(int pclk,int baud);





#ifdef __cplusplus
}
#endif

#endif  //__2440lib_h__
