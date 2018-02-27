#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "2416addr.h"
#include "2416lib.h"
#include "..\inc\def.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\Function.h"
#include "LCD.h"

#include "..\inc\download.h"
#include "..\inc\nand.h"
#include "..\inc\jiema.h"
#include "..\inc\bmp.h"


char new_NO[6] = {0,2,0,2,1,1};	// 各行的默认值
char *Str[6][8] = 
	{
	//"COM2","COM1","COM2","COM1","COM2","COM1","COM2","COM1",	//070517 COM1 COM2换位，以对应后壳
	"COM1","COM2","COM3","COM4","COM1","COM2","COM3","COM4",	//070517 COM1 COM2换位，以对应后壳
	"4800","9600","19200","38400","43000","56000","57600","115200",
	"NONE","ODD","EVEN","NONE","ODD","EVEN","NONE","ODD",
	//"5","6","7","8","6","7","5","6",	// 后4 个不起作用
	"5","6","7","8","5","6","7","8",
	"1","2","1","2","1","2","1","2",
	"0","1","2","3","5","10","15","20"
	};   //显示: 由0 改为60 min   ---080717



extern INT16U IndexBlock;             //every screen's block and page index table
extern INT16U g_PlcCtrlNum;
extern INT16U g_PlcStaNum;
extern INT16U g_PlcCtrLen;
extern INT16U g_PlcStaLen;
extern INT16U g_StarScr;
extern INT16U g_TotalSrceenNum;
extern char g_PlcCtrStr[5];
extern char g_PlcStaStr[5];
extern char PlcType;

extern SYSTEMINFO g_SystemInfo;

extern INT8U IndexBlockBuffer[2048];

extern INT16U X_result;
extern INT16U Y_result;
extern INT8U flag;
extern INT16U CurrentScrNum;
extern SYSTEMCONFIG g_SystemConf;
extern U8 bTSP_DownFlag;

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



