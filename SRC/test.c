#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "2416addr.h"
#include "2416lib.h"
#include "..\inc\nand.h"
#include "..\inc\def.h"
#include "..\inc\lcd.h"
#include "..\inc\systemconf.h"

#include "..\inc\test.h"

extern INTERALMEMORY g_InteralMemory;
extern U8 differCount[24];	// 24 路IN 滤波处理用

//extern static void __irq Eint2Int(void);	// 高速输入2
//extern static void __irq Eint3Int(void);	// 高速输入2
//extern static void __irq Eint4_7Int(void);	// 高速输入2

static void __irq Eint3Int(void)	// 高速输入1
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 3) & 0x1)
	{
		if(0xffff == (g_InteralMemory.Word[10] & 0xffff))
		{
			g_InteralMemory.Word[10] = 0;
			g_InteralMemory.Word[11]++;
		}
		else
			g_InteralMemory.Word[10]++;
	}
		
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}


static void __irq Eint2Int(void)	// 高速输入2
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 2) & 0x1)
	{
		if(0xffff == (g_InteralMemory.Word[12] & 0xffff))
		{
			g_InteralMemory.Word[12] = 0;
			g_InteralMemory.Word[13]++;
		}
		else
			g_InteralMemory.Word[12]++;
	}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}


static void __irq Eint4_7Int(void)
{
	int i;
	for(i=0; i<20; i++);

	if (rEINTPEND & (1<<4))	// Eint4	// 高速输入3
	{
		if((rGPFDAT >> 4) & 0x1)
		{
			if(0xffff == (g_InteralMemory.Word[14] & 0xffff))
			{
				g_InteralMemory.Word[14] = 0;
				g_InteralMemory.Word[15]++;
			}
			else
				g_InteralMemory.Word[14]++;
		}

		rEINTPEND=(1<<4);		
	}
	else if (rEINTPEND & (1<<6))	// Eint6	// 高速输入4
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			if(0xffff == (g_InteralMemory.Word[16] & 0xffff))
			{
				g_InteralMemory.Word[16] = 0;
				g_InteralMemory.Word[17]++;
			}
			else
				g_InteralMemory.Word[16]++;
		}

		rEINTPEND=(1<<6);		
	}

	ClearPending(BIT_EINT4_7);
}



void Test_In_Init()	// IN 5~24 共20 路
{
	DWORD tmp;
	int i;

	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8))
		& (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14)) & (~(0x3<< 16)) & (~(0x3<< 18))
		& (~(0x3<< 22)) & (~(0x3<< 24)) & (~(0x3<< 26));
	rGPECON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8)
		| (0x0<<10) | (0x0<<12) | (0x0<<14) | (0x0<<16) | (0x0<<18)
		| (0x0<<22) | (0x0<<24) | (0x0<<26);			//set GPE 0~9   11~13 as input

	tmp = rGPFCON & (~(0x3<< 10)) & (~(0x3<< 14));
	rGPFCON = tmp | (0x0<<10) | (0x0<<14);		//set GPF5 7 as input	
	
	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8));
	rGPGCON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8);	//set GPG0 1 2 3 4 as input	

	for(i=0; i<24; i++)
		differCount[i] = 0;
	
}

