
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "2416addr.h"
#include "LCD.h"


//volatile static unsigned short LCD_BUFER[LCD_SCR_YSIZE][LCD_SCR_XSIZE];
U16 LCD_BUFER[LCD_SCR_YSIZE][LCD_SCR_XSIZE];



extern U8 g_ASC16_Dot[];
extern U8 g_HZK16_Dot[];
extern U8 g_HZK24S_Dot[];

//extern INT16U CurrentScrNum;


/*-----------------------------------------------------------------------------
 *  LCD PWREN端口初始化, no use
 */
void Lcd_Pwren_Init_On_Off( unsigned char m )
{      
	if( m == TRUE )
	{
		rGPEDAT &= ~(1<<10);		//output 0;
	}
	else
		rGPEDAT |= (1<<10);		//output 1;
	
}


/*-----------------------------------------------------------------------------
 *  TFT LCD数据和控制端口初始化
 */
void Lcd_Port_Init( void )
{
	Lcd_Pwren_Init_On_Off( TRUE ) ;
}





void delayLoop(int count) 
{ 
	volatile int j; 
	for(j = 0; j < count; j++)  ; 
}



 /*
// TFT LCD 功能模块初始化
void Lcd_Init(void)
{   
	U32 k=0,j=0;
	//U8 pagewidth_in_byte=0,offsize_in_byte=0;	
	U16 pagewidth_in_byte=0,offsize_in_byte=0;	

	U8 clkval=0;
	U16 hsync_cnt,vclk_cnt;
	U16 lcd_horizon_value,lcd_line_value;
	U8 lcd_vbpd,lcd_vfpd,lcd_vspw,lcd_hbpd,lcd_hfpd,lcd_hspw;
	U8 lcd_frame_rate;	
	int i;
	unsigned short *pFB;

	rMISCCR = rMISCCR | (1<<28);  // select LCD controller for TFT lcd controller	
			 
	lcd_horizon_value = LTE480_HOZVAL;
	lcd_line_value = LTE480_LINEVAL;
	lcd_vbpd = LTE480_VBPD;
	lcd_vfpd = LTE480_VFPD;
	lcd_vspw = LTE480_VSPW;
	lcd_hbpd = LTE480_HBPD;
	lcd_hfpd = LTE480_HFPD;
	lcd_hspw = LTE480_HSPW;
	lcd_frame_rate = 30;	///LTE480_FRAME_RATE;

	pagewidth_in_byte = lcd_horizon_value/8*18;	
	offsize_in_byte = 0;
	
	rWINCON0 = rWINCON0 & 0xfffffffe;
	rWINCON1 = rWINCON0 & 0xfffffffe;
	
	rVIDTCON0=(12<<16)|(9<<8)|1;

	rVIDTCON1=(30<<16)|(15<<8)|(30);

	rVIDTCON2 = (479<<11)|(639);

			
	rVIDCON0=rVIDCON0 & 0xfffffffc;
	
	
	rVIDCON1 = (VIDCON1_S_HSYNC_INVERTED)|(VIDCON1_S_VSYNC_INVERTED);
	rVIDCON0 = (VIDCON0_S_RGB_IF)|(VIDCON0_S_RGB_PAR)|(VIDCON0_S_VCLK_GATING_OFF)|(VIDCON0_S_CLKDIR_DIVIDED)|(VIDCON0_S_CLKSEL_UPLL)| (3 <<VIDCON0_CLKVAL_F_SHIFT)|(0x3);   
	
	
	           
	hsync_cnt = (lcd_vbpd+lcd_vfpd+lcd_vspw+lcd_line_value);
	//printf("水平同步脉冲数 : 0x%x\n", hsync_cnt);
	
	vclk_cnt = (lcd_hbpd+lcd_hfpd+lcd_hspw+lcd_horizon_value);
	//printf("垂直同步脉冲数 : 0x%x\n", hsync_cnt);

	rWINCON0 = (0<<WINCON_SWAP_S)|(WINCONx_4WORD_BURST<<WINCON_BURSTLEN_S)|(WINCONx_16BPP_565<<WINCON_BPP_S); // 4word burst, 16bpp, 
	rWINCON1 = (0<<WINCON_SWAP_S)|(WINCONx_4WORD_BURST<<WINCON_BURSTLEN_S)|(WINCONx_16BPP_565<<WINCON_BPP_S); // 4word burst, 16bpp, 
	

	///??????	
	rVIDOSD0A = (0<<VIDOSDxAB_HORIZON_X_S)|(0);
	rVIDOSD0B = ((lcd_horizon_value-1)<<VIDOSDxAB_HORIZON_X_S)|(lcd_line_value-1);	
	rVIDOSD1A = (0<<VIDOSDxAB_HORIZON_X_S)|(0);
	rVIDOSD1B = ((lcd_horizon_value-1)<<VIDOSDxAB_HORIZON_X_S)|(lcd_line_value-1);
	rVIDOSD1C = 0x0;


	
	rVIDW00ADD0B0 =(U32)LCD_BUFER;	// (((U32)LCD_BUFER>>21)<<24) | M5D((U32)LCD_BUFER>>1);
		// buffer end address
	rVIDW00ADD1B0 =rVIDW00ADD0B0 + (LCD_WIDTH*LCD_HEIGHT*2);
			// buffer size 	
	rVIDW00ADD2B0 = (offsize_in_byte<<VIDWxADD2_OFFSET_SIZE_S)|(LCD_WIDTH*2);

	rWINCON0 = rWINCON0 | 0x1;

	rINTSUBMSK = rINTSUBMSK | (0xf << 14);	// MASK all LCD Sub Interrupt

	pFB = (unsigned short *)LCD_BUFER;

	for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++)
	{
		*pFB++ = 0x001F;		// Blue
	}

}
*/

