#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "2416addr.h"
#include "..\inc\def.h"
#include "2416lib.h"
#include "..\inc\Function.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\encrypt.h"
//#include "LCD.h"
#include "..\inc\lcd.h"
#include "..\inc\download.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"

#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\app.h"
#include "..\inc\wdt.h"	//WDT设置 by 方佳伟 2017/4/1

//U32 g_Count=0;

extern int adcpsr;			// ADCPSR		
extern int xtal;		// X-tal clock
extern int adcclock;	// ADC clock  1.33M

extern U8 ValidCount;
extern int xdata;
extern int ydata;
extern U8 bTSP_DownFlag;
extern U8 bCalibrateFlag;
extern U8 sampleCount;
extern int xdataSum;
extern int ydataSum;
extern INT16U X_result;
extern INT16U Y_result;
extern INT8U flag;

//extern U8 g_SampleFlag;


extern SYSTEMCONFIG g_SystemConf;
extern SYSTEMINFO g_SystemInfo;
extern INT16U CurrentScrNum;
extern INT16U PreScrNum;

extern INT8U IndexBlockBuffer[2048];
extern INT8U ScreenDataBuffer[620000];
extern INT32U ScrBufferOffset;

extern int WatchRefresh[RefreshNum*2];
extern int NumDispRefresh[RefreshNum];
extern int StickChartRefresh[RefreshNum][2];
extern int SetValueRefresh[RefreshNum];
extern int MultiStateRefresh[RefreshNum][2];
extern int KeepButtonRefresh[RefreshNum][2];        //交替型
extern int KeepButton2Refresh[RefreshNum][2];    //保持型
extern int TimerButtonRefresh[RefreshNum];
extern int OscButtonRefresh[RefreshNum];

extern GLOBALVAR g_GlobalVar;

extern INT16U g_PlcCtrlNum;
extern INT16U g_PlcStaNum;
extern INT16U g_PlcCtrLen;
extern INT16U g_PlcStaLen;

extern char g_PlcCtrStr[5];
extern char g_PlcStaStr[5];

extern int SetValue_PreValue[RefreshNum];
extern int NumDisp_PreValue[RefreshNum];

extern INTERALMEMORY g_InteralMemory;

extern U8 g_download_Flag;

extern U8 g_KeyBoardChange;


/////**********WL模版***********///////
GOBLEADC g_Adc;

void CalibrateXY(void)
{
	X_result = (xdata-g_SystemConf.XLc)/g_SystemConf.Kx+g_SystemConf.Xc;
	Y_result = (ydata-g_SystemConf.YLc)/g_SystemConf.Ky+g_SystemConf.Yc;

#if Rotate180
							X_result = 639 - X_result;
							Y_result = 479 - Y_result;
#endif

}
///****************************///////////


void KeyBoard_Refresh(void)
{
	if(CurrentScrNum>0 && CurrentScrNum<255)
	{
#if	Encrypt
		Encrypt_Operation();
#elif ProductTest
		////////////// 成品测试////////////////
		Test_Run();
		Test_RTC_SetTime();  // 按钮控制
		Test_RTC_ReadTime(); 
		
//#elif AppMode   ///****************WL
		//App_Operation();//////////////////TJ 修改小键盘
#endif  ///****************WL
        App_Operation();
	}


	RefreshWatchDispButton();

	RefreshStickChart();
	
	RefreshMultiState();
	
	RefreshKeepButton();

	RefreshKeep2Button();

	//if(!bTSP_DownFlag)
	//	RefreshButtonUp();

	RefreshNumDispButton();   ///****************WL

	RefreshSetValue();
	
	//Led_on_off();    ///****************WL

}




int ReadAdc(int ch)
{
	int i;
	static int prevCh=-1;

	if(prevCh!=ch)
	{
		rADCMUX = ch;
		//Delay(5);
		for(i=0; i<50000; i++);
		prevCh=ch;
	}
	rADCMUX = ch;
	rADCTSC = rADCTSC & 0xfb;     //Normal ADC conversion & No TS operation
	rADCCON|=0x1;   //start ADC

	while(!(rADCCON & 0x8000));	//check if EC(End of Conversion) flag is high
	return (rADCDAT0&0xfff);
}


void Test_Adc(void)
{
	int a8 = 0; 	//Initialize variables

	//Delay(0);

	adcpsr=(int)((SYS_PCLK/adcclock)-1);
	rADCCON = (1<<14) | (adcpsr<<6)|(1<<3);
	if(UART_PrintStr && 0x23==rUBRDIV1)
	{
		Uart_Printf("The ADC_IN are adjusted to the following values.\n");        
		Uart_Printf("Press any key to exit!!!\n");    
		Uart_Printf("ADC conv. freq.=%d(Hz)\n",(int)(SYS_PCLK/(adcpsr+1))); // ADC Freq. = 2.5MHz, ADC conversion time = 5CYCLES*(1/(ADC Freq.))
	}

	while(1)
	{
		a8=ReadAdc(8);

		if(UART_PrintStr && 0x23==rUBRDIV1)
			Uart_Printf("%04d\n",a8);
	}

	rADCCON &= ~(1<<14);// disable prescaler
	rADCCON |= (1<<2);	// Standby

	//printf("rADCCON = 0x%x\n", rADCCON);
}


////////////////////////////////////////////////////////////






void Ts_SampleStart(void)
{
	DWORD tmp;
   
	rINTSUBMSK &=~(BIT_SUB_ADC); //***********WL***********
	rINTMSK1 &=~(BIT_TIMER4);
	tmp = rTCON & (~(0x7 << 20));
	//rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
	//rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
	rTCON = tmp | (2 << 20);		/* update TCVNTB4, stop					*/
	rTCON = tmp | (5 << 20);		/* interval mode,  start				*/	
}

void Ts_SampleStop(void)
{
	ValidCount = 0;	// 使下一次采样重新开始(不考虑上一个采样点)	
	//**********WL********	
	g_Adc.enable = 0;
	g_Adc.index = 0;
	//********************
	rINTMSK1 |= BIT_TIMER4;
	rTCON &= ~(1 << 20);			/* Timer4, stop							*/
}


void Ts_PowerOn(void)
{
	rSUBSRCPND |= BIT_SUB_ADC;// Clear previous pending bit
	rSUBSRCPND |= BIT_SUB_TC;
	rSRCPND1 |= BIT_ADC;
	rINTMSK1 &=~BIT_ADC;       // ADC Touch Screen Mask bit clear
	//rINTSUBMSK &=~(BIT_SUB_ADC | BIT_SUB_TC);
	rINTSUBMSK &=~(BIT_SUB_TC);
}

void Ts_PowerOff(void)
{
	rINTMSK1 |= (U32)BIT_ADC;		//Mask interrupt bit
	//rINTSUBMSK |= (BIT_SUB_ADC | BIT_SUB_TC);	
	rINTSUBMSK |= (BIT_SUB_TC);	
}

void Ts_Init(void)
{ 
	adcpsr=(int)((SYS_PCLK/adcclock)-1);	
	//rADCDLY=50000;	 //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	//rADCDLY=(int)(xtal/1000)*2;	// 2ms delay(filter effect)
	rADCDLY=(SYS_PCLK/1000000)*500;// 500us delay(input stable) SYS_PCLK = 66.5M (最大用980)	
	
	//rADCCON = rADCCON & ~(0xffff) | (1<<14) | (adcpsr<<6) |(1<<3)|(0<<2)|(1<<1);//read start
	rADCCON = rADCCON & ~(0xffff) | (1<<14) | (adcpsr<<6) |(1<<3)|(0<<2);
	rADCTSC = 0xd3;  //Wfint,XP_PU_dis,XP_Dis,XM_Dis,YP_Dis,YM_En
	pISR_ADC = (int)AdcTsAuto;

	// Timer4 Initialize
	pISR_TIMER4 = (int)Timer4Sampling;	// Timer ISR for ADC sampling  10ms
	rTCFG0 &= ~(0xff << 8); 
	//rTCFG0 |= (10 << 8); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)    Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 16); 
	rTCFG1 |= (0x3 << 16); 	// Interrupt, Mux0=1/16
	//rTCNTB4 = 3784;         		// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}  10ms
	rTCNTB4 = 375/4;         		// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}  10ms
	//rTCMPB4 = 85;

	Adc_XY_Init();    //**********WL
	
	Ts_PowerOn();	// 系统菜单选择后才启用触摸采样
}

BOOL Touch_Pen_Filtering(int *px, int *py)
{
	BOOL RetVal = TRUE;
	// TRUE  : Valid pen sample
	// FALSE : Invalid pen sample
	//static U8 count = 0;	// 需要用无符号值，避免出现负值导致(count > 2)判断出错
	static int x_pre, y_pre;
	//int TmpX, TmpY;
	int dx, dy;
	
	ValidCount++;
	if (ValidCount > 1) 
	{ 
		ValidCount = 1;			

		// difference between x,y[1] and TmpX,Y
		dx = (*px > x_pre) ? (*px  - x_pre) : (x_pre - *px );
		dy = (*py > y_pre) ? (*py  - y_pre) : (y_pre - *py );
		
		if ((dx > FILTER_LIMIT) || (dy > FILTER_LIMIT)) 
		{
			// Invalid pen sample
			x_pre = *px; y_pre = *py; // reserve pen samples		
			RetVal = FALSE;
		} 
		else
		{
			// Valid pen sample
			RetVal = TRUE;
			ValidCount = 0;	// 使下一次采样重新开始(不考虑上一个采样点)			
		}	
		
	} 
	else 
	{ 
		// till 2 samples, no filtering rule
		x_pre = *px; y_pre = *py; // reserve pen samples		
		RetVal = FALSE;	// <- TRUE jylee 2003.03.04 
	}
	
	return RetVal;	
}

void __irq Timer4Sampling(void) // 可通过示波器确定频率
{

	if ( (rADCDAT0 & (1 << 15)) || (rADCDAT1 & (1 << 15)) ) // 0 = Stylus down state; 1 = Stylus up state
	{
		if(g_Adc.enable==0)
		{
			bTSP_DownFlag = FALSE;
			rADCTSC = 0xd3;    		// Down detect, Waiting for interrupt
			rSUBSRCPND |= BIT_SUB_ADC;
			Ts_SampleStop();
		}
	}		
	else // 引掉，使Down 和Up 对称，成组出现
	if (bTSP_DownFlag)
	{
		if(g_Adc.enable==0)
		{
			g_Adc.enable = 1;
			g_Adc.index = 0;
			Adc_SampleStart();
		}
	}
	else
	{
		Ts_SampleStop();
	}

	//ClearPending((U32)BIT_TIMER4);
	rSRCPND1 = BIT_TIMER4;       //Clear pending bit
	rINTPND1 = BIT_TIMER4;
	rINTPND1 = BIT_TIMER4;                    //Prevent an double interrupt pending

}

////************WL**************
void Adc_XY_Init(void)
{
	int i;

	for(i=0;i<TSP_SAMPLE_NUM;i++)
	{
		g_Adc.x[i] = g_Adc.y[i] = 0;
	}

	g_Adc.index = 0;
	g_Adc.enable = 0;
}

