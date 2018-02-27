#include "2416addr.h"
#include "..\inc\def.h"
#include "2416lib.h"
#include "..\inc\systemconf.h"

#include "..\inc\nand.h"


extern SYSTEMCONFIG g_SystemConf;
extern GLOBALVAR g_GlobalVar;

extern INTERALMEMORY g_InteralMemory;

extern INT16U g_PlcCtrlNum,g_PlcStaNum,g_PlcCtrLen,g_PlcStaLen,g_StarScr,g_TotalSrceenNum;


void LoadSysConf(void)
{
	SYSTEMCONFIG *SysConFlash;
	//INT8U PageBuff[528];
	INT8U PageBuff[2048];
	int Block;
		
	//Block = Logical_To_Physical_Block(2);
	Block = Logical_To_Physical_Block(NAND_SYS_CONF);
	ReadPage(Block,0,PageBuff);

	
	
	SysConFlash = (SYSTEMCONFIG *)PageBuff;	
	g_SystemConf.BaudRates = SysConFlash->BaudRates;
	g_SystemConf.ComPort = SysConFlash->ComPort;
	g_SystemConf.ParityMode = SysConFlash->ParityMode;
	g_SystemConf.DataBits = SysConFlash->DataBits;
	g_SystemConf.StopBits = SysConFlash->StopBits;
	g_SystemConf.ScrSaveTimeSet = SysConFlash->ScrSaveTimeSet;                  //屏保
	g_SystemConf.XLc= SysConFlash->XLc;
	g_SystemConf.YLc= SysConFlash->YLc;
	g_SystemConf.Xc = SysConFlash->Xc;
	g_SystemConf.Yc = SysConFlash->Yc;
	g_SystemConf.Kx = SysConFlash->Kx;
	g_SystemConf.Ky = SysConFlash->Ky;
	//if the configuration in the Flash is not correct then load the default config
	if(g_SystemConf.BaudRates!=4800 && g_SystemConf.BaudRates != 9600 && g_SystemConf.BaudRates != 19200 && g_SystemConf.BaudRates != 38400 && g_SystemConf.BaudRates != 43000 && g_SystemConf.BaudRates != 56000 && g_SystemConf.BaudRates != 57600 && g_SystemConf.BaudRates != 115200)
	{
		g_SystemConf.BaudRates=9600;      //////////////
		g_SystemConf.ComPort = 0;
		g_SystemConf.ParityMode = 0;
		g_SystemConf.DataBits = 3;
		g_SystemConf.StopBits = 0;
		g_SystemConf.ScrSaveTimeSet = 1;
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}
	else  if(g_SystemConf.ComPort!=0 && g_SystemConf.ComPort!=1 
		&& g_SystemConf.ComPort!=2 && g_SystemConf.ComPort!=3)
	{ 
		g_SystemConf.ComPort = 0;
		g_SystemConf.ParityMode = 0;
		g_SystemConf.DataBits = 3;
		g_SystemConf.StopBits = 0;
		g_SystemConf.ScrSaveTimeSet = 1;
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}
	else if(g_SystemConf.ParityMode!=0 && g_SystemConf.ParityMode!=1 && g_SystemConf.ParityMode!=2)
	{
		g_SystemConf.ParityMode = 0;
		g_SystemConf.DataBits = 3;
		g_SystemConf.StopBits = 0;
		g_SystemConf.ScrSaveTimeSet = 1;
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}
	else if(g_SystemConf.DataBits!=0 && g_SystemConf.DataBits!=1 && g_SystemConf.DataBits!=2 && g_SystemConf.DataBits!=3)
	{
		g_SystemConf.DataBits = 3;
		g_SystemConf.StopBits = 0;
		g_SystemConf.ScrSaveTimeSet = 1;
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}
	else if(g_SystemConf.StopBits!=0 && g_SystemConf.StopBits!=1)
	{
		g_SystemConf.StopBits = 0;
		g_SystemConf.ScrSaveTimeSet = 1;
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}
	//else if(g_SystemConf.ScrSaveTimeSet !=0 && g_SystemConf.ScrSaveTimeSet !=1)
	//	{
	//		g_SystemConf.ScrSaveTimeSet = 1;
	//	}	
	else if((g_SystemConf.XLc<0x39F) || (g_SystemConf.XLc>0x69F) ||
		(g_SystemConf.YLc<0x955) || (g_SystemConf.YLc>0xB55) ||
		(g_SystemConf.Kx<2.673) || (g_SystemConf.Kx>4.673) ||
		(g_SystemConf.Ky<-5.236) || (g_SystemConf.Ky>-3.236))
	{
		// dx==2,  dy==2 时的参数
		//g_SystemConf.XLc= 0x4E0;
		//g_SystemConf.YLc= 0xA7C;
		//g_SystemConf.Xc = 0x64;	// 100
		//g_SystemConf.Yc = 0x64;	// 100
		//g_SystemConf.Kx = 3.615909;
		//g_SystemConf.Ky = -4.155357;
		
		g_SystemConf.XLc= 0x49F;
		g_SystemConf.YLc= 0xA55;
		g_SystemConf.Xc = 0x64;	// 100
		g_SystemConf.Yc = 0x64;	// 100
		g_SystemConf.Kx = 3.672727;
		g_SystemConf.Ky = -4.235714;
	}

	//make the system configuration effective	
	if(!UART_PrintStr)
		Uart_Init( 0,g_SystemConf.BaudRates );  // 修改波特率

	NumToSaveScrTime();
}



