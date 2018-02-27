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
#include "..\inc\communication.h"
#include "..\inc\function.h"
#include "..\inc\Rtc.h"
#include "..\inc\download.h"

#include "..\inc\encrypt.h"



extern INTERALMEMORY g_InteralMemory;

extern GLOBALVAR g_GlobalVar;

//extern  CurrentScrNum;	// g_InteralMemory.Word[PING_CTRL] = PreScrNum; 会导致数据溢出
//extern  PreScrNum;
extern INT16U CurrentScrNum;
extern INT16U PreScrNum;

extern flag;

extern INT16U g_StarScr;
extern INT16U g_PlcCtrlNum;
extern char g_PlcCtrStr[5];




void Inn_Store_PassFlag_OnOrrOff(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);
	EraseBlock(Block);

	if(1 == g_InteralMemory.KeepBit[6]) //启用
	{
		g_InteralMemory.KeepBit[0]=0xff;
		g_InteralMemory.KeepBit[1]=0xff;
		g_InteralMemory.KeepBit[2]=0xff;
		g_InteralMemory.KeepBit[3]=0xff;
	}
	else
	{
		g_InteralMemory.KeepBit[0]=1;
		g_InteralMemory.KeepBit[1]=1;
		g_InteralMemory.KeepBit[2]=1;
		g_InteralMemory.KeepBit[3]=1;
	}
	
	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];		//Bit 一个字节

	WritePage(Block,0,PageBuff);
}


void Inn_Store_Password_temp(void)
{
	int j;
	INT8U *PageBuff;
	int Block;	

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_Password);
	EraseBlock(Block);

//调入初始密码222222
	for(j=0;j<5;j++)
	{
		g_InteralMemory.KeepWord[300+j*2]=222222 & 0xffff;
		g_InteralMemory.KeepWord[300+j*2+1]=222222>>16;
	}

	g_InteralMemory.KeepWord[320]=222222 & 0xffff;
	g_InteralMemory.KeepWord[321]=222222>>16;

	
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[300];		//KeepWord两个字节

	WritePage(Block,0,PageBuff);
}




void Encrypt_Init()
{
//显示#
	g_InteralMemory.Word[0]=0xffff;
	g_InteralMemory.Word[1]=0xffff;
//密码输入次数
	g_InteralMemory.Word[400]=0;
	g_InteralMemory.Word[401]=0;
	g_InteralMemory.Word[402]=0;
	g_InteralMemory.Word[403]=0;
	g_InteralMemory.Word[404]=0;

	g_InteralMemory.Bit[202]=0;
	
	
	Inn_Load_Password();
	Inn_Load_PassFlag();


//	Inn_Store_PassFlag_Temp();			//!!!!!!!!!!!!!!!
//	Inn_Store_Password_temp();		//!!!!!!!!!!!!!!!


}



//////////////////////////////////
////读(一上电就读出来)
////////////////////////////////
void Inn_Load_Password(void)
{
	INT8U PageBuff[PAGE_SIZE];
	int Block,j;
	//INT16U temp;

	Block = Logical_To_Physical_Block(BLOCK_Password);

	ReadPage(Block,0,PageBuff);

	for(j=0;j<20;j++)  // P_KW300~P_KW339
	{
		g_InteralMemory.KeepWord[300+j*2]=(PageBuff[4*j+1]<<8)|PageBuff[4*j];
		g_InteralMemory.KeepWord[300+j*2+1]=(PageBuff[4*j+3]<<8)|PageBuff[4*j+2];
	}	
	
/////++++++++++++++++++++++++++++++++++++++++++
//如果32位全为f 则调入初始密码
	for(j=0;j<5;j++)
	{
		if((g_InteralMemory.KeepWord[300+j*2] & 0xffff)==0xffff 
			&&  (g_InteralMemory.KeepWord[300+j*2+1] & 0xffff)==0xffff)
		{
			g_InteralMemory.KeepWord[300+j*2]=222222 & 0xffff;
			g_InteralMemory.KeepWord[300+j*2+1]=222222>>16;
		}
	}
	for(j=0;j<3;j++)
	{
		if((g_InteralMemory.KeepWord[320+j*2] & 0xffff)==0xffff 
			&&  (g_InteralMemory.KeepWord[320+j*2+1] & 0xffff)==0xffff)
		{
			g_InteralMemory.KeepWord[320+j*2]=222222 & 0xffff;
			g_InteralMemory.KeepWord[320+j*2+1]=222222>>16;
		}
	}
	
/////++++++++++++++++++++++++++++++++++++++++++

}