void Adc_SampleStart(void)
{
	rADCTSC = (0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(0);	// auto		
	rADCCON |= (1 << 0);				/* Start Auto conversion				*/

}
///******************************

BOOL Ts_GetXY(int *px, int *py)
{
	///***********WL**************************
	//int i,j,k;
	//int temp;
	//int x[TSP_SAMPLE_NUM], y[TSP_SAMPLE_NUM];
	///***************************************
	
	int dx, dy;
	GOBLEADC *pXY=&g_Adc;
	
	if(TRUE == bCalibrateFlag);//触摸校准用
	else if(flag)
	{
		rADCTSC = 0x1d3;
		return 0;
	}

	*px = (pXY->x[TSP_SAMPLE_NUM/2-1]+pXY->x[TSP_SAMPLE_NUM/2])>>1;
	*py = (pXY->y[TSP_SAMPLE_NUM/2-1]+pXY->y[TSP_SAMPLE_NUM/2])>>1;


	rADCTSC = 0x1d3;//(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<< 4)|(0<<3)|(0<<2)|(3);	// Detect UP			
	dx = pXY->x[TSP_SAMPLE_NUM/2]-pXY->x[TSP_SAMPLE_NUM/2-1];
	dy = pXY->y[TSP_SAMPLE_NUM/2]-pXY->y[TSP_SAMPLE_NUM/2-1];

	
	return ((dx > TSP_INVALIDLIMIT || dy > TSP_INVALIDLIMIT) ? FALSE : TRUE);


}

void __irq AdcTsAuto(void)
{
	int TmpX = 0;
	int TmpY = 0;
	GOBLEADC *pXY=&g_Adc;
	//INT16U *pX = &X_result;	
	//INT16U *pY = &Y_result;
	int i;


	if(rSUBSRCPND & (1<<10))
	{
		//ADC
		if(pXY->enable==1)
		{
		
			if(pXY->index<TSP_SAMPLE_NUM)
			{
				pXY->x[pXY->index] = (0xfff & rADCDAT1);
				pXY->y[pXY->index] = (0xfff & rADCDAT0);	
				

				
				for(i=pXY->index;i>0;i--)
				{
					if(pXY->x[i]<pXY->x[i-1])
					{
						TmpX = pXY->x[i];
						pXY->x[i] =pXY->x[i-1];
						pXY->x[i-1] = TmpX;
						
					}

					if(pXY->y[i]<pXY->y[i-1])
					{
						TmpY = pXY->y[i];
						pXY->y[i] =pXY->y[i-1];
						pXY->y[i-1] = TmpY;
						
					}					
				}
		

				pXY->index++;
				
				Adc_SampleStart();
			}
			else
			{
				g_Adc.enable = 0;
				if(Ts_GetXY(&TmpX,&TmpY))
				{
				
					if(Touch_Pen_Filtering(&TmpX, &TmpY)) // 不判断
					{

	
						if(TRUE == bCalibrateFlag)
						{
							xdataSum += TmpX;
							ydataSum += TmpY;
							//if(UART_PrintStr && 0x23==rUBRDIV1)
							//	Uart_Printf("XP=%04d, YP=%04d\n", xdataSum, ydataSum);    //X-position Conversion data
							sampleCount++;
							if(sampleCount >= SAMPLE_TIMES)
							{
								if(UART_PrintStr && 0x23==rUBRDIV1)
									Uart_Printf("XP=%04d, YP=%04d\n", xdataSum, ydataSum);    //X-position Conversion data
								Ts_SampleStop();
							}
						}
						else
						{
							rSUBSRCPND |= BIT_SUB_ADC;
							xdata = TmpX;
							ydata = TmpY;

														
							g_GlobalVar.Timer2Count = 0;                       //屏保 重新计数
							if(g_GlobalVar.ScrSaveFlag == 1)
							{
								g_GlobalVar.ScrSaveFlag = 0;
								Lcd_Pwren_Init_On_Off( TRUE) ;	// 控制灯光开
								//ScrSave_Start();
							}
							else
							{
								flag = 1; // 按下去就有效
								//Beep(1);
							}
							if(UART_PrintStr && 0x23==rUBRDIV1)
								Uart_Printf("XP=%04d, YP=%04d\n", xdata, ydata);    //X-position Conversion data
							Ts_SampleStop();	// 每次触摸只采样一点
						}	
					}

				}
			}
		
		}
		rSUBSRCPND |= BIT_SUB_ADC;
	}
	
	if(rSUBSRCPND & (1<<9))
	{
		//TC
		if(g_Adc.enable==0)
		{
			if((rADCUPDN&0x1) == 0x1)			// Stylus Down
			//if(rADCUPDN == 0x1)			// Stylus Down
			{
				//if(UART_PrintStr && 0x23==rUBRDIV1)
				//	Uart_Printf("rADCUPDN=%x\n", rADCUPDN);
				rADCUPDN &= ~(0x1);		// Clear Down state

				bTSP_DownFlag = TRUE;
				//rADCDLY=(SYS_PCLK/1000000)*500;// 500us delay(input stable) SYS_PCLK = 66.5M (最大用980)			
				//rADCDLY = 50000;
				Ts_SampleStart();
				//rADCTSC = 0x1d3;	// Up detect, Waiting for interrupt  (防止多次进下笔中断)			
				if(UART_PrintStr && 0x23==rUBRDIV1)
					Uart_Printf("Stylus Down\n");
			}
			else if((rADCUPDN&0x2) == 0x2)	// Stylus Up
			//else if(rADCUPDN == 0x2)	// Stylus Up
			{
				//if(UART_PrintStr && 0x23==rUBRDIV1)
				//	Uart_Printf("rADCUPDN=%x\n", rADCUPDN);
				rADCUPDN &= ~(0x1<<1);	// Clear Up state
				bTSP_DownFlag = FALSE;
				Ts_SampleStop();
				rADCTSC = 0xd3;    		// Down detect, Waiting for interrupt
				if(UART_PrintStr && 0x23==rUBRDIV1)
					Uart_Printf("Stylus Up\n\n");
				//flag = 1;
			}

		}
		
		
		rSUBSRCPND |= BIT_SUB_TC;
	}	
	
	ClearPending((U32)BIT_ADC);	
	
}

U8 Ts_Calibrate(void)
{
	int TScreen[5][2]= {	
						320,240,
						540,100,
						100,100,
						100,380,
						540,380
						};	

#if Rotate180
	int TRotate180[5][2]= {	
						320,240,
						100,380,
						540,380,
						540,100,
						100,100
						};	
#endif
	
	int TLogic[5][2] = {0};
	double Kx,Ky;
	int XLa,YLa;
	int dx,dy;
	U8 pointCount = 0;

	bCalibrateFlag = TRUE;
	
	while(pointCount<5)     //采集5点数据
	{
		
		
		Lcd_Clr_Test(WHITE);	
		
#if Rotate180
		Paint_CrossLine(TRotate180[pointCount][0],TRotate180[pointCount][1],20,RED);
#else		
		Paint_CrossLine(TScreen[pointCount][0],TScreen[pointCount][1],20,RED);
#endif

		sampleCount = 0;
		xdataSum = ydataSum = 0;
		while(sampleCount < SAMPLE_TIMES); // 等待每个点的采样次数到达
		
		if(0 != sampleCount)
		{	
		
			TLogic[pointCount][0] = xdataSum/sampleCount;
			TLogic[pointCount][1] = ydataSum/sampleCount;
		}
		pointCount++;
	}

	// KX={(XLB-XLC)/(XB-XC)+(XLE-XLD)/(XE-XD)}/2
	// KY={(YLD- YLC)/(YD-YC)+(YLE-YLB)/(YE-YB)}/2
	Kx = (((double)TLogic[1][0]-TLogic[2][0])/(TScreen[1][0]-TScreen[2][0])
		+ ((double)TLogic[4][0]-TLogic[3][0])/(TScreen[4][0]-TScreen[3][0]))/2;
	Ky = (((double)TLogic[3][1]-TLogic[2][1])/(TScreen[3][1]-TScreen[2][1])
		+ ((double)TLogic[4][1]-TLogic[1][1])/(TScreen[4][1]-TScreen[1][1]))/2;

	g_SystemConf.XLc = TLogic[2][0];
	g_SystemConf.YLc = TLogic[2][1];
	g_SystemConf.Xc = TScreen[2][0];
	g_SystemConf.Yc = TScreen[2][1];
	g_SystemConf.Kx = Kx;
	g_SystemConf.Ky = Ky;
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("g_SystemConf.Kx=%f\n", g_SystemConf.Kx);    
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("g_SystemConf.Ky=%f\n", g_SystemConf.Ky);    


	// 先判断是否在误差范围内
	// XLa=XLc+Kx(Xa-Xc)
	// YLa=YLc+Ky(Ya-Yc)
	XLa = g_SystemConf.XLc + Kx*(TScreen[0][0]-g_SystemConf.Xc);
	YLa = g_SystemConf.YLc + Ky*(TScreen[0][1]-g_SystemConf.Yc);
	
	(XLa>TLogic[0][0]) ? (dx=XLa-TLogic[0][0]) : (dx=TLogic[0][0]-XLa);
	(YLa>TLogic[0][1]) ? (dy=YLa-TLogic[0][1]) : (dy=TLogic[0][1]-YLa);
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("dx=%d\n", dx);    
	if(UART_PrintStr && 0x23==rUBRDIV1)
		Uart_Printf("dy=%d\n", dy);    

	bCalibrateFlag = FALSE; // 退出校准标志
	
	//if((dx<7) && (dy<7)) // 误差范围控制
	if((dx<25) && (dy<25)) // 误差范围控制
	{
		StorSysConf();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




void KeyScan(void)
{
	INT32U w;
//	INT8U a[10];
	CalibrateXY();
	switch (CurrentScrNum)
	{
		case 0x0:                               //Sysytem Scr
		{
			//下载按钮
			if ((X_result>65&&X_result<155) && (Y_result>140&&Y_result<200))         
			{
				//rINTMSK|=(BIT_EINT1|BIT_TIMER2);
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();
				
				Ts_PowerOff();
				g_download_Flag = TRUE;
				DownLoad();					// 下载画面
				g_download_Flag = FALSE;
				Ts_PowerOn();
				
				// 画面下载用的Timer0, 下载时会重新配置rTCFG0，
				//高速输出和DA 启动前需重新配置rTCFG0
				//Test_Init();
				
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1|BIT_TIMER2);
				X_result=0;
				Y_result=0;
				flag = 0;	
			}			
			//管理员按钮	
			if ((X_result>260&&X_result<350) && (Y_result>140&&Y_result<200))
			{				
				//rINTMSK|=BIT_EINT1;
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				
				//while(bTSP_DownFlag);
				//	FeedWacthDog();
				
				//DisplayLightAdjust();
				//Load();
				DisplayUserScr(61);//进入管理员画面，模板第61个画面
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				//DisplaySystemScr(0);
			}			
			//运行按钮
			if ((X_result>455&&X_result<455+90) && (Y_result>140&&Y_result<200))
			{
				//rINTMSK|=BIT_EINT1;
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Run();
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}			
			//参数设置按钮
			if ((X_result>40&&X_result<40+140) && (Y_result>270&&Y_result<270+80))
			{
				//rINTMSK|=BIT_EINT1;
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

		    	DisplayParScr();
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				//Ts_Init();
			}			
			//基点校准按钮
			if ((X_result>235&&X_result<235+140) && (Y_result>270&&Y_result<270+80))
			{
				//rINTMSK|=BIT_EINT1;
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				while(TRUE != Ts_Calibrate());	

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				DisplaySystemScr(0);
			}			
			//画面检查按钮
			if ((X_result>430&&X_result<430+140) && (Y_result>270&&Y_result<270+80))
			{
				//rINTMSK|=BIT_EINT1;
				DisplaySystemScr(1);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				DisplayScrCheck(IndexBlockBuffer[1]);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
		}; break;
		
		case 0xff04:                                    //参数设置画面
		{
			//rINTMSK|=BIT_EINT1;
			ParScrKeyScan();
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			DisplaySystemScr(0);
		};break;
		case 0xff05:                                    //画面检查画面
		{
			//rINTMSK|=BIT_EINT1;
			ScrCheckScan();
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			DisplaySystemScr(0);
		};break;
		case 0xff06:                                    //亮度调节画面
		{	
			//rINTMSK|=BIT_EINT1;
			//LightAdjustScan();
			Load();
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			DisplaySystemScr(0);
		};break;

		case 0xff01:                   //Communication Time Out Scr
		case 0xff02:                   //Communication Error Scr
		case 0xff03:                   //DownLoad Complete Scr
		case 0xff07:					  //DownLoad Version difference
		{
			if ((X_result>200&&X_result<440) && (Y_result>150&&Y_result<330))
			{
			    	//rINTMSK|=BIT_EINT1;
				if(CurrentScrNum == 0xff01)
				{
					DisplayTimeOutScr(1);
				}
				else if(CurrentScrNum == 0xff02)
				{
					DisplayErrScr(1);
				}
				else if(CurrentScrNum == 0xff03)
				{
					DisplayDLCompleteScr(1);
				}
				else if(CurrentScrNum == 0xff07)
				{
					DisplayVerDiffScr(1);
				}
				
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;	
				DisplaySystemScr(0);
			}
		}; break;
		
		default:
			//rINTMSK|=BIT_EINT1;
			UserScrKeyScan();
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			break;


	}
}



void UserScrKeyScan(void)
{
	INT8U temp8U,Type;
	INT16U ElementCount;
	INT16U Left,Top,Right,Bottom;
	INT32U ElementPrt,w;

	INT32U i;

	if(CurrentScrNum != ScreenDataBuffer[0])
		//return;
		while(1);	// 触摸时启动看门狗复位   081022

	ElementCount = ScreenDataBuffer[2];
	ElementCount = ElementCount | (ScreenDataBuffer[3]<<8);
	i=0;
	while(i<ElementCount)
	{
		//FeedWacthDog();		
		ElementPrt = ScreenDataBuffer[4+i*3];
		ElementPrt = ElementPrt | (ScreenDataBuffer[4+i*3+1]<<8);
		ElementPrt = ElementPrt | (ScreenDataBuffer[4+i*3+2]<<16);
		ElementPrt = ElementPrt & 0x00ffffff;
		ScrBufferOffset = ElementPrt;
		temp8U = ScreenDataBuffer[ElementPrt];
		
		switch(temp8U)
		{
			case 1:    //ON/OFF/保持型/交替型按钮
				{
					Type=Get_Element_Region(&Left,&Top,&Right,&Bottom);
					if(Point_in_Region(X_result,Y_result,Left,Top,Right,Bottom,Type))
					{
						Func_KeepButton(ScrBufferOffset);
						return;
					}
				}break;
			case 2:
				{
					Type=Get_Element_Region(&Left,&Top,&Right,&Bottom);
					if(Point_in_Region(X_result,Y_result,Left,Top,Right,Bottom,Type))
					{
						Func_UpButton(ScrBufferOffset);
						return;
					}
				}break;

			case 3:
				{ 
					Type=Get_Element_Region(&Left,&Top,&Right,&Bottom);
					if(Point_in_Region(X_result,Y_result,Left,Top,Right,Bottom,Type))
					{
						Beep(1);
						for(w=0;w<BEEP_DELAY;w++);
			 			Beep(0);
						DrawShiftButton(1);

						//while(bTSP_DownFlag);
						//	FeedWacthDog();

						Func_ShiftButton(ElementPrt);
						return;
					}
				}break;
			case 6:
				{
					Type=Get_Element_Region(&Left,&Top,&Right,&Bottom);
					if(Point_in_Region(X_result,Y_result,Left,Top,Right,Bottom,Type))
					{
						Func_SetValue(ElementPrt);
						return;
					}
				}break;			
		}
		i++;
	}
}

//////////////////////////////////////////////////////
void DisplayKeyBoard(char *MaxValueStr,char *MinValueStr)
{
//	INT8U Info[]={48,1,28,92,37,44};  //CharString:“小键盘”
	char Info[]="KeyBoard";
//	INT8U Max[]={55,78,20,83,54,21,3,26};
	char Max[]="MaxNum:";
//	INT8U Min[]={55,78,48,1,54,21,3,26};
	char Min[]="MinNum:";
	char QuXiao[]={40,1,47,91};//显示中文取消
//	char QuXiao[]="Cancel";//显示英文取消
	char QueDing[]={40,23,22,8}; //显示中文确定
//	char QueDing[]="OK";//显示英文确定

	//INT8U Num[2];
	char Num[2];
	INT16U Left,Top;
	U8 long1;
	U8 long2;
	
	
	wdt_feed_dog();//WDT设置 by 方佳伟 2017/4/1 
	
	Lcd_Button(0+KEYBOARDOFFSET,0,350+KEYBOARDOFFSET,480,9,3,RAISE);  // Background

	Lcd_Fill_Box(3+KEYBOARDOFFSET,3,347+KEYBOARDOFFSET,40, 9);    //Title
//	Lcd_DispHZ_On_Button(151+KEYBOARDOFFSET,7,200+KEYBOARDOFFSET,40,3,13,3,1,0,1,0,0,Info);
	long2 = strlen(Info);
	Lcd_Disp_AscStr(Info,long2, 120+KEYBOARDOFFSET, 7, 2,2,16,0,9);

	Lcd_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,11,2,INSERT);//Number Display area

//	Lcd_DispHZ_On_Button(20+KEYBOARDOFFSET,110,91+KEYBOARDOFFSET,150,4,19,12,0,0,1,0,0,Max);
	long2 = strlen(Max);
	Lcd_Disp_AscStr(Max,long2, 20+KEYBOARDOFFSET, 111, 1,1,8,0,9);

//	Lcd_DispHZ_On_Button(20+KEYBOARDOFFSET,132,91+KEYBOARDOFFSET,150,4,19,12,0,0,1,0,0,Min);
	long2 = strlen(Min);
	Lcd_Disp_AscStr(Min,long2, 20+KEYBOARDOFFSET, 133, 1,1,8,0,9);

	//Display the MaxValue and the MinValue here
	long1=strlen(MaxValueStr);
	Lcd_Disp_AscStr(MaxValueStr,long1, 86+KEYBOARDOFFSET, 112, 1,1,8,0,9);
	long1=strlen(MinValueStr);
	Lcd_Disp_AscStr(MinValueStr,long1, 86+KEYBOARDOFFSET, 134, 1,1,8,0,9);


	//The button of Number 1
	Left = 10+KEYBOARDOFFSET;
	Top = 165;
	Num[0]=3;
	Num[1]=17;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 2
	Left = 93+KEYBOARDOFFSET;
	Top = 165;
	Num[0]=3;
	Num[1]=18;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 3
	Left = 176+KEYBOARDOFFSET;
	Top = 165;
	Num[0]=3;
	Num[1]=19;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of BackSpace
	Left = 259+KEYBOARDOFFSET;
	Top = 165;
	Num[0]=1;
	Num[1]=91;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,4,0,0,0,0,Num);

	//The button of Number 4
	
	Left = 10+KEYBOARDOFFSET;
	Top = 240;
	Num[0]=3;
	Num[1]=20;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 5
	
	Left = 93+KEYBOARDOFFSET;
	Top = 240;
	Num[0]=3;
	Num[1]=21;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 6
	
	Left = 176+KEYBOARDOFFSET;
	Top = 240;
	Num[0]=3;
	Num[1]=22;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of doint
	Left = 259+KEYBOARDOFFSET;
	Top = 240;
	Num[0]=3;
	Num[1]=14;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 7

	Left = 10+KEYBOARDOFFSET;
	Top = 315;
	Num[0]=3;
	Num[1]=23;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 8
	
	Left = 93+KEYBOARDOFFSET;
	Top = 315;
	Num[0]=3;
	Num[1]=24;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of Number 9
	
	Left = 176+KEYBOARDOFFSET;
	Top = 315;
	Num[0]=3;
	Num[1]=25;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);

	//The button of minus sign
	Left = 259+KEYBOARDOFFSET;
	Top = 315;
	Num[0]=3;
	Num[1]=13;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,4,0,0,0,0,Num);


	//The button of Number 0
	Left = 10+KEYBOARDOFFSET;
	Top = 390;
	Num[0]=3;
	Num[1]=16;
	Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
 
	//The button of CANCLE
	Left = 93+KEYBOARDOFFSET;
	Top = 390;
	Lcd_Button(Left,Top,Left+111,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QuXiao);//显示中文取消
	long2 = strlen(QuXiao);