void StorSysConf(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the IntelFlash.
	Block = Logical_To_Physical_Block(NAND_SYS_CONF);
	//if(TRUE != EraseBlock(Block)) //连续坏块时怎么处理?
	//	MarkBadBlk(Block);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_SystemConf;
	WritePage(Block,0,PageBuff);

	//make the system configuration effective.	
	if(!UART_PrintStr)
		Uart_Init(0, g_SystemConf.BaudRates);  // 修改波特率
	
	//if(g_SystemConf.ScrSaveTimeSet)
	//	PLUS(1);
	//else
	//	rINTMSK |= BIT_TIMER2;

}




///////////////////////////////////////////////////////////
////            序号转化成屏保时间        //////////////////
///////////////////////////////////////////////////////////
void NumToSaveScrTime(void)
{
	switch(g_SystemConf.ScrSaveTimeSet)
	{
	case 0:
		g_GlobalVar.ScrSaveTime=0*60; //由0 改为60 min   ---080717
		break;
	case 1:
		g_GlobalVar.ScrSaveTime = 1*60;
		break;
	case 2:
		g_GlobalVar.ScrSaveTime = 2*60;
		break;
	case 3:
		g_GlobalVar.ScrSaveTime = 3*60;
		break;
	case 4:
		g_GlobalVar.ScrSaveTime = 5*60;
		break;
	case 5:
		g_GlobalVar.ScrSaveTime = 10*60;
		break;
	case 6:
		g_GlobalVar.ScrSaveTime = 15*60;
		break;
	case 7:
		g_GlobalVar.ScrSaveTime = 20*60;
		break;
	}

	g_GlobalVar.ScrSaveTime *= 100;		// 10ms *100 = 1s  定时时基改成10ms

}






int power2(int n)
{
	int result = 1;
	int i;

	for(i=0; i<n; i++)
		result *=2;

	return result;	
}

void cal_cpu_bus_clk(void)
{
	U32 val;
	U8 armdiv;
	U8 prediv;
	U16 m;
	U8 p, s;
	//long m;
	long mpll;
	long prediv_clk;
	
	val = rMPLLCON;
	m = (val>>14)&0x3ff;
	p = (val>>5)&0x3f;
	s = val&3;

	//SYS_FCLK = ((m+8)*(FIN/100)*2)/((p+2)*(1<<s))*100;
	mpll = FIN/(p*power2(s));
	mpll *=m;
	//SYS_FCLK = ((long)m*FIN)/(p*power2(s));
	//SYS_FCLK = clk;


	val = rCLKDIV0;
	armdiv = (val>>9) & 0x7;
	switch (armdiv) 
	{
	case 0:
		SYS_FCLK = mpll/1;
		break;
	case 1:
		SYS_FCLK = mpll/2;
		break;
	case 2:
		SYS_FCLK = mpll/3;
		break;
	case 3:
		SYS_FCLK = mpll/4;
		break;
	case 5:
		SYS_FCLK = mpll/6;
		break;
	case 7:
		SYS_FCLK = mpll/8;
		break;
	case 13:
		SYS_FCLK = mpll/12;
		break;
	case 15:
		SYS_FCLK = mpll/16;
		break;
	}


	
	val = rCLKDIV0;
	prediv = (val>>4) & 0x3;
	m = val & 0x3;	
	p = (val>>2) & 0x1;
	//val = rCAMDIV0;
	//s = val>>8;

	prediv_clk = mpll/(prediv+1);
	
	switch (m) 
		{
	case 0:
		SYS_HCLK = prediv_clk;
		break;
	case 1:
		SYS_HCLK = prediv_clk/2;
		break;
	case 2:
		break;
	case 3:
		SYS_HCLK = prediv_clk/4;
		break;
	}
	
	if(p)
		SYS_PCLK = SYS_HCLK/2;
	else
		SYS_PCLK = SYS_HCLK;
	
	//if(s&0x10)
	//	cpu_freq = SYS_HCLK;
	//else
	//	cpu_freq = SYS_FCLK;
		
	//val = rUPLLCON;
	//m = (val>>12)&0xff;
	//p = (val>>4)&0x3f;
	//s = val&3;
	//UPLL = ((m+8)*FIN)/((p+2)*(1<<s));
	//SYS_UCLK = (rCLKDIVN&8)?(UPLL>>1):UPLL;
}