void GetSystenInfo(void)
{
	INT8U PageBuff[2048];
	INT16U j;
	char MapInfoStartOffset;                   //
	char ScreenStrLength;					///
	char PlcStrLength;					///

	//IndexBlock = Logical_To_Physical_Block(1);	
	IndexBlock = Logical_To_Physical_Block(NAND_SCR_INDEX);	
	ReadPage(IndexBlock,5,PageBuff);

	if(PageBuff[0]==0xff&&PageBuff[5]==0xff&&PageBuff[1]==0xff&&PageBuff[2]==0xff&&PageBuff[3]==0xff&&PageBuff[4]==0xff)
		return;

	g_StarScr = PageBuff[2];
	PlcType = PageBuff[1];

	g_PlcCtrLen= PageBuff[3];
	for(j=0;j<g_PlcCtrLen&&j<5;j++)
		g_PlcCtrStr[j]=PageBuff[4+j];
	g_PlcCtrStr[j]='\0';

	g_PlcCtrlNum=PageBuff[5+g_PlcCtrLen];
	g_PlcCtrlNum=(g_PlcCtrlNum<<8)|PageBuff[4+g_PlcCtrLen];

	g_PlcStaLen=PageBuff[6+g_PlcCtrLen];
	for(j=0;j<g_PlcStaLen&&j<5;j++)
		g_PlcStaStr[j]=PageBuff[7+g_PlcCtrLen+j];
	g_PlcStaStr[j]='\0';

	g_PlcStaNum=PageBuff[8+g_PlcCtrLen+g_PlcStaLen];
	g_PlcStaNum=(g_PlcStaNum<<8)|PageBuff[7+g_PlcCtrLen+g_PlcStaLen];

	//////////////////////////////////////////////////////////////////             map  bit info
	MapInfoStartOffset=9+g_PlcCtrLen+g_PlcStaLen;
	g_SystemInfo.IsEnableBitMap=PageBuff[MapInfoStartOffset];
	g_SystemInfo.BitTimeInterval=PageBuff[MapInfoStartOffset+1];
	g_SystemInfo.BitMapCount=PageBuff[MapInfoStartOffset+3];
	g_SystemInfo.BitMapCount=(g_SystemInfo.BitMapCount<<8)|PageBuff[MapInfoStartOffset+2];

	ScreenStrLength=PageBuff[MapInfoStartOffset+4];
	for(j=0;j<ScreenStrLength && j<5;j++)
		g_SystemInfo.ScreenBitMapStr[j]=PageBuff[MapInfoStartOffset+5+j];
	g_SystemInfo.ScreenBitMapStr[j]='\0';

	g_SystemInfo.ScreenBitMapNum=PageBuff[MapInfoStartOffset+5+ScreenStrLength+1];
	g_SystemInfo.ScreenBitMapNum=(g_SystemInfo.ScreenBitMapNum<<8)|PageBuff[MapInfoStartOffset+5+ScreenStrLength];

	PlcStrLength=PageBuff[MapInfoStartOffset+7+ScreenStrLength];

	for(j=0;j<PlcStrLength && j<5;j++)
		g_SystemInfo.PLCBitMapStr[j]=PageBuff[MapInfoStartOffset+8+ScreenStrLength+j];
	g_SystemInfo.PLCBitMapStr[j]='\0';
	g_SystemInfo.PlcBitMapNum=PageBuff[MapInfoStartOffset+8+ScreenStrLength+PlcStrLength+1];
	g_SystemInfo.PlcBitMapNum=(g_SystemInfo.PlcBitMapNum<<8)|PageBuff[MapInfoStartOffset+8+ScreenStrLength+PlcStrLength];

////////////////////////////////////////////////////////////////////////////////		map  word info
	MapInfoStartOffset=MapInfoStartOffset+10+ScreenStrLength+PlcStrLength;
	g_SystemInfo.IsEnableWordMap=PageBuff[MapInfoStartOffset];
	g_SystemInfo.WordTimeInterval=PageBuff[MapInfoStartOffset+1];
	g_SystemInfo.WordMapCount=PageBuff[MapInfoStartOffset+3];
	g_SystemInfo.WordMapCount=(g_SystemInfo.WordMapCount<<8)|PageBuff[MapInfoStartOffset+2];

	ScreenStrLength=PageBuff[MapInfoStartOffset+4];
	for(j=0;j<ScreenStrLength && j<5;j++)
		g_SystemInfo.ScreenWordMapStr[j]=PageBuff[MapInfoStartOffset+5+j];
	g_SystemInfo.ScreenWordMapStr[j]='\0';

	g_SystemInfo.ScreenWordMapNum=PageBuff[MapInfoStartOffset+5+ScreenStrLength+1];
	g_SystemInfo.ScreenWordMapNum=(g_SystemInfo.ScreenWordMapNum<<8)|PageBuff[MapInfoStartOffset+5+ScreenStrLength];

	PlcStrLength=PageBuff[MapInfoStartOffset+7+ScreenStrLength];

	for(j=0;j<PlcStrLength && j<5;j++)
		g_SystemInfo.PLCWordMapStr[j]=PageBuff[MapInfoStartOffset+8+ScreenStrLength+j];
	g_SystemInfo.PLCWordMapStr[j]='\0';
	g_SystemInfo.PlcWordMapNum=PageBuff[MapInfoStartOffset+8+ScreenStrLength+PlcStrLength+1];
	g_SystemInfo.PlcWordMapNum=(g_SystemInfo.PlcWordMapNum<<8)|PageBuff[MapInfoStartOffset+8+ScreenStrLength+PlcStrLength];

	/////////////////////////////////////////////
	g_TotalSrceenNum=PageBuff[MapInfoStartOffset+8+ScreenStrLength+PlcStrLength+2];
	if(g_TotalSrceenNum > 256)
		g_TotalSrceenNum = 0;

		
	g_SystemInfo.LCDType=PageBuff[0];
	g_SystemInfo.PLCType=PlcType;
	g_SystemInfo.StarScr=g_StarScr;
	for(j=0;j<5;j++)
		g_SystemInfo.PlcCtrStr[j]=g_PlcCtrStr[j];
	g_SystemInfo.PlcCtrlNum=g_PlcCtrlNum;
	for(j=0;j<5;j++)
		g_SystemInfo.PlcStaStr[j]=g_PlcStaStr[j];
	g_SystemInfo.PlcStaNum=g_PlcStaNum;
	g_SystemInfo.TotalSrceenNum=g_TotalSrceenNum;

}



void GetScrIndex(void)
{
	int i;

	
	ReadPage(IndexBlock,0,IndexBlockBuffer);
	
	if(IndexBlockBuffer[0]==0xff&&IndexBlockBuffer[5]==0xff
		&&IndexBlockBuffer[1]==0xff&&IndexBlockBuffer[2]==0xff
		&&IndexBlockBuffer[3]==0xff&&IndexBlockBuffer[4]==0xff)
	{
		for(i=0;i<2048;i++)
			IndexBlockBuffer[i]=0;
		return;
	}

}


void Load(void)
{
	 int i;
	//char Num[2],Num1[2];//,Result;
	
	Lcd_Clr(13); 

	Lcd_Button(270,190,370,290,10,0,RAISE);

	flag=0;
	while(1)
	{
		if(flag==1)
		{		
			CalibrateXY();
			if(X_result>270&&X_result<370&&Y_result>190&&Y_result<290)
			{
				flag=0;
				DisplaySystemScr(0);
				return;
			}
#if	Encrypt
//加密设置
			else if(X_result>=2*120&&X_result<=2*200&&Y_result>=2*160&&Y_result<=2*210)   //加密设置
			{
				Beep(1);
				for(i=0;i<BEEP_DELAY;i++);
			 	Beep(0);
				
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				g_GlobalVar.Stop_Communication=1;	//停止通讯

				g_StarScr=STARSCR_ENCRYPT;
				Run();
				
				X_result=0;
				Y_result=0;
				flag = 0;
				return;
			}
#endif
			
			/*else
			{
				Lcd_Fill_Circle(X_result, Y_result, 2, 0);
				Lcd_Circle(X_result, Y_result, 25, 2, 0);	
				
				Beep(1);
				for(i=0;i<BEEP_DELAY;i++);
			 	Beep(0);

				flag = 0;
			}*/
		}
	}
}