//	Lcd_Disp_AscStr(QuXiao,long2, Left+30, Top+18, 1,2,8,0,7);//显示英文取消

	//The button of OK
	Left = 217+KEYBOARDOFFSET;
	Top = 390;
	Lcd_Button(Left,Top,Left+111,Top+65,5,3,RAISE);
	Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QueDing); //显示中文确定
	long2 = strlen(QueDing);
//	Lcd_Disp_AscStr(QueDing,long2, Left+45, Top+18, 1,2,8,0,7);//显示英文确定

}



int KeyBoard(int Max,int Min,U8 DigitPos,U8 DigitNumb)
{
	double MaxValue,MinValue;
	char MaxValueStr[20],MinValueStr[20];
	int a;
	double Result1;
	//double Result;	// 060627 
	int Result;	 
					

	//while(bTSP_DownFlag);
	//	FeedWacthDog();

	
	//rINTMSK &= ~BIT_EINT1;

	switch(DigitPos)
	{
		case 0:
				MaxValue=(double)Max;
				MinValue=(double)Min;
				break;
		case 1:
                		MaxValue=(double)Max/10.0;
				MinValue=(double)Min/10.0;
				break;
		case 2:
				MaxValue=(double)Max/100.0;
				MinValue=(double)Min/100.0;
				break;
		case 3:
				MaxValue=(double)Max/1000.0;
				MinValue=(double)Min/1000.0;
				break;
		case 4:
				MaxValue=(double)Max/10000.0;
				MinValue=(double)Min/10000.0;
				break;
	}

	sprintf(MaxValueStr,"%0.6f",MaxValue);
	for(a=0;a<10;a++)
	{
		if(MaxValueStr[a]=='.')
			break;
	}
	if(DigitNumb==0)
		MaxValueStr[a]='\0';
	else
		MaxValueStr[a+1+DigitNumb]='\0';

	sprintf(MinValueStr,"%0.6f",MinValue);
	for(a=0;a<10;a++)
	{
		if(MinValueStr[a]=='.')
			break;
	}
	if(DigitNumb==0)
		MinValueStr[a]='\0';
	else
		MinValueStr[a+1+DigitNumb]='\0';

	DisplayKeyBoard(MaxValueStr,MinValueStr);
	MaxValue=(double)atof(MaxValueStr);
	MinValue=(double)atof(MinValueStr);

	Result1=KeyBoardScan(MaxValue,MinValue,DigitNumb,0);
	Result = Result1;
	if(Result1!=-1)
	{
		switch(DigitPos)
		{
			case 0:
					Result=(int)(Result1);
					break;
			case 1:
               		Result=(int)(Result1*10);
					break;
			case 2:
					//Result=(int)(Result1*100 );
					Result=(int)(Result1*1000/10);  // 遇到只乘以100；在显示2222.22 或者 4444.44 时会出现2222.21 4444.41
					break;
			case 3:
					Result=(int)(Result1*1000);
					break;
			case 4:
					Result=(int)(Result1*10000);
					break;
		}
	}

	g_KeyBoardChange = TRUE;
	
	if(CurrentScrNum==0)
		DisplaySystemScr(0);
	else
		DisplayUserScr(CurrentScrNum);

	g_KeyBoardChange = FALSE;
	

	return Result;
}