void HaltUndef(void)
{
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("Undefined instruction exception!!!\n");
	while(1);
}

void HaltSwi(void)
{
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("SWI exception!!!\n");
	while(1);
}

void HaltPabort(void)
{
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("Pabort exception!!!\n");
	while(1);
}

void HaltDabort(void)
{
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("Dabort exception!!!\n");
	while(1);
}


void Isr_Init(void)
{
	pISR_UNDEF=(unsigned)HaltUndef;
	pISR_SWI  =(unsigned)HaltSwi;
	pISR_PABORT=(unsigned)HaltPabort;
	pISR_DABORT=(unsigned)HaltDabort;
	//pISR_IRQ	= (unsigned)IsrIRQ;

	
	rINTMOD1 = 0x0;	  // All=IRQ mode
	rINTMSK1 = BIT_ALLMSK;	  // All interrupt is masked.
}


void GlobalData_WriteZero(void)
{
	int i;

	for (i=0; i<2048; i++)
	{
		g_InteralMemory.Bit[i] = 0;
	}

	for (i=0; i<1024; i++)
	{
		g_InteralMemory.Word[i] = 0;
	}

	g_PlcCtrlNum = g_PlcStaNum = g_PlcCtrLen = g_PlcStaLen = g_StarScr = g_TotalSrceenNum = 0;

	g_GlobalVar.TimeReach_1 = g_GlobalVar.TimeReach_2 = g_GlobalVar.TimeReach_3 = 0;
	g_GlobalVar.Time_Alarm_One = 0;
	g_GlobalVar.Time_Alarm_State = 0;
	g_GlobalVar.Stop_Communication = 0;
	
}


void DataSave_Init (void) // 掉电保护中断
{
	DWORD tmp;

	tmp = rGPFCON & (~(0x3<< 0));
	rGPFCON = tmp | (0x2<<0);	//set GPF0 as EINT	

	rGPFUDP &= (~(0x3<< 0)); //GPF0 up down disable

	rEXTINT0 &= 0xFFFFFFF0;
	rEXTINT0 |= (0x2<<0);		// 01x = Falling edge triggered
	
	pISR_EINT0=(unsigned)DataSave_IntHandle;

	rEINTPEND = 0xffffff;

	rSRCPND1 |= BIT_EINT0; //to clear the previous pending states
	rINTPND1 |= BIT_EINT0;

	rINTMSK1 &= ~BIT_EINT0;
}

void DataSave_Load (void)	// 读取掉电保护数据
{
	INT8U PageBuff[2048];
	int Block;
	int i;
	short temp;

	Block = Logical_To_Physical_Block(NAND_DATA_SAVE);

	ReadPage(Block,0,PageBuff);
	for(i=0; i<1024; i++)
	{
		temp = PageBuff[i*2+1];
		temp = (temp <<8) | PageBuff[i*2];

		g_InteralMemory.KeepWord[i] = temp;
	}

	ReadPage(Block,1,PageBuff);
	for(i=0; i<2048; i++)
	{
		g_InteralMemory.KeepBit[i] = PageBuff[i];
	}

}


void __irq DataSave_IntHandle(void)
{
	INT8U *PageBuff;
	int Block;
	//int i;
	
	Block = Logical_To_Physical_Block(NAND_DATA_SAVE);
	
	EraseBlock(Block);
	
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[0];
	WritePage(Block,0,PageBuff);	// 暂时只用第0页（2K）保存数据

	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
	WritePage(Block,1,PageBuff);	// 暂时只用第1页（2K）保存数据

	ClearPending(BIT_EINT0);
	
}


void Led_on_off(void)
{	

	rGPEDAT ^= 0x100;//  GPE8  LED OUT
	
	//rWTCNT=0xc000;
}