void Test_Out_Init()		// OUT 3~16 共14 路
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 18)) & (~(0x3<< 20));
	rGPBCON = tmp | (0x1<<8) | (0x1<<10) | (0x1<<18) | (0x1<<20);	//set GPB4 5 9 10 as output	
	// 输入输出都反向
	rGPBDAT |= (1<<5);	// OUT11	GPB5
	rGPBDAT |= (1<<10);	// OUT12	GPB10
	rGPBDAT |= (1<<9);	// OUT15	GPB9
	rGPBDAT |= (1<<4);	// OUT16	GPB4

	tmp = rGPHCON & (~(0x3<< 6)) & (~(0x3<< 20)) & (~(0x3<< 22)) & (~(0x3<< 26)) & (~(0x3<< 28));
	rGPHCON = tmp | (0x1<<6) | (0x1<<20) | (0x1<<22) | (0x1<<26) | (0x1<<28); //set GPH3 10 11 13 14 as output	
	// 输入输出都反向
	rGPHDAT |= (1<<10);	// OUT8	GPH10
	rGPHDAT |= (1<<14);	// OUT9	GPH14
	rGPHDAT |= (1<<13);	// OUT10	GPH13
	rGPHDAT |= (1<<3);	// OUT13	GPH3
	rGPHDAT |= (1<<11);	// OUT14	GPH11
	
	tmp = rGPLCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 18)) & (~(0x3<< 26));
	rGPLCON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<18) | (0x1<<26); //set GPH0 1 2 9 13 as output	
	// 输入输出都反向
	rGPLDAT |= (1<<1);	// OUT3	GPL1
	rGPLDAT |= (1<<13);	// OUT4	GPL13
	rGPLDAT |= (1<<9);	// OUT5	GPL9
	rGPLDAT |= (1<<0);	// OUT6	GPL0
	rGPLDAT |= (1<<2);	// OUT7	GPL2
	
}



void Test_Init()
{
	Test_In_Init();
	Test_Out_Init();

	Test_HighSpeedIn_Init();
	Test_HighSpeedOut_Init();	// 需重新配置rTCFG0，

	// 画面下载用的Timer0, 下载时会重新配置rTCFG0，
	//DA 启动前需重新配置rTCFG0
//	Test_DA_Init();
}



void Test_HighSpeedIn_Init()	// IN 1~4   4路高速输入
{
	DWORD tmp;
	int i;

	tmp = rGPFCON & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 12));
	rGPFCON = tmp | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<12);	//set GPF2 3 4 6 as EINT	

	rGPFUDP &= (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 12)); //GPF2 3 4 6 up down disable

	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x4<<24);	// Eint6	Rising edge triggered	

	pISR_EINT3= (U32)Eint3Int;		// IN1
	pISR_EINT2= (U32)Eint2Int;		// IN2
	pISR_EINT4_7= (U32)Eint4_7Int;	// IN3 4

	//rINTMOD1 |= BIT_EINT2|BIT_EINT3|BIT_EINT4_7;	// FIQ

	rEINTPEND = 0xffffff;

	rSRCPND1 |= BIT_EINT2|BIT_EINT3|BIT_EINT4_7; //to clear the previous pending states
	rINTPND1 |= BIT_EINT2|BIT_EINT3|BIT_EINT4_7;

	rEINTMASK &= ~((1<<2)|(1<<3)|(1<<4)|(1<<6));
	rINTMSK1 &= ~(BIT_EINT2|BIT_EINT3|BIT_EINT4_7);

	for(i=0; i<8; i++)
		g_InteralMemory.Word[10+i] = 0; 	// 4 路高速输入计数(双字)
	
}

void Test_HighSpeedIn_Stop()
{
	rEINTMASK |=((1<<2)|(1<<3)|(1<<4)|(1<<6));
	rINTMSK1 |=(BIT_EINT2|BIT_EINT3|BIT_EINT4_7);
}




