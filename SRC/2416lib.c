#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "2416slib.h" 
#include "option.h"
#include "..\inc\systemconf.h"
#include "..\inc\lcd.h"


//////

//extern char Image$$RW$$Limit[];
//void *mallocPt=Image$$RW$$Limit;

int whichUart=0;
INT8U Flag_Comm;

extern char Image$$RW$$Limit[];
void *mallocPt=Image$$RW$$Limit;


extern SYSTEMCONFIG g_SystemConf;
extern GLOBALVAR g_GlobalVar;
extern U8 g_download_Flag;


extern TIMER2_V g_Timer2Uart;
extern TIMER2_V g_Timer2Virtual[MaxTimer];
extern U8 g_UartTimeOut;

///Timer0  定义变量*************************************************////



//***************************[ SYSTEM ]***************************************************
void Timer2_Init(void)
{
	g_GlobalVar.ScrSaveFlag = 0;
	g_GlobalVar.Timer2Count = 0;

	// Timer2 Initialize	----ScrSave
	pISR_TIMER2 = (int)Timer2_Proc;	// Timer ISR for DA
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 8); 
	rTCFG1 |= (0x3 << 8); 	// Interrupt, Mux0=1/16
	//rTCNTB2 = 37500;    		// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1 Hz
	rTCNTB2 = 375;    		// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100 Hz -- 10ms
	//rTCMPB2 = 20000;

	Timer2_Start();
}

void Timer2_Start(void)
{
	DWORD tmp;
		
	rINTMSK1 &=~(BIT_TIMER2);
	tmp = rTCON & (~(0xf << 12));	// dead zone Disable
	rTCON = tmp | (2 << 12);		// update TCVNTB2, stop			
	rTCON = tmp | (9 << 12);		// interval mode,  start			
}

void Timer2_Stop(void)
{
	rINTMSK1 |= BIT_TIMER2;
	rTCON &= ~(1 << 12);			// Timer2, stop						
}


void __irq Timer2_Proc(void) 
{
	//int i;

	if((TRUE != g_download_Flag) && (0 != g_GlobalVar.ScrSaveTime)) // 下载画面的时候不屏保
	{
		g_GlobalVar.Timer2Count++;
		if(g_GlobalVar.Timer2Count >= g_GlobalVar.ScrSaveTime)		// 10ms *100 = 1s
		{
			g_GlobalVar.Timer2Count = 0;
			g_GlobalVar.ScrSaveFlag = 1;

			//输出一个IO口  控制灯光的关
			Lcd_Pwren_Init_On_Off( FALSE) ;
			//Timer2_Stop();
		}
	}


	if(TIMER_START == g_Timer2Uart.Timer_Status)
	{
		g_Timer2Uart.Timer_Cur++;
		if(g_Timer2Uart.Timer_Cur >= g_Timer2Uart.Timer_Set)
		{
			g_Timer2Uart.Timer_Cur = 0;
			g_UartTimeOut = TRUE;
		}
	}


	/*for(i=0;i<MaxTimer;i++)
	{
		if(TIMER_START == g_Timer2Virtual[i].Timer_Status)
		{
			g_Timer2Virtual[i].Timer_Cur++;
		}
		if(TIMER_STOP == g_Timer2Virtual[i].Timer_Status)
		{
			g_Timer2Virtual[i].Timer_Cur = 0;
		}
		if(TIMER_PAUSE == g_Timer2Virtual[i].Timer_Status)
		{
			//g_Timer2Virtual.Timer_Cur++;
		}
	}*/


	ClearPending((U32)BIT_TIMER2);

}



void UartTimeOut_Start(void)
{
	g_Timer2Uart.Timer_Status = TIMER_START;
	g_Timer2Uart.Timer_Set = 5;
	g_Timer2Uart.Timer_Cur = 0;

	g_UartTimeOut = FALSE;
}




// 延时函数
void Delay(int time)
{
	for (; time > 0; time--);
}


/*
void Delay(int time)
{
	U32 val = (SYS_PCLK>>3)/1000-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//mux = 1/2

	rTCNTB3 = val;
	rTCMPB3 = val>>1;		// 50%
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);
		while(rTCNTO3<val>>1);
	};
}
*/



