#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "..\inc\def.h"
#include "..\inc\download.h"
#include "..\inc\LCD.h"
#include "2416lib.h"
#include "..\inc\systemconf.h"
#include "2416addr.h"

#include "..\inc\nand.h"
#include "..\inc\systemmenu.h"


extern INT16U X_result;
extern INT16U Y_result;

extern INT16U CurrentScrNum;
extern SYSTEMCONFIG g_SystemConf;

extern INT16U IndexBlock;
extern INT16U IndexBlockOffset;
extern INT8U  ScreenDataBuffer[620000];
extern INT8U IndexBlockBuffer[PAGE_SIZE];


  
void DownLoad(void)
{
	INT16U PhysicalBlock;
	INT32U i;
	INT16U TimeCount,ScrCount,temp;
	INT8U TimeOutFlag,ControlCount,StateCount;
	char LianJieZhong[]={33,12,29,51,54,48};     //CharString "连接中" 
	char XiaZaiZhong[]={47,34,52,56,54,48};      //CharString "下载中"  
	//INT8U ReadBuf[528];
	//INT8U ReadBuf[PAGE_SIZE];

	//int  UartNumTemp;
	char ScreenBitStrLength;
	char PlcBitStrLength;
	char ScreenWordStrLength;
	char PlcWordStrLength;
	//U8   VerDiffFlag = 0;
	U8	 Err =0;
	U8   VerDiffCount = 0;


	U8 u8temp;


	temp = g_SystemConf.ComPort;
	Uart_Select(temp);
	if(!UART_PrintStr)
		Uart_InitDownload(0,38400,DOWNLOAD);
	//Uart_Init(0,38400);

	//FeedWacthDog();
	
	IndexBlock = Logical_To_Physical_Block(NAND_SCR_INDEX);
	IndexBlockOffset = 0;

	PhysicalBlock = Logical_To_Physical_Block(NAND_SCR_INFO);
	IndexBlockBuffer[2] = PhysicalBlock>>8;
	IndexBlockBuffer[3]	= PhysicalBlock;
	IndexBlockBuffer[4] = 0;

	//FeedWacthDog();

	TimeCount = 0;
	Lcd_Clr(7);
	//Lcd_Clr(BLUE);
	Lcd_Display_String(300,170,3,0,7,3,LianJieZhong,0);
	//Lcd_Display_String(300,170,3,BLACK,BLUE,3,LianJieZhong,0);
	Lcd_Jincheng_Tu(0,20,0,0,7,40,250,560,50);
	//Lcd_Jincheng_Tu(0,20,0, BLACK,BLUE, 40,250,560,50);
	rUFCON0= rUFCON0|0x2;           //Clear the FIFO of Uart0             
	while(1)
	{
		//FeedWacthDog();

		if((u8temp=Uart_DownloadGetch(&Err)) == VERSION1)
		{
			if((u8temp=Uart_DownloadGetch(&Err)) == VERSION2)
			{
				Uart_SendByte(VERSION1);
				Uart_SendByte(VERSION2);
				Uart_SendByte(VERSION3);
				
				if((u8temp=Uart_DownloadGetch(&Err))=='E')
				{
					if((u8temp=Uart_DownloadGetch(&Err))=='R')
					{
						if((u8temp=Uart_DownloadGetch(&Err))=='S')
						{
							TimeOutFlag = 0;
							break;
						}
					}
				}
			}
		}
		
		if(Err == 1)                     //
		{
			TimeCount++;
			Lcd_Jincheng_Tu(0,20,TimeCount,0,7,40,250,560,50);
			//Lcd_Jincheng_Tu(0,20,TimeCount,BLACK,BLUE,40,250,560,50);
		}
		else
			VerDiffCount++;

		if(	VerDiffCount >=3)
		{
			DisplayVerDiffScr(0);	//显示版本不对画面
			return;
		}
		if(TimeCount >= 20)
		{
			TimeOutFlag = 1;
			break;
		}
	}
	
	
	if(TimeOutFlag==1)
	{
		DisplayTimeOutScr(0);	//显示超时画面
		return;
	}

	Beep(1);
	for(i=0;i<BEEP_DELAY;i++);
	Beep(0);

	EraseScrFlash();

	Lcd_Clr(7);
	//Lcd_Clr(BLUE);
	Lcd_Display_String(300,170,3,0,7,3,XiaZaiZhong,0);
	//Lcd_Display_String(300,170,3,BLACK,BLUE,3,XiaZaiZhong,0);
	Lcd_Jincheng_Tu(0,100,2,0,7,40,250,560,50);
	//Lcd_Jincheng_Tu(0,100,2,BLACK,BLUE,40,250,560,50);

	//FeedWacthDog();

	Uart_SendByte('H');
	Uart_SendByte('E');
	Uart_SendByte('A');

	
	if(DownSystemInfo() == FALSE)
	{
		DisplayErrScr(0);
		return;
	}

	ControlCount = ScreenDataBuffer[3];
	StateCount = ScreenDataBuffer[3+ControlCount+3];
	

	ScreenBitStrLength=ScreenDataBuffer[13+ControlCount+StateCount];
	PlcBitStrLength=ScreenDataBuffer[16+ControlCount+StateCount+ScreenBitStrLength];
	ScreenWordStrLength=ScreenDataBuffer[23+ControlCount+StateCount+ScreenBitStrLength+PlcBitStrLength];
	PlcWordStrLength=ScreenDataBuffer[26+ControlCount+StateCount+ScreenBitStrLength+PlcBitStrLength+ScreenWordStrLength];
	i=29+ControlCount+StateCount+ScreenBitStrLength+PlcBitStrLength+ScreenWordStrLength+PlcWordStrLength;

//	i = 3 + ControlCount +3 + StateCount+3;
	ScrCount = ScreenDataBuffer[i];

	temp = ScrCount;
	while(ScrCount-- !=0)
	{
		//FeedWacthDog();
	
		Uart_SendByte('S');
		Uart_SendByte('C');
		Uart_SendByte('R');				
		if(DownScreenInfo()==FALSE)
		{
			DisplayErrScr(0);
			return;	
		}
		Lcd_Jincheng_Tu(0,temp,temp-ScrCount,0,7,40,250,560,50);
		//Lcd_Jincheng_Tu(0,temp,temp-ScrCount,BLACK,BLUE,40,250,560,50);
	}
	
	Uart_SendByte('S');
	Uart_SendByte('C');
	Uart_SendByte('R');
	
	if((u8temp=Uart_DownloadGetch(&Err)) == 'E')
	{
		if((u8temp=Uart_DownloadGetch(&Err)) == 'N')
		{
			if((u8temp=Uart_DownloadGetch(&Err)) == 'D')
			{
		
			}
		}
	}
	else
	{
	   	DisplayErrScr(0);
		return;	
	}

	
	
	WritePage(IndexBlock,0,IndexBlockBuffer);

	//FeedWacthDog();

	GetSystenInfo();

	DisplayDLCompleteScr(0);

	Beep(1);
	for(i=0;i<BEEP_DELAY;i++);
	Beep(0);

	if(!UART_PrintStr)
		Uart_Init(0, g_SystemConf.BaudRates); // 修改波特率            

}