void DisplayParScr()
{
	char Info[]={18,46,42,93,41,72,54,35};                 //参数设置
	char TongXunDuanKou[]={45,8,49,22,22,43,31,58,41,72,22,8,3,26};            //CharString:“通讯端口设定:
	char BoTeLvSheDing[]={18,8,44,56,34,42,41,72,22,8,3,26};            //CharString:“波特率设定:
	char JiOuJiaoYan[]={38,70,37,28,48,3,49,73,46,27,41,72,22,8,3,26};            //CharString:“奇偶校验:
	char ShuJuWei[]={42,93,30,61,46,27,41,72,22,8,3,26};            //CharString:“   数据位:
	char TingZhiWei[]={45,3,54,25,46,27,41,72,22,8,3,26};            //CharString:“停止位
	char SrcSave[]={38,33,17,3,41,72,22,8,3,26};            //CharString:屏保设定

	char XuanZe[]={49,1,52,81};            //CharString:“选择
    char XiuGai[]={48,62,24,36};            //CharString:“修改
	char QueDing[]={40,23,22,8};            //CharString:“确定
	char QuXiao[]={40,1,47,91};            //CharString:“取消

	INT16U Left,Top;
	char j,length;
	char num[2];
	
	CurrentScrNum=0xff04;

	switch(g_SystemConf.BaudRates)
	{
		case 4800:
                            new_NO[1]=  0;
				break;
		case 9600:
                            new_NO[1]=  1;
				break;
		case 19200:
                            new_NO[1]=  2;
				break;
		case 38400:
                            new_NO[1]=  3;
				break;
		case 43000:
                            new_NO[1]=  4;
				break;
		case 56000:
                            new_NO[1]=  5;
				break;
		case 57600:
                            new_NO[1]=  6;
				break;
		case 115200:
                            new_NO[1]=  7;
	}


	new_NO[0]=g_SystemConf.ComPort;
	new_NO[2]=g_SystemConf.ParityMode;
	new_NO[3]=g_SystemConf.DataBits;
	new_NO[4]=g_SystemConf.StopBits;
	new_NO[5]=g_SystemConf.ScrSaveTimeSet;
	

	Lcd_Clr(9);	
	Lcd_Fill_Box(0, 0, 320*2,34*2, 11);	
	Lcd_DispHZ_On_Button(100*2,5*2,220*2,20*2,4,0,11,3,0,1,0,0,Info);    //显示参数设置
	Lcd_Fill_Box(0, 206*2, 320*2,240*2, 11);

	Left = 10*2;
	Top = 100;					//43*2;

	Lcd_Display_String(Left,Top,6,0,6,2,TongXunDuanKou,0);  //通讯端口设定:
	Lcd_Fill_Box(162*2,94,214*2,(94+36),11);

	j=new_NO[0];
	length=strlen(Str[0][j]);
   	Lcd_DispAsc_On_Button(162*2,94,214*2,130,length,11,0,1,0,2,Str[0][j]);
	num[0]=3;       
	num[1]=26;                                                   //:
	Lcd_Display_String(168,104,1,0,9,0,num,0);

	Top = 100+48;					// 2*(34*2+9);

	Lcd_Display_String(Left,Top,5,0,9,2,BoTeLvSheDing,0);                  //波特率设定
	j=new_NO[1];
	length=strlen(Str[1][j]);
	Lcd_DispAsc_On_Button(162*2,142,214*2,142+36,length,9,0,1,0,2,Str[1][j]);
	Lcd_Display_String(144,152,1,2,1,0,num,0);

	Top = 100+48*2;					// 2*(34*3+9);

	Lcd_Display_String(Left,Top,7,0,9,2,JiOuJiaoYan,0);                //“奇偶校验:
	j=new_NO[2];
	length=strlen(Str[2][j]);
	Lcd_DispAsc_On_Button(162*2, 190, 2*214, (190+36),length,9,0,1,0,2,Str[2][j]);
	Lcd_Display_String(192,200,1,0,9,0,num,0);

	Top = 100+48*3;					// 2*(34*4+9);

	Lcd_Display_String(Left,Top,5,0,9,2,ShuJuWei,0);                    // 数据位:
	j=new_NO[3];
	length=strlen(Str[3][j]);
	Lcd_DispAsc_On_Button(162*2, 242, 2*214, (242+36),length,9,0,1,0,2,Str[3][j]);
	Lcd_Display_String(144,248,1,0,9,0,num,0);

	Top = 100+48*4;					// 2*(34*5+9);

	Lcd_Display_String(Left,Top,5,0,9,2,TingZhiWei,0);                 // 停止:
	j=new_NO[4];
	length=strlen(Str[4][j]);
	Lcd_DispAsc_On_Button(162*2, 286, 2*214, (286+36),length,9,0,1,0,2,Str[4][j]);
	Lcd_Display_String(144,292,1,0,9,0,num,0);

	Top = 100+48*5;					// 2*(34*5+9);

	Lcd_Display_String(Left,Top,4,0,9,2,SrcSave,0);                 // 屏保:
	j=new_NO[5];
	length=strlen(Str[5][j]);
	Lcd_DispAsc_On_Button(162*2, 334, 2*214, (334+36),length,9,0,1,0,2,Str[5][j]);
	Lcd_Display_String(120,340,1,0,9,0,num,0);


	Lcd_Draw_VLine (35*2, 2*205, 2*216, 11, 3);
	
	Left=220*2;
	Top = 2*(34+13.5);

	Lcd_Display_String(Left,2*43,2,0,9,2,XuanZe,0);                  // 选择
	Lcd_Button(224*2, 2*61,2*264, 2*86,5, 2,RAISE);
	num[0]=1;
	num[1]=92;
	Lcd_DispHZ_On_Button(224*2, 2*61,2*264, 2*86,1,0,5,1,0,0,0,0,num);    //显示 up

	Lcd_Button(2*273, 2*61,2*313, 2*86,5,2,RAISE);
	num[0]=1;
	num[1]=93;
	Lcd_DispHZ_On_Button(2*273, 2*61,2*313, 2*86,1,0,5,1,0,0,0,0,num);    //显示 donw

        Top = 2*(34+43+13.5);

	Lcd_Display_String(Left,Top,2,0,9,2,XiuGai,0);                  // 修改
	Lcd_Button(2*224, Top+18*2, 2*264, Top+43*2,5, 2,RAISE);
	num[0]=1;
	num[1]= 91 ;
	Lcd_DispHZ_On_Button(2*224, Top+18*2, 2*264, Top+43*2,1,0,5,1,0,0,0,0,num);    //显示 +

	Lcd_Button(2*273, Top+18*2, 2*313, Top+43*2,5, 2,RAISE);
	num[0]=1;
	num[1]=90 ;
	Lcd_DispHZ_On_Button(2*273, Top+18*2, 2*313, Top+43*2,1,0,5,1,0,0,0,0,num);    //显示 -

	Top = 2*(34+43*2+13.5);

   	Lcd_Button(Left+8, Top+10, 2*313, Top+70,5, 2,RAISE);
	Lcd_DispHZ_On_Button(Left+8, Top+10, 2*313, Top+70,2,0,5,2,0,0,0,3,QueDing);    //显示 确定

	Top = 2*(34+43*3+13.5);

   	Lcd_Button(Left+8, Top-10, 2*313, Top+50,5,2,RAISE);
	Lcd_DispHZ_On_Button(Left+8, Top-10, 2*313, Top+50,2,0,5,2,0,0,0,3,QuXiao);    //显示 取消

}