// TFT LCD 功能模块初始化
void Lcd_Init(void)
{   
	//U32 k=0,j=0;
	//U8 pagewidth_in_byte=0,offsize_in_byte=0;	
	//U16 pagewidth_in_byte=0;
	U16 offsize_in_byte=0;	

	U16 lcd_horizon_value,lcd_line_value;
	U8 lcd_vbpd,lcd_vfpd,lcd_vspw,lcd_hbpd,lcd_hfpd,lcd_hspw;
	U8 lcd_frame_rate;	
	U8 clkval_def;
	
	int i;
	unsigned short *pFB;

	U8 clkval=0;
	U16 hsync_cnt,vclk_cnt;
	

	rMISCCR = rMISCCR | (1<<28);  // select LCD controller for TFT lcd controller
	
			 
	lcd_horizon_value = LTE480_HOZVAL;
	lcd_line_value = LTE480_LINEVAL;
	
	lcd_vbpd = LTE480_VBPD;
	lcd_vfpd = LTE480_VFPD;
	lcd_vspw = LTE480_VSPW;
	
	lcd_hbpd = LTE480_HBPD;
	lcd_hfpd = LTE480_HFPD;
	lcd_hspw = LTE480_HSPW;
	
	lcd_frame_rate = LTE480_FRAME_RATE;	
	clkval_def = CLKVAL_TFT_640480;
	
	//delayLoop(LCD_DELAY_1MS*10);
	
	//pagewidth_in_byte = lcd_horizon_value/8*18;	
	offsize_in_byte = 0;
	rWINCON0 &= ~0x01;
	rWINCON1 &= ~0x01;


	rVIDCON0 &= (~3); // ENVID Off using Per Frame method				
	//rVIDCON0 = (VIDCON0_S_RGB_IF)|(VIDCON0_S_RGB_PAR)|(VIDCON0_S_VCLK_GATING_OFF)
	//		|(VIDCON0_S_CLKDIR_DIVIDED)|(VIDCON0_S_CLKSEL_UPLL)| (3 <<VIDCON0_CLKVAL_F_SHIFT)|(0x3);   
	rVIDCON0 = (VIDCON0_S_RGB_IF)|(VIDCON0_S_RGB_PAR)|(VIDCON0_S_VCLK_GATING_OFF)
			|(VIDCON0_S_CLKDIR_DIVIDED)|(VIDCON0_S_CLKSEL_HCLK);   
	//rVIDCON0 |= (clkval_def <<VIDCON0_CLKVAL_F_SHIFT);
		
	hsync_cnt = (lcd_vbpd+lcd_vfpd+lcd_vspw+lcd_line_value);
	//printf("水平同步脉冲数 : 0x%x\n", hsync_cnt);	
	vclk_cnt = (lcd_hbpd+lcd_hfpd+lcd_hspw+lcd_horizon_value);
	//printf("垂直同步脉冲数 : 0x%x\n", hsync_cnt);
	clkval = (U8)(((float)SYS_HCLK/(float)(hsync_cnt*vclk_cnt*lcd_frame_rate*2))+0.5)-1;	
	//rVIDCON0 |= (clkval <<VIDCON0_CLKVAL_F_SHIFT);
	rVIDCON0 |= (clkval_def <<VIDCON0_CLKVAL_F_SHIFT);	//VCLK(Hz) = HCLK/[(CLKVAL+1)x2]  16.625
	//rVIDCON0 |= (2 <<VIDCON0_CLKVAL_F_SHIFT);		//VCLK(Hz) = HCLK/[(CLKVAL+1)x2]    22.17

	// 使用!!! rVIDCON0 |= (9 <<VIDCON0_CLKVAL_F_SHIFT); // 13.3M  显示正常(之前必须把示波器探针搭在Tclk 才能正常显示)
	//rVIDCON0 |= (6 <<VIDCON0_CLKVAL_F_SHIFT); // 19M
	//rVIDCON0 |= (5 <<VIDCON0_CLKVAL_F_SHIFT); // 22.2M 可显示蓝色，显示按钮时变成黑屏同时闪烁，示波器搭在Tck后可正常显示，再更新数据后一直在黑屏闪烁
	//rVIDCON0 |= (4 <<VIDCON0_CLKVAL_F_SHIFT); // 26.6M  写数据时闪烁,后正常


	
	rVIDCON1 = (VIDCON1_S_HSYNC_INVERTED)|(VIDCON1_S_VSYNC_INVERTED);

		
	
	////rVIDTCON0=(32<<16)|(9<<8)|1;
	//rVIDTCON0=(12<<16)|(9<<8)|1;
	////rVIDTCON1=(47<<16)|(15<<8)|(95);
	//rVIDTCON1=(30<<16)|(15<<8)|(30);
	//rVIDTCON2 = (479<<11)|(639);
	
	rVIDTCON0=((lcd_vbpd-1)<<VIDTCON0_BPD_S)|((lcd_vfpd-1)<<VIDTCON0_FPD_S)|(lcd_vspw-1);
	rVIDTCON1=((lcd_hbpd-1)<<VIDTCON0_BPD_S)|((lcd_hfpd-1)<<VIDTCON0_FPD_S)|(lcd_hspw-1);
	rVIDTCON2 = ((lcd_line_value-1)<<VIDTCON2_LINEVAL_S)|(lcd_horizon_value-1);

			
	//delayLoop(LCD_DELAY_1MS*10);
	

	//rWINCON0 = (0<<WINCON_SWAP_S)|(WINCONx_4WORD_BURST<<WINCON_BURSTLEN_S)
	//			|(WINCONx_16BPP_565<<WINCON_BPP_S); // 4word burst, 16bpp, 
	rWINCON0 = (0<<WINCON_SWAP_S)|(WINCONx_16WORD_BURST<<WINCON_BURSTLEN_S)
				|(WINCONx_16BPP_565<<WINCON_BPP_S); // 16word burst, 16bpp, 
	//rWINCON1 = (0<<WINCON_SWAP_S)|(WINCONx_4WORD_BURST<<WINCON_BURSTLEN_S)
	//			|(WINCONx_16BPP_565<<WINCON_BPP_S); // 4word burst, 16bpp, 
	rWINCON1 = (0<<WINCON_SWAP_S)|(WINCONx_16WORD_BURST<<WINCON_BURSTLEN_S)
				|(WINCONx_16BPP_565<<WINCON_BPP_S); // 16word burst, 16bpp, 



	///??????	
	rVIDOSD0A = (0<<VIDOSDxAB_HORIZON_X_S)|(0);
	rVIDOSD0B = ((lcd_horizon_value-1)<<VIDOSDxAB_HORIZON_X_S)|(lcd_line_value-1);	
	rVIDOSD1A = (0<<VIDOSDxAB_HORIZON_X_S)|(0);
	rVIDOSD1B = ((lcd_horizon_value-1)<<VIDOSDxAB_HORIZON_X_S)|(lcd_line_value-1);
	rVIDOSD1C = 0x0;

	
	rVIDW00ADD0B0 =(U32)LCD_BUFER;	// (((U32)LCD_BUFER>>21)<<24) | M5D((U32)LCD_BUFER>>1);
		// buffer end address
	rVIDW00ADD1B0 =rVIDW00ADD0B0 + (LCD_WIDTH*LCD_HEIGHT*2);
			// buffer size 	
	rVIDW00ADD2B0 = (offsize_in_byte<<VIDWxADD2_OFFSET_SIZE_S)|(LCD_WIDTH*2);
	
	
	
	
	rWINCON0 |= (1<<WINCON_SWAP_S);	
	rWINCON0 |= rWINCON0 | 0x1;
	
	rVIDCON0 = rVIDCON0 | 0x3;	// Enable video output and logics


	rINTSUBMSK |= (0xf << 14);	// MASK all LCD Sub Interrupt


	pFB = (unsigned short *)LCD_BUFER;
	for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++)
	{
		*pFB++ = 0x001F;		// Blue
	}	
}


//*******************************************************************//
//函数名: Paint_CrossLine
//输入:(x,y) 十字交叉线中心点，dd偏离中心点距离，color为线条颜色
//输出：null
//功能描述:以(x,y)为原点画十字交叉线
//by jane
//*******************************************************************//
void Paint_CrossLine(int x,int y,int dd, U16 color)
{
	Glib_Line(x-dd, y, x+dd, y, color);
	Glib_Line(x-dd, y+1, x+dd, y+1, color);
	Glib_Line(x, y-dd, x, y+dd, color);
	Glib_Line(x+1, y-dd, x+1, y+dd, color);
}



/*-----------------------------------------------------------------------------
 *  LCD屏幕显示垂直翻转
 *  LCD display is flipped vertically
 *  But, think the algorithm by mathematics point.
 *    3I2
 *    4 I 1
 *   --+--   <-8 octants  mathematical cordinate
 *    5 I 8
 *    6I7
 */