INT8U DownSystemInfo(void)
{
	INT16U SysInfoCount,temp;
	INT8U  Page;
	INT8U  CheckSum=0;

	INT8U Err;

	INT32U i;

	SysInfoCount = Uart_DownloadGetch(&Err);
	temp = Uart_DownloadGetch(&Err);
	SysInfoCount = (temp<<8) | SysInfoCount;
	
	if(SysInfoCount==0)
	{
		return FALSE;
	}
	
	for(i=0;i<SysInfoCount;i++)
	{
		ScreenDataBuffer[i] = Uart_DownloadGetch(&Err);
		CheckSum += ScreenDataBuffer[i];
	}
	
	if (Uart_DownloadGetch(&Err) != CheckSum)
	{
		return FALSE;
	}
	if (Uart_DownloadGetch(&Err) != 0xA)
	{
		return FALSE;
	}
	
	if (Uart_DownloadGetch(&Err) != 0xD)
	{
		return FALSE;
	}
	
	if (SysInfoCount>PAGE_SIZE)
	{
		for (Page=5,i=0;Page<5+SysInfoCount/PAGE_SIZE;Page++,i=i+PAGE_SIZE)
		{
			WritePage(IndexBlock,Page,&ScreenDataBuffer[i]);
		}
		
		if ((temp=SysInfoCount % PAGE_SIZE) != 0)	
		{
			for(;temp<PAGE_SIZE;temp++)
			{
				ScreenDataBuffer[i+temp] = 0;	
			}
			WritePage(IndexBlock,Page,&ScreenDataBuffer[i]);
		
		}
	}
	else
	{
		for (i=SysInfoCount;i<PAGE_SIZE;i++)
			ScreenDataBuffer[i] = 0;
		WritePage(IndexBlock,5,ScreenDataBuffer);
	}

	return TRUE;
	
}