void Test_HighSpeedOut_Init()	// OUT 1~2   2路高速输出
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 0)) & (~(0x3<< 2));
	rGPBCON = tmp | (0x2<<0) | (0x2<<2);	//set GPB0 1 as TOUT	


	// Timer1 Initialize	----HS Out1
	pISR_TIMER1 = (int)Timer1_HS_Out1;	// Timer ISR for HS Out
	rTCFG0 &= ~(0xff << 0); 
	rTCFG0 |= (10 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG1 &= ~(0xf << 4); 
	rTCFG1 |= (0x0 << 4); 	// Interrupt, Mux0=1/2
	rTCNTB1 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
	rTCMPB1 = 15;
	//rINTMOD1 |= BIT_TIMER1;	// FIQ
	HS_Out1_Stop();
	g_InteralMemory.Word[20] = 1000;
	g_InteralMemory.Word[21] = 0;

	
	// Timer0 Initialize	----HS Out2
	pISR_TIMER0 = (int)Timer0_HS_Out2;	// Timer ISR for HS Out
	rTCFG0 &= ~(0xff << 0); 
	rTCFG0 |= (10 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG1 &= ~(0xf << 0); 
	rTCFG1 |= (0x0 << 0); 	// Interrupt, Mux0=1/2
	rTCNTB0 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
	rTCMPB0 = 15;
	//rINTMOD1 |= BIT_TIMER0;	// FIQ
	HS_Out2_Stop();
	g_InteralMemory.Word[22] = 1000;
	g_InteralMemory.Word[23] = 0;

}


void HS_Out1_Start(void)
{
	DWORD tmp;
	U32 Out1_Frequence;

	if((0==g_InteralMemory.Word[34]) && (0==g_InteralMemory.Word[35]))
	{
		HS_Out1_Stop();
		return;
	}
	
	Out1_Frequence = (g_InteralMemory.Word[21]<<16) + (g_InteralMemory.Word[20]&0xffff);
	if(0 == Out1_Frequence)
		Out1_Frequence = 1;
	// rTCNTB1  16位Out1_Frequence<47 则rTCNTB1 溢出, 得修改rTCFG0 或rTCFG1
	// Out1_Frequence>3M (100K*30) 这个时候也得考虑
	rTCNTB1= 30*100909/Out1_Frequence;	// 100909  	100K  	100000
	rTCMPB1 = rTCNTB1/2;
	
	rINTMSK1 &=~(BIT_TIMER1);
	tmp = rTCON & (~(0xf << 8));
	rTCON = tmp | (2 << 8);		/* update TCVNTB1, stop					*/
	rTCON = tmp | (9 << 8);		/* interval mode,  start				*/	
}

void HS_Out1_Stop(void)
{
	rINTMSK1 |= BIT_TIMER1;
	rTCON &= ~(1 << 8);			/* Timer1, stop							*/

	//g_InteralMemory.Word[24] = g_InteralMemory.Word[25] = 0;
	g_InteralMemory.Word[28] = g_InteralMemory.Word[29] = 0;
	
	g_InteralMemory.Bit[200] = g_InteralMemory.Bit[201] = 0;
}



void HS_Out2_Start(void)
{
	DWORD tmp;
	U32 Out2_Frequence;
	
	if((0==g_InteralMemory.Word[36]) && (0==g_InteralMemory.Word[37]))
	{
		HS_Out2_Stop();
		return;
	}
	
	Out2_Frequence = (g_InteralMemory.Word[23]<<16) + (g_InteralMemory.Word[22]&0xffff);
	if(0 == Out2_Frequence)
		Out2_Frequence = 1;
	rTCNTB0= 30*100909/Out2_Frequence;	// 100909  	100K  	100000
	rTCMPB0 = rTCNTB0/2;
	
	rINTMSK1 &=~(BIT_TIMER0);
	tmp = rTCON & (~(0xff << 0));	// dead zone Disable
	rTCON = tmp | (2 << 0);		/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 0);		/* interval mode,  start				*/	
}

void HS_Out2_Stop(void)
{
	rINTMSK1 |= BIT_TIMER0;
	rTCON &= ~(1 << 0);			/* Timer0, stop							*/

	//g_InteralMemory.Word[26] = g_InteralMemory.Word[27] = 0;
	g_InteralMemory.Word[38] = g_InteralMemory.Word[39] = 0;
	
	g_InteralMemory.Bit[202] = g_InteralMemory.Bit[203] = 0;
}