void Glib_Line(int x1, int y1, int x2, int y2, U16 color)
{
    int dx, dy, e;
    
    dx = x2 - x1; 
    dy = y2 - y1;
    
    if(dx >= 0)
    {
        if(dy >= 0)        // dy>=0
        {
            if(dx >= dy)   // 1/8 octant
            {
                e = dy-dx/2;
                while(x1 <= x2)
                {
                    LCD_PutPixel(x1, y1, color);
                    if(e > 0) {y1+=1; e-=dx;}
                    x1 += 1;
                    e += dy;
                }
            }
            else        // 2/8 octant
            {
                e = dx-dy/2;
                while(y1 <= y2)
                {
                    LCD_PutPixel(x1, y1, color);
                    if(e > 0) {x1+=1; e-=dy;}
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else           // dy<0
        {
            dy = -dy;   // dy=abs(dy)
            
            if(dx >= dy) // 8/8 octant
            {
                e = dy-dx/2;
                while(x1 <= x2)
                {
                    LCD_PutPixel(x1, y1, color);
                    if(e > 0) {y1-=1; e-=dx;}
                    x1 += 1;
                    e += dy;
                }
            }
            else        // 7/8 octant
            {
                e = dx-dy/2;
                while(y1 >= y2)
                {
                    LCD_PutPixel(x1, y1, color);
                    if(e > 0) {x1+=1; e-=dy;}
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx=-dx;     //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LCD_PutPixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}	
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    LCD_PutPixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}	
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else       // dy<0
        {
            dy=-dy;   // dy=abs(dy)
            
            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LCD_PutPixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}	
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    LCD_PutPixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}	
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 *  在LCD屏幕上用颜色填充一个矩形
 */
void Glib_FilledRectangle(int x1, int y1, int x2, int y2, U16 color)
{
    int i;

    for(i = y1; i <= y2; i++) {    // 用n条直线填满区域!
        Glib_Line(x1, i, x2, i, color);
    }
}


/*-----------------------------------------------------------------------------
 *  在LCD屏幕上指定坐标点画一个指定大小的图片
 */
void Paint_Bmp(int x0, int y0, int h, int l, unsigned char bmp[])
{
    int x, y;
    U32 c;
    int p = 0;
    
    for( y = 0 ; y < l ; y++ )
    {
        for( x = 0 ; x < h ; x++ )
        {
            c = bmp[p+1] | (bmp[p]<<8) ;
            
            if ( ( (x0+x) < LCD_SCR_XSIZE) && ( (y0+y) < LCD_SCR_YSIZE) )
                LCD_BUFER[y0+y][x0+x] = c ;
            
            p = p + 2 ;
        }
    }
}






void Lcd_Display_String(INT16U LeftX,INT16U LeftY,INT8U Count,U16 FontColor,U16 BackColor,
	INT8U Size,char *Ptr,INT8U FlagUnderLine)
{
	INT8U QuMa,WeiMa,XLargeTime,YLargeTime,HzType,HZHeight,HZWidth;
	INT16U i;
	INT32U ZMStartAddr;

	switch(Size)
	{
		case 0: HzType = HZK16x16;
				XLargeTime = 1;
				YLargeTime = 1;
				HZWidth = 16;
				HZHeight = 16;
				break;
		case 1: HzType = HZK16x16;
				XLargeTime = 1;
				YLargeTime = 2;
				HZWidth = 16;
				HZHeight = 32;
				break;
		case 2: HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 1;
				HZWidth = 24;
				HZHeight = 24;
				break;
		case 3: HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 2;
				HZWidth = 24;
				HZHeight = 48;
				break;
		case 4: HzType = HZK16x16;
				XLargeTime = 2;
				YLargeTime = 2;
				HZWidth = 32;
				HZHeight = 32;
				break;
		case 5: HzType = HZK16x16;
				XLargeTime = 2;
				YLargeTime = 4;
				HZWidth = 32;
				HZHeight = 64;
				break;
		case 6: HzType = HZK24x24;
				XLargeTime = 2;
				YLargeTime = 2;
				HZWidth = 48;
				HZHeight = 48;
				break;
		case 7: HzType = HZK24x24;
				XLargeTime = 2;
				YLargeTime = 4;
				HZWidth = 48;
				HZHeight = 96;
				break;
		case 8: HzType = HZK16x16;
				XLargeTime = 4;
				YLargeTime = 4;
				HZWidth = 64;
				HZHeight = 64;
				break;
		case 9: HzType = HZK24x24;
				XLargeTime = 4;
				YLargeTime = 4;
				HZWidth = 96;
				HZHeight = 96;
				break;
		default:HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 1;
				HZWidth = 24;
				HZHeight = 24;
				break;
	}

	for(i=0;i<Count*2;i+=2)
	{
		QuMa = *(Ptr+i);
		WeiMa = *(Ptr+i+1);	
		ZMStartAddr = Get_StartAddr_Of_HanZi(QuMa, WeiMa, HzType);
		Lcd_Disphz(HzType, (char *)ZMStartAddr, LeftX, LeftY, XLargeTime, YLargeTime, FontColor, BackColor);
		if(FlagUnderLine == 1)
		{
			Lcd_Draw_HLine (LeftX, LeftX+HZWidth, LeftY+HZHeight+1, FontColor,1);
		}
		LeftX = LeftX+HZWidth;
		
	}
	
}





INT32U Get_StartAddr_Of_HanZi(INT8U QuMa, INT8U WeiMa, INT8U HzType)
{
	INT32U Result;
	if((QuMa & 0x80)!=0)
	{
		QuMa = QuMa - 0xa0;
		WeiMa = WeiMa - 0xa0;
	}
	if (HzType == HZK24x24)
	{
		Result = ((QuMa-18)*94+WeiMa-1+188)*72L;
		//Result = Result+STARTADDRZIKU24;
		Result = Result+(INT32U)g_HZK24S_Dot;		
	}
	else
	{
		Result = ((QuMa-1)*94+WeiMa-1)*32L;
		//Result = Result+STARTADDRZIKU16;
		Result = Result+(INT32U)g_HZK16_Dot;		
	}
	return Result;
}


//***************************************/*****************************************************************
//** 函数名:void Lcd_Draw_HLine(INT16U usX0,INT16U usY0,INT16U usX1,INT16U usY1,INT8U ucColor, INT16U usWidth)
//** 输 入: INT16U usX0,usX1,usY0,usY1线条端点坐标
//**       INT8U ucColor:线条颜色
//**	INT16U usWidth:线条宽度
//** 输 出: Null
//** 功能描述:以指定的颜色颜色绘制水平线条
//** 全局变量:Null
//*****************************************************************
void Lcd_Draw_HLine (INT16 usX0, INT16 usX1, INT16 usY0, U16 ucColor,INT16U usWidth)
{
	INT16 usLen,temp;

    if (usX1 < usX0)
    {
        temp = usX1;
		usX1 = usX0;
		usX0 = temp;	
    }

    while(usWidth-- > 0)
    {
        usLen = usX1 - usX0 + 1;
        while(usLen-- > 0)
        {
		LCD_PutPixel(usX0 + usLen, usY0,ucColor);
        }
        usY0++;
    }
}



//***************************************/*****************************************************************
//** 函数名:void Lcd_Draw_VLine(INT16U usX0,INT16U usY0,INT16U usX1,INT16U usY1,INT8U ucColor, INT16U usWidth)
//** 输 入: INT16U usX0,usX1,usY0,usY1线条端点坐标
//**       INT8U ucColor:线条颜色
//**	INT16U usWidth:线条宽度
//** 输 出: Null
//** 功能描述:以指定的颜色颜色绘制垂直线条
//** 全局变量:Null
//*****************************************************************
void Lcd_Draw_VLine (INT16 usY0, INT16 usY1, INT16 usX0, U16 ucColor, INT16U usWidth)
{
	INT16 usLen,temp;
	//int i;

	if (usY1 < usY0)
	{
		temp = usY1;
		usY1 = usY0;
		usY0 = temp;	
	}

	while(usWidth-- > 0)
	{
		usLen = usY1 - usY0 + 1;
		while(usLen-- > 0)
		{
			//for(i=0; i<1000; i++);	// 1000
			LCD_PutPixel(usX0, usY0 + usLen, ucColor);
		}
		usX0++;
	}
}



/*-----------------------------------------------------------------------------
 *  test LTV350QV-F05
 */
void Test_Lcd(void)
{	
	int i;
	
	for(i=0; i<16; i++)
	{
		Lcd_Clr_Test(1<<i);
		Lcd_Clr_Test((~0x1)<<i);
	}

	for(i=0; i<16; i++)
	{
		Lcd_Clr_Test(0xff0f+(i<<4));
	}

/*

   // Lcd_Port_Init();
   // Lcd_Init();
    //Lcd_EnvidOnOff(1);

	//Lcd_Clr( (0x00<<11) | (0x00<<5) | (0x00) );     //clear screen
	//Lcd_Clr( 0x7fff);     //clear screen
	//Lcd_Clr( 0x8000);     //clear screen
	//Lcd_Clr( 0x8fff);     //clear screen
	//Lcd_Clr( 0x9000);     //clear screen
	//Lcd_Clr( 0xb000);     //clear screen
	//Lcd_Clr( 0xe000);     //clear screen
	//Lcd_Clr( 0x7fff);     //clear screen
 	//Lcd_Clr( 0xffff);     //clear screen
    
  
//    Lcd_ClearScr(0xffff);       //fill all screen with some color
//    Glib_FilledRectangle(  50,  50,  270, 190, 0x0000);   //fill a Rectangle with some color
//    Glib_FilledRectangle( 100, 100,  220, 140, 0xf800);   //fill a Rectangle with some color
    
    //LOGO
	//Glib_FilledRectangle(0, 0, 320, 35, 0x0);
	
	Delay(50);	
	Glib_FilledRectangle( 5, 35, 315, 40, 0xf000);	//Red,top
	Glib_FilledRectangle(10, 40, 310, 45, 0x07e0);	//Green,top
	Glib_FilledRectangle(15, 45, 305, 50, 0x001f);	//Blue,top
	//Glib_FilledRectangle( 5, 35, 315, 40, 0xffff);	//Red,top
	//Glib_FilledRectangle(10, 40, 310, 45, 0xffff);	//Green,top
	//Glib_FilledRectangle(15, 45, 305, 50, 0xffff);	//Blue,top

	Delay(50);
	Glib_FilledRectangle( 5, 37, 9, 200, 0xf800);	//Red,left
	Glib_FilledRectangle(10, 45, 15, 195, 0x07e0);	//Green,left
	Glib_FilledRectangle(16, 50, 20, 190, 0x001f);	//Blue,left

	Delay(50);
	Glib_FilledRectangle(311, 40, 315, 200, 0xf800);	//Red,right
	Glib_FilledRectangle(305, 45, 310, 195, 0x07e0);	//Green,right
	Glib_FilledRectangle(300, 50, 304, 190, 0x001f);	//Blue,right

	//Delay(50);
	//Paint_Bmp(20, 50, 280, 190, uCdragon_logo);			//picture
	//Delay(50);
	
	Glib_FilledRectangle( 5, 200, 315, 206, 0xf800);	//Red,button
	Glib_FilledRectangle( 10,195, 310, 200, 0x07e0);	//Green,button
	Glib_FilledRectangle( 16,190, 304, 195, 0x001f);	//Blue,button

	Glib_FilledRectangle(0, 206, 320, 240, 0x0fff);
	*/
   
}




/*-----------------------------------------------------------------------------
 *  TFT LCD全屏填充特定颜色单元或清屏
 */
void Lcd_Clr_Test( U16 c)
{
	unsigned int x, y;

	for( y = 0 ; y < LCD_SCR_YSIZE ; y++ )
	{
		for( x = 0 ; x < LCD_SCR_XSIZE ; x++ )
		{
			LCD_BUFER[y][x] = c ;
		}
	}
}




/*-----------------------------------------------------------------------------
 *  TFT LCD全屏填充特定颜色单元或清屏
 */
void Lcd_Clr( U16 c)
{
	unsigned int x, y;
	//U8 colorR,colorG,colorB;
	//U16 colorOri;
	//colorOri = c;

	for( y = 0 ; y < LCD_SCR_YSIZE ; y++ )
	{
		for( x = 0 ; x < LCD_SCR_XSIZE ; x++ )
		{
			/*if((CurrentScrNum>0) && (CurrentScrNum<256))
			{
				c = colorOri&0xff;
				colorR = ((c>>5)&0x7)<<2;	// 3-->5
				colorG = ((c>>2)&0x7)<<3;	// 3-->6
				colorB = (c&0x3)<<3;		// 2-->5
				c = (colorR<<11) | (colorG<<5) |(colorB);
			}*/
			//LCD_BUFER[y][x] = c ;
			LCD_BUFER[y][x] = NumberToColor(c) ;
		}
	}
}



/*-----------------------------------------------------------------------------
 *  320 x 240 TFT LCD单个象素的显示数据输出
 */
void LCD_PutPixel(U32 x,U32 y,U16 c)
{
	if(x<LCD_SCR_XSIZE && y<LCD_SCR_YSIZE)
	{
		LCD_BUFER[y][x] = NumberToColor(c) ;
	}
}

// 对于256色位图，只有低8位数据有效
void LCD_PutPixel_Bmp(INT16U x, INT16U y,INT16U c)
{
	U8 colorR,colorG,colorB;

	if(x>639)
		x=639;

	if(y>479)
		y=479;

	//x ^= 0x3;
	
	c = c&0xff;
	colorR = ((c>>5)&0x7)<<2;	// 3-->5
	colorG = ((c>>2)&0x7)<<3;	// 3-->6
	colorB = (c&0x3)<<3;		// 2-->5
	c = (colorR<<11) | (colorG<<5) |(colorB);
	LCD_BUFER[(y)][(x)]=c ;
}

// 565bmp 24bbp转换而来，bmp_type = 4;
void LCD_PutPixel_Bmp565(INT16U x, INT16U y,INT16U c)
{

	if(x>639)
		x=639;

	if(y>479)
		y=479;
	
	LCD_BUFER[(y)][(x)]=c ;
}


//*****************************************************************
//** 函数名:void Lcd_Fill_Box(INT16U usX0,INT16U usY0,INT16U usX1,INT16U usY1,INT8U ucColor)
//** 输 入: INT16U usX0,usX1,usY0,usY1矩形顶点坐标
//**       INT8U ucColor:填充的颜色
//** 输 出: Null
//** 功能描述:以指定颜色填充指定区域(可用做局部清屏),不建议采用该函数.建议使用LcdClrRect(),效率更高。
//** 全局变量:Null
//*****************************************************************
void Lcd_Fill_Box (INT16U X_Left, INT16U Y_Left, INT16U X_Right, INT16U Y_Right,  U16 Color)
{
	INT16U Xtemp,Ytemp,temp;

	if (X_Right<X_Left)
	{
		temp = X_Right;
	 	X_Right = X_Left;
	 	X_Left = temp;
	}
	if (Y_Right<Y_Left)
	{
		temp = Y_Right;
	 	Y_Right = Y_Left;
	 	Y_Left = temp;
	}

	
	Xtemp = X_Left;
	Ytemp = Y_Left;
	while (Ytemp <= Y_Right)
	{
		while (Xtemp <= X_Right)
		{
			LCD_PutPixel(Xtemp,Ytemp,Color);
			//LCD_BUFER[Ytemp][Xtemp] = NumberToColor(Color) ;
			Xtemp++;
		}
		Xtemp = X_Left;
		Ytemp++;
	}
	
}




void Lcd_DispAsc_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount,U16 BackColor,U16 FontColor,INT8U ZiSize,
						  INT8U HAlign, INT16U Width,char *ptr)
{
	INT8U HZWidth,HZHeight,LCount;
	INT16U XStart,YStart,Margin;
	INT8U  X_MM,Y_MM;

	char str_ori[] = {"####"};

	LeftX = LeftX+Width;
	LeftY = LeftY+Width;
	RightX = RightX-Width;
	RightY = RightY-Width;

	switch(ZiSize)
	{
		case 0: HZWidth = 8;							 //16*16   
				HZHeight = 16;
				X_MM=Y_MM=1;
				break;

		case 1: HZWidth = 8;							 //16*32   
				HZHeight = 32;
				X_MM=1;
				Y_MM=2;
				break;

		case 2:	HZWidth = 12;                           //24*24              12   
				HZHeight = 24;
				X_MM=Y_MM=1;
				break;

		case 3: HZWidth = 12;							 //24*48   
				HZHeight = 48;
				X_MM=1;
				Y_MM=3;
				break;

		case 4: HZWidth = 16;                                //  32*32            //
				HZHeight = 32;
				X_MM=Y_MM=2;
				break;

		case 5: HZWidth = 16;									 //32*64   
				HZHeight = 64;
				X_MM=2;
				Y_MM=4;
				break;

		case 6: HZWidth = 24;								//48*48                 
				HZHeight = 48;
				X_MM=3;
				Y_MM=3;
				break;

		case 7: HZWidth = 24;									 //48*96 
				HZHeight = 96;
				X_MM=3;
				Y_MM=6;
				break;

		case 8: HZWidth = 32;								//64*64
				HZHeight = 64;
				X_MM=4;
				Y_MM=4;
				break;

		case 9: HZWidth = 48;								//96*96
				HZHeight = 96;
				X_MM=6;
				Y_MM=6;
				break;

		default:HZWidth = 8;
				HZHeight = 16;
				X_MM=1;
				Y_MM=1;
				break;
	}

	LCount = (RightX - LeftX)/HZWidth;
	Margin = RightX - LeftX - ZiCount*HZWidth;
	
	switch (HAlign)
		{
		case 1: XStart = LeftX;
					break;

		case 0: XStart = LeftX+Margin/2;
					break;

		case 2: XStart = LeftX+Margin;
					break;
		
		default: XStart = LeftX+Margin/2;
				   	 break;
		}

	YStart=LeftY+((RightY-LeftY)-HZHeight)/2;

	if (ZiCount>LCount)///显示不下
	{ 
		Lcd_Disp_AscStr(str_ori,ZiCount,XStart,YStart,X_MM,Y_MM,HZWidth,FontColor,BackColor);
		return;
	}

    Lcd_Disp_AscStr(ptr,ZiCount,XStart,YStart,X_MM,Y_MM,HZWidth,FontColor,BackColor);
	

}


void Lcd_Disp_AscStr(char *AscStr,INT8U Count, INT16U Left, INT16U Top, INT8U  X_MM,
	INT8U Y_MM,INT8U Offset,U16 FontColor,U16 BLACKCOLOR)
{
		INT8U  Asc,i;
		for(i=0;i<Count;i++)
		{
			Asc=*(AscStr+i);
			Lcd_Disp_Asc( Asc,Left, Top,X_MM,Y_MM,FontColor,BLACKCOLOR);
			Left += Offset;
		}
}

void Lcd_Disp_Asc(INT8U Asc, INT16U Left, INT16U Top, INT8U XLargeTime,INT8U YLargeTime,
	U16 FontColor,U16 BLACKCOLOR)
{
	INT8U *Offset,flag;
	INT8U i,j,c,temp,XLtemp,YLtemp;
	INT16U Xtemp,Ytemp,Xtemp1,Ytemp1;

	//Offset=Asc*16+STARTADDRASC;
	Offset=Asc*16+g_ASC16_Dot;

	Xtemp = Left;
	Ytemp = Top;
	Xtemp1 = Left;
	Ytemp1 = Top;
	XLtemp=XLargeTime;
	YLtemp=YLargeTime;

	for(i=0;i<16;i++)
	{
		temp=*Offset++;
		for(j=0;j<8;j++)
		{
			flag=0;
			if ((temp << j) & 0x80)
			{
				c = FontColor;
			}
			else
			{
				flag=1;
			}
			//if(flag!=1)
			{
				while (YLtemp-- != 0) 
				{				
					while (XLtemp-- != 0)
					{
						if(flag!=1)
							LCD_PutPixel(Xtemp,Ytemp,c);
						Xtemp++;
					}
					Ytemp++;
					Xtemp = Xtemp1;
					XLtemp = XLargeTime;
				}
			}
			Ytemp = Ytemp1;
			Xtemp1 = Xtemp1+XLargeTime;
			Xtemp = Xtemp1;
			YLtemp = YLargeTime;
		}
		Ytemp = Ytemp1+YLargeTime;
		Ytemp1=Ytemp;
		Xtemp1=Left;
		Xtemp=Left;
	}

}






//****************************************************//
//name: lcd_disphz
//entry:(x,y) XLargeTime,YLargeTime,Blackcolor,*disp
//function:enlarge display
//****************************************************//

void Lcd_Disphz(INT8U HzType, char *DISP, INT16U x, INT16U y, INT8U XLargeTime,
				INT8U YLargeTime,U16 FontColor,U16 BLACKCOLOR)
{
	INT16U i,j,k,Xtemp,Ytemp,XLtemp,YLtemp,Xtemp1,Ytemp1;
	INT8U  temp,c,X_Length,Y_Length,flag;

	Xtemp = x;
	Ytemp = y;
	Xtemp1 = x;
	Ytemp1 = y;
	XLtemp = XLargeTime;
	YLtemp = YLargeTime;
	
	switch (HzType)
	{
		case HZK8x8: 	X_Length=8, Y_Length=8;  break;
		case HZK16x16: 	X_Length=16,Y_Length=16; break;
		case HZK24x24: 	X_Length=24,Y_Length=24; break;
		default:	    X_Length=24,Y_Length=24;
				
	}
	if (HzType == HZK24x24 )
	{
		for (j=0; j<X_Length; j++)
		{
			for (i=0; i<(Y_Length/8); i++)
			{				
				temp = *DISP++;
				for (k=0; k<8; k++)
				{
					flag=0;
					if ((temp << k) & 0x80)
					{
						c = FontColor;
					}
					else
					{
						flag=1;
					}

					while (YLtemp-- != 0) 
					{				
						while (XLtemp-- != 0)
						{
							if(flag!=1)
								LCD_PutPixel(Xtemp,Ytemp,c);
							Xtemp++;
						}
						Ytemp++;
						Xtemp = Xtemp1;
						XLtemp = XLargeTime;
					}
					Xtemp = Xtemp1;
					Ytemp1 = Ytemp1+YLargeTime;
					Ytemp = Ytemp1;
					YLtemp = YLargeTime;

				}
			}
			Xtemp1 = Xtemp1+XLargeTime;
			Xtemp = Xtemp1;
			Ytemp = y;
			Ytemp1 = y;
		}
	}
	else
	{
		for (j=0; j<Y_Length; j++)
		{
			for (i=0; i<(X_Length/8); i++)
			{
				temp = *DISP++;					
				for (k=0; k<8; k++)				
				{
					flag=0;
					if ((temp << k) & 0x80)
					{
						c = FontColor;
					}
					else
					{
						flag=1;
					}

					while (YLtemp-- != 0) 
					{				
						while (XLtemp-- != 0)
						{
							if(flag!=1)
								LCD_PutPixel(Xtemp,Ytemp,c);
							Xtemp++;
						}
						Ytemp++;
						Xtemp = Xtemp1;
						XLtemp = XLargeTime;
					}
						Ytemp = Ytemp1;
					Xtemp1 = Xtemp1+XLargeTime;
					Xtemp = Xtemp1;
					YLtemp = YLargeTime;

				}
			
			}
			Ytemp1 = Ytemp1+YLargeTime;
			Ytemp = Ytemp1;
			Xtemp = x;
			Xtemp1 = x;
		}

	}
}


///////////////////////////////////////////////////////////

void Lcd_DispHZ_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount, U16 ZiColor, U16 BackColor,INT8U ZiSize,
						  INT8U HAlign, INT8U VAlign,INT8U FlagUnderLine,INT16U Width,
						  char *ptr)
{
	INT8U HZWidth,HZHeight,LCount,LineNum;
	INT16U XStart,YStart,Margin;
	INT16U Offset = 0;

	LeftX = LeftX+Width;
	LeftY = LeftY+Width;
	RightX = RightX-Width;
	RightY = RightY-Width;

	switch(ZiSize)
	{
		case 0: HZWidth = 16;
				HZHeight = 16;
				break;

		case 1: HZWidth = 16;
				HZHeight = 32;
				break;

		case 2:	HZWidth = 24;
				HZHeight = 24;
				break;

		case 3: HZWidth = 24;
				HZHeight = 48;
				break;

		case 4: HZWidth = 32;
				HZHeight = 32;
				break;

		case 5: HZWidth = 32;
				HZHeight = 64;
				break;

		case 6: HZWidth = 48;
				HZHeight = 48;
				break;

		case 7: HZWidth = 48;
				HZHeight = 96;
				break;

		case 8: HZWidth = 64;
				HZHeight = 64;
				break;

		case 9: HZWidth = 96;
				HZHeight = 96;
				break;

		default:HZWidth = 24;
				HZHeight = 24;
				break;
	}

	if(FlagUnderLine == 1)
	{
		HZHeight = HZHeight+3;
	}

	LCount = (RightX - LeftX)/HZWidth;
	Margin = RightX - LeftX - LCount*HZWidth;
	if (ZiCount%LCount == 0)
		LineNum = ZiCount/LCount;
	else
		LineNum = ZiCount/LCount+1;

	switch (VAlign)
	{
		case 1: YStart = LeftY;
				break;

		case 0: YStart = (RightY-LeftY-LineNum*HZHeight)/2+LeftY;
				break;

		case 2: YStart = RightY-LineNum*HZHeight;
				break;
		
		default: YStart = (RightY-LeftY-LineNum*HZHeight)/2+LeftY;
				break;
	}

	while(ZiCount/LCount != 0)
	{

		switch (HAlign)
		{
			case 1: XStart = LeftX;
					break;

			case 0: XStart = LeftX+Margin/2;
					break;

			case 2: XStart = LeftX+Margin;
					break;
		
			default: XStart = LeftX+Margin/2;
				   	 break;
		}
		Lcd_Display_String(XStart,YStart,LCount,ZiColor,BackColor,ZiSize,(ptr+Offset),FlagUnderLine);
		Offset = Offset + LCount*2;
		YStart = YStart + HZHeight;
		ZiCount = ZiCount - LCount;
	}

	if (ZiCount != 0)
	{
		Margin = RightX - LeftX - ZiCount*HZWidth;

		switch (HAlign)
		{
			case 1: XStart = LeftX;
					break;

			case 0: XStart = LeftX+Margin/2;
					break;

			case 2: XStart = LeftX+Margin;
					break;
		
			default: XStart = LeftX+Margin/2;
				   	 break;
		}
		
		Lcd_Display_String(XStart,YStart,ZiCount,ZiColor,BackColor,ZiSize,(ptr+Offset),FlagUnderLine);
	}
}


/////////////////////////////////////////////////////////////////////////////

void Lcd_DispHZ_Asc_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount, INT16U ZiColor, INT16U BackColor,INT8U ZiSize,
						  INT8U HAlign, INT8U VAlign,INT8U FlagUnderLine,INT16U Width,
						  char *ptr)
{
	INT8U HZWidth,HZHeight,LineNum,AscWidth,temp8,OffsetStep,tempLine;
	INT16U YStart,Margin;
	INT32U Offset = 0;
	INT16U ArrayX[30][2],X_MM,Y_MM;
	INT16U RegionWidth,RegionHeight,tempwidth,LineWidth;
	INT8U HzType,XLargeTime,YLargeTime;
	INT8U QuMa,WeiMa;
	INT32U ZMStartAddr;

				
	LeftX = LeftX+Width;
	LeftY = LeftY+Width;
	RightX = RightX-Width;
	RightY = RightY-Width;
	RegionWidth=RightX-LeftX;
	RegionHeight=RightY-LeftY;
	LineNum=0;

	if(ZiCount==0)
		return;

	switch(ZiSize)
	{
		case 0: HZWidth = 16;
				AscWidth=8;
				X_MM=1;
				Y_MM=1;
				HZHeight = 16;
				break;

		case 1: HZWidth = 16;
				AscWidth=8;
				X_MM=1;
				Y_MM=2;
				HZHeight = 32;
				break;

		case 2:	HZWidth = 24;
				AscWidth=12;
				X_MM=1;
				Y_MM=1;
				HZHeight = 24;
				break;

		case 3: HZWidth = 24;
				AscWidth=12;
				X_MM=1;
				Y_MM=3;
				HZHeight = 48;
				break;

		case 4: HZWidth = 32;
				AscWidth=16;
				X_MM=2;
				Y_MM=2;
				HZHeight = 32;
				break;

		case 5: HZWidth = 32;
				AscWidth=16;
				X_MM=2;
				Y_MM=4;
				HZHeight = 64;
				break;

		case 6: HZWidth = 48;
				AscWidth=24;
				X_MM=3;
				Y_MM=3;
				HZHeight = 48;
				break;

		case 7: HZWidth = 48;
				AscWidth=24;
				X_MM=3;
				Y_MM=6;
				HZHeight = 96;
				break;

		case 8: HZWidth = 64;
				AscWidth=32;
				X_MM=4;
				Y_MM=4;
				HZHeight = 64;
				break;

		case 9: HZWidth = 96;
				AscWidth=48;
				X_MM=6;
				Y_MM=6;
				HZHeight = 96;
				break;

		default:HZWidth = 24;
				AscWidth=12;
				X_MM=1;
				Y_MM=1;
				HZHeight = 24;
				break;
	}

	if(FlagUnderLine == 1)
	{
		HZHeight = HZHeight+3;
	}

	while((LineNum+1)*HZHeight<=RegionHeight && Offset<ZiCount)
	{
		tempwidth=0;
		LineWidth=0;
		temp8=*(ptr+Offset);
		if((temp8 & 0x80)!=0x80)              //asc
		{
			tempwidth=AscWidth;
			OffsetStep=1;
		}
		else                                //hz
		{
			tempwidth=HZWidth;
			OffsetStep=2;
		}

		while(((LineWidth+tempwidth)<=RegionWidth ) && Offset<ZiCount )
		{
			Offset +=OffsetStep;
			LineWidth+=tempwidth;
			temp8=*(ptr+Offset);

			if((temp8 & 0x80)!=0x80)              //asc
			{
				tempwidth=AscWidth;
				OffsetStep=1;
			}
			else                                //hz
			{
				tempwidth=HZWidth;
				OffsetStep=2;
			}

		}
		
		Margin=RegionWidth-LineWidth;
		switch (HAlign)
		{
			case 1: ArrayX[LineNum][0] = LeftX;
					ArrayX[LineNum][1] = Offset;
					break;

			case 0: ArrayX[LineNum][0] = LeftX+Margin/2;
					ArrayX[LineNum][1] = Offset;
					break;

			case 2: ArrayX[LineNum][0] = LeftX+Margin;
					ArrayX[LineNum][1] = Offset;
					break;
		
			default: ArrayX[LineNum][0] = LeftX+Margin/2;
					 ArrayX[LineNum][1] = Offset;
				   	 break;
		}

		LineNum++;

	}
	
	Margin=RegionHeight-LineNum*HZHeight;
	switch (VAlign)
	{
		case 1: YStart = LeftY;
				break;

		case 0: YStart = (Margin)/2+LeftY;
				break;

		case 2: YStart = LeftY+Margin;
				break;
		
		default: YStart = (Margin)/2+LeftY;
				break;
	}



	switch(ZiSize)
	{
		case 0: HzType = HZK16x16;
				XLargeTime = 1;
				YLargeTime = 1;
			
				break;
		case 1: HzType = HZK16x16;
				XLargeTime = 1;
				YLargeTime = 2;
			
				break;
		case 2: HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 1;
			
				break;
		case 3: HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 2;
			
				break;
		case 4: HzType = HZK16x16;
				XLargeTime = 2;
				YLargeTime = 2;
				
				break;
		case 5: HzType = HZK16x16;
				XLargeTime = 2;
				YLargeTime = 4;
				
				break;
		case 6: HzType = HZK24x24;
				XLargeTime = 2;
				YLargeTime = 2;
			
				break;
		case 7: HzType = HZK24x24;
				XLargeTime = 2;
				YLargeTime = 4;
			
				break;
		case 8: HzType = HZK16x16;
				XLargeTime = 4;
				YLargeTime = 4;
			
				break;
		case 9: HzType = HZK24x24;
				XLargeTime = 4;
				YLargeTime = 4;
			
				break;
		default:HzType = HZK24x24;
				XLargeTime = 1;
				YLargeTime = 1;
			
				break;
	}



	Offset=0;
	tempLine=0;
	while(tempLine<LineNum)
	{
		LeftX=ArrayX[tempLine][0];

		while(Offset<ArrayX[tempLine][1])
		{
			temp8=*(ptr+Offset);
			if((temp8 & 0x80)!=0x80)              //asc
			{
				
				Lcd_Disp_Asc(temp8,LeftX, YStart, X_MM,Y_MM,ZiColor,BackColor);
				LeftX+=AscWidth;
				Offset+=1;
			}
			else                                //hz
			{
				QuMa = *(ptr+Offset);
				WeiMa = *(ptr+Offset+1);	
				ZMStartAddr = Get_StartAddr_Of_HanZi(QuMa, WeiMa, HzType);
				Lcd_Disphz(HzType, (char*)ZMStartAddr, LeftX,  YStart, XLargeTime,YLargeTime,ZiColor,BackColor);
				LeftX+=HZWidth;
				Offset+=2;
			}
		}
		YStart+=HZHeight;
		tempLine++;
	}


}



void Lcd_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,U16 FaceColor, 
	INT16 Width,INT8U DispButtonType)
{
	INT16 i,temp;
	//INT8U UpColor,DownColor;
	INT16U UpColor,DownColor;
	if ((Width >= fabs(X_Right-X_Left)) || (Width >= fabs(Y_Right-Y_Left)))
		Width = 0;

	if (X_Right<X_Left)
	{
		temp = X_Right;
	 	X_Right = X_Left;
	 	X_Left = temp;
	}
	if (Y_Right<Y_Left)
	{
		temp = Y_Right;
	 	Y_Right = Y_Left;
	 	Y_Left = temp;
	}

	if (DispButtonType == INSERT)
	{
		UpColor = 0;		// 0
		DownColor = 19;
		//UpColor = BLACK;
		//DownColor = GREEN_BLUE;
	}
	else
	{
		UpColor = 19;
		DownColor = 0;
		//UpColor = GREEN_BLUE;
		//DownColor = BLACK;
	}

	
	for (i=0;i<Width;i++)
	{
		Lcd_Draw_HLine(X_Left+i, X_Right-i, Y_Left+i, UpColor,1);
	}

	for (i=0;i<Width;i++)
	{
		Lcd_Draw_VLine(Y_Left+i, Y_Right-i, X_Left+i, UpColor,1);
		//Lcd_Draw_VLine(Y_Left+i, Y_Right-i, X_Left+i, 17, 1);
	}

	for (i=0;i<Width;i++)
	{
		Lcd_Draw_HLine(X_Left+i, X_Right-i, Y_Right-i, 	DownColor,1);
	}

	for (i=0;i<Width;i++)
	{
		Lcd_Draw_VLine(Y_Left+i, Y_Right-i, X_Right-i, 	DownColor,1);
	}

	Lcd_Fill_Box (X_Left+Width,Y_Left+Width, X_Right-Width,Y_Right-Width, FaceColor);
}






void Lcd_Jincheng_Tu(INT16U StartNum,INT16U EndNum,INT16U CurrentNum,
	INT16U FrontColor,INT16U BackColor,
	INT16U XLeft,INT16U YLeft,INT16U Lenght,INT16U Width)
{
	INT16U CurrentLength;
	INT16U XRight,YRight;

	XRight = XLeft + Lenght;
	YRight = YLeft + Width;
	
	Lcd_Button(XLeft, YLeft, XRight, YRight,BackColor,3,INSERT);
	CurrentLength = (CurrentNum-StartNum)*(Lenght-6)/(EndNum-StartNum);

	XRight = XLeft +3 + CurrentLength;
	YRight = YLeft + Width;
	Lcd_Fill_Box((XLeft+3), (YLeft+3), XRight, (YRight-3),FrontColor);	


}








//***************************************/*****************************************************************
//** 函数名:void Lcd_ellipse(INT16U Left,INT16U Top,INT16U Right,INT16U Bottom,INT8U Width,INT8U Color)
//** 输 入: INT16U Left,INT16U Top,INT16U Right,INT16U Bottom 规定椭圆矩形区域左上点和右下点的坐标
//**        INT8U Color:线条颜色
//**    	INT16U Width:线条宽度
//** 输 出: Null
//** 功能描述:以指定的颜色和宽度绘制椭圆
//** 全局变量:Null
//*****************************************************************
void Lcd_Fill_Ellipse(INT16U Left,INT16U Top,INT16U Right,INT16U Bottom,INT16U Color)
{
	INT16U x1,y1,CenterX,CenterY,temp,a,b;

	if(Left>Right)
	{
		temp = Left;
		Left = Right;
		Right = temp;
	}
	if(Top>Bottom)
	{
		temp = Top;
		Top = Bottom;
		Bottom = temp;
	}

	CenterX = Left + (Right-Left)/2;
	CenterY = Top + (Bottom-Top)/2;
	a=(Right-Left)/2;
	b=(Bottom-Top)/2;



	for(y1=0;y1<b;y1++)
	{
		x1 = sqrt(1.0*(Right-Left)*(Right-Left)/4 - (1.0*(Right-Left)/(Bottom-Top))*(1.0*(Right-Left)/(Bottom-Top))*y1*y1);
		Lcd_Draw_HLine (CenterX-x1, CenterX+x1, CenterY+y1, Color,1);
		Lcd_Draw_HLine (CenterX-x1, CenterX+x1, CenterY-y1, Color,1);
	}


}



void Lcd_Ellipse_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,
	INT16U FaceColor, INT16 Width,INT8U DispButtonType)
{
	if(DispButtonType==RAISE)
	{
		Lcd_Fill_Ellipse(X_Left,Y_Left, X_Right,Y_Right,0);
	}
	else
	{
		Lcd_Fill_Ellipse(X_Left,Y_Left, X_Right,Y_Right,19);

	}
	Lcd_Fill_Ellipse(X_Left+Width,Y_Left+Width, X_Right-Width,Y_Right-Width,FaceColor);
}





void Lcd_Arc(INT16U Centre_x, INT16U Centre_y, INT16U  Radius,double StartDegree,
	double EndDegree,U8 Width, INT16U Color)
{
	double degree;
	int X,Y;
	int Radius1=Radius;
	while(Width--)
	{
		for(degree=StartDegree;degree<=EndDegree;degree=degree+PI/360)
		{
			X=Radius1*sin(degree);
			Y=Radius1*cos(degree);
			LCD_PutPixel(Centre_x+X,Centre_y-Y,Color);
		}
		Radius1--;
	}
}


void Lcd_Circle(INT16U centre_x, INT16U centre_y, INT16U  radius, INT8U width, INT8U color)
{
	INT16U temp_centre_x,temp_centre_y,temp_radius;
	temp_radius = radius;
	
	while (width-- != 0)
	{
		for (temp_centre_x=0; temp_centre_x<=temp_radius; temp_centre_x++)
		{
			temp_centre_y = sqrt(temp_radius*temp_radius - temp_centre_x*temp_centre_x);
			LCD_PutPixel(centre_x+temp_centre_x, centre_y+temp_centre_y,color);
			LCD_PutPixel(centre_x+temp_centre_x, centre_y-temp_centre_y,color);
			LCD_PutPixel(centre_x-temp_centre_x, centre_y+temp_centre_y,color);
			LCD_PutPixel(centre_x-temp_centre_x, centre_y-temp_centre_y,color);
		}
	
		for (temp_centre_y=0; temp_centre_y<=temp_radius; temp_centre_y++)
		{
			temp_centre_x = sqrt(temp_radius*temp_radius - temp_centre_y*temp_centre_y);
			LCD_PutPixel(centre_x+temp_centre_x, centre_y+temp_centre_y,color);
			LCD_PutPixel(centre_x+temp_centre_x, centre_y-temp_centre_y,color);
			LCD_PutPixel(centre_x-temp_centre_x, centre_y+temp_centre_y,color);
			LCD_PutPixel(centre_x-temp_centre_x, centre_y-temp_centre_y,color);
		}
		temp_radius--;
	}
}





void Lcd_Fill_Circle(INT16U Centre_x, INT16U Centre_y, INT16U  Radius, INT16U Color)
{
	INT16U x,y;

	for(y=0; y<=Radius; y++)
	{
		x=sqrt(Radius*Radius - y*y);
		Lcd_Draw_HLine (Centre_x-x, Centre_x+x, Centre_y+y, Color,1);
		Lcd_Draw_HLine (Centre_x-x, Centre_x+x, Centre_y-y, Color,1);
	}
}


void Lcd_Circle_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,
	INT16U FaceColor, INT16 Width,INT8U DispButtonType)
{
	INT16U Centre_x,Centre_y,Radius;
	
	Centre_x=(X_Right-X_Left)/2+X_Left;
	Centre_y=(Y_Right-Y_Left)/2+Y_Left;
	Radius=(X_Right-X_Left)/2;
	if(DispButtonType==RAISE)
	{
		Lcd_Fill_Circle(Centre_x, Centre_y, Radius, 15);
	}
	else
	{
		Lcd_Fill_Circle(Centre_x, Centre_y, Radius, 0);
	}
	Radius=Radius-Width;
	Lcd_Fill_Circle(Centre_x, Centre_y, Radius, FaceColor);

	
}