void Inn_Load_PassFlag(void)
{
	INT8U PageBuff[PAGE_SIZE];
	int Block,j;
	//INT16U temp;

	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);

	ReadPage(Block,0,PageBuff);

	for(j=0;j<=8;j++)
	{
		g_InteralMemory.KeepBit[0+j]=PageBuff[j];
	}	

	if(0 != g_InteralMemory.KeepBit[7] && 1 != g_InteralMemory.KeepBit[7])
		g_InteralMemory.KeepBit[7] = 0; //初始化

	if(0 != g_InteralMemory.KeepBit[8] && 1 != g_InteralMemory.KeepBit[8])
		g_InteralMemory.KeepBit[8] = 0; //初始化
	
}


//////////////////////////////////
////写
////////////////////////////////
void Inn_Store_Password(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_Password);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[300];		//KeepWord两个字节

	WritePage(Block,0,PageBuff);
}


void Inn_Store_PassFlag(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];		//Bit 一个字节

	WritePage(Block,0,PageBuff);
}






//////////////////////////////////
////总操作
////////////////////////////////
void Encrypt_Operation()
{
	//Read_DateTime();
	//Write_DateTime();
	RTC_SetTime();  // 按钮控制
	RTC_ReadTime(); 


	TimeReach_Judge();
	TimeOUt_Screen();

	Time_Alarm();	

	encrypt();


	if(0 == g_InteralMemory.KeepBit[7]) //已停用
	{
		if(1 == g_InteralMemory.KeepBit[6]) //重新启用
		{
			Inn_Store_PassFlag_OnOrrOff();	
			g_InteralMemory.KeepBit[7] = 1;
		}
	}
	if(1 == g_InteralMemory.KeepBit[7]) //已启用
	{
		if(0 == g_InteralMemory.KeepBit[6]) //使停用
		{
			Inn_Store_PassFlag_OnOrrOff();	
			g_InteralMemory.KeepBit[7] = 0;
		}
	}
	
	
	//开始运行
	if(g_InteralMemory.Bit[201]==1)
	{
		g_InteralMemory.Bit[201]=0;
		g_GlobalVar.Stop_Communication=0;
		g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开机画面
	}
	

	//密码错误返回
	if(g_InteralMemory.Bit[204]==1)
	{
		g_InteralMemory.Bit[204] = 0;
		g_InteralMemory.Word[PING_CTRL] = PreScrNum;
		g_InteralMemory.Bit[205] = 0; //清除  密码错误画面标志
	}
	
	
}