void Port_Init(void)
{
	DWORD tmp;

	//CAUTION:Follow the configuration order for setting the ports. 
	// 1) setting value(GPnDAT) 
	// 2) setting control register  (GPnCON)
	// 3) configure pull-up resistor(GPnUP)  

	//32bit data bus configuration  
	//*** PORT A GROUP
	//Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16 GPA15 GPA14 GPA13 GPA12  
	//Signal : nFCE nRSTOUT nFRE   nFWE  ALE   CLE  nGCS5 nGCS4 nGCS3 nGCS2 nGCS1 
	//Binary :  1     1      1  , 1   1   1    1   ,  1     1     1     1
	//Ports  : GPA11   GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1  GPA0
	//Signal : ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0 
	//Binary :  1       1      1      1   , 1       1      1      1   ,  1       1     1      1         
	rGPACON = 0x7fffff; 
	tmp = rGPACON & (~(0x1 << 16));
	rGPACON = tmp | (0<<16);		//set GPA16 as output 		 LCD BackLight
	rGPADAT |= (1<<16);		//output 1;    LCD BackLight Off

	//**** PORT B GROUP
	//Ports  : GPB10    GPB9    GPB8    GPB7    GPB6     GPB5    GPB4   GPB3   GPB2     GPB1      GPB0
	//Signal : nXDREQ0 nXDACK0 nXDREQ1 nXDACK1 nSS_KBD nDIS_OFF L3CLOCK L3DATA L3MODE nIrDATXDEN Keyboard
	//Setting: INPUT  OUTPUT   INPUT  OUTPUT   INPUT   OUTPUT   OUTPUT OUTPUT OUTPUT   OUTPUT    OUTPUT 
	//Binary :   00  ,  01       00  ,   01      00   ,  01       01  ,   01     01   ,  01        01  
	//rGPBCON = 0x000150;(youlong)
	rGPBCON = 0x55555555;
	rGPBUDP  = 0xffffffff;     // The pull up function is disabled GPB[10:0]

	tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<2) | (0x2<<6);	//set GPB1 3 as TOUT	

	tmp = rGPBCON & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 18)) & (~(0x3<< 20));
	rGPBCON = tmp | (0x1<<8) | (0x1<<10) | (0x1<<18) | (0x1<<20);	//set GPB4 5 9 10 as output	
	// 输入输出都反向
	rGPBDAT |= (1<<5);	// OUT11	GPB5
	rGPBDAT |= (1<<10);	// OUT12	GPB10
	rGPBDAT |= (1<<9);	// OUT15	GPB9
	rGPBDAT |= (1<<4);	// OUT16	GPB4

	//*** PORT C GROUP for youlong
	//Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
	//Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND  
	//Binary :  10   10  , 10    10  , 10    10  , 10   10  , 10     10  ,  10   10 , 10     10 , 10   10
	//rGPCCON = 0xaaaaaaaa;       
	//rGPCUP  = 0xffff;     // The pull up function is disabled GPC[15:0] 

	//*** PORT C GROUP
	//Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
	//Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND  
	//Binary :  10   10  , 10    10  , 10    10  , 10   01  , 01     01  ,  01   10 , 10     10 , 10   10
	rGPCCON = 0xaaa956aa;       
	rGPCUDP  = 0xffff;     // The pull up function is disabled GPC[15:0]     

	//*** PORT D GROUP
	//Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 GPD0
	//Signal : VD23  VD22  VD21  VD20  VD19  VD18  VD17 VD16 VD15 VD14 VD13 VD12 VD11 VD10 VD9  VD8
	//Binary : 10    10  , 10    10  , 10    10  , 10   10 , 10   10 , 10   10 , 10   10 ,10   10
	rGPDCON = 0xaaaaaaaa;       
	rGPDUDP  = 0xffff;     // The pull up function is disabled GPD[15:0]

	//*** PORT E GROUP
	//Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4  
	//Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK IN 
	//Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     00  ,     
	//-------------------------------------------------------------------------------------------------------
	//Ports  :  GPE3   GPE2  GPE1    GPE0    
	//Signal :  IN     IN    IN      IN  
	//Binary :  00     00  ,  00      00 
	//rGPECON = 0xaaaaaaaa;       
	//rGPEUP  = 0xffff;     // The pull up function is disabled GPE[15:0]
	rGPECON = 0x55555555; // All Output      
	rGPEUDP  = 0xffffffff;     

	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8))
		& (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14)) & (~(0x3<< 16)) & (~(0x3<< 18))
		& (~(0x3<< 22)) & (~(0x3<< 24)) & (~(0x3<< 26));
	rGPECON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8)
		| (0x0<<10) | (0x0<<12) | (0x0<<14) | (0x0<<16) | (0x0<<18)
		| (0x0<<22) | (0x0<<24) | (0x0<<26);			//set GPE0~9 11~13 as input

	tmp = rGPECON & (~(0x3<< 16)) & (~(0x3<< 18)) & (~(0x3<< 20));
	rGPECON = tmp | (0x1<<16) | (0x1<<18) | (0x1<<20);	// GPE8,9,10 as output
	
	 //*** PORT F GROUP
	//Ports  : GPF7   GPF6   GPF5   GPF4      GPF3     GPF2  GPF1   GPF0
	//Signal : nLED_8 nLED_4 nLED_2 nLED_1 nIRQ_PCMCIA EINT2 KBDINT EINT0
	//Setting: Output Output Output Output    EINT3    EINT2 EINT1  EINT0
	//Binary :  01      01 ,  01     01  ,     10       10  , 10     10
	rGPFCON = 0x55555555;
	rGPFUDP  = 0xffffffff;     // The pull up function is disabled GPF[7:0]

	tmp = rGPFCON & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 12));
	rGPFCON = tmp | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<12);	//set GPF2 3 4 6 as EINT	
	rGPFUDP &= (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 12)); //GPF2 3 4 6 up down disable

	tmp = rGPFCON & (~(0x3<< 10)) & (~(0x3<< 14));
	rGPFCON = tmp | (0x0<<10) | (0x0<<14);		//set GPF5 7 as input	

	//*** PORT G GROUP
	//Ports  : GPG15 GPG14 GPG13 GPG12 GPG11    GPG10    GPG9     GPG8     GPG7      GPG6    
	//Signal : nYPON  YMON nXPON XMON  EINT19 DMAMODE1 DMAMODE0 DMASTART KBDSPICLK KBDSPIMOSI
	//Setting: nYPON  YMON nXPON XMON  EINT19  Output   Output   Output   SPICLK1    SPIMOSI1
	//Binary :   11    11 , 11    11  , 10      01    ,   01       01   ,    11         11
	//-----------------------------------------------------------------------------------------
	//Ports  :    GPG5       GPG4    GPG3    GPG2    GPG1    GPG0    
	//Signal : KBDSPIMISO LCD_PWREN EINT11 nSS_SPI IRQ_LAN IRQ_PCMCIA
	//Setting:  SPIMISO1  LCD_PWRDN EINT11   nSS0   EINT9    EINT8
	//Binary :     11         11   ,  10      11  ,  10        10
	rGPGCON = 0x55555555;// GPG9 input without pull-up
	rGPGUDP  = 0xffffffff;    // The pull up function is disabled GPG[15:0]

	rGPGCON &= ~(3<<10);
	rGPGCON |= (1<<10);		//set GPG5 as output	Beep
	rGPGDAT &= ~(1<<5);		//GPG5 output 0;	

	//rGPGCON &= ~(3<<0);
	//rGPGCON |= (1<<0);		//set GPG0 as output	通讯灯COM
	

	tmp = rGPGCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8));
	rGPGCON = tmp | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8);		//set GPG1 2 3 4 as input	

	//*** PORT H GROUP
	//Ports  :  GPH10    GPH9  GPH8 GPH7  GPH6  GPH5 GPH4 GPH3 GPH2 GPH1  GPH0 
	//Signal : CLKOUT1 CLKOUT0 UCLK nCTS1 nRTS1 RXD1 TXD1 RXD0 TXD0 nRTS0 nCTS0
	//Binary :   10   ,  10     10 , 11    11  , 10   10 , 10   10 , 10    10
	//rGPHCON = 0x00faaa;
	//rGPHUDP  = 0x7ff;    // The pull up function is disabled GPH[10:0]
	rGPHCON = 0x1555aaaa;
	rGPHUDP  = 0x7ff;    // The pull up function is disabled GPH[10:0]

	tmp = rGPHCON & (~(0x3<< 6)) & (~(0x3<< 20)) & (~(0x3<< 22)) & (~(0x3<< 26)) & (~(0x3<< 28));
	rGPHCON = tmp | (0x1<<6) | (0x1<<20) | (0x1<<22) | (0x1<<26) | (0x1<<28); //set GPH3 10 11 13 14 as output	
	// 输入输出都反向
	rGPHDAT |= (1<<10);	// OUT8	GPH10
	rGPHDAT |= (1<<14);	// OUT9	GPH14
	rGPHDAT |= (1<<13);	// OUT10	GPH13
	rGPHDAT |= (1<<3);	// OUT13	GPH3
	rGPHDAT |= (1<<11);	// OUT14	GPH11
	
	// Added for S3C2440X, DonGo
	//*** PORT J GROUP
	//Ports  : GPJ12   GPJ11       GPJ10    GPJ9    GPJ8      GPJ7      GPJ6      GPJ5      GPJ4      GPJ3      GPJ2      GPJ1     GPJ0 
	//Signal : CAMRESET CAMPCLKOUT CAMHREF CAMVSYNC CAMPCLKIN CAMDAT[7] CAMDAT[6] CAMDAT[5] CAMDAT[4] CAMDAT[3] CAMDAT[2] CAMDAT[1] CAMDAT[0] 
	//Binary :   10      10       10        10       10        10        10        10       10         10        10        10      10
	//rGPJCON = 0x02aaaaaa;
	//rGPJUDP  = 0x1fff;    // The pull up function is disabled GPH[10:0]


	//*** PORT K GROUP
	rGPKCON = 0x55555555;
	rGPKUDP  = 0xffffffff;   

	//*** PORT L GROUP
	rGPLCON = 0x55555555;
	rGPLUDP  = 0xffffffff;   

	tmp = rGPLCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 18)) & (~(0x3<< 26));
	rGPLCON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<18) | (0x1<<26); //set GPH0 1 2 9 13 as output	
	// 输入输出都反向
	rGPLDAT |= (1<<1);	// OUT3	GPL1
	rGPLDAT |= (1<<13);	// OUT4	GPL13
	rGPLDAT |= (1<<9);	// OUT5	GPL9
	rGPLDAT |= (1<<0);	// OUT6	GPL0
	rGPLDAT |= (1<<2);	// OUT7	GPL2


	//*** PORT M GROUP
	rGPMCON = 0xA;
	rGPMUDP  = 0x0;   

	//External interrupt will be falling edge triggered. 
	rEXTINT0 = 0x22222222;    // EINT[7:0]
	rEXTINT1 = 0x22222222;    // EINT[15:8]
	rEXTINT2 = 0x22222222;    // EINT[23:16]
}



