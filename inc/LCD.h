/*********************************************************************
;* Project Name : s3c2450x
;*
;* Copyright 2006 by Samsung Electronics, Inc.
;* All rights reserved.
;*
;* Project Description :
;* This software is only for verifying functions of the s3c2450x
;* Anybody can use this code without our permission.
;**********************************************************************/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

//#include "bsp_cfg.h"

#define LCD_DELAY_1MS	180000	//180000	//on the basis of 540MHz
#define GUISWAP(a, b) {a^=b; b^=a; a^=b;}



#define SHARP		1
#define ZHONGFU		2

//#define LCD_Type	SHARP
#define LCD_Type	ZHONGFU

#if(LCD_Type == SHARP)
#define LTE480_VBPD		(32)	//13
#define LTE480_VFPD		(10)///(3)///(14)	///	// 5-1	
#define LTE480_VSPW		(2)///(6)///(2)		///	// 1-1

#define LTE480_HBPD		(31) //31
#define LTE480_HFPD		(16) //16
#define LTE480_HSPW		(31) //31

//#define LTE480_FRAME_RATE	(25)		// M
#define LTE480_FRAME_RATE	(30)		// M
#define CLKVAL_TFT_640480       (3)	//  
//#define CLKVAL_TFT_640480       (4)	//  

#elif(LCD_Type == ZHONGFU)
#define LTE480_VBPD		(29)	//29
#define LTE480_VFPD		(3)  // 3
#define LTE480_VSPW		(6) // 6

#define LTE480_HBPD		(53) //53
#define LTE480_HFPD		(35) //35
#define LTE480_HSPW	(73) //73

//#define LTE480_FRAME_RATE	(25)		// M
#define LTE480_FRAME_RATE	(30)		// M
//#define CLKVAL_TFT_640480       (9)	//  画面稍微有点抖动
#define CLKVAL_TFT_640480       (3)	//  6
// 使用!!! rVIDCON0 |= (9 <<VIDCON0_CLKVAL_F_SHIFT);	// 13.3M  显示正常(之前必须把示波器探针搭在Tclk 才能正常显示)
														// 画面稍微有点抖动
//rVIDCON0 |= (6 <<VIDCON0_CLKVAL_F_SHIFT); // 19M 显示正常
//rVIDCON0 |= (5 <<VIDCON0_CLKVAL_F_SHIFT); // 22.2M 可显示蓝色，显示按钮时变成黑屏同时闪烁，示波器搭在Tck后可正常显示，再更新数据后一直在黑屏闪烁
											// 喇叭响时屏幕闪烁
//rVIDCON0 |= (4 <<VIDCON0_CLKVAL_F_SHIFT); // 26.6M  写数据时闪烁,后正常
											// 喇叭响时屏幕闪烁
#endif


#define LTE480_HOZVAL	(640)///(640)	//// Horizontal pixel 800
#define LTE480_LINEVAL	(480)// Vertical pixel 480

#define LCD_WIDTH	640	//800
#define LCD_HEIGHT	480

//#endif

//* VIDCON0	
#define VIDCON0_S_RGB_IF					(0<<22)
#define VIDCON0_S_RGB_PAR					(0<<13)
#define VIDCON0_S_BGR_PAR					(1<<13)
#define VIDCON0_S_CLKVAL_F_AlWAYS_UPDATE	(0<<12)
#define VIDCON0_S_CLKVAL_F_SOF_UPDATE		(1<<12)
#define VIDCON0_S_VCLK_GATING_ON			(0<<5)
#define VIDCON0_S_VCLK_GATING_OFF			(1<<5)
#define VIDCON0_S_CLKDIR_DIRECT				(0<<4)
#define VIDCON0_S_CLKDIR_DIVIDED			(1<<4)
#define VIDCON0_S_CLKSEL_HCLK				(0<<2)
#define VIDCON0_S_CLKSEL_UPLL				(1<<2)
#define VIDCON0_S_ENVID_OFF					(0<<1)
#define VIDCON0_S_EVVID_ON					(1<<1)
#define VIDCON0_S_ENVID_F_OFF				(0<<0)
#define VIDCON0_S_ENVID_F_ON				(1<<0)
//bit shift
#define VIDCON0_CLKVAL_F_SHIFT				(6)

//* VIDCON1
#define VIDCON1_S_VCLK_FALL_EDGE_FETCH		(0<<7)
#define VIDCON1_S_VCLK_RISE_EDGE_FETCH		(1<<7)
#define VIDCON1_S_HSYNC_INVERTED			(1<<6)
#define VIDCON1_S_VSYNC_INVERTED			(1<<5)
#define VIDCON1_S_VDEN_INVERTED				(1<<4)


//* VIDTCON0,1
//bit shift
#define VIDTCON0_BPD_S				(16)
#define VIDTCON0_FPD_S				(8)
#define VIDTCON0_SPW_S				(0)