void __irq Timer1_HS_Out1(void) // 可通过示波器确定频率
{
	U32 setNum, currentNum;

	if(0xffff == (g_InteralMemory.Word[24] & 0xffff))
	{
		g_InteralMemory.Word[24] = 0;
		g_InteralMemory.Word[25]++;
	}
	else
		g_InteralMemory.Word[24]++;

	if(0xffff == (g_InteralMemory.Word[28] & 0xffff))
	{
		g_InteralMemory.Word[28] = 0;
		g_InteralMemory.Word[29]++;
	}
	else
		g_InteralMemory.Word[28]++;

	setNum = (g_InteralMemory.Word[35]<<16) + (g_InteralMemory.Word[34]&0xffff);
	currentNum = (g_InteralMemory.Word[29]<<16) + (g_InteralMemory.Word[28]&0xffff);

	if(currentNum >= setNum)
	{
		HS_Out1_Stop();
	}


	ClearPending((U32)BIT_TIMER1);

}



void __irq Timer0_HS_Out2(void) // 可通过示波器确定频率
{
	U32 setNum, currentNum;

	if(0xffff == (g_InteralMemory.Word[26] & 0xffff))
	{
		g_InteralMemory.Word[26] = 0;
		g_InteralMemory.Word[27]++;
	}
	else
		g_InteralMemory.Word[26]++;

	if(0xffff == (g_InteralMemory.Word[38] & 0xffff))
	{
		g_InteralMemory.Word[38] = 0;
		g_InteralMemory.Word[39]++;
	}
	else
		g_InteralMemory.Word[38]++;


	setNum = (g_InteralMemory.Word[37]<<16) + (g_InteralMemory.Word[36]&0xffff);
	currentNum = (g_InteralMemory.Word[39]<<16) + (g_InteralMemory.Word[38]&0xffff);

	if(currentNum >= setNum)
	{
		HS_Out2_Stop();
	}


	ClearPending((U32)BIT_TIMER0);

}



void Test_DA_Init()
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	


	// Timer3 Initialize	----DA
	pISR_TIMER3 = (int)Timer3_DA;	// Timer ISR for DA
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
	rTCNTB3 = 300;    			// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1K
	rTCMPB3 = 150;
	DA_Stop();
	g_InteralMemory.Word[30] = 0;

}


void DA_Start(void)
{
	DWORD tmp;
	
	rTCNTB3= 300;	
	rTCMPB3 = (rTCNTB3*g_InteralMemory.Word[30])/(10000*1.326);// 最大10V , 小数点后3 位10*1000
	if(rTCMPB3 == rTCNTB3)
		rTCMPB3 -= 1;

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	
	
	rINTMSK1 &=~(BIT_TIMER3);
	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
	rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
	rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
}

void DA_Stop(void)
{
	DWORD tmp;

	rTCNTB3= 300;	
	rINTMSK1 |= BIT_TIMER3;
	rTCON &= ~(1 << 16);			/* Timer3, stop							*/

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (1<<6);	//set GPB3 as OUT	
	rGPBDAT &= ~(1<<3);

	g_InteralMemory.Word[32] = g_InteralMemory.Word[33] = 0;
}



void __irq Timer3_DA(void) // 可通过示波器确定频率
{
	if(0xffff == (g_InteralMemory.Word[32] & 0xffff))
	{
		g_InteralMemory.Word[32] = 0;
		g_InteralMemory.Word[33]++;
	}
	else
		g_InteralMemory.Word[32]++;


	ClearPending((U32)BIT_TIMER3);

}


////////////////////////////////////////////////////////////////////////////