//*************************[ MPLL ]*******************************
void ChangeMPllValue(int mdiv,int pdiv,int sdiv)
{
    rMPLLCON = (mdiv<<14) | (pdiv<<5) | sdiv;		// 400  3  1    -->   MPPL = 800000000
}



void ChangeClockDivider(int armdiv_val, int prediv_val, int hdivn_val,int pdivn_val)
{
	int armdiv=1, prediv=2, hdivn=1, pdivn=1;
	
	switch(armdiv_val) 
	{
		case 11: armdiv=0; break;		// 0	 :	ARMCLK	= MPLL/1
		case 12: armdiv=1; break;		// 1	 :	ARMCLK	= MPLL/2
		case 13: armdiv=2; break;		// 2	 :	ARMCLK	= MPLL/3
		case 14: armdiv=3; break;		// 3	 :	ARMCLK	= MPLL/4
		case 16: armdiv=5; break;		// 5	 :	ARMCLK	= MPLL/6
		case 18: armdiv=7; break;		// 7	 :	ARMCLK	= MPLL/8
		case 112: armdiv=13; break;	// 13 :	ARMCLK	= MPLL/12
		case 116: armdiv=15; break;	// 15 :	ARMCLK	= MPLL/16
	}

	switch(prediv_val) 
	{
		case 11: prediv=0; break;		// 0  :	PREDIV_CLK	= MPLL
		case 12: prediv=1; break;		// 1 :	PREDIV_CLK	= MPLL/2
		case 13: prediv=2; break;		// 2 :	PREDIV_CLK	= MPLL/3
		case 14: prediv=3; break;		// 3 :	PREDIV_CLK	= MPLL/4
	}

	switch(hdivn_val) 
	{
		case 11: hdivn=0; break;		// 0  :	HCLK	= PREDIV_CLK
		case 12: hdivn=1; break;		// 0x1 :	HCLK	= PREDIV_CLK/2
		case 14: hdivn=3; break;		// 0x3 :	HCLK	= PREDIV_CLK/4
	}
	
	switch(pdivn_val) 
	{
		case 11: pdivn=0; break;		// 0	 :	PCLK	= HCLK
		case 12: pdivn=1; break;		// 1	 :	PCLK	= HCLK/2
	}
	
	//Uart_Printf("Clock division change [hdiv:%x, pdiv:%x]\n", hdivn, pdivn);
	//rCLKDIV0 = (hdivn<<1) | pdivn;
	rCLKDIV0 = (armdiv<<9)| (prediv<<4) | (pdivn<<2) | hdivn;

	
  //  if(hdivn!=0)
    //    MMU_SetAsyncBusMode();
    //else 
    //    MMU_SetFastBusMode();
}