INT8U DownScreenInfo(void)
{
	INT32U ScrInfoCount,i,temp,Block;
	INT8U  Page;
	INT8U  CheckSum=0;
	INT16U  ScreenNum;
	INT16U LogicalBlock;
	int j;

	INT8U Err;
	

	ScrInfoCount = Uart_DownloadGetch(&Err);
	temp = Uart_DownloadGetch(&Err);
	ScrInfoCount = (temp<<8) | ScrInfoCount;
	temp = Uart_DownloadGetch(&Err);
	ScrInfoCount = (temp<<16) | ScrInfoCount;
	temp = Uart_DownloadGetch(&Err);
	ScrInfoCount = (temp<<24) | ScrInfoCount;

	if(ScrInfoCount==0)
	{
		return FALSE;
	}

	for(i=0;i<ScrInfoCount;i++)
	{
		ScreenDataBuffer[i] = Uart_DownloadGetch(&Err);
		CheckSum += ScreenDataBuffer[i];
	}

	if (Uart_DownloadGetch(&Err) != CheckSum)
	{
		return FALSE;
	}
	if (Uart_DownloadGetch(&Err) != 0xA)
	{
		return FALSE;
	}
	
	if (Uart_DownloadGetch(&Err) != 0xD)
	{
		return FALSE;
	}
	
	Block = IndexBlockBuffer[IndexBlockOffset+2];
	Block = (Block<<8) | IndexBlockBuffer[IndexBlockOffset+3];	
	Page = IndexBlockBuffer[IndexBlockOffset+4];		

	if (ScrInfoCount>PAGE_SIZE)
	{	
		i=0;

		while((ScrInfoCount/PAGE_SIZE) != 0)
		{
			WritePage(Block,Page,&ScreenDataBuffer[i]);
			Page= Page+1;
			if (Page > 63)
			{
				LogicalBlock = Physical_To_Logical_Block(Block);
				LogicalBlock++;
				Block = Logical_To_Physical_Block(LogicalBlock);
				Page = 0;
			}
			ScrInfoCount = ScrInfoCount-PAGE_SIZE;
			i=i+PAGE_SIZE;		
		}
		if (ScrInfoCount != 0)
		{
			for (j=ScrInfoCount;j<PAGE_SIZE;j++)
				ScreenDataBuffer[j] = 0;

			WritePage(Block,Page,&ScreenDataBuffer[i]);
			Page= Page+1;
			if (Page > 63)
			{
				LogicalBlock = Physical_To_Logical_Block(Block);
				LogicalBlock++;
				Block = Logical_To_Physical_Block(LogicalBlock);
				Page = 0;
			}

		}
	}
	else
	{
		for (i=ScrInfoCount;i<PAGE_SIZE;i++)
			ScreenDataBuffer[i] = 0;

		WritePage(Block,Page,ScreenDataBuffer);
		Page= Page+1;
		if (Page > 63)
		{
			LogicalBlock = Physical_To_Logical_Block(Block);
			LogicalBlock++;
			Block = Logical_To_Physical_Block(LogicalBlock);
			Page = 0;
		}

	}
	ScreenNum = ScreenDataBuffer[0];
	IndexBlockBuffer[IndexBlockOffset] = 0;
	IndexBlockBuffer[IndexBlockOffset+1] = ScreenNum;  		
	IndexBlockOffset = IndexBlockOffset+5;
	IndexBlockBuffer[IndexBlockOffset+2] = (Block>>8);
	IndexBlockBuffer[IndexBlockOffset+3] = Block;
	IndexBlockBuffer[IndexBlockOffset+4] = Page;

	return TRUE;
}


void DisplayVerDiffScr(INT8U State)
{
	char Info[]={41,47,47,34,46,27,27,90,40,77,28,94,16,70,17,30,18,27,38,5,37,68};      //CharString:“上下位机软件版本不匹配”
	char QueDing[]={40,23,22,8};               //CharString:“确定”
	INT16U LeftX,LeftY;
	LeftX = 180;
	LeftY = 150;

	Lcd_Clr(16);                                 //This Instruction can be removed 
	//Lcd_Clr(RED);                                 //This Instruction can be removed 

	CurrentScrNum = 0xff07; 
	Lcd_Fill_Box (LeftX, LeftY, 460, 330, 4);
	//Lcd_Fill_Box (LeftX, LeftY, 460, 330, RED);
	Lcd_DispHZ_On_Button(LeftX,LeftY+20,460,300,11,13,4,3,0,1,0,0,Info);
	//Lcd_DispHZ_On_Button(LeftX,LeftY+20,460,300, 11,BLACK,GREEN,3, 0,1,0,0,Info);
	if(State==0)
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,RAISE);
	else
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,INSERT);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,INSERT);
	Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
	//Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, BLACK, BLUE,2,0,0,0,3,QueDing);

}