void ParScrKeyScan()
{
	char QueDing[]={40,23,22,8};            //CharString:“确定
	char QuXiao[]={40,1,47,91};            //CharString:“取消
	int Left,Top;
	//int Rate;
	char new_i,old_i;

	int w,baud;
	char Num[2];
	
	new_i=old_i=0;

	while(1)
	{
		//FeedWacthDog();
		if(flag==1)
		{
			CalibrateXY();		
			if(X_result>=224*2&&X_result<=313*2&&Y_result>=138.5*2&&Y_result<=2*168.5)   //button 确定
			{
				//rINTMSK|=BIT_EINT1;
				Left = 220*2;
				Top = 2*(34+43*2+13.5);
				Lcd_Button(Left+8, Top+10, 2*313, Top+70,5, 2,INSERT);
	            		Lcd_DispHZ_On_Button(Left+8, Top+10, 2*313, Top+70,2,0,9,2,0,0,0,3,QueDing); 

				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
				
 				baud=atoi(Str[1][new_NO[1]]);                         //把波特率改为int型
				g_SystemConf.BaudRates=baud;
				g_SystemConf.ComPort=new_NO[0];
				g_SystemConf.ParityMode=new_NO[2];
				g_SystemConf.DataBits=new_NO[3];
				g_SystemConf.StopBits=new_NO[4];
				g_SystemConf.ScrSaveTimeSet=new_NO[5];

				NumToSaveScrTime();
		
				StorSysConf();

				old_i=0;

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				return;
			}

			if(X_result>=273*2&&X_result<=2*313&&Y_result>=2*61&&Y_result<=2*86)   //button down  you
			{
				//rINTMSK|=BIT_EINT1;
				Left = 2*273;
				Top = 2*61;
				Num[0]=1;
				Num[1]=93;
				Lcd_Button(2*273, Top,2*313, 2*86,5,2,INSERT);
				Lcd_DispHZ_On_Button(2*273, Top,2*313, 2*86,1,0,9,1,0,0,0,0,Num); 
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
			
				if(old_i!=5)
					new_i++;
				else
					new_i=0;
				
				DrawBackColor(3,old_i,new_i);
				old_i=new_i;			
           		
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(2*273, Top,2*313, 2*86,5,2,RAISE);
				Lcd_DispHZ_On_Button(2*273, Top,2*313, 2*86,1,0,9,1,0,0,0,0,Num); 
			
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*224&&X_result<=2*264&&Y_result>=2*61&&Y_result<=2*86)   //button up  zuo
			{
				//rINTMSK|=BIT_EINT1;
				Num[0]=1;
				Num[1]=92;
				Lcd_Button(2*224, 2*61,2*264, 2*86,5, 2,INSERT);
				Lcd_DispHZ_On_Button(2*224, 2*61,2*264, 2*86,1,0,9,1,0,0,0,0,Num); 
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
			
				if(old_i!=0)
					new_i--;
				else
			  		new_i=5;

				DrawBackColor(3,old_i,new_i);
				old_i=new_i;

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(2*224, 2*61,2*264, 2*86,5, 2,RAISE);
				Lcd_DispHZ_On_Button(2*224, 2*61,2*264, 2*86,1,0,9,1,0,0,0,0,Num); 			

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*224&&X_result<=2*264&&Y_result>=2*108.5&&Y_result<=2*133.5)   //button -
			{
				//rINTMSK|=BIT_EINT1;
			    	Top = 2*(34+43+13.5);
				Num[0]=1;
				Num[1]=91;
			   	 Lcd_Button(2*224, Top+18*2, 2*264, Top+43*2,5, 2,INSERT);
				Lcd_DispHZ_On_Button(2*224, Top+18*2, 2*264, Top+43*2,1,0,9,1,0,0,0,0,Num); 
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
				
 				DrawBackColor(2,old_i,new_i);
				old_i=new_i;
			
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(2*224, Top+18*2, 2*264, Top+43*2,5, 2,RAISE);
				Lcd_DispHZ_On_Button(2*224, Top+18*2, 2*264, Top+43*2,1,0,9,1,0,0,0,0,Num); 
			

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*273&&X_result<=2*313&&Y_result>=2*108.5&&Y_result<=2*133.5)   //button +
			{
				//rINTMSK|=BIT_EINT1;
				Top = 2*(34+43+13.5);
				Num[0]=1;
				Num[1]=90;
				Lcd_Button(2*273, Top+36, 2*313, Top+43*2,5, 2,INSERT);
			 	Lcd_DispHZ_On_Button(2*273, Top+36, 2*313, Top+43*2,1,0,9,1,0,0,0,0,Num); 
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
				
 				DrawBackColor(1,old_i,new_i);
				old_i=new_i;			
	
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				Lcd_Button(2*273, Top+36, 2*313, Top+43*2,5, 2,RAISE);
				Lcd_DispHZ_On_Button(2*273, Top+36, 2*313, Top+43*2,1,0,9,1,0,0,0,0,Num); 
			
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
		
			if(X_result>=2*224&&X_result<=2*313&&Y_result>=2*171.5&&Y_result<=2*201.5)   //button 取消
			{
				//rINTMSK|=BIT_EINT1;
				Left = 2*220;
				Top = 2*(34+43*3+13.5);
				Lcd_Button(Left+8, Top-10, 2*313, Top+50,5,2,INSERT);
				Lcd_DispHZ_On_Button(Left+8, Top-10, 2*313, Top+50,2,0,9,2,0,0,0,3,QuXiao);    //显示 取消
				
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
				
 				old_i=0;

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				return;
			}
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			X_result=0;
			Y_result=0;
			flag=0;
		}
	}			
}



void DrawBackColor(char flag,char old_i,char new_i)
{
	int bottom,top;
	int bottom1,top1;

	int j,length;

	switch(new_i)
	{
		case 0:
			{
				bottom=130    ;
				top= 94     ;
			};break;

		case 1:
			{
				bottom=  142+36   ;
				top= 142    ;
			};break;

		case 2:	
			{
				bottom=  190+36;
				top=  190 ;
			};break;

		 case 3:
			{
				bottom=  242+36;
				top= 242    ;
			};break;

		 case 4:
			{
				bottom= 286+36;
				top=   286 ;
			};break;
		 case 5:
			{
				bottom= 334+36;
				top=   334 ;
			};break;

	}
	
	switch(old_i)
	{
		case 0:
			{
				bottom1=130    ;
				top1= 94     ;
			};break;

		case 1:
			{
				bottom1=  142+36   ;
				top1= 142    ;
			};break;

		case 2:	
			{
				bottom1=  190+36;
				top1=  190 ;
			};break;

		 case 3:
			{
				bottom1=  242+36;
				top1= 242    ;
			};break;

		 case 4:
			{
				bottom1= 286+36;
				top1=   286 ;
			};break;
		 case 5:
			{
				bottom1= 334+36;
				top1=   334 ;
			};break;
	}
	
	if(flag==1)                              //button  +
	{	
		if(new_i==0 || new_i==3)
		{
			if(new_NO[new_i]!=3)
				new_NO[new_i]++;
			else
				new_NO[new_i]=0;
		}
		else if(new_i==1 || new_i==5)
		{
			if(new_NO[new_i]!=7)
				new_NO[new_i]++;
			else
				new_NO[new_i]=0;
		}
		else if(new_i==2)
		{
			if(new_NO[new_i]!=2)
				new_NO[new_i]++;
			else
				new_NO[new_i]=0;
		}
		else if(new_i==4)
		{
			if(new_NO[new_i]==0)
				new_NO[new_i]=1;
			else
				new_NO[new_i]=0;
		}
	}

	if(flag==2)                              //button  -
	{
		if(new_i==0 || new_i==3)
		{
			if(new_NO[new_i]!=0)
				new_NO[new_i]--;
			else
				new_NO[new_i]=3;
		}
		else if(new_i==1 ||new_i==5)
		{
			if(new_NO[new_i]!=0)
				new_NO[new_i]--;
			else
				new_NO[new_i]=7;

		}
		else if(new_i==2)
		{
			if(new_NO[new_i]!=0)
				new_NO[new_i]--;
			else
				new_NO[new_i]=2;
		}
		else if(new_i==4)
		{
			if(new_NO[new_i]==1)
				new_NO[new_i]=0;
			else
				new_NO[new_i]=1;
		
		}
	}

	if(flag==3)	// button  Up Down
	{
		if(new_i!=old_i)
		{
			Lcd_Fill_Box( 2*162,top1 ,2*214,bottom1,9);   
  			j=new_NO[old_i];
			length=strlen(Str[old_i][j]);
			Lcd_DispAsc_On_Button(2*162,top1 ,2*214,bottom1,length,1,0,1,0,2,Str[old_i][j]);
		}
	}
	
	Lcd_Fill_Box( 2*162,top ,2*214,bottom,11);                              //画背景
	
	j=new_NO[new_i];
	length=strlen(Str[new_i][j]);
	Lcd_DispAsc_On_Button(2*162,top ,2*214,bottom,length,11,0,1,0,2,Str[new_i][j]);
}





void DisplayScrCheck(char CurrentSrcNum)
{
	char Info[]={27,13,35,70,28,76,18,73};                      //画面检查
	char JianCha[]={28,76,18,73};
	char FanHui[]={23,21,27,56};
	char All[]={51,6,51,35,19,44,48,82,25,18,51,48,27,13,35,70,42,93,3,26};    ///应用程序共有画面数:
	char Current[]={21,17,39,16,28,76,18,73,27,13,35,70,26,37,3,26};           // 当前检查画面号:
	char Tishi[]={50,27,23,89,27,13,35,70,28,76,18,73,45,74,17,47,3,12,16,20,40,46,50,66,28,92,23,21,27,56};   //安任意建返回

	char num[2];
	char str[4]={'\0','\0','\0','\0'};
	
	CurrentScrNum=0xff05;
	
	Lcd_Clr(9);	
	Lcd_Fill_Box(0, 0, 320*2,34*2, 11);	
	Lcd_DispHZ_On_Button(2*100,2*5,2*220,2*20,4,0,11,3,0,1,0,0,Info);    //显示画面检查
	Lcd_Fill_Box(0, 206*2, 2*320,2*240, 11);
	Lcd_DispHZ_On_Button(0, 206*2, 2*320,2*234,15,0,11,1,0,0,0,0,Tishi);  

	Lcd_DispHZ_On_Button(0,2*40,2*320,2*56,9,0,6,2,0,1,0,0,All);  //应用程序共有画面数:
	num[0]=3;
	num[1]=26;
	Lcd_Display_String(430,80,1,0,9,0,num,0);                     	//:
	sprintf(str,"%d",g_TotalSrceenNum);
	Lcd_DispAsc_On_Button(440,79,440+50,2*56,strlen(str),9,0,1,0,0,str);

	Lcd_DispHZ_On_Button(0,60*2,2*320,2*76,7,0,9,2,0,1,0,0,Current); //当前检查画面号:
	Lcd_Display_String(406,120,1,0,9,0,num,0);                     	//:
	sprintf(str,"%d",CurrentSrcNum);
	Lcd_Fill_Box(120*2, 2*86, 2*200, 2*117,11);       
	Lcd_DispAsc_On_Button(120*2, 2*86, 2*200, 2*117,strlen(str),11,0,1,0,0,str);

	Lcd_Button(50*2, 2*87, 2*100, 2*117,5, 2,RAISE);
	num[0]=1;
	num[1]= 92;
	Lcd_DispHZ_On_Button(50*2, 2*87, 2*100, 2*117,1,0,5,1,0,0,0,0,num);    //显示 +

	
	Lcd_Button(220*2, 2*87, 2*270, 2*117,5, 2,RAISE);
	num[0]=1;
	num[1]= 93;
	Lcd_DispHZ_On_Button(220*2, 2*87, 2*270, 2*117,1,0,5,1,0,0,0,0,num);    //显示 -

	Lcd_Button(120*2, 2*124, 2*200, 2*158,5, 2,RAISE);
	Lcd_DispHZ_On_Button(120*2, 2*124, 2*200, 2*158,2,0,5,2,0,0,0,0,JianCha);    //检查

	Lcd_Button(120*2, 2*165, 2*200, 2*199,5, 2,RAISE);
	Lcd_DispHZ_On_Button(120*2, 2*165, 2*200, 2*199,2,0,5,2,0,0,0,0,FanHui);    //返回

}




void ScrCheckScan(void)
{
	char num[2];
	int w,i;
	char JianCha[]={28,76,18,73};
	char FanHui[]={23,21,27,56};
	char CurrentScrNum=0;	
	INT16U src[256]={0};
	
	for(i=0;i<g_TotalSrceenNum;i++)
		src[i]=IndexBlockBuffer[5*i+1];
	sort(src,g_TotalSrceenNum);

        while(1)
	{
		//FeedWacthDog();
		if(flag==1)
		{
			CalibrateXY();
			if(X_result>=2*50&&X_result<=2*100&&Y_result>=2*87&&Y_result<=2*117)   //button +
			{
				//rINTMSK|=BIT_EINT1;
				Lcd_Button(2*50, 2*87, 2*100, 2*117,5, 2,INSERT);
				num[0]=1;
				num[1]= 92;
				Lcd_DispHZ_On_Button(2*50, 2*87, 2*100, 2*117,1,0,5,1,0,0,0,0,num);    //显示 +
 			

				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
		 			
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				CurrentScrNum=ChangeScreenNum(CurrentScrNum,0,src);
				Lcd_Button(2*50, 2*87, 2*100, 2*117,5, 2,RAISE);
				Lcd_DispHZ_On_Button(2*50, 2*87, 2*100, 2*117,1,0,5,1,0,0,0,0,num);    //显示 +
			
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*220&&X_result<=2*270&&Y_result>=2*87&&Y_result<=2*117)   //button -
			{
				//rINTMSK|=BIT_EINT1;
				Lcd_Button(2*220, 2*87, 2*270, 2*117,5, 2,INSERT);
				num[0]=1;
				num[1]= 93;
				Lcd_DispHZ_On_Button(2*220, 2*87, 2*270, 2*117,1,0,5,1,0,0,0,0,num);    //显示 -
			
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
						
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				CurrentScrNum=ChangeScreenNum(CurrentScrNum,1,src);
				Lcd_Button(2*220, 2*87, 2*270, 2*117,5, 2,RAISE);
				Lcd_DispHZ_On_Button(2*220, 2*87, 2*270, 2*117,1,0,5,1,0,0,0,0,num);    //显示 -
			
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*120&&X_result<=2*200&&Y_result>=2*124&&Y_result<=2*158)   //button 检查
			{
				//rINTMSK|=BIT_EINT1;
				Lcd_Button(2*120, 2*124, 2*200, 2*158,5, 2,INSERT);
				Lcd_DispHZ_On_Button(2*120, 2*124, 2*200, 2*158,2,0,5,2,0,0,0,0,JianCha);    //检查
			
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

		       	DisplayCheckUserScr(src[CurrentScrNum]);
						
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				
				while(!bTSP_DownFlag);	// 取反
					//FeedWacthDog();
				while(bTSP_DownFlag);
					//FeedWacthDog();

				DisplayScrCheck(src[CurrentScrNum]);
				for(i=0;i<RefreshNum;i++)
				{
					WatchRefresh[2*i]=0;
					WatchRefresh[2*i+1]=0;
					StickChartRefresh[i][0]= 0;
					StickChartRefresh[i][1]= -1;	//以使第一次刷新
					KeepButtonRefresh[i][0]=0;
					KeepButtonRefresh[i][1]=0;
					KeepButton2Refresh[i][0]=0;
					KeepButton2Refresh[i][1]=0;
					NumDispRefresh[i]=0;
					SetValueRefresh[i] = 0;
					MultiStateRefresh[i][0]=0;
					MultiStateRefresh[i][1]=0;
					TimerButtonRefresh[i]=0;
					OscButtonRefresh[i]=0;
				}
				X_result=0;
				Y_result=0;
				flag = 0;
			}

			if(X_result>=2*120&&X_result<=2*200&&Y_result>=2*165&&Y_result<=2*199)   //button 返回
			{
				//rINTMSK|=BIT_EINT1;
				Lcd_Button(2*120,2*165, 2*200, 2*199,5, 2,INSERT);
				Lcd_DispHZ_On_Button(2*120,2*165, 2*200, 2*199,2,0,5,2,0,0,0,0,FanHui);    //返回
			
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);
						
				//while(bTSP_DownFlag);
				//	FeedWacthDog();

 				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
				return;
			}
			if(X_result>=2*120&&X_result<=2*200&&Y_result>=2*86&&Y_result<=2*117)   //button 直接输入			
			{
				//rINTMSK|=BIT_EINT1;
				Beep(1);
				for(w=0;w<BEEP_DELAY;w++);
				Beep(0);

				//while(bTSP_DownFlag);
				//	FeedWacthDog();

				flag=0;
				CurrentScrNum=Func_SrcCheck(src,CurrentScrNum);
				DisplayScrCheck(src[CurrentScrNum]);           ///////////////

				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				X_result=0;
				Y_result=0;
				flag = 0;
			}
			//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
			X_result=0;
			Y_result=0;
			flag=0;
		}
	}
}