//***************************[ UART ]******************************

void Uart_Init(int pclk,int baud)
{
	int i;

	if(pclk == 0)
		pclk    = SYS_PCLK;

	rUFCON0 = rUFCON1 = rUFCON2 = rUFCON3 = 0x0;		//FIFO disable	rUFCON0=0x1;
	rUMCON0 = rUMCON1 = rUMCON2 = rUMCON3 = 0x0;
	rULCON0 = rULCON1 = rULCON2 = rULCON3 = 0x0;		// 默认N 8 1

	i=g_SystemConf.ComPort;	// 全局变量初始为0 (不初始化的情况,且重新上电)
	Uart_Select(i);


	if(i==0)	//UART0   RS232
	{
		if(g_SystemConf.ParityMode==1)                     //奇偶
			rULCON0 |= 0x20;
		else if(g_SystemConf.ParityMode==2)
			rULCON0 |= 0x28;
		
		if(g_SystemConf.DataBits==1)               //数据位
			rULCON0 |= 0x01;
		else if(g_SystemConf.DataBits==2)
			rULCON0 |= 0x02;
		else if(g_SystemConf.DataBits==3)
			rULCON0 |= 0x03;

		if(g_SystemConf.StopBits==1)                          //停止位
			rULCON0 |= 0x04;

		rUCON0  = 0x245;   // Control register
		rUBRDIV0 = ( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0
	}
	else if(i==1)	//UART1  
	{
		if(g_SystemConf.ParityMode==1)                     //奇偶
			rULCON1 |= 0x20;
		else if(g_SystemConf.ParityMode==2)
			rULCON1 |= 0x28;
		
		if(g_SystemConf.DataBits==1)               //数据位
			rULCON1 |= 0x01;
		else if(g_SystemConf.DataBits==2)
			rULCON1 |= 0x02;
		else if(g_SystemConf.DataBits==3)
			rULCON1 |= 0x03;

		if(g_SystemConf.StopBits==1)                          //停止位
			rULCON1 |= 0x04;

		rUCON1  = 0x245;   // Control register
		rUBRDIV1 = ( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0
	}
	else if(i==2)	//UART2  RS422
	{
		if(g_SystemConf.ParityMode==1)                     //奇偶
			rULCON2 |= 0x20;
		else if(g_SystemConf.ParityMode==2)
			rULCON2 |= 0x28;
		
		if(g_SystemConf.DataBits==1)               //数据位
			rULCON2 |= 0x01;
		else if(g_SystemConf.DataBits==2)
			rULCON2 |= 0x02;
		else if(g_SystemConf.DataBits==3)
			rULCON2 |= 0x03;

		if(g_SystemConf.StopBits==1)                          //停止位
			rULCON2 |= 0x04;

		rUCON2  = 0x245;   // Control register
		rUBRDIV2 = ( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0
	}
	else if(i==3)	//UART3  RS485
	{
		if(g_SystemConf.ParityMode==1)                     //奇偶
			rULCON3 |= 0x20;
		else if(g_SystemConf.ParityMode==2)
			rULCON3 |= 0x28;
		
		if(g_SystemConf.DataBits==1)               //数据位
			rULCON3 |= 0x01;
		else if(g_SystemConf.DataBits==2)
			rULCON3 |= 0x02;
		else if(g_SystemConf.DataBits==3)
			rULCON3 |= 0x03;

		if(g_SystemConf.StopBits==1)                          //停止位
			rULCON3 |= 0x04;

		rUCON3  = 0x245;   // Control register
		rUBRDIV3 = ( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0
	}

	for(i=0;i<100;i++);
}




void Uart_InitDownload(int pclk,int baud,int Flag)
{
	int i;
	if(pclk==0)
	pclk=SYS_PCLK;



	rUFCON0=0x0;     //FIFO disable

	rUMCON0=0x0;

	//UART0
	if(Flag == DOWNLOAD)//download
	{
		rULCON0=0x03;     //Normal,No parity,1 stop,8 bit
	}
	if(Flag == FP0)
	{
		rULCON0 = 0x23;     //Normal,o,1 stop,8 bit
	}
	if(Flag == NANO||Flag==FX)
	{
		rULCON0 = 0x2a;     //Normal,e,1 stop,7 bit
	}

	rUCON0=0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
	rUBRDIV0=( (int)(pclk/16.0/baud + 0.5) -1 );

	//UART1
	if(Flag == DOWNLOAD)//download
	{
		rULCON1=0x03;     //Normal,No parity,1 stop,8 bit
	}
	if(Flag == FP0)
	{
		rULCON1 = 0x23;     //Normal,o,1 stop,8 bit
	}
	if(Flag == NANO||Flag==FX)
	{
		rULCON1 = 0x2a;     //Normal,e,1 stop,7 bit
	}

	rUCON1=0x245;
	rUBRDIV1=( (int)(pclk/16.0/baud + 0.5) -1 );

	//UART2
	if(Flag == DOWNLOAD)//download
	{
		rULCON2=0x03;     //Normal,No parity,1 stop,8 bit
	}
	if(Flag == FP0)
	{
		rULCON2= 0x23;     //Normal,o,1 stop,8 bit
	}
	if(Flag == NANO||Flag==FX)
	{
		rULCON2 = 0x2a;     //Normal,e,1 stop,7 bit
	}

	rUCON2=0x245;
	rUBRDIV2=( (int)(pclk/16.0/baud + 0.5) -1 );

	//UART3
	if(Flag == DOWNLOAD)//download
	{
		rULCON3=0x03;     //Normal,No parity,1 stop,8 bit
	}
	if(Flag == FP0)
	{
		rULCON3= 0x23;     //Normal,o,1 stop,8 bit
	}
	if(Flag == NANO||Flag==FX)
	{
		rULCON3 = 0x2a;     //Normal,e,1 stop,7 bit
	}

	rUCON3=0x245;
	rUBRDIV3=( (int)(pclk/16.0/baud + 0.5) -1 );

	for(i=0;i<100;i++);
}


//===================================================================
void Uart_Select(int ch)
{
    whichUart = ch;
}




//*****************************************************************************


void Uart_TxEmpty(int ch)
{
	if(ch==0)
		while(!(rUTRSTAT0 & 0x4)); //wait until tx shifter is empty.
	else if(ch==1)
		while(!(rUTRSTAT1 & 0x4)); //wait until tx shifter is empty.
	else if(ch==2)
		while(!(rUTRSTAT2 & 0x4)); //wait until tx shifter is empty.
	else if(ch==3)
		while(!(rUTRSTAT3 & 0x4)); //wait until tx shifter is empty.

}



void Unitel_SendByte(int data)
{
	unsigned char temp;

	//FeedWacthDog();


	if(whichUart==0)
    	{
		while(!(rUTRSTAT0 & 0x2)); //Wait until THR is empty.
		temp = (unsigned char)data;
		WrUTXH0(data);
   	}

	else if(whichUart==1)
    	{	
		while(!(rUTRSTAT1 & 0x4));  //Wait until THR is empty.
	
		WrUTXH1(data);

		Uart_TxEmpty(1);
	}
	else if(whichUart==2)
    	{	
		while(!(rUTRSTAT2 & 0x4));  //Wait until THR is empty.
	
		WrUTXH2(data);

		Uart_TxEmpty(2);
	}
	else if(whichUart==3)
    	{	
		while(!(rUTRSTAT3 & 0x4));  //Wait until THR is empty.
	
		WrUTXH3(data);

		Uart_TxEmpty(3);
	}
		
}

//=====================================================================
void Uart_SendByte(int data)
{
	if(whichUart==0)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT0 & 0x2));
			// Delay(1);                 //because the slow response of hyper_terminal 
			WrUTXH0('\r');
		}
		while(!(rUTRSTAT0 & 0x2));   //Wait until THR is empty.
		//  Delay(1);
		WrUTXH0(data);
	}
	else if(whichUart==1)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT1 & 0x2));
			//Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH1 = '\r';
		}
		while(!(rUTRSTAT1 & 0x2));   //Wait until THR is empty.
		//Delay(1);
		rUTXH1 = data;
	}   
	else if(whichUart==2)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT2 & 0x2));
			//Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH2 = '\r';
		}
		while(!(rUTRSTAT2 & 0x2));   //Wait until THR is empty.
		//Delay(1);
		rUTXH2 = data;
	}       
	else if(whichUart==3)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT3 & 0x2));
			//Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH3 = '\r';
		}
		while(!(rUTRSTAT3 & 0x2));   //Wait until THR is empty.
		//Delay(1);
		rUTXH3 = data;
	}       
}               