void Test_GetIn()
{
	int i;
	
	g_InteralMemory.Bit[50] = (rGPFDAT >> 3) & 0x1;	// IN1	GPF3
	g_InteralMemory.Bit[51] = (rGPFDAT >> 2) & 0x1;	// IN2	GPF2
	g_InteralMemory.Bit[52] = (rGPFDAT >> 4) & 0x1;	// IN1	GPF3
	g_InteralMemory.Bit[53] = (rGPFDAT >> 6) & 0x1;	// IN2	GPF6
	
	g_InteralMemory.Bit[54] = (rGPGDAT >> 2) & 0x1;	// IN5	GPG2
	g_InteralMemory.Bit[55] = (rGPGDAT >> 4) & 0x1;	// IN6	GPG4

	g_InteralMemory.Bit[56] = (rGPEDAT >> 3) & 0x1;	// IN7	GPE3
	g_InteralMemory.Bit[57] = (rGPEDAT >> 11) & 0x1;	// IN8	GPE11

	g_InteralMemory.Bit[58] = (rGPGDAT >> 3) & 0x1;	// IN9	GPG3
	g_InteralMemory.Bit[59] = (rGPGDAT >> 1) & 0x1;	// IN10	GPG1

	g_InteralMemory.Bit[60] = (rGPFDAT >> 7) & 0x1;	// IN11	GPF7
	g_InteralMemory.Bit[61] = (rGPFDAT >> 5) & 0x1;	// IN12	GPF5

	g_InteralMemory.Bit[62] = (rGPEDAT >> 7) & 0x1;	// IN13	GPE7
	g_InteralMemory.Bit[63] = (rGPEDAT >> 6) & 0x1;	// IN14	GPE6
	g_InteralMemory.Bit[64] = (rGPEDAT >> 8) & 0x1;	// IN15	GPE8
	//g_InteralMemory.Bit[65] = (rGPEDAT >> 10) & 0x1;	// IN16	GPE10
	g_InteralMemory.Bit[65] = (rGPGDAT >> 0) & 0x1;	// IN16	GPG0
	g_InteralMemory.Bit[66] = (rGPEDAT >> 1) & 0x1;	// IN17	GPE1
	g_InteralMemory.Bit[67] = (rGPEDAT >> 13) & 0x1;	// IN18	GPE13
	g_InteralMemory.Bit[68] = (rGPEDAT >> 4) & 0x1;	// IN19	GPE4
	g_InteralMemory.Bit[69] = (rGPEDAT >> 2) & 0x1;	// IN20	GPE2
	g_InteralMemory.Bit[70] = (rGPEDAT >> 0) & 0x1;	// IN21	GPE0
	g_InteralMemory.Bit[71] = (rGPEDAT >> 12) & 0x1;	// IN22	GPE12
	g_InteralMemory.Bit[72] = (rGPEDAT >> 9) & 0x1;	// IN23	GPE9
	g_InteralMemory.Bit[73] = (rGPEDAT >> 5) & 0x1;	// IN24	GPE5


	for(i=0; i<24; i++)
	{
		if(g_InteralMemory.Bit[50+i] != g_InteralMemory.Bit[i])
			differCount[i]++;

		if(differCount[i]>1) // 控制主循环多少次才确认IN 变化
		{
			g_InteralMemory.Bit[i] = g_InteralMemory.Bit[50+i];
			differCount[i] = 0;
		}
	}

	// 映射到OUT
	for(i=2; i<16; i++)
		g_InteralMemory.Bit[100+i] = g_InteralMemory.Bit[i];
}