double KeyBoardScan(double MaxValue,double MinValue,U8 DigitNumb,char Flag)
{
	char Str[10]={0};
	char QuXiao[]={40,1,47,91}; //显示中文取消
//	char  QuXiao[]="Cancel"; //显示英文取消
	char QueDing[]={40,23,22,8};//显示中文确定
//	char  QueDing[]="OK";//显示英文确定
	U8 i=0;
	INT16U Left,Top;
	int w,l;
	char Num[2],Num1[2];//,Result;
	double CurrentValue;

	int count = 0;

//++++++++++++++++++++++++++++++
	char Str_password[10]={0};
	char Num1_P[2];
	int l_P;
//++++++++++++++++++++++++++++++

	U8 long2;

	
	while(1)
	{	
		count++;
		
		/*if(count > 2000000)
		{
			count = 0;
			FeedWacthDog();		
		}*/

		if(count > 10000)
		{
			count = 0;
			KeyBoard_Refresh();
		}
	
		if(flag==1)
		{
			CalibrateXY();
			if(X_result>=10+KEYBOARDOFFSET && X_result<=80+KEYBOARDOFFSET && Y_result>=165&&Y_result<=230)   //button 1
			{
				//rINTMSK|=BIT_EINT1;
				Left = 10+KEYBOARDOFFSET;
				Top = 165;
				Num[0]=3;
				Num[1]=17;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='1';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=93+KEYBOARDOFFSET && X_result<=163+KEYBOARDOFFSET &&Y_result>=165&&Y_result<=230)  //button 2
			{
				//rINTMSK|=BIT_EINT1;
				Left = 93+KEYBOARDOFFSET;
				Top = 165;
				Num[0]=3;
				Num[1]=18;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='2';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=176+KEYBOARDOFFSET && X_result<=246+KEYBOARDOFFSET && Y_result>=165&&Y_result<=230)  //button 3
			{
				//rINTMSK|=BIT_EINT1;
				Left = 176+KEYBOARDOFFSET;
				Top = 165;
				Num[0]=3;
				Num[1]=19;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='3';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=10+KEYBOARDOFFSET && X_result<=80+KEYBOARDOFFSET && Y_result>=240&&Y_result<=305)  //button 4
			{
				//rINTMSK|=BIT_EINT1;
				Left = 10+KEYBOARDOFFSET;
				Top = 240;
				Num[0]=3;
				Num[1]=20;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='4';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=93+KEYBOARDOFFSET && X_result<=163+KEYBOARDOFFSET && Y_result>=240&&Y_result<=305)  //button 5
			{
				//rINTMSK|=BIT_EINT1;
				Left = 93+KEYBOARDOFFSET;
				Top = 240;
				Num[0]=3;
				Num[1]=21;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='5';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=176+KEYBOARDOFFSET && X_result<=246+KEYBOARDOFFSET && Y_result>=240&&Y_result<=305)  //button 6
			{
				//rINTMSK|=BIT_EINT1;
				Left = 176+KEYBOARDOFFSET;
				Top = 240;
				Num[0]=3;
				Num[1]=22;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='6';
				Num1[1]='\0';
				AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=10+KEYBOARDOFFSET && X_result<=80+KEYBOARDOFFSET && Y_result>=315&&Y_result<=380)  //button 7
			{
				//rINTMSK|=BIT_EINT1;
				Left = 10+KEYBOARDOFFSET;
				Top = 315;
				Num[0]=3;
				Num[1]=23;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='7';
				Num1[1]='\0';
				AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=93+KEYBOARDOFFSET && X_result<=163+KEYBOARDOFFSET && Y_result>=315&&Y_result<=380)  //button 8
			{
				//rINTMSK|=BIT_EINT1;
				Left = 93+KEYBOARDOFFSET;
				Top = 315;
				Num[0]=3;
				Num[1]=24;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='8';
				Num1[1]='\0';
				AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=176+KEYBOARDOFFSET && X_result<=246+KEYBOARDOFFSET && Y_result>=315&&Y_result<=380)      //button 9
			{
				//rINTMSK|=BIT_EINT1;
				Left = 176+KEYBOARDOFFSET;
				Top = 315;
				Num[0]=3;
				Num[1]=25;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='9';
				Num1[1]='\0';
				AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			if(X_result>=10+KEYBOARDOFFSET && X_result<=80+KEYBOARDOFFSET && Y_result>=390&&Y_result<=455)  //button 0
			{
				//rINTMSK|=BIT_EINT1;
				Left = 10+KEYBOARDOFFSET;
				Top = 390;
				Num[0]=3;
				Num[1]=16;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='0';
				Num1[1]='\0';
				AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=259+KEYBOARDOFFSET && X_result<=329+KEYBOARDOFFSET && Y_result>=240&&Y_result<=305)  //button .
			{
				//rINTMSK|=BIT_EINT1;
				Left = 259+KEYBOARDOFFSET;
				Top = 240;
				Num[0]=3;
				Num[1]=14;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				Num1[0]='.';
				Num1[1]='\0';
			    	AddNum(Str,DigitNumb,Num1);
//++++++++++++++++++++++++++++++++++++++
				Num1_P[0]='*';
				Num1_P[1]='\0';
				AddNum(Str_password,DigitNumb,Num1_P);
//++++++++++++++++++++++++++++++++++++++
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			
			if(X_result>=259+KEYBOARDOFFSET && X_result<=329+KEYBOARDOFFSET && Y_result>=165&&Y_result<=230)  //button 退格
			{
				//rINTMSK|=BIT_EINT1;
				Left = 259+KEYBOARDOFFSET;
				Top = 165;
				Num[0]=1;
				Num[1]=91;
				Lcd_Button(Left,Top,Left+70,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);
				l=strlen(Str);
				if(l!=0)
					*(Str+l-1)='\0';
				l=strlen(Str);
//++++++++++++++++++++++++++++++++++++++
				l_P=strlen(Str_password);
				if(l_P!=0)
					*(Str_password+l_P-1)='\0';
				l_P=strlen(Str_password);
//++++++++++++++++++++++++++++++++++++++				
				Lcd_Fill_Box(42+KEYBOARDOFFSET,52,298+KEYBOARDOFFSET,93,11);
				if(g_InteralMemory.Word[101]>60 
					&& g_InteralMemory.Word[101]!=73
					&& g_InteralMemory.Word[101]!=76 && g_InteralMemory.Word[101]!=78)
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str_password),11,0,1,2,2,Str_password);
				else
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+70,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+70,Top+65,1,0,5,1,0,0,0,0,Num);
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=93+KEYBOARDOFFSET && X_result<=204+KEYBOARDOFFSET && Y_result>=390&&Y_result<=455)  //button cancel
			{
				//rINTMSK|=BIT_EINT1;
				Left = 93+KEYBOARDOFFSET;
				Top = 390;
				
				Lcd_Button(Left,Top,Left+111,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QuXiao);//显示中文取消
				
				long2 = strlen(QuXiao);
				//Lcd_Disp_AscStr(QuXiao,long2, Left+30, Top+18, 1,2,8,0,7); // 显示英文取消
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();
				
				
				Lcd_Button(Left,Top,Left+111,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QuXiao);//显示中文取消

				long2 = strlen(QuXiao);
				//Lcd_Disp_AscStr(QuXiao,long2, Left+30, Top+18, 1,2,8,0,7);   // 显示英文取消 
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				return -1;
			}

			if(X_result>=217+KEYBOARDOFFSET && X_result<=328+KEYBOARDOFFSET && Y_result>=390&&Y_result<=455)  //button ok
			{
				//rINTMSK|=BIT_EINT1;
				Left = 217+KEYBOARDOFFSET;
				Top = 390;
				Lcd_Button(Left,Top,Left+111,Top+65,5,3,INSERT);
				Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QueDing); //显示中文确定
				long2 = strlen(QueDing);
				//Lcd_Disp_AscStr(QueDing,long2, Left+45, Top+18, 1,2,8,0,7); //显示英文确定
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
			 	Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(Left,Top,Left+111,Top+65,5,3,RAISE);
				Lcd_DispHZ_On_Button(Left,Top,Left+111,Top+65,2,0,5,1,0,0,0,0,QueDing);//显示中文确定
				long2 = strlen(QueDing);
				//Lcd_Disp_AscStr(QueDing,long2, Left+45, Top+18, 1,2,8,0,7); //显示英文确定
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;

				if(strlen(Str)==0)
				{
					strcpy(Str,"Error!");
					Lcd_Fill_Box(42+KEYBOARDOFFSET,52,298+KEYBOARDOFFSET,93,11);
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);
					for(i=0;i<10;i++)
						Str[i]='\0';

					continue;
				}

				CurrentValue=atof(Str);
				
				if(Flag==1)                      //专为画面检查使用
				{ 
					if(SrcCheckOk(CurrentValue))						
						return CurrentValue;
					else
					{
						strcpy(Str,"Error!");

						Lcd_Fill_Box(42+KEYBOARDOFFSET,52,298+KEYBOARDOFFSET,93,11);
						Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);
					
						for(i=0;i<10;i++)
							Str[i]='\0';
					}
					continue;
				}

				if(CurrentValue>MaxValue||CurrentValue<MinValue)
				{
					strcpy(Str,"Error!");
					Lcd_Fill_Box(42+KEYBOARDOFFSET,52,298+KEYBOARDOFFSET,93,11);
					Lcd_DispAsc_On_Button(40+KEYBOARDOFFSET,50,300+KEYBOARDOFFSET,95,strlen(Str),11,0,1,2,2,Str);
					for(i=0;i<10;i++)
					{
						Str[i]='\0';
						Str_password[i]='\0';
					}
				}
				else
					return CurrentValue;
			}
			flag=0;
		}
	}
}


