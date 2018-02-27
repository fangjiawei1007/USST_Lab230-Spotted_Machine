#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "..\inc\def.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\bmp.h"
#include "..\inc\systemconf.h"

#include "..\inc\nand.h"
#include "..\inc\lcd.h"


extern INT16U Current_Scr_Block;
extern INT8U  Current_Scr_Page;
extern INT16U Next_Scr_Block;   
extern INT8U  Next_Scr_Page;
extern INT16U IndexBlock;             //every screen's block and page index table
extern INT8U  ScreenDataBuffer[620000];
extern INT32U ScrBufferOffset;
//extern INT8U IndexBlockBuffer[528*3];
extern INT16U CurrentScrNum;
extern INT16U PreScrNum;
extern int WatchRefresh[RefreshNum*2];
extern int NumDispRefresh[RefreshNum];
extern int StickChartRefresh[RefreshNum][2];
extern int SetValueRefresh[RefreshNum];
extern int MultiStateRefresh[RefreshNum][2];
extern int KeepButtonRefresh[RefreshNum][2];        //交替型
extern int KeepButton2Refresh[RefreshNum][2];    //保持型
extern int TimerButtonRefresh[RefreshNum];
extern int OscButtonRefresh[RefreshNum];

extern char PlcType;
extern INT16U g_PlcCtrlNum;
extern INT16U g_PlcStaNum;
extern INT16U g_PlcCtrLen;
extern INT16U g_PlcStaLen;
extern INT16U g_StarScr;
extern char g_PlcCtrStr[5];
extern char g_PlcStaStr[5];
//extern INT8U aLcdActiveBuffer[480][640];

extern GLOBALVAR g_GlobalVar;
extern SYSTEMINFO g_SystemInfo;

extern int SetValue_PreValue[RefreshNum];
extern int NumDisp_PreValue[RefreshNum];


extern U8 g_com_Flag ;

extern INTERALMEMORY g_InteralMemory;

extern U8 g_KeyBoardChange;



void Get_CurNext_Block_Page(INT16U Specify_Scr_Num)   //Specify_Scr_Num: from 1 to 528
{
	INT8U Read_Buf[PAGE_SIZE];
	INT16U i;
	INT16U Block;
	INT8U Page=0;
	INT8U Flag=0;
	
	//while (Page <= 4)  // 2K大页面，只要用到第0页
	{
    		ReadPage(IndexBlock,Page,Read_Buf);
		for (i=0;i<=400;i++)	// PAGE_SIZE/5
		{
			Block=Read_Buf[5*i];
			Block=(Block<<8) | Read_Buf[5*i+1];
			if (Specify_Scr_Num == Block)
			{	
				Flag=1;
				Current_Scr_Block=Read_Buf[5*i+2];
				Current_Scr_Block=(Current_Scr_Block<<8) | Read_Buf[5*i+3];
				Current_Scr_Page=Read_Buf[5*i+4];
				//i=i+1;
				Next_Scr_Block=Read_Buf[5*(i+1)+2];
				Next_Scr_Block=(Next_Scr_Block<<8) | Read_Buf[5*(i+1)+3];
				Next_Scr_Page=Read_Buf[5*(i+1)+4];
			}
			
			if (Flag == 1)
			{
				break;
			}
			
		}
	}
}


/********************************************************/
/*功能：把指定的画面信息从Flash中搬到内存缓冲区中          */
/*输入：INT16U Specify_Scr_Num(指定的画面号)             */
/*返回：NULL                                         */
/********************************************************/
void GetScreenData(INT16U Specify_Scr_Num)
{
	INT16U CurrentBlock,NextBlock;
	INT16U CurrentPage,NextPage;
	INT32U i,count=0,Block,TempBlock;
	INT8U *ptr,Page;
	INT8U Flag;
	INT8U Read_Buf[PAGE_SIZE];
//	int w;

	Get_CurNext_Block_Page(Specify_Scr_Num);
	CurrentBlock = Current_Scr_Block;
	CurrentPage = Current_Scr_Page;
	NextBlock = Next_Scr_Block;
	NextPage = Next_Scr_Page;

	if (CurrentBlock == NextBlock)
	{
		for (Page=CurrentPage;Page<NextPage;Page++)
		{
			ReadPage(CurrentBlock,Page,Read_Buf);

			for (i=0;i<PAGE_SIZE;i++)
			{
				ScreenDataBuffer[count++] = Read_Buf[i];
			}

		}	
	}
	else
	{
		ptr=malloc(2*(NextBlock-CurrentBlock));
		if (Get_Invalid_Block(CurrentBlock,NextBlock,ptr) == 0)
		{
			Block = CurrentBlock;
			Page = CurrentPage;
			while ((Block != NextBlock) || (Page != NextPage))
			{
				ReadPage(Block,Page,Read_Buf);
				for (i=0;i<PAGE_SIZE;i++)
				{
					ScreenDataBuffer[count++] = Read_Buf[i];
				}
				Page = Page+1;
				if (Page>63)
				{
					Page = 0;
					Block = Block+1;
				}

			}
		}
		else
		{
			Block = CurrentBlock;
			Page = CurrentPage;
			while ((Block != NextBlock) || (Page != NextPage))
			{
				ReadPage(Block,Page,Read_Buf);
				for (i=0;i<PAGE_SIZE;i++)
				{
					ScreenDataBuffer[count++] = Read_Buf[i];
				}
				Page = Page+1;
				if (Page>63)
				{
					Page = 0;
					Block = Block+1;
					while (Block != NextBlock)
					{
						for (i=1;i<=(*ptr)*2;i=i+2)
						{
							TempBlock = *(ptr+i);
							TempBlock = (TempBlock<<8) | *(ptr+i+1);
							if (Block == TempBlock)
							{
								Flag = 1;
							}
							else
							{
								Flag = 0;
							}

							if (Flag == 1)
							{
								break;
							}
						}
						
						if (Flag == 0)
						{
							break;
						}
						else
						{
							Block = Block+1;
						}
					}
				}

			}

		}
		free(ptr);
	}

}