//***************************************/*****************************************************************
//** 函数名:void Lcd_Draw_Line(INT16U usX0,INT16U usY0,INT16U usX1,INT16U usY1,INT8U ucColor, INT16U usWidth)
//** 输 入: INT16U usX0,usX1,usY0,usY1线条端点坐标
//**       INT8U ucColor:线条颜色
//**	INT16U usWidth:线条宽度
//** 输 出: Null
//** 功能描述:以指定的颜色颜色绘制线条
//** 全局变量:Null
//*****************************************************************
void Lcd_Draw_XieLine (INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1, 
	INT16U ucColor, INT16U usWidth)
{
	INT16 usDx;
	INT16 usDy;
	INT16 y_sign;
	INT16 x_sign;
	INT16 decision;
	INT16 wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if (usY0 == usY1)
	{
		Lcd_Draw_HLine (usX0, usX1, usY0,ucColor,  usWidth);
		return;
	}
	if (usX0 == usX1)
	{
		Lcd_Draw_VLine (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if (((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))))
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if (usDx >= usDy)
    {
        for (wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy)
        {
            if (decision >= usDx)
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for (wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++)
            {

				LCD_PutPixel(wCurx, wpy, ucColor);
            }

            for (wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++)
            {
				LCD_PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for (wCurx = usX0, wCury = usY0, wNextx = usX1,
                wNexty = usY1, decision = (usDy >> 1);
            wCury <= wNexty; wCury++, wNexty--, decision += usDx)
        {
            if (decision >= usDy)
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for (wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++)
            {
                 LCD_PutPixel(wpx, wCury, ucColor); 
            }

            for (wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++)
            {
				LCD_PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}



///////////////////////////////////////////////////

void Lcd_Disp_Graph(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U GraphType)
{
	INT16 X,Y,i,j,m;
	INT16 xx=X_Right-X_Left;
	INT16 yy=Y_Right-Y_Left;

	switch(GraphType)
	{
	case 0:	for(Y=Y_Left+1;Y<Y_Right;Y=Y+2)	
			Lcd_Draw_HLine (X_Left, X_Right, Y, Color, 1);
		break;
	case 1:
		for(X=X_Left+1;X<X_Right;X=X+2)	
	    		Lcd_Draw_VLine (Y_Left, Y_Right, X, Color, 1);
		break;
	case 2:
		if(xx<=yy)
		{
			for (i=X_Left,j=Y_Right;i<X_Right;i=i+4,j=j-4)
			{
				Lcd_Draw_XieLine(i,Y_Right,X_Left,j,Color,1);	
			}
			m=i-X_Right;
			for(i=Y_Right-m;j>Y_Left;j=j-4,i=i-4)
			{
				Lcd_Draw_XieLine(X_Left,j,X_Right,i,Color,1);	
			}
			m=Y_Left-j;
			for(j=X_Left+m;j<=X_Right;i=i-4,j=j+4)
			{
				Lcd_Draw_XieLine(j,Y_Left,X_Right,i,Color,1);	
			}
		}

		else
		{
			for(i=X_Left,j=Y_Right;j>Y_Left;i=i+4,j=j-4)
			{	
				Lcd_Draw_XieLine(i,Y_Right,X_Left,j,Color,1);	
			}
			m=Y_Left-j;
			for(j=X_Left+m;i<X_Right;i=i+4,j=j+4)
			{	
				Lcd_Draw_XieLine(j,Y_Left,i,Y_Right,Color,1);	
			}
			m=i-X_Right;
			for(i=Y_Right-m;i>=Y_Left;i=i-4,j=j+4)
			{
				Lcd_Draw_XieLine(j,Y_Left,X_Right,i,Color,1);							
			}
		}
		break;
	
	case 3:		
		if(xx<=yy)
		{
			for (i=X_Left,j=Y_Left;i<X_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,X_Left,j,Color,1);
			}
			m=i-X_Right;
			for(i=Y_Left+m;j<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(X_Left,j,X_Right,i,Color,1);
			}
			m=j-Y_Right;
			for(j=X_Left+m;j<X_Right;j=j+4,i=i+4)
			{
				Lcd_Draw_XieLine(j,Y_Right,X_Right,i,Color,1);
			}
		}
		else
		{
			for(i=X_Left,j=Y_Left;j<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,X_Left,j, Color, 1);
			}
			m=j-Y_Right;
			for(j=X_Left+m;i<X_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,j,Y_Right,Color,1);
			}
			m=i-X_Right;
			for(i=Y_Left+m;i<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(X_Right,i,j,Y_Right,Color,1);
			}
		}
		break;
	
	case 4:
		for(Y=Y_Left+1;Y<Y_Right;Y=Y+1)	
			Lcd_Draw_HLine (X_Left, X_Right, Y, Color, 1);
		for(X=X_Left+1;X<X_Right;X=X+1)	
	    		Lcd_Draw_VLine (Y_Left, Y_Right, X, Color, 1);
		break;
	
	case 5:
		if(xx<=yy)
		{
			for (i=X_Left,j=Y_Right;i<X_Right;i=i+4,j=j-4)
			{
				Lcd_Draw_XieLine(i,Y_Right,X_Left,j,Color,1);	
			}
			m=i-X_Right;
			for(i=Y_Right-m;j>Y_Left;j=j-4,i=i-4)
			{
				Lcd_Draw_XieLine(X_Left,j,X_Right,i,Color,1);	
			}
			m=Y_Left-j;
			for(j=X_Left+m;j<=X_Right;i=i-4,j=j+4)
			{
				Lcd_Draw_XieLine(j,Y_Left,X_Right,i,Color,1);	
			}


			for (i=X_Left,j=Y_Left;i<X_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,X_Left,j,Color,1);
			}
			m=i-X_Right;
			for(i=Y_Left+m;j<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(X_Left,j,X_Right,i,Color,1);
			}
			m=j-Y_Right;
			for(j=X_Left+m;j<X_Right;j=j+4,i=i+4)
			{
				Lcd_Draw_XieLine(j,Y_Right,X_Right,i,Color,1);
			}
	
		}

		else
		{
			for(i=X_Left,j=Y_Right;j>Y_Left;i=i+4,j=j-4)
			{	
				Lcd_Draw_XieLine(i,Y_Right,X_Left,j,Color,1);	
			}
			m=Y_Left-j;
			for(j=X_Left+m;i<X_Right;i=i+4,j=j+4)
			{	
				Lcd_Draw_XieLine(j,Y_Left,i,Y_Right,Color,1);	
			}
			m=i-X_Right;
			for(i=Y_Right-m;i>=Y_Left;i=i-4,j=j+4)
			{
				Lcd_Draw_XieLine(j,Y_Left,X_Right,i,Color,1);							
			}
			

			for(i=X_Left,j=Y_Left;j<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,X_Left,j, Color, 1);
			}
			m=j-Y_Right;
			for(j=X_Left+m;i<X_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(i,Y_Left,j,Y_Right,Color,1);
			}
			m=i-X_Right;
			for(i=Y_Left+m;i<Y_Right;i=i+4,j=j+4)
			{
				Lcd_Draw_XieLine(X_Right,i,j,Y_Right,Color,1);
			}

		}
		
		break;
	

	case 6:
		Lcd_Fill_Box (X_Left,Y_Left,X_Right,Y_Right,Color);
		break;
	
	}
}

void Lcd_Disp_CurrentValue(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y)
{	
	INT16U m_Point1x,m_Point1y,m_Point2x,m_Point2y;

   	if (Direction==0)
   	{		   
		m_Point1x=X_Left+1;
		m_Point1y=Y_Right-y;
		m_Point2x=X_Right-1;
		m_Point2y=Y_Right;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
	
   	}
   	else if (Direction==1)
   	{ 	
		m_Point1x=X_Left+1;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Right-1;
		m_Point2y=Y_Left+y;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);	
			
   	}

	else if (Direction==2)
  	 { 
		m_Point1x=X_Right-x;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Right-1;
		m_Point2y=Y_Right-1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
	
  	 }

	else if (Direction==3)
  	 { 	
		m_Point1x=X_Left+1;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Left+1+x;
		m_Point2y=Y_Right-1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);			
 	  }

}