void AddNum(char *Str,U8 DigitNum,char *Num)
{
	int i=0;
	short DigitNumb;
	Lcd_Fill_Box(42+KEYBOARDOFFSET,52,298+KEYBOARDOFFSET,93,11);

	if(*Num=='.')
	{
		while(*(Str+i)!='\0')
		{
			if(*(Str+i)=='.')
			   return;	
			i++;
		}
	}
	i=0;

	if(strlen(Str)==1&&*(Str)=='0')
	{
		strcpy(Str,Num);
		return;
	}
	
	while(*(Str+i)!='\0'&&*(Str+i)!='.')
	{
		i++;
	}
	if(*(Str+i)=='.')
		DigitNumb=(strlen(Str)-i)-1;
	else
		DigitNumb=(strlen(Str)-i);
	
	if(DigitNumb>=DigitNum&&DigitNum!=0||strlen(Str)==9)
		return;
	else
		strcat(Str,Num);

}





/********************************************************/
/*功能：获取元件的区域类型和大小                          */
/*输入：ScrBufferOffset(元件信息的起始地址)              */
/*返回：函数返回区域类型（RECT或CIRCLE），区域坐标存在以指*/
/*      针形式传过来的参数(Left,Top,Right,Bottom)中      */
/********************************************************/
INT8U Get_Element_Region(INT16U *Left,INT16U *Top,INT16U *Right,INT16U *Bottom)
{
	INT32U temp;
	INT8U RdStringLen,WrStringLen,Type=RECT,ElementType;
	INT8U *ptr = ScreenDataBuffer;
	int StarAddr,i;
	
	i = ScrBufferOffset;          //yuan jian de ru kou zhi zhen 
	ElementType = ScreenDataBuffer[i];
	
	if(ElementType == 1)
	{
		RdStringLen = *(ptr+i+4);
		WrStringLen = *(ptr+i+4+RdStringLen+2+1);
		Type = *(ptr+i+4+RdStringLen+2+1+WrStringLen+2+1);
		temp = 4+RdStringLen+2+1+WrStringLen+2+1+1;

		*Left = *(ptr+i+temp+1);
		*Left = ((*Left)<<8) | (*(ptr+i+temp));
		*Top = *(ptr+i+temp+3);
		*Top = ((*Top)<<8) | (*(ptr+i+temp+2));
		temp = temp+4;
		*Right = *(ptr+i+temp+1);
		*Right = ((*Right)<<8) | (*(ptr+i+temp));
		*Bottom = *(ptr+i+temp+3);
		*Bottom = ((*Bottom)<<8) | (*(ptr+i+temp+2));
		temp = temp+4;
		return Type;
	}
	
	if(ElementType == 2)
	{
		RdStringLen = *(ptr+i+9);
		WrStringLen = *(ptr+i+9+RdStringLen+2+1);
		Type = *(ptr+i+9+RdStringLen+2+1+WrStringLen+2+1);
		temp = 9+RdStringLen+2+1+WrStringLen+2+1+1;

		*Left = *(ptr+i+temp+1);
		*Left = ((*Left)<<8) | (*(ptr+i+temp));
		*Top = *(ptr+i+temp+3);
		*Top = ((*Top)<<8) | (*(ptr+i+temp+2));
		temp = temp+4;
		*Right = *(ptr+i+temp+1);
		*Right = ((*Right)<<8) | (*(ptr+i+temp));
		*Bottom = *(ptr+i+temp+3);
		*Bottom = ((*Bottom)<<8) | (*(ptr+i+temp+2));
		temp = temp+4;
		return Type;
	}
	
	if(ElementType == 3)
	{
		Type = *(ptr+i+3);
		*Left = *(ptr+i+5);
		*Left = ((*Left)<<8) | (*(ptr+i+4));
		*Top = *(ptr+i+7);
		*Top = ((*Top)<<8) | (*(ptr+i+6));
		*Right = *(ptr+i+9);
		*Right = ((*Right)<<8) | (*(ptr+i+8));
		*Bottom = *(ptr+i+11);
		*Bottom = ((*Bottom)<<8) | (*(ptr+i+10));
		return Type;
	}	
	
	if(ElementType == 6)
	{
		Type = 1;
		RdStringLen = *(ptr+i+10+4);
		WrStringLen = *(ptr+i+11+4+RdStringLen+2);
		StarAddr=i;
		*Left=ScreenDataBuffer[StarAddr+17+4+WrStringLen+RdStringLen];
		*Left=(*Left<<8)|ScreenDataBuffer[StarAddr+16+4+WrStringLen+RdStringLen];
		*Top=ScreenDataBuffer[StarAddr+19+4+WrStringLen+RdStringLen];
		*Top=(*Top<<8)|ScreenDataBuffer[StarAddr+18+4+WrStringLen+RdStringLen];
		*Right=ScreenDataBuffer[StarAddr+21+4+WrStringLen+RdStringLen];
		*Right=(*Right<<8)|ScreenDataBuffer[StarAddr+20+4+WrStringLen+RdStringLen];
		*Bottom=ScreenDataBuffer[StarAddr+23+4+WrStringLen+RdStringLen];
		*Bottom=(*Bottom<<8)|ScreenDataBuffer[StarAddr+22+4+WrStringLen+RdStringLen];

		return Type;
	}
	
	return (INT8U)-1;
}




/*********************************************************************/
/*功能：判断所给点是否在所给区域内                                     */
/*输入：待判断的点坐标(PointX,PointY),区域坐标(Left,Top,Right,Bottom) */
/*返回：TRUE(点在区域内)                                              */
/*      FALSE(点不在区域内)                                           */
/*********************************************************************/
INT8U Point_in_Region(INT16U PointX,INT16U PointY,INT16U Left,INT16U Top,INT16U Right,INT16U Bottom,INT8U RegionType)
{
	INT16U Centre_x,Centre_y,Radius;
	int Distance;

	if(RegionType==CIRCLE)
	{
		Centre_x=(Right-Left)/2+Left;
		Centre_y=(Bottom-Top)/2+Top;
		Radius=(Right-Left)/2;
		Distance = sqrt((PointX-Centre_x)*(PointX-Centre_x)+(PointY-Centre_y)*(PointY-Centre_y));
		if(Distance>Radius)
			return FALSE;
		else
			return TRUE;
	}
	else
	{		
		if(PointX>=Left && PointX<=Right && PointY>=Top && PointY<=Bottom)
			return TRUE;
		else
			return FALSE;		
	}		
}



int Max(int a,int b)
{
	return (a>b?a:b);
}

int Min(int a,int b)
{
	return (a<b?a:b);
}



void Check_Ctrl_State(void)
{
	
	int CtrlValue;
	int CtrlValue1;

	CtrlValue = ReadWord(g_PlcCtrlNum,g_PlcCtrStr);
	CtrlValue1 = ReadWord(g_PlcCtrlNum,g_PlcCtrStr);
	if(CtrlValue!=CurrentScrNum && CtrlValue>0 && CtrlValue<255 && CtrlValue==CtrlValue1)
	{
		DisplayUserScr(CtrlValue);
	}

	CtrlValue = ReadWord(g_PlcStaNum,g_PlcStaStr);

	if(CtrlValue!=CurrentScrNum)
		WriteWord(g_PlcStaNum,g_PlcStaStr,CurrentScrNum);
}




/********************************************************/
/*功能：保持/交替/设on/设off型按钮的功能函数              */
/*输入：INT16U StarAddr(元件信息的起始地址)              */
/*返回：NULL                                            */
/*                                                      */
/********************************************************/

void Func_KeepButton(INT16U StarAddr)
{
	INT8U SubType,temp;
	int w,j;
	INT16U PlcReadNum,PlcWriteNum,PlcReadLong,PlcWriteLong;
	char PlcReadStr[5],PlcWriteStr[5];

	SubType= ScreenDataBuffer[StarAddr+1];
	PlcReadLong= ScreenDataBuffer[StarAddr+4];
	for(j=0;j<PlcReadLong;j++)
		PlcReadStr[j]=ScreenDataBuffer[StarAddr+5+j];
	PlcReadStr[j]='\0';

	PlcReadNum=ScreenDataBuffer[StarAddr+6+PlcReadLong];
	PlcReadNum=(PlcReadNum<<8)|ScreenDataBuffer[StarAddr+5+PlcReadLong];

	PlcWriteLong=ScreenDataBuffer[StarAddr+7+PlcReadLong];
	for(j=0;j<PlcWriteLong;j++)
		PlcWriteStr[j]=ScreenDataBuffer[StarAddr+8+PlcReadLong+j];
	PlcWriteStr[j]='\0';
	PlcWriteNum=ScreenDataBuffer[StarAddr+9+PlcWriteLong+PlcReadLong];
	PlcWriteNum=(PlcWriteNum<<8)|ScreenDataBuffer[StarAddr+8+PlcWriteLong+PlcReadLong];

	Beep(1);
	for(w=0;w<BEEP_DELAY;w++);
	Beep(0);

	//+++++++ 080927 +++++++++++++
	if(0 != g_GlobalVar.Keep_flag)  //确保 设on 和保持型 弹起之后才能按其它
	{
		flag=0;
		return;
	}
	//+++++++++++++++++++++++++
	
	if(SubType != 3)
		DrawKeepButton(1,1);

	
	switch(SubType)
	{
		case 0:	//设On
			WriteBit(PlcWriteNum,PlcWriteStr,1);
			//++++++++++++++++++++++++++++++++++
			flag=0;
			g_GlobalVar.Keep_flag=1;
			g_GlobalVar.Screen_Flag=0;	//不允许切换画面
			g_GlobalVar.Keep_StarAddr=StarAddr;
			//++++++++++++++++++++++++++++++++++
			break;
		case 1:	//保持
			WriteBit(PlcWriteNum,PlcWriteStr,1);
			//++++++++++++++++++++++++++++++++++
			flag=0;
			g_GlobalVar.Keep_flag=2;
			g_GlobalVar.Screen_Flag=0;	//不允许切换画面
			g_GlobalVar.Keep_StarAddr=StarAddr;
			strcpy(g_GlobalVar.Keep_PlcWriteStr,PlcWriteStr);
			g_GlobalVar.Keep_PlcWriteNum=PlcWriteNum;
			//++++++++++++++++++++++++++++++++++
			break;
	   	case 2:	//设Off
			WriteBit(PlcWriteNum,PlcWriteStr,0);
			//++++++++++++++++++++++++++++++++++
			flag=0;
			g_GlobalVar.Keep_flag=3;
			g_GlobalVar.Screen_Flag=0;	//不允许切换画面
			g_GlobalVar.Keep_StarAddr=StarAddr;
			//++++++++++++++++++++++++++++++++++
			break;
		case 3:	//交替
			temp=ReadBit(PlcReadNum,PlcReadStr);
			temp=!temp;
  			WriteBit(PlcWriteNum,PlcWriteStr,temp);
			//+++  070601  +++++
			flag=0;
			RefreshKeepButton();
 			//+++++++++++++++
	}			
}