void DisplayTimeOutScr(INT8U State)
{
	char Info[]={45,8,49,22,19,12,42,17};      //CharString:“通讯超时”
	char QueDing[]={40,23,22,8};               //CharString:“确定”
	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;

	//Lcd_Clr(7);                                 //This Instruction can be removed 
	//Lcd_Clr(RED);                                 //This Instruction can be removed 

	CurrentScrNum = 0xff01; 
	Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
	//Lcd_Fill_Box (LeftX, LeftY, 440, 330, RED);
	Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,4,13,4,3,0,1,0,0,Info);
	//Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300, 4,BLACK,GREEN,3, 0,1,0,0,Info);
	if(State==0)
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,RAISE);
	else
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,INSERT);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,INSERT);
	Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
	//Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, BLACK, BLUE,2,0,0,0,3,QueDing);

	
}




void DisplayErrScr(INT8U State)
{
	char Info[]={45,8,49,22,20,77,46,83};   //CharString:“通讯错误”
	char QueDing[]={40,23,22,8};            //CharString:“确定”
	
	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;

//	Lcd_Clr(7);                                 //This Instruction can be removed  
	
	CurrentScrNum = 0xff02;
	Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
	//Lcd_Fill_Box (LeftX, LeftY, 440, 330, RED);
	Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,4,13,4,3,0,1,0,0,Info);
	//Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300, 4,RED,GREEN,3,0,1,0,0,Info);
	if(State==0)
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,RAISE);
	else
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,INSERT);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,INSERT);
	Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2,0, 7,2,0,0,0,3,QueDing);
	//Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2,BLACK, BLUE,2,0,0,0,3,QueDing);


}





void DisplayDLCompleteScr(INT8U State)
{
	char Info[]={47,34,52,56,45,74,17,47};  //CharString:“下载完毕”
	char QueDing[]={40,23,22,8};			 //CharString:“确定

	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;

//	Lcd_Clr(7);                                 //This Instruction can be removed  
	
	CurrentScrNum = 0xff03;
	Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
	//Lcd_Fill_Box (LeftX, LeftY, 440, 330, GREEN);
	Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,4,13,4,3,0,1,0,0,Info);
	//Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,4,BLACK,GREEN,3,0,1,0,0,Info);
	if(State==0)
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,RAISE);
	else
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,INSERT);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,INSERT);
	Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
	//Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, BLACK, BLUE,2,0,0,0,3,QueDing);
}





void EraseScrFlash(void)
{
	INT16U BlkIndex;
	char Info[]={41,33,20,70,18,33,19,93,54,48}; //CharString "闪存擦除中"
	int BlockSysConf;
	
	BlockSysConf = Logical_To_Physical_Block(NAND_SYS_CONF);
	

	Lcd_Clr(7);
	//Lcd_Clr(BLUE);
	Lcd_Display_String(272,180,5,0,7,3,Info,0);
	//Lcd_Display_String(272,180,5,0,BLUE,GREEN,Info,0);
	//for (BlkIndex=NAND_SCR_INFO; BlkIndex<1024; BlkIndex++)
	//for (BlkIndex=1; BlkIndex<1024; BlkIndex++)
	for (BlkIndex=NAND_SCR_INDEX; BlkIndex<BLOCK_Password; BlkIndex++)
	{
		if(BlkIndex == BlockSysConf)
			continue;
		if(TRUE != EraseBlock(BlkIndex)) //连续坏块时怎么处理?
		{
			MarkBadBlk(BlkIndex);
		}
		if(BlkIndex%50==0 || BlkIndex==(BLOCK_Password-1))   // 80    1023
		{
			Lcd_Jincheng_Tu(1,BLOCK_Password,BlkIndex,0,7,40,250,560,50);
			//Lcd_Jincheng_Tu(1,1024,BlkIndex, BLACK,BLUE,40,250,560,50);
			//FeedWacthDog();
		}
	}
}