char ChangeScreenNum(char srcnum,char flag,INT16U b[])
{
	char str[4]={'\0','\0','\0','\0'};
		
	if(flag==0)                                // +
	{
		if(srcnum==g_TotalSrceenNum-1)	
			return srcnum;
		else
		{
			srcnum++;						
			sprintf(str,"%d",b[srcnum]);
			Lcd_Fill_Box(2*120, 2*86, 2*200, 2*117,11);
			Lcd_DispAsc_On_Button(2*120, 2*86, 2*200, 2*117,strlen(str),11,0,1,0,0,str);
		}
	}
	else                                        // -
	{
		if(srcnum==0)	
			return srcnum;
		else
		{
			srcnum--;		
			sprintf(str,"%d",b[srcnum]);
			Lcd_Fill_Box(2*120, 2*86, 2*200, 2*117,11);
			Lcd_DispAsc_On_Button(2*120, 2*86, 2*200, 2*117,strlen(str),11,0,1,0,0,str);
		}
	}	
	return srcnum;		
}

void sort(INT16U b[],INT16U Count)
{
	INT16U i,j,t;
	
	if(Count==0)
		return;
	for(i=Count-1;i>=1;i--)
	{
		for(j=0;j<i;j++)
		{
			if(b[j]>b[j+1])
			{
				t=b[j+1];
				b[j+1]=b[j];
				b[j]=t;
			}
		}
	}
}