//====================================================================
void Uart_SendString(char *pt)
{
    while(*pt)
        Uart_SendByte(*pt++);
}


//=====================================================================
//If you don't use vsprintf(), the code size is reduced very much.
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
}


void putch(int data)
{
	if(whichUart==0)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT0 & 0x2));
			Delay(1);                 //because the slow response of hyper_terminal 
			WrUTXH0('\r');
		}
		while(!(rUTRSTAT0 & 0x2));   //Wait until THR is empty.
		Delay(1);
		WrUTXH0(data);
	}
	else if(whichUart==1)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT1 & 0x2));
			Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH1 = '\r';
		}
		while(!(rUTRSTAT1 & 0x2));   //Wait until THR is empty.
		Delay(1);
		rUTXH1 = data;
	}   
	else if(whichUart==2)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT2 & 0x2));
			Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH2 = '\r';
		}
		while(!(rUTRSTAT2 & 0x2));   //Wait until THR is empty.
		Delay(1);
		rUTXH2 = data;
	}       
	else if(whichUart==3)
	{
		if(data=='\n')
		{
			while(!(rUTRSTAT3 & 0x2));
			Delay(1);                 //because the slow response of hyper_terminal 
			rUTXH3 = '\r';
		}
		while(!(rUTRSTAT3 & 0x2));   //Wait until THR is empty.
		Delay(1);
		rUTXH3 = data;
	}       
}               