void Lcd_Disp_TargetValue(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16U x1,INT16U y1)
{
	INT16U m_Point1x,m_Point1y,m_Point2x,m_Point2y;

  	if (Direction==0)
   	{		   
		m_Point1x=X_Left+1;
		m_Point1y=Y_Right-y1-1;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Right-y1+1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);				
   	}
  	 else if (Direction==1)
   	{ 	
		m_Point1x=X_Left+1;
		m_Point1y=Y_Left+y1-1;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Left+y1+1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);		
  	 }

	else if (Direction==2)
  	 { 
		m_Point1x=X_Right-x1-1;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Right-x1+1;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);	
	}
	else if (Direction==3)
   	{ 	
		m_Point1x=X_Left-1+x1;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Left+x1+1;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);			
   	}
}





void Lcd_Disp_LoWRange(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y,INT16 x2,INT16 y2)
{
	INT16U m_Point1x,m_Point1y,m_Point2x,m_Point2y;
	if (Direction==0)
   	{		   
		m_Point1x=X_Left+1;
		m_Point1y=Y_Right-y+1;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Right-y+y2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}
   	else if (Direction==1)
   	{ 	
		m_Point1x=X_Left+1;
		m_Point1y=Y_Left+y-y2;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Left+y-1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}

	else if (Direction==2)
  	 { 
		m_Point1x=X_Right-x+1;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Right-x+x2;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}

	else if (Direction==3)
   	{ 	
		m_Point1x=X_Left+x-x2;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Left+x-1;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}
}