////////// 交替型按钮刷新/////////////////
void RefreshKeepButton(void)
{
	int StarAddr,i,j,PlcReadLong,PlcReadNum;
	char PlcReadStr[5];
	char stats;
	//int k;

	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;

		if(KeepButtonRefresh[i][0]!=0)
		{
			//FeedWacthDog();

			StarAddr=KeepButtonRefresh[i][0];
			ScrBufferOffset = StarAddr;
	
			PlcReadLong= ScreenDataBuffer[StarAddr+4];
			for(j=0;j<PlcReadLong;j++)
				PlcReadStr[j]=ScreenDataBuffer[StarAddr+5+j];
			PlcReadStr[j]='\0';

			PlcReadNum=ScreenDataBuffer[StarAddr+6+PlcReadLong];
			PlcReadNum=(PlcReadNum<<8)|ScreenDataBuffer[StarAddr+5+PlcReadLong];
			
			/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
				&& (0 == strcmp("M", PlcReadStr)))
			{
				strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
				for(k=0; k<DELAY_TIME_TRAN; k++); //100000
			}
			else if(0 == strcmp("M", PlcReadStr))
				for(k=0; k<DELAY_TIME; k++);*/

			stats=ReadBit(PlcReadNum,PlcReadStr);

			if(stats == 0xff)	//通讯出错
				continue;

			if(stats!=KeepButtonRefresh[i][1])
			{
				DrawKeepButton(stats,1);
				KeepButtonRefresh[i][1]=stats;
			}
		}
		else
			return;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
//////////////   3-26-对保持型按钮的刷新/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void RefreshKeep2Button(void)
{
	int StarAddr,i,j,PlcReadLong,PlcReadNum;
	char PlcReadStr[5];
	char stats;
	//int k;

	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;
		
		if(KeepButton2Refresh[i][0]!=0)
		{
			//FeedWacthDog();

			StarAddr=KeepButton2Refresh[i][0];
			ScrBufferOffset = StarAddr;
	
			PlcReadLong= ScreenDataBuffer[StarAddr+4];
			for(j=0;j<PlcReadLong;j++)
				PlcReadStr[j]=ScreenDataBuffer[StarAddr+5+j];
			PlcReadStr[j]='\0';

			PlcReadNum=ScreenDataBuffer[StarAddr+6+PlcReadLong];
			PlcReadNum=(PlcReadNum<<8)|ScreenDataBuffer[StarAddr+5+PlcReadLong];
			
			/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
				&& (0 == strcmp("M", PlcReadStr)))
			{
				strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
				for(k=0; k<DELAY_TIME_TRAN; k++);
			}
			else if(0 == strcmp("M", PlcReadStr))
				for(k=0; k<DELAY_TIME; k++);*/

			stats=ReadBit(PlcReadNum,PlcReadStr);

			if(stats == 0xff)	//通讯出错
				continue;

			if(stats!=KeepButton2Refresh[i][1])
			{
				DrawKeepButton(stats,1);
				KeepButton2Refresh[i][1]=stats;
			}
		}
		else
			return;
	}

}




///////////////////////////////////////////////////////////////////////////////////////////
///////////////   刷新按钮的弹起  主要针对 设on off 保持 交替 按钮  7-4 //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void RefreshButtonUp()
{
	 switch (g_GlobalVar.Keep_flag)
	 {
	 case 1:                                        // 设 On and Off
	 case 3: 
	 	{
			g_GlobalVar.Keep_flag=0;
			g_GlobalVar.Screen_Flag=1;	//允许切换画面
			ScrBufferOffset = g_GlobalVar.Keep_StarAddr;
			DrawKeepButton(0,1);
		}break;
	 case 2:                                        // 保持   
		{
			g_GlobalVar.Keep_flag=0;
			g_GlobalVar.Screen_Flag=1;	//允许切换画面
			ScrBufferOffset = g_GlobalVar.Keep_StarAddr;
			DrawKeepButton(0,1);
			WriteBit(g_GlobalVar.Keep_PlcWriteNum,g_GlobalVar.Keep_PlcWriteStr,0);
		}break;
	 }
}





/********************************************************/
/*功能：换画面按钮的功能函数                             */
/*输入：INT16U StarAddr(元件信息的起始地址)              */
/*返回：NULL                                           */
/*                                                     */
/********************************************************/
void Func_ShiftButton(int StarAddr)
{
	INT8U SubType;
	INT8U ScrNum;
	//int i;

	SubType = ScreenDataBuffer[StarAddr+1];
	if(SubType ==  1)
	{
		DisplayUserScr(PreScrNum);
	}
	else
	{
		ScrNum = ScreenDataBuffer[StarAddr+2];
	
		DisplayUserScr(ScrNum);
	}
}



void Func_SetValue(INT16 StarAddr)
{	
  	int WritePlcValue;
	INT8U WordOrDWord,DigitPos,DigitNumb,ButtonBackColor;
	INT16U PlcWriteNum,PlcWriteLong,PlcReadLong;
	int PlcMaxValue,PlcMinValue;
	int j,w;
	char PlcWriteStr[5];
	INT16U Left,Top,Right,Bottom;

	WordOrDWord=ScreenDataBuffer[StarAddr+1];///////////////////////////]

	DigitPos=ScreenDataBuffer[StarAddr+3];
	DigitNumb=ScreenDataBuffer[StarAddr+4];

	PlcMaxValue=ScreenDataBuffer[StarAddr+8];
	PlcMaxValue=(PlcMaxValue<<8)|ScreenDataBuffer[StarAddr+7];
	PlcMaxValue=(PlcMaxValue<<8)|ScreenDataBuffer[StarAddr+6];
	PlcMaxValue=(PlcMaxValue<<8)|ScreenDataBuffer[StarAddr+5];
	
	PlcMinValue=ScreenDataBuffer[StarAddr+12];
	PlcMinValue=(PlcMinValue<<8)|ScreenDataBuffer[StarAddr+11];
	PlcMinValue=(PlcMinValue<<8)|ScreenDataBuffer[StarAddr+10];
	PlcMinValue=(PlcMinValue<<8)|ScreenDataBuffer[StarAddr+9];

	PlcReadLong=ScreenDataBuffer[StarAddr+10+4];

	PlcWriteLong=ScreenDataBuffer[StarAddr+13+4+PlcReadLong];
	for(j=0;j<PlcWriteLong;j++)
		PlcWriteStr[j]=ScreenDataBuffer[StarAddr+14+4+PlcReadLong+j];
	PlcWriteStr[j]='\0';

	PlcWriteNum=ScreenDataBuffer[StarAddr+15+4+PlcWriteLong+PlcReadLong];
	PlcWriteNum=(PlcWriteNum<<8)|ScreenDataBuffer[StarAddr+14+4+PlcWriteLong+PlcReadLong];

	Left=ScreenDataBuffer[StarAddr+17+4+PlcWriteLong+PlcReadLong];
	Left=(Left<<8)|ScreenDataBuffer[StarAddr+16+4+PlcWriteLong+PlcReadLong];
	Top=ScreenDataBuffer[StarAddr+19+4+PlcWriteLong+PlcReadLong];
	Top=(Top<<8)|ScreenDataBuffer[StarAddr+18+4+PlcWriteLong+PlcReadLong];
	Right=ScreenDataBuffer[StarAddr+21+4+PlcWriteLong+PlcReadLong];
	Right=(Right<<8)|ScreenDataBuffer[StarAddr+20+4+PlcWriteLong+PlcReadLong];
	Bottom=ScreenDataBuffer[StarAddr+23+4+PlcWriteLong+PlcReadLong];
	Bottom=(Bottom<<8)|ScreenDataBuffer[StarAddr+22+4+PlcWriteLong+PlcReadLong];
	ButtonBackColor=ScreenDataBuffer[StarAddr+24+4+PlcWriteLong+PlcReadLong];

	Beep(1);
	for(w=0;w<BEEP_DELAY;w++);
	Beep(0);


	Lcd_Button( Right-28, Top+4, Right-4, Bottom-4,ButtonBackColor, 2,INSERT);
		
	flag=0;  /////////////////////

	WritePlcValue=KeyBoard(PlcMaxValue,PlcMinValue,DigitPos,DigitNumb);
	if(WritePlcValue == -1)
		return;
	if(WordOrDWord==0)
		WriteWord(PlcWriteNum,PlcWriteStr,WritePlcValue);
	else/////////////////////////////////////////////
	{
		WriteWord(PlcWriteNum+1,PlcWriteStr,(WritePlcValue>>16));////////060627
		WriteWord(PlcWriteNum,PlcWriteStr,(WritePlcValue & 0xffff));///////////
	}
}