//void Test_SetOut(U8 outNum, U8 state)
void Test_SetOut()
{
	if(g_InteralMemory.Bit[102] != g_InteralMemory.Bit[152])
	{
		(g_InteralMemory.Bit[102]) ? (rGPLDAT |= (1<<1)) : (rGPLDAT &= ~(1<<1));	// OUT3	GPL1
		g_InteralMemory.Bit[152] = g_InteralMemory.Bit[102];
	}
	if(g_InteralMemory.Bit[103] != g_InteralMemory.Bit[153])
	{
		(g_InteralMemory.Bit[103]) ? (rGPLDAT |= (1<<13)) : (rGPLDAT &= ~(1<<13));	// OUT4	GPL13
		g_InteralMemory.Bit[153] = g_InteralMemory.Bit[103];
	}
	if(g_InteralMemory.Bit[104] != g_InteralMemory.Bit[154])
	{
		(g_InteralMemory.Bit[104]) ? (rGPLDAT |= (1<<9)) : (rGPLDAT &= ~(1<<9));	// OUT5	GPL9
		g_InteralMemory.Bit[154] = g_InteralMemory.Bit[104];
	}
	if(g_InteralMemory.Bit[105] != g_InteralMemory.Bit[155])
	{
		(g_InteralMemory.Bit[105]) ? (rGPLDAT |= (1<<0)) : (rGPLDAT &= ~(1<<0));	// OUT6	GPL0
		g_InteralMemory.Bit[155] = g_InteralMemory.Bit[105];
	}
	if(g_InteralMemory.Bit[106] != g_InteralMemory.Bit[156])
	{
		(g_InteralMemory.Bit[106]) ? (rGPLDAT |= (1<<2)) : (rGPLDAT &= ~(1<<2));	// OUT7	GPL2
		g_InteralMemory.Bit[156] = g_InteralMemory.Bit[106];
	}
	if(g_InteralMemory.Bit[107] != g_InteralMemory.Bit[157])
	{
		(g_InteralMemory.Bit[107]) ? (rGPHDAT |= (1<<10)) : (rGPHDAT &= ~(1<<10));	// OUT8	GPH10
		g_InteralMemory.Bit[157] = g_InteralMemory.Bit[107];
	}
	if(g_InteralMemory.Bit[108] != g_InteralMemory.Bit[158])
	{
		(g_InteralMemory.Bit[108]) ? (rGPHDAT |= (1<<14)) : (rGPHDAT &= ~(1<<14));	// OUT9	GPH14
		g_InteralMemory.Bit[158] = g_InteralMemory.Bit[108];
	}
	if(g_InteralMemory.Bit[109] != g_InteralMemory.Bit[159])
	{
		(g_InteralMemory.Bit[109]) ? (rGPHDAT |= (1<<13)) : (rGPHDAT &= ~(1<<13));	// OUT10	GPH13
		g_InteralMemory.Bit[159] = g_InteralMemory.Bit[109];
	}
	if(g_InteralMemory.Bit[110] != g_InteralMemory.Bit[160])
	{
		(g_InteralMemory.Bit[110]) ? (rGPBDAT |= (1<<5)) : (rGPBDAT &= ~(1<<5));	// OUT11	GPB5
		g_InteralMemory.Bit[160] = g_InteralMemory.Bit[110];
	}
	if(g_InteralMemory.Bit[111] != g_InteralMemory.Bit[161])
	{
		(g_InteralMemory.Bit[111]) ? (rGPBDAT |= (1<<10)) : (rGPBDAT &= ~(1<<10));	// OUT12	GPB10
		g_InteralMemory.Bit[161] = g_InteralMemory.Bit[111];
	}
	if(g_InteralMemory.Bit[112] != g_InteralMemory.Bit[162])
	{
		(g_InteralMemory.Bit[112]) ? (rGPHDAT |= (1<<3)) : (rGPHDAT &= ~(1<<3));	// OUT13	GPH3
		g_InteralMemory.Bit[162] = g_InteralMemory.Bit[112];
	}
	if(g_InteralMemory.Bit[113] != g_InteralMemory.Bit[163])
	{
		(g_InteralMemory.Bit[113]) ? (rGPHDAT |= (1<<11)) : (rGPHDAT &= ~(1<<11));	// OUT14	GPH11
		g_InteralMemory.Bit[163] = g_InteralMemory.Bit[113];
	}
	if(g_InteralMemory.Bit[114] != g_InteralMemory.Bit[164])
	{
		(g_InteralMemory.Bit[114]) ? (rGPBDAT |= (1<<9)) : (rGPBDAT &= ~(1<<9));	// OUT15	GPB9
		g_InteralMemory.Bit[164] = g_InteralMemory.Bit[114];
	}
	if(g_InteralMemory.Bit[115] != g_InteralMemory.Bit[165])
	{
		(g_InteralMemory.Bit[115]) ? (rGPBDAT |= (1<<4)) : (rGPBDAT &= ~(1<<4));	// OUT16	GPB4
		g_InteralMemory.Bit[165] = g_InteralMemory.Bit[115];
	}
	
}