void Lcd_Disp_HighRange(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y,INT16 x3,INT16 y3)
{
	INT16U m_Point1x,m_Point1y,m_Point2x,m_Point2y;
	if (Direction==0)
   	{		   
		m_Point1x=X_Left+1;
		m_Point1y=Y_Right-y-y3;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Right-y-1;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}
	else if (Direction==1)
   	{ 	
		m_Point1x=X_Left+1;
		m_Point1y=Y_Left+y+1;
		m_Point2x=X_Right-2;
		m_Point2y=Y_Left+y+y3;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);   			
   	}

	else if (Direction==2)
   	{ 
		m_Point1x=X_Right-x-x3;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Right-x-1;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);

   	}

	else if (Direction==3)
   	{ 	
		m_Point1x=X_Left+1+x;
		m_Point1y=Y_Left+1;
		m_Point2x=X_Left+x+x3;
		m_Point2y=Y_Right-2;
		Lcd_Disp_Graph(m_Point1x,m_Point1y,m_Point2x,m_Point2y,Color,GraphType);
   	}

}




INT16U NumberToColor(INT16U n)
{
	switch(n)
	{
		case  0:			return	0x0000;//		break;
		case  1:			return	0xA514;//		break;
		case  2:			return	0x0010;//		break;
		case  3:			return	0xF800;//		break;
		case  4:			return	0x8010;//		break;
		case  5:			return	0x0410;//		break;
		case  6:			return 	0xF81F;//		break;
		case  7:			return	0x07E0;//		break;
		case  8:			return	0xFFE0;//		break;
		case  9:			return	0x8410;//		break;
		case  10:		return	0xA65E;//		break;
		case  11:		return	0xC618;//		break;
		case  12:		return	0x001F;//		break;
		case  13:		return	0x07FF;//		break;
		case  14:		return	0x8400;//		break;
		case  15:		return	0x8000;//		break;
		case  16:		return	0xC6F8;//		break;
		case  17:		return	0x0400;//		break;
		case  18:		return	0xFFDE;//		break;
		case  19:		return 	0xFFFF;//		break;
		
		default: return 0xF800;
	}
}