void DisplayCheckUserScr(INT16U ScrNumber)
{
	INT16U ComponentCount;
	INT8U temp8U;
	INT32U i=0;
	int ScrPrt;
	 
	GetScreenData(ScrNumber);
	temp8U = ScreenDataBuffer[1];

   	Lcd_Clr(temp8U);

	ComponentCount = ScreenDataBuffer[3];
	ComponentCount = ComponentCount<<8 | ScreenDataBuffer[2];
	for(i=0;i<ComponentCount;i++)
	{
		ScrPrt = ScreenDataBuffer[4+i*3];
		ScrPrt = ScrPrt | (ScreenDataBuffer[4+i*3+1]<<8);
		ScrPrt = ScrPrt | (ScreenDataBuffer[4+i*3+2]<<16);
		ScrPrt = ScrPrt & 0x00ffffff;
		ScrBufferOffset = ScrPrt;
		temp8U = ScreenDataBuffer[ScrPrt];
		
		switch(temp8U)
		{
			case 0:break;
			case 1:
				{
					DrawKeepButton(0,0);
					break;
				}
			case 2:
				{
					DrawUpButton(0);
					break;
				}
			case 3:
				{
					DrawShiftButton(0);
					break;
				}
			case 4:
				{
					DrawMultiState(0,0);
					break;
				}
			case 6:
				{
					DrawSetValueButton(0);
					break;
				}
			case 7:
				{
					DrawWatchDispButton(0,0,0);
					break;
				}
			case 8:
				{
					//DrawNumDispButton(0, 0);
					DrawNumDispButton(0);
					break;
				}
			case 9:
				{
					DrawStickChart();
					break;
				}
			case 10:
				{
					DrawStaticText();
					break;
				}
			case 20:
				{
					DrawBmp(ScrBufferOffset);
					break;
				}
			case 22:
				{
					//DrawTimerButton(0);
					break;
				}
			case 25:
				{
					//DrawOscButton(0);
					break;
				}

		}		
	}
	
}

char Func_SrcCheck(INT16U data[],char num)
{
	char max,min,i;
	char maxstr[4]={'\0','\0','\0','\0'};
	char minstr[4]={'\0','\0','\0','\0'};
	int result;
	//char str[4]={'\0','\0','\0','\0'};

	min=(char)data[0];
	max=(char)data[g_TotalSrceenNum-1];

	sprintf(maxstr,"%d",max);
	sprintf(minstr,"%d",min);

	DisplayKeyBoard(maxstr,minstr);
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

	result=(int)KeyBoardScan(max,min,0,1);
	if(result==-1)                            //小健盘输如 取消
	{
		return num;
	}
	for(i=0;i<g_TotalSrceenNum;i++)
	{
		if(result==data[i])
		       	result=i;
	}
	return result;
}


char SrcCheckOk(double num)
{
	char data=(char)num;
	char i;
	for(i=0;i<g_TotalSrceenNum;i++)
	{
		if(data==IndexBlockBuffer[i*5+1])
			return data;
	}
	return 0;
}