void RefreshSetValue(void)
{
	INT16U PlcReadNum,PlcReadLong,PlcWriteLong;
	char PlcReadStr[5],DispStr[10],WordOrDWord;
	int CurrentPlcValue,CurrentPlcValue1;
	int StarAddr,i,j,a;
	U8 DigitPos,DigitNumb;
	U16 ValueBackColor,ValueColor;
	INT16U Left,Top,Right,Bottom;
	INT8U ZiSize,HAlign;
	double DispValue;
	//int k;

	//g_Count++;
	
	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;

		if(SetValueRefresh[i]!=0)
		{
			//FeedWacthDog();

			StarAddr=SetValueRefresh[i];
			WordOrDWord=ScreenDataBuffer[StarAddr+1];//////////////////

			DigitPos=ScreenDataBuffer[StarAddr+3];
			DigitNumb=ScreenDataBuffer[StarAddr+4];
			ValueBackColor=(U16)ScreenDataBuffer[StarAddr+9+4];

			PlcReadLong= (U16)ScreenDataBuffer[StarAddr+10+4];
			for(j=0;j<PlcReadLong;j++)
				PlcReadStr[j]=(char)ScreenDataBuffer[StarAddr+11+4+j];
			PlcReadStr[j]='\0';

			PlcReadNum=(U16)ScreenDataBuffer[StarAddr+12+4+PlcReadLong];
			PlcReadNum=(PlcReadNum<<8)|(U16)ScreenDataBuffer[StarAddr+11+4+PlcReadLong];
			PlcWriteLong=(U16)ScreenDataBuffer[StarAddr+13+4+PlcReadLong];

			Left=(U16)ScreenDataBuffer[StarAddr+17+4+PlcWriteLong+PlcReadLong];
			Left=(Left<<8)|(U16)ScreenDataBuffer[StarAddr+16+4+PlcWriteLong+PlcReadLong];
			Top=(U16)ScreenDataBuffer[StarAddr+19+4+PlcWriteLong+PlcReadLong];
			Top=(Top<<8)|(U16)ScreenDataBuffer[StarAddr+18+4+PlcWriteLong+PlcReadLong];
			Right=(U16)ScreenDataBuffer[StarAddr+21+4+PlcWriteLong+PlcReadLong];
			Right=(Right<<8)|(U16)ScreenDataBuffer[StarAddr+20+4+PlcWriteLong+PlcReadLong];
			Bottom=(U16)ScreenDataBuffer[StarAddr+23+4+PlcWriteLong+PlcReadLong];
			Bottom=(Bottom<<8)|(U16)ScreenDataBuffer[StarAddr+22+4+PlcWriteLong+PlcReadLong];
			
			ValueColor=(U16)ScreenDataBuffer[StarAddr+25+4+PlcReadLong+PlcWriteLong];
			ZiSize=ScreenDataBuffer[StarAddr+26+4+PlcReadLong+PlcWriteLong];
			HAlign=ScreenDataBuffer[StarAddr+27+4+PlcReadLong+PlcWriteLong];

			if(WordOrDWord==0)
			{
				/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
					&& (0 == strcmp("MW", PlcReadStr)))
				{
					strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
					for(k=0; k<DELAY_TIME_TRAN; k++);
				}
				else if(0 == strcmp("MW", PlcReadStr))
					for(k=0; k<DELAY_TIME; k++);*/

				CurrentPlcValue=ReadWord(PlcReadNum,PlcReadStr);///////////////
			}
			else
			{
				/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
					&& (0 == strcmp("MW", PlcReadStr)))
				{
					strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
					for(k=0; k<DELAY_TIME_TRAN; k++);
				}
				else if(0 == strcmp("MW", PlcReadStr))
					for(k=0; k<DELAY_TIME; k++);*/

				CurrentPlcValue=ReadWord(PlcReadNum,PlcReadStr);///////////////
				//CurrentPlcValue=CurrentPlcValue & 0x0000ffff;//+++++++
				CurrentPlcValue1=ReadWord(PlcReadNum+1,PlcReadStr);/////////////

				if((0 == strcmp("P_W", PlcReadStr)) 
					|| (0 == strcmp("P_KW", PlcReadStr)))
				{
					CurrentPlcValue=(CurrentPlcValue1<<16) | (CurrentPlcValue & 0x0000ffff);////////
				}
				else
				{
					//if(CurrentPlcValue==-1 && CurrentPlcValue1==-1)
					// 只要一个错误(通讯错误回-1 , 正常为返回0xffff)
					if(CurrentPlcValue==-1 || CurrentPlcValue1==-1)	
						continue;
					else
						CurrentPlcValue=(CurrentPlcValue1<<16) | (CurrentPlcValue & 0x0000ffff);////////
				}
			}
			if(CurrentPlcValue==-1)
			{
				//+++  100203  ++++++++++++++
				if((0 == strcmp("P_W", PlcReadStr)) 
					|| (0 == strcmp("P_KW", PlcReadStr)))
				{
					if(SetValue_PreValue[i] == CurrentPlcValue)
						continue;	// 值没变化，直接返回
					else
						SetValue_PreValue[i] = CurrentPlcValue;

					Lcd_Fill_Box(Left+6, Top+6, Right-32, Bottom-6,ValueBackColor);
					// Lcd_DispAsc_On_Button(Left+4, Top+4, Right-32, Bottom-4,a,
					//	ValueBackColor,ValueColor,ZiSize,HAlign,2,"####");
					Lcd_DispHZ_Asc_On_Button(Left+4, Top+4, Right-30, Bottom-4, 4,
						ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");	// ####
				}
				//++++++++++++++++++++++++++
				
				continue;
			}

			
			if(SetValue_PreValue[i] == CurrentPlcValue)
				continue;	// 值没变化，直接返回
			else
				SetValue_PreValue[i] = CurrentPlcValue;


			//+++  100517  ++++++++++++++
			/*if(((50 == PlcReadNum) && (0 == strcmp("P_W", PlcReadStr))) ||		// P_W50 
				((3000 == PlcReadNum) && (0 == strcmp("MW", PlcReadStr))))	// MW3000 
			{
				sprintf(DispStr,"%x",CurrentPlcValue);
				Lcd_Fill_Box(Left+6, Top+6, Right-32, Bottom-6,ValueBackColor);
				a = strlen(DispStr);
				Lcd_DispAsc_On_Button(Left+4, Top+4, Right-32, Bottom-4,a,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
			}
			else*/
			{
				switch(DigitPos)
				{
					case 0:
							DispValue=(double)CurrentPlcValue;
							break;
					case 1:
							DispValue=(double)CurrentPlcValue/10.0;
							break;
					case 2:
							DispValue=(double)CurrentPlcValue/100.0;
							break;
					case 3:
							DispValue=(double)CurrentPlcValue/1000.0;
							break;
					case 4:
							DispValue=(double)CurrentPlcValue/10000.0;
							break;
				}

				sprintf(DispStr,"%f",DispValue);
				if(DispStr[0]=='0'&&DispStr[1]=='.')
				{
					a=1;
				}
				else
				{
					for(a=0;a<10;a++)
					{
						if(DispStr[a]=='.')
							break;
					}
				}
				Lcd_Fill_Box(Left+6, Top+6, Right-32, Bottom-6,ValueBackColor/*-(g_Count%10)*/);

				if(DigitNumb==0)
					 Lcd_DispAsc_On_Button(Left+4, Top+4, Right-32, Bottom-4,a,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
				else
					 Lcd_DispAsc_On_Button(Left+4, Top+4, Right-32, Bottom-4,a+1+DigitNumb,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
			}
						
		}
		else
			return;
	}

}




/********************************************************/
/*功能：加减型按钮的功能函数              */
/*输入：INT16U StarAddr(元件信息的起始地址)              */
/*返回：NULL                                            */
/*                                                      */
/********************************************************/
void Func_UpButton(int StarAddr)
{
	INT8U SubType,WordOrDWord;//,temp;
	INT16U Step,PlcReadNum,PlcWriteNum,PlcReadLong,PlcWriteLong;
	int LimitValue;
	INT32U CurrentValue,CurrentValue1;	// 060628
	int w,j;
	char PlcReadStr[5],PlcWriteStr[5];

	SubType= ScreenDataBuffer[StarAddr+1];
	WordOrDWord=ScreenDataBuffer[StarAddr+2];
	Step=ScreenDataBuffer[StarAddr+4];
	Step=(Step<<8)|ScreenDataBuffer[StarAddr+3];
	LimitValue=ScreenDataBuffer[StarAddr+8];
	LimitValue=(LimitValue<<8)|ScreenDataBuffer[StarAddr+7];
	LimitValue=(LimitValue<<8)|ScreenDataBuffer[StarAddr+6];
	LimitValue=(LimitValue<<8)|ScreenDataBuffer[StarAddr+5];
	PlcReadLong= ScreenDataBuffer[StarAddr+9];
	for(j=0;j<PlcReadLong;j++)
		PlcReadStr[j]=ScreenDataBuffer[StarAddr+10+j];
	PlcReadStr[j]='\0';

	PlcReadNum=ScreenDataBuffer[StarAddr+11+PlcReadLong];
	PlcReadNum=(PlcReadNum<<8)|ScreenDataBuffer[StarAddr+10+PlcReadLong];

	PlcWriteLong=ScreenDataBuffer[StarAddr+12+PlcReadLong];
	for(j=0;j<PlcWriteLong;j++)
		PlcWriteStr[j]=ScreenDataBuffer[StarAddr+13+PlcReadLong+j];
	PlcWriteStr[j]='\0';

	PlcWriteNum=ScreenDataBuffer[StarAddr+14+PlcWriteLong+PlcReadLong];
	PlcWriteNum=(PlcWriteNum<<8)|ScreenDataBuffer[StarAddr+13+PlcWriteLong+PlcReadLong];

	Beep(1);
	for(w=0;w<BEEP_DELAY;w++);
	Beep(0);
	DrawUpButton(1);

	if(WordOrDWord==0)
		CurrentValue=ReadWord(PlcReadNum,PlcReadStr);
	else
	{
		CurrentValue=ReadWord(PlcReadNum,PlcReadStr);
		CurrentValue1=ReadWord(PlcReadNum+1,PlcReadStr);
		if((CurrentValue & 0xffff)==-1)		//060628
		{
			DrawUpButton(0);
			return;
		}
		else
			CurrentValue=(CurrentValue1<<16)+(CurrentValue & 0xffff);	//060628
	}

	if(CurrentValue==-1)
	{	
		//while(bTSP_DownFlag);
		//	FeedWacthDog();
			
		DrawUpButton(0);
		return;
	}

	switch(SubType)
	{
		case 0:
			if(CurrentValue>LimitValue-Step)
				CurrentValue=LimitValue;
			else
				CurrentValue=CurrentValue+Step;
			if(WordOrDWord==0)
				WriteWord(PlcWriteNum,PlcWriteStr,CurrentValue);
			else
			{
				WriteWord(PlcWriteNum+1,PlcWriteStr,(CurrentValue>>16));	//060628
				WriteWord(PlcWriteNum,PlcWriteStr,(CurrentValue & 0xffff));	//060628
			}
			break;
		case 1:
			if(CurrentValue<LimitValue+Step)
				CurrentValue=LimitValue;
			else
				CurrentValue=CurrentValue-Step;
			if(WordOrDWord==0)
				WriteWord(PlcWriteNum,PlcWriteStr,Max(LimitValue,CurrentValue));
			else
			{
				WriteWord(PlcWriteNum+1,PlcWriteStr,(CurrentValue>>16));	//060628
				WriteWord(PlcWriteNum,PlcWriteStr,(CurrentValue & 0xffff));	//060628
			}
			break;
	}
	
	//while(bTSP_DownFlag);
	//	FeedWacthDog();
			

	DrawUpButton(0);

	////// 070516 加减值后跟着刷新//////
	UpDown_NumDisp(PlcWriteNum,CurrentValue);
	////////////////////////////////////////////
}