void Test_Run()
{
	Test_GetIn();
	Test_SetOut();

	// HS_Out1
	if(0 == g_InteralMemory.Bit[201])	// HS_Out1 当前状态停止
	{
		if(1 == g_InteralMemory.Bit[200])	// Set HS_Out1
		{
			HS_Out1_Start();
			g_InteralMemory.Bit[201] = g_InteralMemory.Bit[200];
		}
	}
	if(1 == g_InteralMemory.Bit[201])	// HS_Out1 当前状态启动
	{
		if(0 == g_InteralMemory.Bit[200])	// ReSet HS_Out1
		{
			HS_Out1_Stop();
			g_InteralMemory.Bit[201] = g_InteralMemory.Bit[200];
		}
	}

	// HS_Out2
	if(0 == g_InteralMemory.Bit[203])	// HS_Out2 当前状态停止
	{
		if(1 == g_InteralMemory.Bit[202])	// Set HS_Out2
		{
			HS_Out2_Start();
			g_InteralMemory.Bit[203] = g_InteralMemory.Bit[202];
		}
	}
	if(1 == g_InteralMemory.Bit[203])	// HS_Out2 当前状态启动
	{
		if(0 == g_InteralMemory.Bit[202])	// ReSet HS_Out2
		{
			HS_Out2_Stop();
			g_InteralMemory.Bit[203] = g_InteralMemory.Bit[202];
		}
	}


	// DA
	if(0 == g_InteralMemory.Bit[211])	// DA 当前状态停止
	{
		if(1 == g_InteralMemory.Bit[210])	// Set DA
		{
			DA_Start();
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}
	if(1 == g_InteralMemory.Bit[211])	// DA 当前状态启动
	{
		if(0 == g_InteralMemory.Bit[210])	// ReSet DA
		{
			DA_Stop();
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}


	//Clear
	 //input channel 1
	if(1 == g_InteralMemory.Bit[220])
	{
		g_InteralMemory.Bit[220] = 0;
		g_InteralMemory.Word[10] = g_InteralMemory.Word[11] = 0;		
	}
	   //input channel 2
	if(1 == g_InteralMemory.Bit[221])
	{
		g_InteralMemory.Bit[221] = 0;
		g_InteralMemory.Word[12] = g_InteralMemory.Word[13] = 0;		
	}	
	   //input channel 3	
	if(1 == g_InteralMemory.Bit[222])
	{
		g_InteralMemory.Bit[222] = 0;
		g_InteralMemory.Word[14] = g_InteralMemory.Word[15] = 0;		
	}	
	   //input channel 4	
	if(1 == g_InteralMemory.Bit[223])
	{
		g_InteralMemory.Bit[223] = 0;
		g_InteralMemory.Word[16] = g_InteralMemory.Word[17] = 0;		
	}	
	
	//output1 set number	
	if(1 == g_InteralMemory.Bit[224])
	{
		g_InteralMemory.Bit[224] = 0;
		g_InteralMemory.Word[24] = g_InteralMemory.Word[25] = 0;		
	}
	   //output2 set number	
	if(1 == g_InteralMemory.Bit[225])
	{
		g_InteralMemory.Bit[225] = 0;
		g_InteralMemory.Word[26] = g_InteralMemory.Word[27] = 0;		
	}	

	

}