// VIDTCON2
//bit shift
#define VIDTCON2_LINEVAL_S			(11)
#define VIDTCON2_HOZVAL_S			(0)
/*
// WINCON0
//shift
#define WINCON0_INRGB_S				(13)
*/
// WINCON1to4
#define WINCONx_BIT_SWAP_ON			(1<<2)	//shift on basis of half-word swap
#define WINCONx_BYTE_SWAP_ON		(1<<1)	//shift on basis of half-word swap
#define WINCONx_HALFW_SWAP_ON		(1<<0)	//shift on basis of half-word swap
#define WINCONx_4WORD_BURST			(2)
#define WINCONx_8WORD_BURST			(1)
#define WINCONx_16WORD_BURST		(0)
#define WINCONx_PLANE_BLENDING		(0)
#define WINCONx_PIXEL_BLENDING		(1)
#define WINCONx_1BPP_PALLET			(0)
#define WINCONx_2BPP_PALLET			(1)
#define WINCONx_4BPP_PALLET			(2)
#define WINCONx_8BPP_PALLET			(3)
#define WINCONx_8BPP_NO_PALLET		(4)
#define WINCONx_16BPP_565			(5)
#define WINCONx_16BPP_A555			(6)
#define WINCONx_16BPP_1555			(7)
#define WINCONx_18BPP_666			(8)
#define WINCONx_18BPP_A665			(9)
#define WINCONx_19BPP_A666			(10)
#define WINCONx_24BPP_888			(11)
#define WINCONx_24BPP_A887			(12)
#define WINCONx_25BPP_A888			(13)
#define WINCONx_ALPHA_MODE_0		(0)
#define WINCONx_ALPHA_MODE_1		(1)


//bit shift
#define WINCON_SWAP_S				(16)
#define WINCON_BURSTLEN_S			(9)
#define WINCON_BLENDING_S			(6)
#define WINCON_BPP_S				(2)
#define WINCON_ALPHA_S				(1)

//* VIDWxADD2
//bit shift
#define VIDWxADD2_OFFSET_SIZE_S		(13)
#define VIDWxADD2_PAGE_WIDTH_S		(0)

//* VIDOSDxA,B,C
//bit shift
#define VIDOSDxAB_HORIZON_X_S		(11)
#define VIDOSDxAB_VERTICAL_Y_S		(0)
#define VIDOSDxC_ALPHA0_S		(12)

#define LCD_DEN		(1<<14)
#define LCD_DSERI	(1<<11)
#define LCD_DCLK	(1<<10)

#define LCD_DEN_BIT		14
#define LCD_DSERI_BIT	11
#define LCD_DCLK_BIT	10

#define LCD_nRESET		1



//#define LCD_RESET     (0)

//#if 1	//org.
#define LCD_DEN_Lo		(s2450IOP->GPLDAT &= ~LCD_DEN)
#define LCD_DEN_Hi		(s2450IOP->GPLDAT |=	LCD_DEN)
#define LCD_DCLK_Lo		(s2450IOP->GPLDAT &= ~LCD_DCLK)
#define LCD_DCLK_Hi		(s2450IOP->GPLDAT |=	LCD_DCLK)
#define LCD_DSERI_Lo	(s2450IOP->GPLDAT &= ~LCD_DSERI)
#define LCD_DSERI_Hi	(s2450IOP->GPLDAT |=	LCD_DSERI)

#define    LCD_SCR_XSIZE           (640)           // virtual screen  
#define    LCD_SCR_YSIZE           (480)

#define    LCD_SCR_XSIZE_TFT       (640)           // virtual screen  
#define    LCD_SCR_YSIZE_TFT       (480)

#define    LCD_XSIZE_TFT           (640)   
#define    LCD_YSIZE_TFT           (480)

#define    LCD_XSIZE_STN           (640)
#define    LCD_YSIZE_STN           (480)

#define    LCD_XSIZE_CSTN          (640)
#define    LCD_YSIZE_CSTN          (480)


//------------------------------------------------------------------------------
//  Define: LCD_ARRAY_SIZE_XXX
#define    LCD_ARRAY_SIZE_STN_1BIT     (LCD_SCR_XSIZE/8*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_STN_2BIT     (LCD_SCR_XSIZE/4*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_STN_4BIT     (LCD_SCR_XSIZE/2*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_CSTN_8BIT    (LCD_SCR_XSIZE/1*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_CSTN_12BIT   (LCD_SCR_XSIZE*2*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_TFT_8BIT     (LCD_SCR_XSIZE/1*LCD_SCR_YSIZE)
#define    LCD_ARRAY_SIZE_TFT_16BIT    (LCD_SCR_XSIZE*2*LCD_SCR_YSIZE)

//------------------------------------------------------------------------------
//  Define: LCD_HOZVAL_XXX
#define    LCD_HOZVAL_STN          (LCD_XSIZE_STN/4-1)
#define    LCD_HOZVAL_CSTN         (LCD_XSIZE_CSTN*3/8-1)
#define    LCD_HOZVAL_TFT          (LCD_XSIZE_TFT-1)

//------------------------------------------------------------------------------
//  Define: LCD_LINEVAL_XXX
#define    LCD_LINEVAL_STN         (LCD_YSIZE_STN-1)
#define    LCD_LINEVAL_CSTN        (LCD_YSIZE_CSTN-1)
#define    LCD_LINEVAL_TFT         (LCD_YSIZE_TFT-1)
#ifdef __cplusplus
}
#endif
#endif //#ifndef __DISPLAY_H__