////// 070516 加减值后跟着刷新//////
//g_GlobalVar.Which_NumDisp=i; 放在RefreshNumDispButton(void) 中
//数值显示元件地址和偏移地址在DrawNumDispButton(1); 中赋值 
////////////////////////////////////////////
void UpDown_NumDisp(INT16U plcWriteNum,INT32U CurrentPlcValue)
{
	INT16U i,j;
	INT8U WordOrDWord,ValueBackColor,ButtonBackColor,ValueColor;
	INT16U LeftX,LeftY,RightX,RightY,ReadPlcNum;
	INT8U Width,ZiSize,HAlign;
	INT32U temp;
	INT8U RdStringLen,DigitPos,DigitNumb;
	double CurrentDispValue;
	char str[12],a,PlcReadStr[5];
	INT8U *ptr;

	for(j=0;j<RefreshNum;j++)
	{
		if(plcWriteNum==g_GlobalVar.UpD_NumDisp[j][0])
		{
			ScrBufferOffset=g_GlobalVar.UpD_NumDisp[j][1];
			break;
		}
	}
	if((plcWriteNum==0) || (j>=RefreshNum))
		return;

	ptr = ScreenDataBuffer;
	i = ScrBufferOffset;
	
	temp = 0;
	WordOrDWord=*(ptr+i+temp+1);//////////////

	temp=temp+3;
	DigitPos=*(ptr+i+temp);
	temp++;
	DigitNumb=*(ptr+i+temp);
	temp=temp+3+2;
	ValueBackColor=*(ptr+i+temp);
	temp++;
	temp+=4+4;
	RdStringLen = *(ptr+i+temp);
	for(j=0;j<RdStringLen;j++)
		PlcReadStr[j]=*(ptr+i+1+temp+j);
	PlcReadStr[j]='\0';
   	temp = temp+1+RdStringLen;
	
	ReadPlcNum=*(ptr+i+temp+1);
	ReadPlcNum=(ReadPlcNum<<8)|*(ptr+i+temp);
	temp=temp+2;

	LeftX = *(ptr+i+temp+1);
	LeftX = (LeftX<<8) | (*(ptr+i+temp));
	LeftY = *(ptr+i+temp+3);
	LeftY = (LeftY<<8) | (*(ptr+i+temp+2));
	temp = temp+4;
	RightX = *(ptr+i+temp+1);
	RightX = (RightX<<8) | (*(ptr+i+temp));
	RightY = *(ptr+i+temp+3);
	RightY = (RightY<<8) | (*(ptr+i+temp+2));
	temp = temp+4;
	Width = *(ptr+i+temp);
	temp = temp+1;
	ButtonBackColor = *(ptr+i+temp);
	temp = temp+1;
	temp +=4;
	ValueColor=*(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	
	if(CurrentPlcValue==-1)
		return;
	switch(DigitPos)
	{
		case 0:
			CurrentDispValue=(double)CurrentPlcValue;
			break;
		case 1:
			CurrentDispValue=(double)CurrentPlcValue/10.0;
			break;
		case 2:
			CurrentDispValue=(double)CurrentPlcValue/100.0;
			break;
		case 3:
			CurrentDispValue=(double)CurrentPlcValue/1000.0;
			break;
		case 4:
			CurrentDispValue=(double)CurrentPlcValue/10000.0;
	}
	
	sprintf(str,"%f",CurrentDispValue);
	if(str[0]=='0'&&str[1]=='.')
	{
		a=1;
	}
	else
	{
		for(a=0;a<12;a++)
		{
			if(str[a]=='.')
				break;
		}
	}
	Lcd_Fill_Box(LeftX+Width+2, LeftY+Width+2, RightX-Width-2, RightY-Width-2,ValueBackColor);
		if(DigitNumb==0)
		 Lcd_DispAsc_On_Button(LeftX+2, LeftY+2, RightX-2, RightY-2,a,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
	else
		 Lcd_DispAsc_On_Button(LeftX+2, LeftY+2, RightX-2, RightY-2,a+1+DigitNumb,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
}




void RefreshWatchDispButton(void)
{
	INT16U PlcReadNum,PlcReadLong;
	char PlcReadStr[5];
	int CurrentValue;
	int StarAddr,i,j,PrePlcValue;
	
	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;
		
		if(WatchRefresh[i*2]!=0)
		{
			//FeedWacthDog();
		
			StarAddr = WatchRefresh[i*2];
			PlcReadLong = ScreenDataBuffer[StarAddr+1];
			for(j=0;j<PlcReadLong;j++)
				PlcReadStr[j]=ScreenDataBuffer[StarAddr+2+j];
			PlcReadStr[j]='\0';

			PlcReadNum =  ScreenDataBuffer[StarAddr+1+1+PlcReadLong+1];
			PlcReadNum =  (PlcReadNum<<8) | ScreenDataBuffer[StarAddr+1+1+PlcReadLong];

			CurrentValue = ReadWord(PlcReadNum,PlcReadStr);
			if(CurrentValue==-1)
				continue;
			PrePlcValue= WatchRefresh[i*2+1];
			if(CurrentValue!=PrePlcValue)
			{	
				ScrBufferOffset = StarAddr;
				DrawWatchDispButton(CurrentValue,1,PrePlcValue);
				WatchRefresh[i*2+1] = CurrentValue;
			}
		}
		else
		{
			break;
		}
	}
}



void RefreshNumDispButton(void)
{
	U8 i;

	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;
		
		if(NumDispRefresh[i]!=0)
		{
			//FeedWacthDog();
		
			ScrBufferOffset = NumDispRefresh[i];

			////// 070516 加减值后跟着刷新//////
			g_GlobalVar.Which_NumDisp=i;
			//数值显示元件地址和偏移地址在DrawNumDispButton(1); 中赋值 
			////////////////////////////////////////////

			//DrawNumDispButton(1, i);
			DrawNumDispButton(1);
		}
		else
			break;
	}
}




/********************************************************/
/*功能：棒图功能函数              */
/*输入：INT16U StarAddr(元件信息的起始地址)              */
/*返回：NULL                                            */
/*                                                      */
/********************************************************/
void RefreshStickChart(void)
{
	char i;

	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;
		
		if(StickChartRefresh[i][0]!=0)
		{
			//FeedWacthDog();
		
			ScrBufferOffset = StickChartRefresh[i][0];
			Func_StickChart(i);
		}
		else
			break;
	}
}


void Func_StickChart(INT16U which_chart)   
{
	INT32U i,j;
	INT8U BackColor,Color,TargetColor,LowRangeColor,HighRangeColor;
	INT16U LeftX,LeftY,RightX,RightY;//,Radius;
	
	
	INT16U cx,cy,cx1,cy1,cx2,cy2,cx3,cy3;
	INT8U Direction,GraphType,TargetFlag,RangeFlag;
	INT16U Maximum,Minimum,TargetValue,LowLimited,HighLimited;
	int CurrentValue;
	
	INT32U temp;
	INT16U RdStringLen,PlcReadNum;
	char PlcReadStr[5];
	INT8U *ptr = ScreenDataBuffer;      
 
	
	i = ScrBufferOffset;          //yuan jian de ru kou zhi zhen 
		
	Direction = *(ptr+i+1);
	GraphType = *(ptr+i+2);
	Maximum = *(ptr+i+6);
	Maximum = (Maximum<<8)|*(ptr+i+5);
	Minimum = *(ptr+i+8);
	Minimum = (Minimum<<8)|*(ptr+i+7);
	TargetValue = *(ptr+i+10);
	TargetValue = (TargetValue<<8)|*(ptr+i+9);
	LowLimited = *(ptr+i+12);
	LowLimited = (LowLimited<<8)|*(ptr+i+11);
	HighLimited = *(ptr+i+14);
	HighLimited = (HighLimited<<8)|*(ptr+i+13);
	Color = *(ptr+i+15);
	BackColor = *(ptr+i+16);
	TargetColor = *(ptr+i+17);
	LowRangeColor = *(ptr+i+18);
	HighRangeColor = *(ptr+i+19);
	TargetFlag = *(ptr+i+20);
	RangeFlag = *(ptr+i+21);
	
	RdStringLen = *(ptr+i+22);	
	for(j=0;j<RdStringLen;j++)
			PlcReadStr[j]=*(ptr+i+23+j);
	PlcReadStr[j]='\0';

	temp = 22+RdStringLen+1;
	PlcReadNum =  ScreenDataBuffer[i+temp+1];
	PlcReadNum =  (PlcReadNum<<8) | ScreenDataBuffer[i+temp];
	temp+=2;
	CurrentValue=ReadWord(PlcReadNum,PlcReadStr);
	if(CurrentValue==-1)
		return;	

	if(CurrentValue==StickChartRefresh[which_chart][1])
		return;
	else
		StickChartRefresh[which_chart][1]=CurrentValue;

	
	if(CurrentValue>=Maximum)
		CurrentValue =Maximum;
	if(CurrentValue<=Minimum)
		CurrentValue =Minimum;	

	LeftX = *(ptr+i+temp+1);
	LeftX = (LeftX<<8) | (*(ptr+i+temp));
	LeftY = *(ptr+i+temp+3);
	LeftY = (LeftY<<8) | (*(ptr+i+temp+2));
	temp = temp+4;
	RightX = *(ptr+i+temp+1);
	RightX = (RightX<<8) | (*(ptr+i+temp));
	RightY = *(ptr+i+temp+3);
	RightY = (RightY<<8) | (*(ptr+i+temp+2));
	    
	cx=(RightX-LeftX)*(CurrentValue-Minimum)/(Maximum-Minimum);
	cy=(RightY-LeftY)*(CurrentValue-Minimum)/(Maximum-Minimum);	
	
	cx1=(RightX-LeftX)*(TargetValue-Minimum)/(Maximum-Minimum);
	cy1=(RightY-LeftY)*(TargetValue-Minimum)/(Maximum-Minimum);	
	
	cx2=(RightX-LeftX)*(TargetValue-LowLimited)/(Maximum-Minimum);
	cy2=(RightY-LeftY)*(TargetValue-LowLimited)/(Maximum-Minimum);	
	
	cx3=(RightX-LeftX)*(HighLimited-TargetValue)/(Maximum-Minimum);
	cy3=(RightY-LeftY)*(HighLimited-TargetValue)/(Maximum-Minimum);	
 
	Lcd_Fill_Box (LeftX,LeftY,RightX,RightY,BackColor);
	Lcd_Draw_HLine (LeftX,RightX,LeftY,0,2);
	Lcd_Draw_HLine (LeftX,RightX,RightY,0,2);
	Lcd_Draw_VLine (LeftY,RightY,LeftX,0,2);
	Lcd_Draw_VLine (LeftY,RightY,RightX,0,2);

	 Lcd_Disp_CurrentValue(LeftX,LeftY,RightX,RightY,Color,Direction,GraphType,cx,cy);
}


void RefreshMultiState(void)
{
	int i,j,StarAddr;
	U8 FlagWord;
	INT16U RdPlcLen,RdPlcNum;
	char RdPlcStr[5];
	int temp;

	for(i=0;i<RefreshNum;i++)
	{
		if(flag==1)
			return;

		if(MultiStateRefresh[i][0]!=0)
		{
			//FeedWacthDog();
		
			StarAddr=MultiStateRefresh[i][0];
			ScrBufferOffset=StarAddr;
			FlagWord=ScreenDataBuffer[StarAddr+1];
			RdPlcLen=ScreenDataBuffer[StarAddr+34];
			for(j=0;j<RdPlcLen;j++)
				RdPlcStr[j]=ScreenDataBuffer[StarAddr+35+j];
			RdPlcStr[j]='\0';
			RdPlcNum=ScreenDataBuffer[StarAddr+35+RdPlcLen+1];
			RdPlcNum=(RdPlcNum<<8)|ScreenDataBuffer[StarAddr+35+RdPlcLen];

			if(FlagWord==0)
			{
				temp=ReadBit(RdPlcNum,RdPlcStr);

				if(temp == 0xff)	//通讯出错
					continue;

				if(temp==32)
				{
					DrawMultiState(32,1);
					continue;
				}
			
				else if(temp!=MultiStateRefresh[i][1])
				{
					DrawMultiState(temp,1);
					MultiStateRefresh[i][1]=temp;
				}
			}
			else
			{
				temp=ReadWord(RdPlcNum,RdPlcStr);

				if(temp==-1)	//通讯出错
					continue;

				if(temp!=MultiStateRefresh[i][1])
				{
					DrawMultiState(temp,1);
					MultiStateRefresh[i][1]=temp;
				}
			}
		}
	}
}


