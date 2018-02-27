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



extern INT8U  ScreenDataBuffer[620000];

extern SYSTEMINFO g_SystemInfo;

void DrawBmp(int OffSet)
{	
	INT8U CtlType, bmpType;
	INT16U LeftX,LeftY,Width, Height,temp16;
	INT8U *ptr = ScreenDataBuffer;
	int i,j,k;

	U8 rgbHigh, rgbLow;
	U16 rgb16;
	
	i = OffSet;

	CtlType = *(ptr+i);           //Check the ObjID
	i++;
	if(CtlType != 20)
		return;

	bmpType = *(ptr+i);			//Check the Bmp type
	i++;
	
	//if((g_SystemInfo.LCDType ==0 && bmpType != 1) || (g_SystemInfo.LCDType  ==1 && bmpType != 2)) 
	if((g_SystemInfo.LCDType ==0 && bmpType != 1) 
		|| (g_SystemInfo.LCDType  ==1 && bmpType != 2)
		//|| (g_SystemInfo.LCDType  ==1 && bmpType != 3)
		|| (g_SystemInfo.LCDType  ==1 && bmpType != 4)) // 565bmp 24bbp转换而来，bmpType = 4;

		return;
	

	LeftX = *(ptr+i+1);
	LeftX = (LeftX<<8) | (*(ptr+i));
	i+=2;
	LeftY = *(ptr+i+1);
	LeftY = (LeftY<<8) | (*(ptr+i));
	i+=2;
	
	Width = *(ptr+i+1);
	Width = (Width<<8) | (*(ptr+i));
	i+=2;
	Height = *(ptr+i+1);
	Height = (Height<<8) | (*(ptr+i));
	i+=2;


	temp16 = Width % 4 ;
	if(temp16!=0)
		temp16 = 4 - temp16;

	for(j=LeftY+Height-1;j>=LeftY;j--)
	{
		for(k=LeftX;k<LeftX+Width;k++)
		{	
			if(2 == bmpType)	// 256
			{
				LCD_PutPixel_Bmp(k, j,*(ptr+i));
				i++;
			}
			else if(4 == bmpType)	// 565bmp
			{
				rgbLow= *(ptr+i);
				rgbHigh= *(ptr+i+1);
				rgb16 = (rgbHigh<<8) + rgbLow;
				LCD_PutPixel_Bmp565(k, j,rgb16);
				i += 2;
			}
		}
		if(2 == bmpType)	// 256
			i +=  temp16;
		//else if(4 == bmpType) // 565bmp (应引掉, 24bbp 每行多补的字节没有编译进文件)
		//	i += (temp16*16/8)%4;
	}

	return;
	    
}