//////////////////////////////////
////加密
////////////////////////////////
void encrypt()
{
////管理员密码


if(g_InteralMemory.Bit[603]==1)  //取消
	{
	
		g_InteralMemory.Bit[603]=0;
		RefreshButtonUp();		
		DisplaySystemScr(0);
		
	
	}
	

	if(g_InteralMemory.Bit[150]==1)
	{
		g_InteralMemory.Bit[150]=0;
		if (g_InteralMemory.KeepWord[300]==g_InteralMemory.Word[150] 
			&&  g_InteralMemory.KeepWord[301]==g_InteralMemory.Word[151])
		{
			g_InteralMemory.Word[PING_CTRL]=65;//跳体统设置
			g_InteralMemory.Word[150]=0;
			g_InteralMemory.Word[151]=0;

			g_InteralMemory.Word[400]=0;
		}
		else	
		{
			g_InteralMemory.Word[PING_CTRL]=75;//跳到报错
			g_InteralMemory.Word[400]++;
		}
//密码输入错误超过5次
		if(g_InteralMemory.Word[400]>=5)
		{
			g_InteralMemory.Word[PING_CTRL]=80;
		}		
	}


////第一次工作密码或附加码
	if(g_InteralMemory.Bit[155]==1)
	{
		g_InteralMemory.Bit[155]=0;
		if (g_InteralMemory.KeepWord[302]==g_InteralMemory.Word[190] 
			&&  g_InteralMemory.KeepWord[303]==g_InteralMemory.Word[191])
		{
			g_InteralMemory.KeepBit[0]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
			g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志
			
			g_InteralMemory.Word[190]=0;
			g_InteralMemory.Word[191]=0;

			g_InteralMemory.Word[401]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[192] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[193]
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_1();
		
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
				g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志
				
				g_InteralMemory.Word[192]=0;
				g_InteralMemory.Word[193]=0;

				g_InteralMemory.Word[401]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//跳到报错
				g_InteralMemory.Bit[205] = 1; //密码错误画面标志
				g_InteralMemory.Word[401]++;
			}
//密码输入错误超过5次
			if(g_InteralMemory.Word[401]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}

////第二次工作密码或附加码
	if(g_InteralMemory.Bit[156]==1)
	{
		g_InteralMemory.Bit[156]=0;
		if (g_InteralMemory.KeepWord[304]==g_InteralMemory.Word[194] 
			&&  g_InteralMemory.KeepWord[305]==g_InteralMemory.Word[195])
		{
			g_InteralMemory.KeepBit[1]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
			g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志

			g_InteralMemory.Word[194]=0;
			g_InteralMemory.Word[195]=0;

			g_InteralMemory.Word[402]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[196] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[197]
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_2();
				
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
				g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志

				g_InteralMemory.Word[196]=0;
				g_InteralMemory.Word[197]=0;

				g_InteralMemory.Word[402]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//跳到报错
				g_InteralMemory.Bit[205] = 1; //密码错误画面标志
				g_InteralMemory.Word[402]++;
			}
//密码输入错误超过5次
			if(g_InteralMemory.Word[402]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}

////第三次工作密码或附加码
	if(g_InteralMemory.Bit[157]==1)
	{
		g_InteralMemory.Bit[157]=0;
		if (g_InteralMemory.KeepWord[306]==g_InteralMemory.Word[198] 
			&&  g_InteralMemory.KeepWord[307]==g_InteralMemory.Word[199])
		{
			g_InteralMemory.KeepBit[2]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
			g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志

			g_InteralMemory.Word[198]=0;
			g_InteralMemory.Word[199]=0;

			g_InteralMemory.Word[403]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[200] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[201] 
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_3();
		
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//跳开始画面
				g_InteralMemory.Bit[207] = 0; //清除   到期密码输入画面标志

				g_InteralMemory.Word[200]=0;
				g_InteralMemory.Word[201]=0;

				g_InteralMemory.Word[403]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//跳到报错
				g_InteralMemory.Bit[205] = 1; //密码错误画面标志
				g_InteralMemory.Word[403]++;
			}
//密码输入错误超过5次
			if(g_InteralMemory.Word[403]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}






/////修改管理员密码	
	if(g_InteralMemory.Bit[151]==1)
	{
		g_InteralMemory.Bit[151]=0;
		if(g_InteralMemory.KeepWord[300]==g_InteralMemory.Word[168] 
			&&  g_InteralMemory.KeepWord[301]==g_InteralMemory.Word[169] )
		{
			if (g_InteralMemory.Word[170]==g_InteralMemory.Word[172] 
				&& g_InteralMemory.Word[171]==g_InteralMemory.Word[173])
			{
				if(g_InteralMemory.Word[170]!=0 || g_InteralMemory.Word[171]!=0)
				{
					g_InteralMemory.KeepWord[300]=g_InteralMemory.Word[170];
					g_InteralMemory.KeepWord[301]=g_InteralMemory.Word[171];
					Inn_Store_Password();
					g_InteralMemory.Word[PING_CTRL]=65;
					g_InteralMemory.Word[168]=0;
					g_InteralMemory.Word[169]=0;
					g_InteralMemory.Word[170]=0;
					g_InteralMemory.Word[171]=0;
					g_InteralMemory.Word[172]=0;
					g_InteralMemory.Word[173]=0;
				}
			}
			else
				g_InteralMemory.Word[PING_CTRL]=77;//跳到比较错误	
		}		
		else
		{
			g_InteralMemory.Word[PING_CTRL]=75;//跳到出错画面
		}
	}
	
	


/////工作密码设定
	if(g_InteralMemory.Bit[152]==1)
	{
		g_InteralMemory.Bit[152]=0;
		Inn_Store_Password();
		g_InteralMemory.Word[PING_CTRL]=65;		//跳体统设置
	}

	
/////工作到期时间设定	
	if(g_InteralMemory.Bit[153]==1)	
	{
		g_InteralMemory.Bit[153]=0;
		
		g_InteralMemory.Word[PING_CTRL]=65;//跳体统设置
		Inn_Store_Password();					
	}

}
		




//////////////////////////////////
////到期时间推迟
////////////////////////////////
void Time_Delay_1()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//第一次到期时间
	day=g_InteralMemory.KeepWord[312];
	month=g_InteralMemory.KeepWord[311]+total+1;
	year=g_InteralMemory.KeepWord[310];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[310]=year;
	g_InteralMemory.KeepWord[311]=month;
	g_InteralMemory.KeepWord[312]=day;

//第二次到期时间
	day=g_InteralMemory.KeepWord[315];
	month=g_InteralMemory.KeepWord[314]+total+1;
	year=g_InteralMemory.KeepWord[313];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[313]=year;
	g_InteralMemory.KeepWord[314]=month;
	g_InteralMemory.KeepWord[315]=day;

//第三次到期时间
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}


void Time_Delay_2()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//第二次到期时间
	day=g_InteralMemory.KeepWord[315];
	month=g_InteralMemory.KeepWord[314]+total+1;
	year=g_InteralMemory.KeepWord[313];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[313]=year;
	g_InteralMemory.KeepWord[314]=month;
	g_InteralMemory.KeepWord[315]=day;

//第三次到期时间
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}


void Time_Delay_3()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//第三次到期时间
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}