void Lcd_Pwren_Init_On_Off( unsigned char m );
void Lcd_Port_Init( void );

void delayLoop(int count);
void Lcd_Init(void);


void Paint_CrossLine(int x,int y,int dd, U16 color);
void Glib_Line(int x1, int y1, int x2, int y2, U16 color);
void Glib_FilledRectangle(int x1, int y1, int x2, int y2, U16 color);
void Paint_Bmp(int x0, int y0, int h, int l, unsigned char bmp[]);
//void Test_Lcd(void);



/////////////////////////////////////////////////////////
void Lcd_Display_String(INT16U LeftX,INT16U LeftY,INT8U Count,U16 FontColor,U16 BackColor,
	INT8U Size,char *Ptr,INT8U FlagUnderLine);
INT32U Get_StartAddr_Of_HanZi(INT8U QuMa, INT8U WeiMa, INT8U HzType);
void Lcd_Draw_HLine (INT16 usX0, INT16 usX1, INT16 usY0, U16 ucColor,INT16U usWidth);
void Lcd_Draw_VLine (INT16 usY0, INT16 usY1, INT16 usX0, U16 ucColor, INT16U usWidth);

void Test_Lcd(void);
void Lcd_Clr_Test( U16 c);
void Lcd_Clr( U16 c);
void LCD_PutPixel(U32 x,U32 y,U16 c);
void LCD_PutPixel_Bmp(INT16U x, INT16U y,INT16U c);
void LCD_PutPixel_Bmp565(INT16U x, INT16U y,INT16U c);

void Lcd_Fill_Box (INT16U X_Left, INT16U Y_Left, INT16U X_Right, INT16U Y_Right,  U16 Color);
void Lcd_DispAsc_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount,U16 BackColor,U16 FontColor,INT8U ZiSize,
						  INT8U HAlign, INT16U Width,char *ptr);
void Lcd_Disp_AscStr(char *AscStr,INT8U Count, INT16U Left, INT16U Top, INT8U  X_MM,
	INT8U Y_MM,INT8U Offset,U16 FontColor,U16 BLACKCOLOR);
void Lcd_Disp_Asc(INT8U Asc, INT16U Left, INT16U Top, INT8U XLargeTime,INT8U YLargeTime,
	U16 FontColor,U16 BLACKCOLOR);
void Lcd_Disphz(INT8U HzType, char *DISP, INT16U x, INT16U y, INT8U XLargeTime,
				INT8U YLargeTime,U16 FontColor,U16 BLACKCOLOR);
void Lcd_DispHZ_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount, U16 ZiColor, U16 BackColor,INT8U ZiSize,
						  INT8U HAlign, INT8U VAlign,INT8U FlagUnderLine,INT16U Width,
						  char *ptr);
void Lcd_DispHZ_Asc_On_Button(INT16U LeftX, INT16U LeftY, INT16U RightX, INT16U RightY,
						  INT32U ZiCount, INT16U ZiColor, INT16U BackColor,INT8U ZiSize,
						  INT8U HAlign, INT8U VAlign,INT8U FlagUnderLine,INT16U Width,
						  char *ptr);

void Lcd_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,U16 FaceColor, 
	INT16 Width,INT8U DispButtonType);



void Lcd_Jincheng_Tu(INT16U StartNum,INT16U EndNum,INT16U CurrentNum,
	INT16U FrontColor,INT16U BackColor,
	INT16U XLeft,INT16U YLeft,INT16U Lenght,INT16U Width);


void Lcd_Fill_Ellipse(INT16U Left,INT16U Top,INT16U Right,INT16U Bottom,INT16U Color);
void Lcd_Ellipse_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,
	INT16U FaceColor, INT16 Width,INT8U DispButtonType);


void Lcd_Arc(INT16U Centre_x, INT16U Centre_y, INT16U  Radius,double StartDegree,
	double EndDegree,U8 Width, INT16U Color);
void Lcd_Circle(INT16U centre_x, INT16U centre_y, INT16U  radius, INT8U width, INT8U color);
void Lcd_Fill_Circle(INT16U Centre_x, INT16U Centre_y, INT16U  Radius, INT16U Color);
void Lcd_Circle_Button(INT16 X_Left, INT16 Y_Left, INT16 X_Right, INT16 Y_Right,
	INT16U FaceColor, INT16 Width,INT8U DispButtonType);
void Lcd_Draw_XieLine (INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1, 
	INT16U ucColor, INT16U usWidth);
void Lcd_Disp_Graph(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U GraphType);
void Lcd_Disp_CurrentValue(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y);
void Lcd_Disp_TargetValue(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16U x1,INT16U y1);
void Lcd_Disp_LoWRange(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y,INT16 x2,INT16 y2);
void Lcd_Disp_HighRange(INT16U X_Left,INT16U Y_Left,INT16U X_Right,INT16U Y_Right,
	INT16U Color,INT8U Direction,INT8U GraphType,INT16 x,INT16 y,INT16 x3,INT16 y3);




INT16U NumberToColor(INT16U n);