//====================================================================
void putStr(char *pt)
{
    while(*pt)
        putch(*pt++);
}




//=====================================================================
char getch(void)
{
	if(whichUart==0)
	{       
		while(!(rUTRSTAT0 & 0x1)); //Receive data ready
		return RdURXH0();
	}
	else if(whichUart==1)
	{       
		while(!(rUTRSTAT1 & 0x1)); //Receive data ready
		return RdURXH1();
	}
	else if(whichUart==2)
	{
		while(!(rUTRSTAT2 & 0x1)); //Receive data ready
		return RdURXH2();
	}
	else if(whichUart==3)
	{
		while(!(rUTRSTAT3 & 0x1)); //Receive data ready
		return RdURXH3();
	}
	return 0x0;
}






char Uart_Getch(void)
{
	//INT8U temp=0;
	INT8U temp=0xff;
	INT16U Flag_Comm = 0;	
	int i;
	U32 ErrorLoop;

	//FeedWacthDog();
	
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;


	if(whichUart==0)
	{	    
		while(!(rUTRSTAT0 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
		{
			for(i=0;i<50;i++);	// 2000
			Flag_Comm++;
			//if(TRUE == g_UartTimeOut)
			//	break;
		}
		if(Flag_Comm >= ErrorLoop)
		{
			temp=0;
		}
		else
		{
			temp=RdURXH0();
			g_Timer2Uart.Timer_Cur = 0;
		}
		//return temp;

	}
	else if(whichUart==1)
	{		
		while(!(rUTRSTAT1 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
		{
			for(i=0;i<50;i++);	// 2000
			Flag_Comm++;
			//if(TRUE == g_UartTimeOut)
			//	break;
		}
		if(Flag_Comm >= ErrorLoop)
		{
			temp=0xff;
		}
		else
		{
			temp=RdURXH1();
			g_Timer2Uart.Timer_Cur = 0;
		}

		//return temp;
	}
	else if(whichUart==2)
	{		
		while(!(rUTRSTAT2 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
		{
			for(i=0;i<50;i++);	// 2000
			Flag_Comm++;
			//if(TRUE == g_UartTimeOut)
			//	break;
		}
		if(Flag_Comm >= ErrorLoop)
		{
			temp=0xff;
		}
		else
		{
			temp=RdURXH2();
			g_Timer2Uart.Timer_Cur = 0;
		}

		//return temp;
	}
	else if(whichUart==3)
	{		
		while(!(rUTRSTAT3 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
		{
			for(i=0;i<50;i++);	// 2000
			Flag_Comm++;
			//if(TRUE == g_UartTimeOut)
			//	break;
		}
		if(Flag_Comm >= ErrorLoop)
		{
			temp=0xff;
		}
		else
		{
			temp=RdURXH3();
			g_Timer2Uart.Timer_Cur = 0;
		}

		//return temp;
	}
	
	return temp;
	
}






char Uart_DownloadGetch(U8 * ErrFlag)
{
	INT8U temp=0xff;
	
	Flag_Comm = 0;	
	
	pISR_TIMER0 = (int)Timer0Done;

	rINTMSK1 &=~(BIT_TIMER0);


	*ErrFlag = 0;

	rTCFG0 &= 0xffffff00;
	rTCFG0 |= 0xff;

	rTCFG1 &= 0xfffffff0;
	rTCFG1 |= 0x2;

	rTCNTB0=50000;

	rTCMPB0=1000-700;


	rTCON &= 0xfffffff0;	//auto reload, inverter off, manual update, dead zone off
	rTCON |= 0xa;

	rTCON &= 0xfffffff0;	//start PWM operation
	rTCON |= 0x9;


	//FeedWacthDog();

	if(whichUart==0)
	{	    
		while(!(rUTRSTAT0 & 0x1)) //Receive data read
		{
			//FeedWacthDog();
			if (Flag_Comm == 1)
				break;
		}
		rINTMSK1 |= BIT_TIMER0;

		rTCON &= 0xfffffff0;
		rTCON |= 0xa;

		if(Flag_Comm == 1)
		{
			temp=0xff;
			*ErrFlag = 1;
		}
		else 
			temp=RdURXH0();
		rINTMSK1 &= ~(BIT_EINT1);
		//return temp;

	}
	else  if(whichUart==1)
	{
		while(!(rUTRSTAT1 & 0x1)) //Receive data ready
		{
			//FeedWacthDog();
			if (Flag_Comm == 1)
				break;
		}
		rINTMSK1 |= BIT_TIMER0;

		rTCON &= 0xfffffff0;
		rTCON |= 0xa;

		if(Flag_Comm == 1)
		{
			temp=0;
			*ErrFlag = 1;
		}
		else 
			temp=RdURXH1();
		//rINTMSK1 &= ~(BIT_EINT1);
		//return temp;		
	}
	else  if(whichUart==2)
	{
		while(!(rUTRSTAT2 & 0x1)) //Receive data ready
		{
			//FeedWacthDog();
			if (Flag_Comm == 1)
				break;
		}
		rINTMSK1 |= BIT_TIMER0;

		rTCON &= 0xfffffff0;
		rTCON |= 0xa;

		if(Flag_Comm == 1)
		{
			temp=0;
			*ErrFlag = 1;
		}
		else 
			temp=RdURXH2();
		//rINTMSK1 &= ~(BIT_EINT1);
		//return temp;		
	}
	else  if(whichUart==3)
	{
		while(!(rUTRSTAT3 & 0x1)) //Receive data ready
		{
			//FeedWacthDog();
			if (Flag_Comm == 1)
				break;
		}
		rINTMSK1 |= BIT_TIMER0;

		rTCON &= 0xfffffff0;
		rTCON |= 0xa;

		if(Flag_Comm == 1)
		{
			temp=0;
			*ErrFlag = 1;
		}
		else 
			temp=RdURXH3();
		//rINTMSK1 &= ~(BIT_EINT1);
		//return temp;		
	}
	
	return temp;
}




void __irq Timer0Done(void)		// 0.65Hz		1.5s
{
	Flag_Comm = 1;

	rTCON &= 0xfffffff0;
	rTCON |= 0xa; 

	rSRCPND1 = BIT_TIMER0;       //Clear pending bit
	rINTPND1 = BIT_TIMER0;
	rINTPND1 = BIT_TIMER0;                    //Prevent an double interrupt pending

	rINTMSK1|=BIT_TIMER0; 
	
}




//***************************[ BOARD BEEP ]*******************************
/*
void Buzzer_Freq_Set( U32 freq )
{
	rGPBCON &= ~3;			//set GPB0 as tout0, pwm output
	rGPBCON |= 2;
		
	rTCFG0 &= ~0xff;
	rTCFG0 |= 15;			//prescaler = 15+1
	rTCFG1 &= ~0xf;
	rTCFG1 |= 2;			//mux = 1/8
	rTCNTB0 = (SYS_PCLK>>7)/freq;
	rTCMPB0 = rTCNTB0>>1;	// 50%
	rTCON &= ~0x1f;
	rTCON |= 0xb;			//disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0
	rTCON &= ~2;			//clear manual update bit
}

void Buzzer_Stop( void )
{
	rGPBCON &= ~3;			//set GPB0 as output
	rGPBCON |= 1;
	rGPBDAT &= ~1;
}

//[ BOARD BEEP ]
void Beep(U32 freq, U32 ms)
{
	Buzzer_Freq_Set( freq ) ;
	Delay( ms ) ;
	Buzzer_Stop() ;
}
*/

void Beep(int BeepStatus)
{
	if (BeepStatus==0)
		rGPEDAT &= ~(1<<9);		//GPE9 output 0;
	else
		rGPEDAT |= (1<<9);		//GPE9 output 1;

}



/************************* General Library **********************/

void * malloc(unsigned nbyte) 
/*Very simple; Use malloc() & free() like Stack*/
//void *mallocPt=Image$$RW$$Limit;
{
    void *returnPt=mallocPt;

    mallocPt= (int *)mallocPt+nbyte/4+((nbyte%4)>0); //to align 4byte

    if( (int)mallocPt > HEAPEND )
    {
		mallocPt=returnPt;
		return NULL;
    }
    return returnPt;
}


void free(void *pt)
{
    mallocPt=pt;
}



//////////////////////////////////////////////////////////////////////////
//////////////   090721  下载程序到NandFlash  //////////////////////////
//////////////////////////////////////////////////////////////////////////
void Uart_GetString(char *string)
{
	char *string2 = string;
	char c;
	while((c = Uart_Getch_DNW())!='\r')
	{
		if(c=='\b')
		{
			if( (int)string2 < (int)string )
			{
				Uart_Printf("\b \b");
				string--;
			}
		}
		else 
		{
			*string++ = c;
			Uart_SendByte(c);
		}
	}
	*string='\0';
	Uart_SendByte('\n');
}


void SDRAM_test(unsigned int rambase,unsigned int ramend)
{
	unsigned int base,end,i,j,k,flag=1;
	char temp[10];
	base=rambase;
	end =ramend;
	Uart_Printf("\nTest SDRAM?(y/n)\n");
	Uart_GetString(temp);
	while(flag)
	{
		if((temp[0]=='n')||(temp[0]=='N')) 
			flag=0;
		else if((temp[0]=='y')||(temp[0]=='Y'))
		{
			Uart_Printf("SDRAM test start!\n");
			for(k=1;k<=(1<<7);k<<=1)
			{
				Uart_Printf(".");
				j=0;
				for(i=base;i<end;i++)
				{
					((volatile unsigned char *)base)[j++] = k;
				}
				j=0;  
				for(i=base;i<end;i++)
				{
					if(((volatile unsigned char *)base)[j++] != k)
					Uart_Printf("SDRAM error !!!"); 
				}  
			} 
			Uart_Printf("\nSDRAM test complete!");  
			flag=0;
		}  
		else
		{
			Uart_Printf("\nInvalid input!\n");
			Uart_GetString(temp);  
		}  
	}
}

char Uart_Getch_DNW(void)
{
	char temp = 0xff;
	
	if(whichUart==0)
	{       
		while(!(rUTRSTAT0 & 0x1)); //Receive data ready
		temp = RdURXH0();
	}
	else if(whichUart==1)
	{       
		while(!(rUTRSTAT1 & 0x1)); //Receive data ready
		temp = RdURXH1();
	}
	else if(whichUart==2)
	{
		while(!(rUTRSTAT2 & 0x1)); //Receive data ready
		temp = RdURXH2();
	}
	else if(whichUart==3)
	{
		while(!(rUTRSTAT3 & 0x1)); //Receive data ready
		temp = RdURXH3();
	}
	
	return temp;
}



void Uart_Init_DNW(int pclk,int baud)
{
	int i;
	
	if(pclk == 0)
		pclk    =  SYS_PCLK;

	Uart_Select(0);	// Select Uart0 232  ,  DNW 使用。LoadSysConf 再重新设置


	rUFCON0 = 0x0;   //UART channel 0 FIFO control register, FIFO disable
	rUMCON0 = 0x0;   //UART chaneel 0 MODEM control register, AFC disable

	rULCON0 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits
	 //    [10]       [9]     [8]        [7]        [6]      [5]         [4]           [3:2]        [1:0]
	 // Clock Sel,  Tx Int,  Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode
	 //     0          1       0    ,     0          1        0           0     ,       01          01
	 //   PCLK       Level    Pulse    Disable    Generate  Normal      Normal        Interrupt or Polling
	rUCON0  = 0x245;   // Control register
	rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0

	for(i=0;i<100;i++);
}