void DrawKeepButton(INT8U Statue,INT8U refreshflag)
{
	INT32U i;
	INT8U SubType;
	INT16U BlackColor;
	INT16U LeftX,LeftY,RightX,RightY;//,Radius;
	INT8U Width,FlagUnderLine;
	INT8U Type,HAlign,VAlign;
	INT8U ZiSize;//,ZiQuma,ZiWeima;
	INT16U ZiColor;
	INT32U ZiCount,temp;
	INT8U RdStringLen,WrStringLen;
	INT8U *ptr = ScreenDataBuffer;
	//char *ptr = ScreenDataBuffer;
 
	i = ScrBufferOffset;          //yuan jian de ru kou zhi zhen 
	SubType= ScreenDataBuffer[i+1];
	if (Statue == 0)
	{
		BlackColor = *(ptr+i+2);
	}
	else
	{
		BlackColor = *(ptr+i+3);
	}
	
	RdStringLen = *(ptr+i+4);
	WrStringLen = *(ptr+i+4+RdStringLen+2+1);
	Type = *(ptr+i+4+RdStringLen+2+1+WrStringLen+2+1);
	temp = 4+RdStringLen+2+1+WrStringLen+2+1+1;

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
	
	if (Type == 1)
	{
		if (Statue == 0)
		{
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,RAISE);

		}
		else
		{
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,INSERT);
		}	
	}
	else 
	{
		if (Statue == 0)
		{
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,RAISE);
		}
		else
		{
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,INSERT);
		}
	}
	
	if (Statue == 0)
	{
		temp = temp+1;
		ZiCount = *(ptr+i+temp+3);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp);
	//	ZiCount /= 2;
		temp = temp+4;
		ZiColor = *(ptr+i+temp);
		temp = temp+1;
		ZiSize = *(ptr+i+temp);
		temp = temp+1;
		HAlign = *(ptr+i+temp);
		temp = temp+1;
		VAlign = *(ptr+i+temp);
		temp = temp+1;
		FlagUnderLine = *(ptr+i+temp);
		temp = temp+1;
		temp = temp+1;
		 Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
	}
	else
	{
		ZiCount = *(ptr+i+temp+4);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+3);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	//	ZiCount /= 2;
	//	temp = temp+10+ZiCount*2;
		temp = temp+10+ZiCount;
		temp = temp+1;
		ZiCount = *(ptr+i+temp+3);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
		ZiCount = (ZiCount<<8) | *(ptr+i+temp);
	//	ZiCount /= 2;
		temp = temp+4;
		ZiColor = *(ptr+i+temp);
		temp = temp+1;
		ZiSize = *(ptr+i+temp);
		temp = temp+1;
		HAlign = *(ptr+i+temp);
		temp = temp+1;
		VAlign = *(ptr+i+temp);
		temp = temp+1;
		FlagUnderLine = *(ptr+i+temp);
		temp = temp+1;
		temp = temp+1;
		 Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
	}

	if(SubType==3 && refreshflag==0)
	{
		for(i=0;i<RefreshNum;i++)
		{
			if(KeepButtonRefresh[i][0]==0)
			{
				KeepButtonRefresh[i][0]=ScrBufferOffset;
				KeepButtonRefresh[i][1]=0;
				break;
			}
		}
	}

	if(SubType==1 && refreshflag==0)
	{
		for(i=0;i<RefreshNum;i++)
		{
			if(KeepButton2Refresh[i][0]==0)
			{
				KeepButton2Refresh[i][0]=ScrBufferOffset;
				KeepButton2Refresh[i][1]=0;
				break;
			}
		}
	}
	
}


void DrawUpButton(INT8U State)
{
	INT16U i;
	INT16U BlackColor;
	INT16U LeftX,LeftY,RightX,RightY;//,Radius;
	INT8U Width,FlagUnderLine ;
	INT8U Type,HAlign,VAlign;
	INT8U ZiSize;//,ZiQuma,ZiWeima;
	INT16U ZiColor;
	INT32U ZiCount,temp;
	INT8U RdStringLen,WrStringLen;
	INT8U *ptr = ScreenDataBuffer;

	i = ScrBufferOffset;
	temp = 0;
	temp = temp+9;/////////////////////////
	RdStringLen = *(ptr+i+temp);
	temp = temp+1+RdStringLen+2;
	WrStringLen = *(ptr+i+temp);
	temp = temp+1+WrStringLen+2;

	Type = *(ptr+i+temp);
	temp = temp+1;
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
	BlackColor = *(ptr+i+temp);

	if (Type == 1)
	{
		if(State == 1)		
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,INSERT);
		else
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,RAISE);

	}
	else 
	{
		if(State == 1)	
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,INSERT);
		else
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,RAISE);	
	}

	temp = temp+1;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	 Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
}



void DrawShiftButton(INT8U State)
{
	INT16U i;
	INT16U BlackColor;
	INT16U LeftX,LeftY,RightX,RightY;//,Radius;
	INT8U Width,FlagUnderLine;
	INT8U Type,HAlign,VAlign;
	INT8U ZiSize;//,ZiQuma,ZiWeima;
	INT16U ZiColor;
	INT32U ZiCount,temp;
	INT8U *ptr = ScreenDataBuffer;
	
	i = ScrBufferOffset;
	Type = *(ptr+i+3);
	temp = 3;
	temp = temp+1;
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
	BlackColor = *(ptr+i+temp);
	
	if (Type == 1)
	{	
		if(State == 1)
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,INSERT);	
		else
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,RAISE);

	}
	else 
	{
		if(State == 1)	
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,INSERT);
		else
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,BlackColor,Width,RAISE);
	}

	temp = temp+1;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	 Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		
}


void DrawMultiState(U8 state,U8 FlagRefresh)
{
	INT16U i;
	INT16U BlackColor;
	INT16U LeftX,LeftY,RightX,RightY;
	INT8U Width,FlagUnderLine,FlagWord;
	INT8U HAlign,VAlign,Shape;
	INT8U ZiSize;//,ZiQuma,ZiWeima;
	INT16U ZiColor;
	INT32U ZiCount,temp;
	INT8U RdStringLen;
	INT8U *ptr = ScreenDataBuffer;
        
	i = ScrBufferOffset;  //yuan jian de ru kou zhi zhen 
	temp=1;
	FlagWord=*(ptr+i+temp);
	temp++;

	BlackColor=*(ptr+i+temp+state);
	temp=temp+32;
	RdStringLen=*(ptr+i+temp);
	temp=temp+1+RdStringLen+2;
	Shape=*(ptr+i+temp);
	temp++;
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
	temp++;
	
	if(Shape==2)
	{
		if(state>31)
			Lcd_Ellipse_Button(LeftX, LeftY, RightX, RightY,7, Width,RAISE);
		else
			Lcd_Ellipse_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,RAISE);

	}

	else
	{
		if(state>31)
			Lcd_Button( LeftX, LeftY, RightX, RightY,7, Width,RAISE);
		else
			Lcd_Button( LeftX, LeftY, RightX, RightY,BlackColor, Width,RAISE);
		
		
	}

	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==0)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		if(FlagRefresh==0)
		{
			for(i=0;i<RefreshNum;i++)
			{
				if(MultiStateRefresh[i][0]==0)
				{
					MultiStateRefresh[i][0]=ScrBufferOffset;
					MultiStateRefresh[i][1]=0;
					break;
				}
			}
		}
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==1)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==2)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==3)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==4)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}
 
	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==5)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==6)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==7)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==8)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==9)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==10)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==11)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==12)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==13)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==14)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==15)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==16)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==17)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==18)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==19)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==20)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==21)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==22)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==23)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==24)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==25)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==26)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==27)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==28)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==29)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==30)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

		temp=temp+ZiCount;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	ZiColor = *(ptr+i+temp);
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);
	temp = temp+1;
	VAlign = *(ptr+i+temp);
	temp = temp+1;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	if(state==31)
	{
		Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,ZiColor,BlackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
		return;
	}

	ZiCount=4;
	Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,1,7,ZiSize,HAlign,VAlign,FlagUnderLine,Width,"出错");