/*
U16 C24ToC16(U8 R, U8 G, U8 B)
{	
	U8 colorR,colorG,colorB;
	U16 color16;
	colorR = R>>3;	// 5
	colorG = G>>2;	// 6
	colorB = B>>3;	// 5
	color16 = (colorR<<11) | (colorG<<5) |(colorB);
	return color16;
}

	U16 C16[20];
	C16[0] = C24ToC16(0,0,0);
	C16[1] = C24ToC16(160,160,164);
	C16[2] = C24ToC16(0,0,128);
	C16[3] = C24ToC16(255,0,0);
	C16[4] = C24ToC16(128,0,128);
	C16[5] = C24ToC16(0,128,128);
	C16[6] = C24ToC16(255,0,255);
	C16[7] = C24ToC16(0,255,0);
	C16[8] = C24ToC16(255,255,0);
	C16[9] = C24ToC16(128,128,128);
	C16[10] = C24ToC16(166,202,240);
	C16[11] = C24ToC16(192,192,192);
	C16[12] = C24ToC16(0,0,255);
	C16[13] = C24ToC16(0,255,255);
	C16[14] = C24ToC16(128,128,0);
	C16[15] = C24ToC16(128,0,0);
	C16[16] = C24ToC16(192,220,192);
	C16[17] = C24ToC16(0,128,0);
	C16[18] = C24ToC16(255,251,240);
	C16[19] = C24ToC16(255,255,255);
*/