void DisplaySystemScr(INT8U State)
{
	char Info[]={47,21,45,19,18,43,21,5};  //CharString:“系统菜单”
	char XiaZai[]={47,34,52,56};            //CharString:“下载
	char YunXing[]={52,43,48,48};            //CharString:“运行
	char ShangZai[]={25,60,32,77,52,17};            //CharString:“管理员
	
	char CanShuSheZhe[]={18,46,42,93,41,72,54,35};            //CharString:“参数设置
	char JiDianJiaoZhun[]={27,89,21,67,48,3,55,28};            //CharString:“基点校准
	char HuaMianJianCha[]={27,13,35,70,28,76,18,73};            //CharString:“画面检查
	INT16U Left,Top;
	
	//INT8U *Str=VERSION;
	char Str[] = VERSION;

	CurrentScrNum = 0x0;                            	

	if(State == 0)
	{
		Lcd_Clr(9);	
		//Lcd_Clr(BLUE);	
		Lcd_Fill_Box (0, 0, 640,68, 11);
		//Lcd_Fill_Box (0, 0, 640,68, GREEN); //Red,top
		Lcd_DispHZ_On_Button(200,10,440,40,4,0,11,3,0,1,0,0,Info);
		//Lcd_DispHZ_On_Button(200,10,440,40, 4,BLACK,GREEN,3, 0,1,0,0, Info);

		Lcd_Fill_Box (0, 425, 640,480, 11);
		//Lcd_Fill_Box (0, 425, 640,480, GREEN);
		Lcd_DispAsc_On_Button(0, 425, 639, 479, strlen(Str),11,0,4,2,0,Str);
		//Lcd_DispAsc_On_Button(0, 425, 639, 479, strlen(Str),GREEN,BLACK,4, 2,0,Str);
	}
	Left = 65;
	Top = 140;
	if((State == 1) && ((X_result>65&&X_result<155) && (Y_result>140&&Y_result<200)))
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,2, 0, 5,2,0,0,0,3,XiaZai);
		//Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60, 2, BLACK, GREEN,2,0,0,0,3,XiaZai);
	}
	else
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,2, 0, 5,2,0,0,0,3,XiaZai);
		//Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,2, BLACK, GREEN,2,0,0,0,3,XiaZai);
	}

	
	Left = 260;
	Top = 140;
	if((State == 1) && ((X_result>260&&X_result<350) && (Y_result>140&&Y_result<200)))
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,3, 0, 5,2,0,0,0,3,ShangZai);
		//Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,2, BLACK, GREEN,2,0,0,0,3,ShangZai);
	}
	else
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,3, 0, 5,2,0,0,0,3,ShangZai);
		//Lcd_DispHZ_On_Button(Left, Top, Left+90, Top+60,2, BLACK, GREEN,2,0,0,0,3,ShangZai);
	}


	Left = 455;
	Top = 140;
	if((State == 1) && ((X_result>455&&X_result<455+90) && (Y_result>140&&Y_result<200)))
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left,Top, Left+90, Top+60,2, 0, 5,2,0,0,0,3,YunXing);
		//Lcd_DispHZ_On_Button(Left,Top, Left+90, Top+60,2, BLACK, GREEN,2,0,0,0,3,YunXing);
	}
	else
	{
		Lcd_Button(Left,Top,Left+90,Top+60,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+90,Top+60,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left,Top, Left+90, Top+60,2, 0, 5,2,0,0,0,3,YunXing);
		//Lcd_DispHZ_On_Button(Left,Top, Left+90, Top+60,2, BLACK, GREEN,2,0,0,0,3,YunXing);
	}

	Left = 40;
	Top = 270;
	if((State == 1) && ((X_result>40&&X_result<40+140) && (Y_result>270&&Y_result<270+80)))
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,CanShuSheZhe);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, BLACK, GREEN,2,0,0,0,3,CanShuSheZhe);
	} 
	else
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,CanShuSheZhe);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, BLACK, GREEN,2,0,0,0,3,CanShuSheZhe);
	} 


	Left = 235;
	Top = 270;
	if((State == 1) && ((X_result>235&&X_result<235+140) && (Y_result>270&&Y_result<270+80)))
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,JiDianJiaoZhun);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80, 4, BLACK, GREEN,2, 0,0,0,3,JiDianJiaoZhun);
	} 
	else
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,JiDianJiaoZhun);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80, 4, BLACK, GREEN,2, 0,0,0,3,JiDianJiaoZhun);
	} 

	Left = 430;
	Top = 270;
	if((State == 1) && ((X_result>430&&X_result<430+140) && (Y_result>270&&Y_result<270+80)))
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,INSERT);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,INSERT);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,HuaMianJianCha);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, BLACK, GREEN,2,0,0,0,3,HuaMianJianCha);
	}
	else
	{
		Lcd_Button(Left,Top,Left+140,Top+80,5,3,RAISE);
		//Lcd_Button(Left,Top,Left+140,Top+80,GREEN,3,RAISE);
		Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, 0, 5,2,0,0,0,3,HuaMianJianCha);
		//Lcd_DispHZ_On_Button(Left,Top, Left+140, Top+80,4, BLACK, GREEN,2,0,0,0,3,HuaMianJianCha);
	}	

}