//////////////////////////////////
////到期判断
////////////////////////////////
void TimeReach_Judge()
{
//第一次到期
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[310])
	{
		g_GlobalVar.TimeReach_1=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[310]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[311])
		{
			g_GlobalVar.TimeReach_1=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[310]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[311]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[312])
			{
				g_GlobalVar.TimeReach_1=1;
			}
			else
			{
				g_GlobalVar.TimeReach_1=0;
			}
		}
	}

//第二次到期
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[313])
	{
		g_GlobalVar.TimeReach_2=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[313]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[314])
		{
			g_GlobalVar.TimeReach_2=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[313]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[314]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[315])
			{
				g_GlobalVar.TimeReach_2=1;
			}
			else
			{
				g_GlobalVar.TimeReach_2=0;
			}
		}
	}

//第三次到期
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[316])
	{
		g_GlobalVar.TimeReach_3=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[316]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[317])
		{
			g_GlobalVar.TimeReach_3=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[316]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[317]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[318])
			{
				g_GlobalVar.TimeReach_3=1;
			}
			else
			{
				g_GlobalVar.TimeReach_3=0;
			}
		}
	}


}




//////////////////////////////////
////跳出到期画面
////////////////////////////////
void TimeOUt_Screen()
{
	if(g_GlobalVar.Stop_Communication==0)
	{
		if(g_InteralMemory.KeepBit[2]!=1)		//没正确输入过密码
		{
			//if(g_GlobalVar.TimeReach_3==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75  
			//	&&  g_InteralMemory.Word[PING_STATE]!=80)		
			//且不在到期提示画面和密码错误画面
			if((g_GlobalVar.TimeReach_3==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{	
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,64);	//第三次到期
				g_InteralMemory.Bit[207] = 1; //到期密码输入画面标志
				
/*			
				if(g_StarScr==STARSCR_ENCRYPT)
					g_InteralMemory.Word[PING_CTRL]=64;	//第三次到期
				else
					WriteWord(g_PlcCtrlNum,g_PlcCtrStr,64);	
*/				
/*		
				g_InteralMemory.Word[PING_CTRL]=64;	//第三次到期
				if(g_InteralMemory.Word[PING_CTRL] != CurrentScrNum)
				{
					PreScrNum = CurrentScrNum;
					CurrentScrNum = g_InteralMemory.Word[PING_CTRL];
				}
*/				
			}
		}
		if(g_InteralMemory.KeepBit[1]!=1)		//没正确输入过密码
		{
			//if(g_GlobalVar.TimeReach_2==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75  &&  g_InteralMemory.Word[PING_STATE]!=80)
			if((g_GlobalVar.TimeReach_2==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{			
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,63);		//第二次到期				
				g_InteralMemory.Bit[207] = 1; //到期密码输入画面标志
			}
		}
		if(g_InteralMemory.KeepBit[0]!=1)		//没正确输入过密码
		{
			//if(g_GlobalVar.TimeReach_1==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75 &&  g_InteralMemory.Word[PING_STATE]!=80)
			if((g_GlobalVar.TimeReach_1==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{			
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,62);	//第一次到期
				g_InteralMemory.Bit[207] = 1; //到期密码输入画面标志
			}
		}
	}
}



//////////////////////////////////
////到期提示
////////////////////////////////
void Time_Alarm()
{
	INT16U year,month,day;

	if(g_GlobalVar.Time_Alarm_One==0)
	{
		if(g_InteralMemory.Bit[202]==1)
		{
//			g_InteralMemory.Word[100]=60;
			g_InteralMemory.Word[PING_CTRL] = PreScrNum;
			g_InteralMemory.Bit[203] = 0;	 //清除    到期提示画面标志
			g_GlobalVar.Time_Alarm_One=1;

			g_GlobalVar.Time_Alarm_State = 0;
		}
	}

	if(g_InteralMemory.KeepBit[0]!=1 && g_InteralMemory.Bit[202]!=1)	// 第一次没到期
	{
		year=g_InteralMemory.Word[330];
		month=g_InteralMemory.Word[331];
		day=g_InteralMemory.Word[332];

		day=day+10;
		if(day>28)
		{
			day=day-28;
			month++;
		}
		if(month>12)
		{
			month=month-12;
			year++;
		}	
		
		if(year>g_InteralMemory.KeepWord[310])
		{
			g_InteralMemory.Word[PING_CTRL]=81;
			g_InteralMemory.Bit[203] = 1; //到期提示画面标志
			g_GlobalVar.Time_Alarm_State = 1;

			g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
			g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
			g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
		}
		else
		{
			if(year==g_InteralMemory.KeepWord[310]
				&& month>g_InteralMemory.KeepWord[311])
			{
				g_InteralMemory.Word[PING_CTRL]=81;
				g_InteralMemory.Bit[203] = 1; //到期提示画面标志
				g_GlobalVar.Time_Alarm_State = 1;

				g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
				g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
				g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
			}
			else
			{
				if(year==g_InteralMemory.KeepWord[310]
					&& month==g_InteralMemory.KeepWord[311]
					&& day>=g_InteralMemory.KeepWord[312])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //到期提示画面标志
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
				}
			}
		}
	}
	else
	{
		if(g_InteralMemory.KeepBit[1]!=1 && g_InteralMemory.Bit[202]!=1)	// 第二次没到期
		{
			year=g_InteralMemory.Word[330];
			month=g_InteralMemory.Word[331];
			day=g_InteralMemory.Word[332];

			day=day+10;
			if(day>28)
			{
				day=day-28;
				month++;
			}
			if(month>12)
			{
				month=month-12;
				year++;
			}	
			
			if(year>g_InteralMemory.KeepWord[313])
			{
				g_InteralMemory.Word[PING_CTRL]=81;
				g_InteralMemory.Bit[203] = 1; //到期提示画面标志
				g_GlobalVar.Time_Alarm_State = 1;

				g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
				g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
				g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
			}
			else
			{
				if(year==g_InteralMemory.KeepWord[313]
					&& month>g_InteralMemory.KeepWord[314])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //到期提示画面标志
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
				}
				else
				{
					if(year==g_InteralMemory.KeepWord[313]
						&& month==g_InteralMemory.KeepWord[314]
						&& day>=g_InteralMemory.KeepWord[315])
					{
						g_InteralMemory.Word[PING_CTRL]=81;
						g_InteralMemory.Bit[203] = 1; //到期提示画面标志
						g_GlobalVar.Time_Alarm_State = 1;

						g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
						g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
						g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
					}
				}
			}
		}
		else
		{
			if(g_InteralMemory.KeepBit[2]!=1 && g_InteralMemory.Bit[202]!=1)	// 第三次没到期
			{
				year=g_InteralMemory.Word[330];
				month=g_InteralMemory.Word[331];
				day=g_InteralMemory.Word[332];

				day=day+10;
				if(day>28)
				{
					day=day-28;
					month++;
				}
				if(month>12)
				{
					month=month-12;
					year++;
				}	
				
				if(year>g_InteralMemory.KeepWord[316])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //到期提示画面标志
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
				}
				else
				{
					if(year==g_InteralMemory.KeepWord[316]
						&& month>g_InteralMemory.KeepWord[317])
					{
						g_InteralMemory.Word[PING_CTRL]=81;
						g_InteralMemory.Bit[203] = 1; //到期提示画面标志
						g_GlobalVar.Time_Alarm_State = 1;

						g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
						g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
						g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
					}
					else
					{
						if(year==g_InteralMemory.KeepWord[316]
							&& month==g_InteralMemory.KeepWord[317]
							&& day>=g_InteralMemory.KeepWord[318])
						{
							g_InteralMemory.Word[PING_CTRL]=81;
							g_InteralMemory.Bit[203] = 1; //到期提示画面标志
							g_GlobalVar.Time_Alarm_State = 1;

							g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
							g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
							g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
						}
					}
				}
			}

		}

	}

}