//	Lcd_Refresh();
}




void DrawSetValueButton(U8 Statue)
{
	INT16U i;
	INT16U ValueBackColor,ButtonBackColor,ValueColor;
	INT16U LeftX,LeftY,RightX,RightY;
	INT8U ZiCount,ZiSize,HAlign;
	INT32U temp;
	INT8U RdStringLen,WrStringLen;
	INT8U *ptr = ScreenDataBuffer;

	INT16U j;
	char PlcReadStr[5];

	//U16 WhichSetValue;
	U8 WhichSetValue;
	U8 DigitPos,DigitNumb;
	double DispValue;
	char DispStr[10], a;

	INT16U PlcReadNum;

	

	i = ScrBufferOffset;


	DigitPos=*(ptr+i+3);
	DigitNumb=*(ptr+i+4);

	
	temp = 0;
	temp=temp+9+4;///////////////////////
	ValueBackColor=*(ptr+i+temp);
	temp++;
	RdStringLen = *(ptr+i+temp);
	for(j=0;j<RdStringLen;j++)
		PlcReadStr[j]=*(ptr+i+1+temp+j);
	PlcReadStr[j]='\0';

	PlcReadNum = (*(ptr+i+1+temp+RdStringLen+1)<<8) | (*(ptr+i+1+temp+RdStringLen));

	temp = temp+1+RdStringLen+2;
	WrStringLen = *(ptr+i+temp);
	temp = temp+1+WrStringLen+2;

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
	ButtonBackColor=*(ptr+i+temp);
	temp++;
	ValueColor=*(ptr+i+temp);

	Lcd_Button( LeftX, LeftY, RightX, RightY,ButtonBackColor, 2,RAISE);
	Lcd_Button( LeftX+4, LeftY+4, RightX-30, RightY-4,ValueBackColor, 2,INSERT);
	if(Statue==0)
		Lcd_Button( RightX-28, LeftY+4, RightX-4, RightY-4,ButtonBackColor, 2,RAISE);
	else
		Lcd_Button( RightX-28, LeftY+4, RightX-4, RightY-4,ButtonBackColor, 2,INSERT);
	
	ZiCount =4;
	temp = temp+1;
	ZiSize = *(ptr+i+temp);
	temp = temp+1;
	HAlign = *(ptr+i+temp);

	if(FALSE == g_KeyBoardChange)
	//if(-1 == SetValue_PreValue[WhichSetValue])
	{
		Lcd_DispHZ_Asc_On_Button(LeftX+4, LeftY+4, RightX-30, RightY-4, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");
		//Lcd_DispHZ_Asc_On_Button(LeftX+4, LeftY+4, RightX-30, RightY-4, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"0");

		for(i=0;i<RefreshNum;i++)
		{
			if(SetValueRefresh[i]==0)
			{
				SetValueRefresh[i]=ScrBufferOffset;
				break;
			}
		}
	}
	else
	{
		for(WhichSetValue=0; WhichSetValue<RefreshNum; WhichSetValue++)
		{
			if(SetValueRefresh[WhichSetValue] == ScrBufferOffset)
			{
				break;
			}
		}
		
		if(WhichSetValue >= RefreshNum)
		{
			Lcd_DispHZ_Asc_On_Button(LeftX+4, LeftY+4, RightX-30, RightY-4, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");
		}
		else
		{	
			if(-1 == SetValue_PreValue[WhichSetValue])
			{
				Lcd_DispHZ_Asc_On_Button(LeftX+4, LeftY+4, RightX-30, RightY-4, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");
			}
			else
			{
				//+++  100517  ++++++++++++++
				/*if(((50 == PlcReadNum) && (0 == strcmp("P_W", PlcReadStr))) ||		// P_W50 
					((3000 == PlcReadNum) && (0 == strcmp("MW", PlcReadStr))))	// MW3000 
				{
					sprintf(DispStr,"%x",SetValue_PreValue[WhichSetValue]);
					a = strlen(DispStr);
					Lcd_DispAsc_On_Button(LeftX+4, LeftY+4, RightX-32, RightY-4,a,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
				}
				else*/
				{
					switch(DigitPos)
					{
						case 0:
								DispValue=(double)SetValue_PreValue[WhichSetValue];
								break;
						case 1:
								DispValue=(double)SetValue_PreValue[WhichSetValue]/10.0;
								break;
						case 2:
								DispValue=(double)SetValue_PreValue[WhichSetValue]/100.0;
								break;
						case 3:
								DispValue=(double)SetValue_PreValue[WhichSetValue]/1000.0;
								break;
						case 4:
								DispValue=(double)SetValue_PreValue[WhichSetValue]/10000.0;
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
					//Lcd_Fill_Box(Left+6, Top+6, Right-32, Bottom-6,ValueBackColor/*-(g_Count%10)*/);

					if(DigitNumb==0)
						 Lcd_DispAsc_On_Button(LeftX+4, LeftY+4, RightX-32, RightY-4,a,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
					else
						 Lcd_DispAsc_On_Button(LeftX+4, LeftY+4, RightX-32, RightY-4,a+1+DigitNumb,ValueBackColor,ValueColor,ZiSize,HAlign,2,DispStr);
				}
			}
		}
	}

	
	if(!((strcmp(PlcReadStr,"P_KW")==0) ||(strcmp(PlcReadStr,"P_W")==0 )))
		g_com_Flag = TRUE;  // 通讯标志
			

}




void DrawWatchDispButton(INT16U CurrentPlcValue,INT8U FlagRefresh,INT16U PrePlcValue)
{
	INT16U i,Radius;
	INT16U PointerColor,WatchBackColor,KeduColor,DiQuColor,GaoQuColor;
	INT16U LeftX,LeftY,RightX,RightY,CenterX,CenterY,TempStartX,TempStartY,TempEndX,TempEndY,DispX,DispY;
	INT16U PlcMaxValue,PlcMinValue,DispMaxValue,DispMinValue,GaoXianValue,DiXianValue;
	INT8U  TotalDegree,DispValueFlag,DispScopeFlag,MajorFenDu,MinorFenDu,DigitNumb;
	INT32U temp,HalfRadius;
	INT8U RdStringLen,n,n1;
	INT8U *ptr = ScreenDataBuffer;
	double pi = 3.1415926535;
	double CurrentDispValue,PreDispValue;
	double degreebase,degree,degree1,startdegree,enddegree;
	double MajorKeduValue;
	char a;
	char str[20];
	char dangqianzhi[]={21,17,39,16,54,21};

	i = ScrBufferOffset;
	temp = 0;
	temp++;
	RdStringLen = *(ptr+i+temp);
	temp = temp+1+RdStringLen+2;
	PlcMaxValue=*(ptr+i+temp+1);
	PlcMaxValue=(PlcMaxValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	PlcMinValue=*(ptr+i+temp+1);
	PlcMinValue=(PlcMinValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	temp++;
	PointerColor=*(ptr+i+temp);
	temp++;
	TotalDegree=*(ptr+i+temp);
	temp++;
	KeduColor=*(ptr+i+temp);
	temp++;
	MajorFenDu=*(ptr+i+temp);
	temp++;
	MinorFenDu=*(ptr+i+temp);
	temp++;
	DigitNumb=*(ptr+i+temp);
	temp++;
	DispMinValue=*(ptr+i+temp+1);
	DispMinValue=(DispMinValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	DispMaxValue=*(ptr+i+temp+1);
	DispMaxValue=(DispMaxValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	DiQuColor=*(ptr+i+temp);
	temp++;
	GaoQuColor=*(ptr+i+temp);
	temp++;
	DiXianValue=*(ptr+i+temp+1);
	DiXianValue=(DiXianValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	GaoXianValue=*(ptr+i+temp+1);
	GaoXianValue=(GaoXianValue<<8)|*(ptr+i+temp);
	temp=temp+2;
	DispValueFlag=*(ptr+i+temp);
	temp++;
	DispScopeFlag=*(ptr+i+temp);
	temp++;
	WatchBackColor=*(ptr+i+temp);
	temp++;
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
	
	if(FlagRefresh==0)
	{
		Lcd_Circle_Button(LeftX, LeftY, RightX, RightY,WatchBackColor,1,RAISE);//画表盘
		
	}

	CenterX=LeftX+(RightX-LeftX)/2;
	CenterY=LeftY+(RightY-LeftY)/2;
	
	Radius=(RightY-LeftY)/2-5;
	HalfRadius=Radius/2;

	if(CurrentPlcValue>PlcMaxValue)
	{
		CurrentPlcValue = PlcMaxValue;
	}
	if(CurrentPlcValue<PlcMinValue)
	{
		CurrentPlcValue = PlcMinValue;
	}

	if(PrePlcValue>PlcMaxValue)
	{
		PrePlcValue = PlcMaxValue;
	}
	if(PrePlcValue<PlcMinValue)
	{
		PrePlcValue = PlcMinValue;
	}
	CurrentDispValue=DispMinValue+(CurrentPlcValue-PlcMinValue)*(DispMaxValue-DispMinValue)/(PlcMaxValue-PlcMinValue);
	PreDispValue=DispMinValue+(PrePlcValue-PlcMinValue)*(DispMaxValue-DispMinValue)/(PlcMaxValue-PlcMinValue);

	if(TotalDegree==1)
	{
			///擦指针
		if(FlagRefresh!=0 && CurrentDispValue!=PreDispValue)
		{
			degree1=(PreDispValue-DispMinValue)/(DispMaxValue-DispMinValue)*2*pi;
			
			TempEndX=CenterX+Radius*sin(degree1);
			TempEndY=CenterY-Radius*cos(degree1);
			if(abs(TempEndX-CenterX)<=1)
				Lcd_Draw_VLine(CenterY,TempEndY,CenterX,WatchBackColor,3);
			else if(abs(TempEndY-CenterY)<=1)
			{
				Lcd_Draw_HLine(CenterX,TempEndX,TempEndY,WatchBackColor,3);
			}
			else
		    	Lcd_Draw_XieLine(CenterX,CenterY,TempEndX,TempEndY,WatchBackColor,3);	
		
		}
	
		Lcd_Arc(CenterX,CenterY,Radius,0,2*pi,1,KeduColor);//画圆刻度
	
		for(n=0;n<MajorFenDu-1;n++)
		{
			degreebase=(2*pi*n)/(MajorFenDu-1);//主刻度

			TempStartX=CenterX+Radius*sin(degreebase);
			TempStartY=CenterY-Radius*cos(degreebase);

			TempEndX=TempStartX-5*sin(degreebase);
			TempEndY=TempStartY+5*cos(degreebase);

			DispX=TempStartX-10*sin(degreebase);
			DispY=TempStartY+10*cos(degreebase);

			if(abs(TempEndX-TempStartX)<=1)
			{
				Lcd_Draw_VLine(TempStartY,TempEndY,TempStartX,KeduColor,2);
			}
			else if(abs(TempEndY-TempStartY)<=1)
			{
				Lcd_Draw_HLine(TempStartX,TempEndX,TempStartY,KeduColor,2);
			}
			else
				Lcd_Draw_XieLine(TempStartX,TempStartY,TempEndX,TempEndY,KeduColor,2);//主刻度
				
			MajorKeduValue = (double)(DispMinValue+(DispMaxValue-DispMinValue)*n/(MajorFenDu-1));   //显示刻度值
			sprintf(str,"%.3f",MajorKeduValue);
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
			/////以下处理显示位置
			if(abs(DispX-CenterX)<=1&&DispY>CenterY)
			{
				DispX=DispX-(a+DigitNumb+1)*4;
				DispY=DispY-16;
			}
			else if(abs(DispX-CenterX)<=1&&DispY<CenterY)
				DispX=DispX-(a+DigitNumb+1)*4;
			else if(DispX>CenterX&&abs(DispY-CenterY)<=1)
			{
				DispX=DispX-(a+DigitNumb+1)*8;
				DispY=DispY-6;
			}
			else if(DispX<CenterX&&abs(DispY-CenterY)<=1)
				DispY=DispY-6;

			else if(DispX>CenterX&&DispY<CenterY)
				DispX=DispX-(a+DigitNumb+1)*6;//*8;
			else if(DispX>CenterX&&DispY>CenterY)
			{
				DispX=DispX-(a+DigitNumb+1)*6;//*8;
				DispY=DispY-16;
			}
			else if(DispX<CenterX&&DispY>CenterY)
			{
				DispY=DispY-16;
				DispX=DispX-8;
			}
								
			if(DigitNumb==0)
				Lcd_Disp_AscStr(str,a,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
			else
				Lcd_Disp_AscStr(str,a+1+DigitNumb,DispX,DispY,1,1,8,KeduColor,WatchBackColor);

			for(n1=1;n1<=MinorFenDu;n1++)
			{
				degree=degreebase+(n1*2*pi)/((MinorFenDu+1)*(MajorFenDu-1));
				TempStartX=CenterX+Radius*sin(degree);
				TempStartY=CenterY-Radius*cos(degree);
				TempEndX=TempStartX-5*sin(degree);
				TempEndY=TempStartY+5*cos(degree);
				
				Lcd_Draw_XieLine(TempStartX,TempStartY,TempEndX,TempEndY,KeduColor,1);//次刻度
			}
		}
	/////显示当前值
		if(DispValueFlag==1)
		{
			DispX=CenterX-24;
			DispY=CenterY-36;
			Lcd_Display_String(DispX,DispY,3,1,WatchBackColor,0,dangqianzhi,0);
			sprintf(str,"%.3f",CurrentDispValue);
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
			DispX=CenterX-(a+DigitNumb+1)*4;
			DispY=CenterY-19;
			if(DigitNumb==0)
				Lcd_Disp_AscStr(str,a,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
			else
				Lcd_Disp_AscStr(str,a+1+DigitNumb,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
		}
    
		////// 显示安全范围
		if(DispScopeFlag==1)
		{
			startdegree=0;
			enddegree=((DiXianValue-DispMinValue)*2*pi)/(DispMaxValue-DispMinValue);
			Lcd_Arc(CenterX,CenterY,HalfRadius,0,enddegree,3,DiQuColor);

			startdegree=enddegree;
			enddegree=((GaoXianValue-DispMinValue)*2*pi)/(DispMaxValue-DispMinValue);
			Lcd_Arc(CenterX,CenterY,HalfRadius,startdegree,enddegree,3,0);

			startdegree=enddegree;
			Lcd_Arc(CenterX,CenterY,HalfRadius,startdegree,2*pi,3,GaoQuColor);
         
		}
					///画指针

		degree1=(CurrentDispValue-DispMinValue)/(DispMaxValue-DispMinValue)*2*pi;
		TempEndX=CenterX+Radius*sin(degree1);
		TempEndY=CenterY-Radius*cos(degree1);
		if(abs(TempEndX-CenterX)<=1)
			Lcd_Draw_VLine(CenterY,TempEndY,CenterX,PointerColor,3);
		else if(abs(TempEndY-CenterY)<=1)
		{
			Lcd_Draw_HLine(CenterX,TempEndX,TempEndY,PointerColor,3);
		}
		else
		    Lcd_Draw_XieLine(CenterX,CenterY,TempEndX,TempEndY,PointerColor,3);	
			
	}
	else
	{
				///擦指针
		if(FlagRefresh!=0 && CurrentDispValue!=PreDispValue)
		{
			degree1=(PreDispValue-DispMinValue)/(DispMaxValue-DispMinValue)*(5*pi/3)+pi/6;
			TempEndX=CenterX-Radius*sin(degree1);
			TempEndY=CenterY+Radius*cos(degree1);

			if(abs(TempEndX-CenterX)<=1)
				Lcd_Draw_VLine(CenterY,TempEndY,CenterX,WatchBackColor,3);
			else if(abs(TempEndY-CenterY)<=1)
			{
				Lcd_Draw_HLine(CenterX,TempEndX,TempEndY,WatchBackColor,3);
			}
			else
		    	Lcd_Draw_XieLine(CenterX,CenterY,TempEndX,TempEndY,WatchBackColor,3);	
		}
		   	Lcd_Arc(CenterX,CenterY,Radius,7*pi/6,17*pi/6,1,KeduColor);//画圆刻度

		for(n=0;n<MajorFenDu;n++)
		{
			degreebase=(5*pi*n)/(3*(MajorFenDu-1))+pi/6;

			TempStartX=CenterX-Radius*sin(degreebase);
			TempStartY=CenterY+Radius*cos(degreebase);

			TempEndX=TempStartX+5*sin(degreebase);
			TempEndY=TempStartY-5*cos(degreebase);

			DispX=TempStartX+10*sin(degreebase);
			DispY=TempStartY-10*cos(degreebase);

			if(abs(TempEndX-TempStartX)<=1)
				Lcd_Draw_VLine(TempStartY,TempEndY,TempStartX,KeduColor,2);
			else if(abs(TempEndY-TempStartY)<=1)
			{
				Lcd_Draw_HLine(TempStartX,TempEndX,TempStartY,KeduColor,2);
			}
			else
				Lcd_Draw_XieLine(TempStartX,TempStartY,TempEndX,TempEndY,KeduColor,2);//主刻度


			MajorKeduValue= (double)(DispMinValue+(DispMaxValue-DispMinValue)*n/(MajorFenDu-1));//显示刻度值
			sprintf(str,"%.3f",MajorKeduValue);
			//sprintf(str,"%f",(float)1.234);
			//gcvt(MajorKeduValue,6,str);
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

			/////以下处理显示位置
			if(abs(DispX-CenterX)<=1&&DispY>CenterY)
			{
				DispX=DispX-(a+DigitNumb+1)*4;
				DispY=DispY-16;
			}
			else if(abs(DispX-CenterX)<=1&&DispY<CenterY)
				DispX=DispX-(a+DigitNumb+1)*4;
			else if(DispX>CenterX&&abs(DispY-CenterY)<=1)
			{
				DispX=DispX-(a+DigitNumb+1)*8;
				DispY=DispY-8;
			}
			else if(DispX<CenterX&&abs(DispY-CenterY)<=1)
				DispY=DispY-8;

			else if(DispX>CenterX&&DispY<CenterY)
				DispX=DispX-(a+DigitNumb+1)*6;//*8;
			else if(DispX>CenterX&&DispY>CenterY)
			{
				DispX=DispX-(a+DigitNumb+1)*6;//*8;
				DispY=DispY-16;
			}
			else if(DispX<CenterX&&DispY>CenterY)
			{
				DispY=DispY-16;
				DispX=DispX-8;
			}
			
				
			if(DigitNumb==0)
				Lcd_Disp_AscStr(str,a,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
			else
				Lcd_Disp_AscStr(str,a+1+DigitNumb,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
			
			if(n==MajorFenDu-1)
			break;

			for(n1=1;n1<=MinorFenDu;n1++)
			{
				degree=degreebase+(n1*5*pi)/(3*(MinorFenDu+1)*(MajorFenDu-1));
				TempStartX=CenterX-Radius*sin(degree);
				TempStartY=CenterY+Radius*cos(degree);
				TempEndX=TempStartX+5*sin(degree);
				TempEndY=TempStartY-5*cos(degree);
				
				Lcd_Draw_XieLine(TempStartX,TempStartY,TempEndX,TempEndY,KeduColor,1);//次刻度
			}
		}

		/////显示当前值
		if(DispValueFlag==1)
		{
			DispX=CenterX-24;
			DispY=CenterY-36;
				Lcd_Display_String(DispX,DispY,3,1,WatchBackColor,0,dangqianzhi,0);
			sprintf(str,"%.3f",CurrentDispValue);
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
			DispX=CenterX-(a+DigitNumb+1)*4;
			DispY=CenterY-19;
			Lcd_Fill_Box(CenterX-23,CenterY-18,CenterX+23,CenterY-2,WatchBackColor);
			if(DigitNumb==0)
				Lcd_Disp_AscStr(str,a,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
			else
				Lcd_Disp_AscStr(str,a+1+DigitNumb,DispX,DispY,1,1,8,KeduColor,WatchBackColor);
		}
	
		////// 显示安全范围
		if(DispScopeFlag==1)
		{
			startdegree=7*pi/6;
			enddegree=7*pi/6+(DiXianValue-DispMinValue)*(5*pi/3)/(DispMaxValue-DispMinValue);
			Lcd_Arc(CenterX,CenterY,HalfRadius,startdegree,enddegree,3,DiQuColor);

			startdegree=enddegree;
			enddegree=7*pi/6+(GaoXianValue-DispMinValue)*(5*pi/3)/(DispMaxValue-DispMinValue);
			Lcd_Arc(CenterX,CenterY,HalfRadius,startdegree,enddegree,3,0);

			startdegree=enddegree;
			Lcd_Arc(CenterX,CenterY,HalfRadius,startdegree,17*pi/6,3,GaoQuColor);
           
		}
				
		degree1=(CurrentDispValue-DispMinValue)/(DispMaxValue-DispMinValue)*(5*pi/3)+pi/6;
		TempEndX=CenterX-Radius*sin(degree1);
		TempEndY=CenterY+Radius*cos(degree1);
		if(abs(TempEndX-CenterX)<=1)
				Lcd_Draw_VLine(CenterY,TempEndY,CenterX,PointerColor,3);
		else if(abs(TempEndY-CenterY)<=1)
		{
			Lcd_Draw_HLine(CenterX,TempEndX,TempEndY,PointerColor,3);
		}
		else
		    Lcd_Draw_XieLine(CenterX,CenterY,TempEndX,TempEndY,PointerColor,3);	
	
	}
	Lcd_Circle_Button(CenterX-5, CenterY-5, CenterX+5, CenterY+5,2,1,INSERT);//画中心小圆

	if(FlagRefresh==0)
	{
		for(i=0;i<RefreshNum;i++)
		{
			if(WatchRefresh[i*2]==0)
			{
				WatchRefresh[i*2]=ScrBufferOffset;
				WatchRefresh[i*2+1]=PlcMinValue;
				break;
			}
		}
	}

}



//void DrawNumDispButton(U8 FlagRefresh, U8 WhichNumDisp)
void DrawNumDispButton(U8 FlagRefresh)
{
	INT16U i,j;
	int CurrentPlcValue,CurrentPlcValue1;
	INT16U WordOrDWord,ValueBackColor,ButtonBackColor,ValueColor;
	INT16U LeftX,LeftY,RightX,RightY,ReadPlcNum;
	INT8U Width,ZiCount,ZiSize,HAlign;
	INT32U temp;
	INT8U RdStringLen,DigitPos,DigitNumb;
	double CurrentDispValue;
	char str[12],a,PlcReadStr[5];
	INT8U *ptr = ScreenDataBuffer;
	//int k;

	U8 WhichNumDisp;

	
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


	if(FlagRefresh==0)
	{
		if(FALSE == g_KeyBoardChange)
		//if(-1 == NumDisp_PreValue[WhichNumDisp])
		{
			for(i=0;i<RefreshNum;i++)
			{
				if(NumDispRefresh[i]==0)
				{
					NumDispRefresh[i]=ScrBufferOffset;
					break;
				}
			}
		}
	}
	
	//显示跟刷新都会用到
	for(WhichNumDisp=0; WhichNumDisp<RefreshNum; WhichNumDisp++)
	{
		if(NumDispRefresh[WhichNumDisp] == ScrBufferOffset)
		{
			break;
		}
	}



	if(FlagRefresh==0)
	{
		//Lcd_Button( LeftX, LeftY, RightX, RightY,ButtonBackColor, 2,RAISE);
		//Lcd_Button( LeftX+Width, LeftY+Width, RightX-Width, RightY-Width,ValueBackColor, 2,INSERT);
		Lcd_Button( LeftX, LeftY, RightX, RightY,ValueBackColor, Width, INSERT);
		
		if(FALSE == g_KeyBoardChange)
		//if(-1 == NumDisp_PreValue[WhichNumDisp])
		{
			ZiCount =4;
			Lcd_DispHZ_Asc_On_Button(LeftX+Width+LW, LeftY+Width+LW, RightX-Width-LW, RightY-Width-LW, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");
			//Lcd_DispHZ_Asc_On_Button(LeftX+Width+2, LeftY+Width+2, RightX-Width-2, RightY-Width-2, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"0");
		}
		else
		{		
			if(-1 == NumDisp_PreValue[WhichNumDisp])
			{
				ZiCount =4;
				Lcd_DispHZ_Asc_On_Button(LeftX+Width+LW, LeftY+Width+LW, RightX-Width-LW, RightY-Width-LW, ZiCount,ValueColor,ValueBackColor,ZiSize,HAlign,0,0,2,"####");
			}
			else
			{
				//+++  100517  ++++++++++++++
				/*if(((50 == ReadPlcNum) && (0 == strcmp("P_W", PlcReadStr))) ||		// P_W50 
					((3000 == ReadPlcNum) && (0 == strcmp("MW", PlcReadStr))))	// MW3000 
				{
					sprintf(str,"%x",NumDisp_PreValue[WhichNumDisp]);
					Lcd_Fill_Box(LeftX+Width+2, LeftY+Width+2, RightX-Width-2, RightY-Width-2,ValueBackColor);
					a = strlen(str);
					Lcd_DispAsc_On_Button(LeftX+2, LeftY+2, RightX-2, RightY-2,a,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
				}
				else*/
				{
					switch(DigitPos)
					{
						case 0:
							CurrentDispValue=(double)NumDisp_PreValue[WhichNumDisp];
							break;
						case 1:
							CurrentDispValue=(double)NumDisp_PreValue[WhichNumDisp]/10.0;
							break;
						case 2:
							CurrentDispValue=(double)NumDisp_PreValue[WhichNumDisp]/100.0;
							break;
						case 3:
							CurrentDispValue=(double)NumDisp_PreValue[WhichNumDisp]/1000.0;
							break;
						case 4:
							CurrentDispValue=(double)NumDisp_PreValue[WhichNumDisp]/10000.0;
					}
					
					sprintf(str,"%.3f",CurrentDispValue);
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
					Lcd_Fill_Box(LeftX+Width+LW, LeftY+Width+LW, RightX-Width-LW, RightY-Width-LW,ValueBackColor);
			    		if(DigitNumb==0)
						 Lcd_DispAsc_On_Button(LeftX+LW, LeftY+LW, RightX-LW, RightY-LW,a,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
					else
						 Lcd_DispAsc_On_Button(LeftX+LW, LeftY+LW, RightX-LW, RightY-LW,a+1+DigitNumb,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
				}
			}
		}


		if(!((strcmp(PlcReadStr,"P_KW")==0) ||(strcmp(PlcReadStr,"P_W")==0 )))
			g_com_Flag = TRUE;  // 通讯标志
			
	}	
	else 
	{
		if(WordOrDWord==0)
		{
			/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
				&& (0 == strcmp("MW", PlcReadStr)))
			{
				strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
				for(k=0; k<DELAY_TIME_TRAN; k++); //100000
			}
			else if(0 == strcmp("MW", PlcReadStr))
				for(k=0; k<DELAY_TIME; k++);*/

			CurrentPlcValue=ReadWord(ReadPlcNum,PlcReadStr);
		}
		else
		{
			/*if((0 != strcmp(g_GlobalVar.PrePlcStr, PlcReadStr)) 
				&& (0 == strcmp("MW", PlcReadStr)))
			{
				strcpy(g_GlobalVar.PrePlcStr, PlcReadStr);
				for(k=0; k<DELAY_TIME_TRAN; k++); //100000
			}
			else if(0 == strcmp("MW", PlcReadStr))
				for(k=0; k<DELAY_TIME; k++);*/

			CurrentPlcValue=ReadWord(ReadPlcNum,PlcReadStr);
			//CurrentPlcValue=CurrentPlcValue & 0x0000ffff;	//060627
			CurrentPlcValue1=ReadWord(ReadPlcNum+1,PlcReadStr);

			if((0 == strcmp("P_W", PlcReadStr)) 
				|| (0 == strcmp("P_KW", PlcReadStr)))
			{
				CurrentPlcValue=(CurrentPlcValue1<<16) | (CurrentPlcValue & 0x0000ffff);	//060627
			}
			else
			{
				//if(CurrentPlcValue==-1 && CurrentPlcValue1==-1)
				// 只要一个错误(通讯错误回-1 , 正常为返回0xffff)
				if(CurrentPlcValue==-1||CurrentPlcValue1==-1)
					return;
				else
					CurrentPlcValue=(CurrentPlcValue1<<16) | (CurrentPlcValue & 0x0000ffff);	//060627
			}

		}
		if(CurrentPlcValue==-1)
			return;

		if(NumDisp_PreValue[WhichNumDisp] == CurrentPlcValue)
			return;	// 值没变化，直接返回
		else
			NumDisp_PreValue[WhichNumDisp] = CurrentPlcValue;


		//+++  100517  ++++++++++++++
		/*if(((50 == ReadPlcNum) && (0 == strcmp("P_W", PlcReadStr))) ||		// P_W50 
			((3000 == ReadPlcNum) && (0 == strcmp("MW", PlcReadStr))))	// MW3000 
		{
			sprintf(str,"%x",NumDisp_PreValue[WhichNumDisp]);
			Lcd_Fill_Box(LeftX+Width+2, LeftY+Width+2, RightX-Width-2, RightY-Width-2,ValueBackColor);
			a = strlen(str);	// a == 1
			Lcd_DispAsc_On_Button(LeftX+2, LeftY+2, RightX-2, RightY-2,a,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
		}
		else*/
		{
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
			
			sprintf(str,"%.3f",CurrentDispValue);
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
			Lcd_Fill_Box(LeftX+Width+LW, LeftY+Width+LW, RightX-Width-LW, RightY-Width-LW,ValueBackColor);
	    		if(DigitNumb==0)
				 Lcd_DispAsc_On_Button(LeftX+LW, LeftY+LW, RightX-LW, RightY-LW,a,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
			else
				 Lcd_DispAsc_On_Button(LeftX+LW, LeftY+LW, RightX-LW, RightY-LW,a+1+DigitNumb,ValueBackColor,ValueColor,ZiSize,HAlign,Width,str);
		}
		



		////// 070516 加减值后跟着刷新//////
		//数值显示元件地址和偏移地址在DrawNumDispButton(1); 中赋值 
		g_GlobalVar.UpD_NumDisp[g_GlobalVar.Which_NumDisp][0]=ReadPlcNum;
		g_GlobalVar.UpD_NumDisp[g_GlobalVar.Which_NumDisp][1]=ScrBufferOffset;
		////////////////////////////////////////////

	}
}




/////////////////////////////////////////////StickChart//////
void DrawStickChart(void)
{
	INT16U i;
	INT16U BackColor,Color,TargetColor,LowRangeColor,HighRangeColor;
	INT16U LeftX,LeftY,RightX,RightY;//,Radius;
	
	INT16U cx,cy,cx1,cy1,cx2,cy2,cx3,cy3;
	INT8U Direction,GraphType,TargetFlag,RangeFlag;
	INT16U Maximum,Minimum,TargetValue,LowLimited,HighLimited,CurrentValue;
	
	INT32U temp;
	INT8U RdStringLen;
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
	temp = 22+1+RdStringLen+2;	

	LeftX = *(ptr+i+temp+1);
	LeftX = (LeftX<<8) | (*(ptr+i+temp));
	LeftY = *(ptr+i+temp+3);
	LeftY = (LeftY<<8) | (*(ptr+i+temp+2));
	temp = temp+4;
	RightX = *(ptr+i+temp+1);
	RightX = (RightX<<8) | (*(ptr+i+temp));
	RightY = *(ptr+i+temp+3);
	RightY = (RightY<<8) | (*(ptr+i+temp+2));
	    
	CurrentValue=(Maximum-Minimum)/2;    
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


	if(TargetFlag==0&&RangeFlag==0)
	{		
		 Lcd_Disp_CurrentValue(LeftX,LeftY,RightX,RightY,Color,Direction,GraphType,cx,cy);
	}
	else if(TargetFlag==1&&RangeFlag==0)
	{							
		Lcd_Disp_TargetValue(LeftX,LeftY,RightX,RightY,TargetColor,Direction,GraphType,cx1,cy1);
		Lcd_Disp_CurrentValue(LeftX,LeftY,RightX,RightY,Color,Direction,GraphType,cx,cy);
	}
	
    	else if(TargetFlag==0&&RangeFlag==1)
	{										
		Lcd_Disp_LoWRange(LeftX,LeftY,RightX,RightY,LowRangeColor,Direction,GraphType,cx1,cy1,cx2,cy2);					
		Lcd_Disp_HighRange(LeftX,LeftY,RightX,RightY,HighRangeColor,Direction,GraphType,cx1,cy1,cx3,cy3);
		Lcd_Disp_CurrentValue(LeftX,LeftY,RightX,RightY,Color,Direction,GraphType,cx,cy);
	}
	
	else if(TargetFlag==1&&RangeFlag==1)
	{					
		Lcd_Disp_LoWRange(LeftX,LeftY,RightX,RightY,LowRangeColor,Direction,GraphType,cx1,cy1,cx2,cy2);						
		Lcd_Disp_HighRange(LeftX,LeftY,RightX,RightY,HighRangeColor,Direction,GraphType,cx1,cy1,cx3,cy3);	
		Lcd_Disp_TargetValue(LeftX,LeftY,RightX,RightY,TargetColor,Direction,GraphType,cx1,cy1);
		Lcd_Disp_CurrentValue(LeftX,LeftY,RightX,RightY,Color,Direction,GraphType,cx,cy);
	}
	
	for(i=0;i<RefreshNum;i++)
	{
		if(StickChartRefresh[i][0]==0)
		{
			StickChartRefresh[i][0]=ScrBufferOffset;
			StickChartRefresh[i][1]= -1;	//以使第一次刷新
			break;
		}
	}
	
}



void DrawStaticText()
{	
	INT8U temp;
	INT16U TextColor,TextBackColor,KuangColor;
	INT16U LeftX,LeftY,RightX,RightY;
	INT8U Width,ZiSize,HAlign,VAlign,Shape,KuangFlag,FlagUnderLine;
	INT8U *ptr = ScreenDataBuffer;
	int ZiCount;
	int i;
	
	i = ScrBufferOffset;
	temp=0;
	temp++;
	Shape=*(ptr+i+temp);
	temp++;
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
	Width=*(ptr+i+temp);
	temp++;
	TextBackColor=*(ptr+i+temp);
	temp++;
	KuangFlag=*(ptr+i+temp);
	temp++;
	KuangColor=*(ptr+i+temp);
	temp++;
	ZiCount = *(ptr+i+temp+3);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+2);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp+1);
	ZiCount = (ZiCount<<8) | *(ptr+i+temp);
//	ZiCount /= 2;
	temp = temp+4;
	TextColor=*(ptr+i+temp);
	temp++;
	ZiSize=*(ptr+i+temp);
	temp++;
	HAlign = *(ptr+i+temp);
	temp++;
	VAlign = *(ptr+i+temp);
	temp++;
	FlagUnderLine = *(ptr+i+temp);
	temp = temp+1;
	temp = temp+1;
	
	if(Shape==1)
	{
		if(Width !=0)
			Lcd_Fill_Box(LeftX,LeftY,RightX,RightY,1);
		Lcd_Fill_Box(LeftX+Width,LeftY+Width, RightX-Width,RightY-Width,TextBackColor);	
	}
	else
	{
		if(Width != 0)
			Lcd_Fill_Ellipse(LeftX,LeftY, RightX,RightY,1);
		Lcd_Fill_Ellipse(LeftX+Width,LeftY+Width, RightX-Width,RightY-Width,TextBackColor);
	}
	 Lcd_DispHZ_Asc_On_Button(LeftX,LeftY, RightX,RightY, ZiCount,TextColor,TextBackColor,ZiSize,HAlign,VAlign,FlagUnderLine,Width,(char*)(ptr+i+temp));
	
	    
}



void DisplayUserScr(INT16U ScrNum)
{
	INT16U ComponentCount;
	INT8U temp8U;//StarPage,EndPage,
	INT16U i=0;
	int ScrPrt;//,w;

	char str[20];
	sprintf(str,"%f",(float)1.234);

	for(i=0;i<RefreshNum;i++)
	{
		////// 070810 加减值后跟着刷新--PLC地址为0 则不提前刷新//////
		g_GlobalVar.UpD_NumDisp[i][0]=0;
		g_GlobalVar.UpD_NumDisp[i][1]=0;
	
		WatchRefresh[2*i]=0;
		WatchRefresh[2*i+1]=0;
		StickChartRefresh[i][0]= 0;
		StickChartRefresh[i][1]= -1;	//以使第一次刷新
		KeepButtonRefresh[i][0]=0;
		KeepButtonRefresh[i][1]=0;
		KeepButton2Refresh[i][0]=0;
		KeepButton2Refresh[i][1]=0;
		//NumDispRefresh[i]=0;
		//SetValueRefresh[i] = 0;
		MultiStateRefresh[i][0]=0;
		MultiStateRefresh[i][1]=0;
		TimerButtonRefresh[i]=0;
		OscButtonRefresh[i]=0;

		if(FALSE == g_KeyBoardChange)
		{
			NumDispRefresh[i]=0;
			SetValueRefresh[i] = 0;
			
			SetValue_PreValue[i] = -1;	//以使第一次刷新
			NumDisp_PreValue[i] = -1;	//以使第一次刷新
		}
	}

	if(ScrNum!=CurrentScrNum)
	{
		PreScrNum = CurrentScrNum;
		CurrentScrNum = ScrNum;
	}

	
	GetScreenData(ScrNum);

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
		//	case 5:
		//		{
		//			DrawShiftButton(0);
		//			break;
		//		}
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
	
	/*if(ScrNum!=CurrentScrNum)
	{
		PreScrNum = CurrentScrNum;
		CurrentScrNum = ScrNum;
	}*/

	WriteWord(g_PlcCtrlNum,g_PlcCtrStr,CurrentScrNum);

}



void Run()
{
	if(g_StarScr==0)
	{
		return;
	}

	/*
	if(0 == g_InteralMemory.KeepBit[8]) //英文
		DisplayUserScr(g_StarScr);	
	else if(1 == g_InteralMemory.KeepBit[8]) //中文
		DisplayUserScr(g_StarScr + SCR_RELATIVE);	
	*/
		
	DisplayUserScr(g_StarScr);		
		
}



/*void TestTTT()
{
	char str[20];
	//char str[20]={0};
	sprintf(str,"%f",(float)1.234);
}*/




