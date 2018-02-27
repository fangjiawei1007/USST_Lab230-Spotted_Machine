
#include <string.h>
#include <stdio.h>
#include "..\inc\2416addr.h"
#include "..\inc\2416lib.h"
#include "..\inc\systemconf.h"

#include "..\inc\def.h"


#include "..\inc\Rtc.h"


#define WRDATA      (1)
#define RDDATA      (2)





/*
struct rtc_regs {
	unsigned char   secs;
	unsigned char   mins;
	unsigned char   hours;    
	unsigned char   mday;    //day of month
	unsigned char   wday;   //day of week
	unsigned char   mon;
	unsigned char   year;
	unsigned char   cs;
};
*/

unsigned int epoch = 2000;  /* 芯片中”年”为0x00时，对应2000年 */

unsigned char days_in_mo[] = 
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


typedef struct tI2C {
	unsigned char *pData;   /* 数据缓冲区 */
	volatile int DataCount; /* 等待传输的数据长度 */
	volatile int Status;    /* 状态 */
	volatile int Mode;      /* 模式：读/写 */
	volatile int Pt;        /* pData中待传输数据的位置 */
}tS3C24xx_I2C, *ptS3C24xx_I2C;

static tS3C24xx_I2C g_tS3C24xx_I2C;



extern INTERALMEMORY g_InteralMemory;




/*
void RTC_SetTime(void)
{
	int year,month,date,hour,min,sec,weekday;

   	rRTCCON  = rRTCCON  | 0x1;		//RTC Control enable

	rBCDYEAR = year;
       rBCDMON = month;
	rBCDDAY = weekday ;
	
	rBCDDATE = date   ;
	rBCDHOUR = hour ;
	
	rBCDMIN = min  ;
	rBCDSEC = sec ;


	rRTCCON  = rRTCCON  & ~(0x1);	//RTC Control disable

	
    if(rBCDYEAR==0x99) 
       	year = 0x1999;
  	else 
       	year = 0x2000 + rBCDYEAR;
              	
	month   = rBCDMON;
	weekday = rBCDDAY;
	date    = rBCDDATE;
	hour    = rBCDHOUR;
	min     = rBCDMIN;
	sec     = rBCDSEC;

  
	//printf("%2x : %2x : %2x  %10s,  %2x/%2x/%4x\n",hour,min,sec,day[weekday],month,date,year);
}

*/


void RTC_Init(void)
{
	struct rtc_time dt;

	int year,month,date,hour,min,sec;

	pcf8563_get_datetime(&dt);
	year = dt.tm_year;
	month = dt.tm_mon;
	//rBCDDATE = dt.tm_wday;
	date = dt.tm_mday;
	hour = dt.tm_hour;
	min = dt.tm_min;
	sec = dt.tm_sec;
	
	rRTCCON  = rRTCCON  | 0x1;		//RTC Control enable

	rBCDYEAR =( ((year/10%10)<<4) + (year%10) );
	rBCDMON  =( ((month/10%10)<<4)+ (month%10));
	//rBCDDAY  = rBCDDAY  & ~(0x7)  | 0x3;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
	rBCDDAY  = dt.tm_wday  & ~(0x7)  | 0x3;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
	rBCDDATE =( ((date/10%10)<<4) + (date%10) );		
	rBCDHOUR =( ((hour/10%10)<<4) + (hour%10) );
	rBCDMIN  =( ((min/10%10)<<4)  + (min%10)  );
	rBCDSEC  =( ((sec/10%10)<<4)  + (sec%10)  );     	

	rRTCCON  = rRTCCON  & ~(0x1);	//RTC Control disable
	
}


// 产品检验用(画面不一样)
void Test_RTC_SetTime(void)
{
	struct rtc_time dt;
	int year,month,date,hour,min,sec;

	dt.tm_year = year = g_InteralMemory.Word[51];
	dt.tm_mon = month = g_InteralMemory.Word[53];
	//rBCDDATE = g_InteralMemory.Word[63];
	dt.tm_wday = g_InteralMemory.Word[63];
	dt.tm_mday = date = g_InteralMemory.Word[55];	
	dt.tm_hour = hour = g_InteralMemory.Word[57];
	dt.tm_min = min = g_InteralMemory.Word[59];
	dt.tm_sec = sec = g_InteralMemory.Word[61];


	if(g_InteralMemory.Bit[250]==1)	//修改RTC 显示时间
	{
		if(!(g_InteralMemory.Word[51]==0 && g_InteralMemory.Word[53]==0 
			&& g_InteralMemory.Word[55]==0))
		{
			if(pcf8563_set_datetime(&dt))
			{
				if(UART_PrintStr && 0x23==rUBRDIV1) // 115200
					Uart_Printf("Set RTC failed!\n\r");
			}
			else
			{
				rRTCCON  = rRTCCON  | 0x1;		//RTC Control enable

			    	rBCDYEAR =( ((year/10%10)<<4) + (year%10) );
			    	rBCDMON  =( ((month/10%10)<<4)+ (month%10));
				//rBCDDAY  = rBCDDAY  & ~(0x7)  | 0x3;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
			    	rBCDDAY  = g_InteralMemory.Word[63]  & 0x7;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
			    	rBCDDATE =( ((date/10%10)<<4) + (date%10) );		
			    	rBCDHOUR =( ((hour/10%10)<<4) + (hour%10) );
			    	rBCDMIN  =( ((min/10%10)<<4)  + (min%10)  );
			    	rBCDSEC  =( ((sec/10%10)<<4)  + (sec%10)  );     	

				rRTCCON  = rRTCCON  & ~(0x1);	//RTC Control disable
			}
		}
		
		g_InteralMemory.Bit[250]=0;	
	}	
}

void Test_RTC_ReadTime(void)
{
	if(rBCDYEAR==0x99) 
       	g_InteralMemory.Word[50] = 1999;	//0x1999
  	else 
       	g_InteralMemory.Word[50] = 2000 + (rBCDYEAR>>4)*10 + (rBCDYEAR&0xf);  // 0x2000
              	
	g_InteralMemory.Word[52]   = (rBCDMON>>4)*10 + (rBCDMON&0xf);	
	g_InteralMemory.Word[62]    = rBCDDAY&0x7;	// 星期
	g_InteralMemory.Word[54] = (rBCDDATE>>4)*10 + (rBCDDATE&0xf);	// 日
	g_InteralMemory.Word[56]    = (rBCDHOUR>>4)*10 + (rBCDHOUR&0xf);
	g_InteralMemory.Word[58]     = (rBCDMIN>>4)*10 + (rBCDMIN&0xf);
	g_InteralMemory.Word[60]     = (rBCDSEC>>4)*10 + (rBCDSEC&0xf);

}


////////////////
void RTC_SetTime(void)
{
	struct rtc_time dt;
	int year,month,date,hour,min,sec;

	dt.tm_year = year = g_InteralMemory.Word[320];
	dt.tm_mon = month = g_InteralMemory.Word[321];
	//rBCDDATE = g_InteralMemory.Word[326];
	//dt.tm_wday = g_InteralMemory.Word[326];
	dt.tm_mday = date = g_InteralMemory.Word[322];	
	dt.tm_hour = hour = g_InteralMemory.Word[323];
	dt.tm_min = min = g_InteralMemory.Word[324];
	dt.tm_sec = sec = g_InteralMemory.Word[325];


	if(g_InteralMemory.Bit[158]==1)	//修改RTC 显示时间
	{
		g_InteralMemory.Bit[158]=0;	

		if(!(g_InteralMemory.Word[320]==0 && g_InteralMemory.Word[321]==0 
			&& g_InteralMemory.Word[322]==0 && g_InteralMemory.Word[323]==0))
		{			
		/*	if(pcf8563_set_datetime(&dt))
			{
				if(UART_PrintStr && 0x23==rUBRDIV1) // 115200
					Uart_Printf("Set RTC failed!\n\r");
			}
			else
			*/{
				rRTCCON  = rRTCCON  | 0x1;		//RTC Control enable

			    	rBCDYEAR =( ((year/10%10)<<4) + (year%10) );
			    	rBCDMON  =( ((month/10%10)<<4)+ (month%10));
				//rBCDDAY  = rBCDDAY  & ~(0x7)  | 0x3;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
			    	rBCDDAY  = g_InteralMemory.Word[63]  & 0x7;		//SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
			    	rBCDDATE =( ((date/10%10)<<4) + (date%10) );		
			    	rBCDHOUR =( ((hour/10%10)<<4) + (hour%10) );
			    	rBCDMIN  =( ((min/10%10)<<4)  + (min%10)  );
			    	rBCDSEC  =( ((sec/10%10)<<4)  + (sec%10)  );     	

				rRTCCON  = rRTCCON  & ~(0x1);	//RTC Control disable

				g_InteralMemory.Word[PING_CTRL]=65;//回主选单
			}
		}		
	}
}

void RTC_ReadTime(void)
{	
	if(rBCDYEAR==0x99) 
       	g_InteralMemory.Word[330] = 1999;	//0x1999
  	else 
       	g_InteralMemory.Word[330] = 2000 + (rBCDYEAR>>4)*10 + (rBCDYEAR&0xf);  // 0x2000
              	
	g_InteralMemory.Word[331]   = (rBCDMON>>4)*10 + (rBCDMON&0xf);	
	//g_InteralMemory.Word[336]    = rBCDDAY&0x7;	// 星期
	g_InteralMemory.Word[332] = (rBCDDATE>>4)*10 + (rBCDDATE&0xf);	// 日
	g_InteralMemory.Word[333]    = (rBCDHOUR>>4)*10 + (rBCDHOUR&0xf);
	g_InteralMemory.Word[334]     = (rBCDMIN>>4)*10 + (rBCDMIN&0xf);
	g_InteralMemory.Word[335]     = (rBCDSEC>>4)*10 + (rBCDSEC&0xf);

}



// 产品检验用(画面不一样)
void Test_Write_DateTime()
{
	struct rtc_time dt;

	if(g_InteralMemory.Bit[250]==1)	//修改RTC 显示时间
	{
		if(!(g_InteralMemory.Word[51]==0 && g_InteralMemory.Word[53]==0 
			&& g_InteralMemory.Word[55]==0))
		{
			dt.tm_year = g_InteralMemory.Word[51];
			dt.tm_mon = g_InteralMemory.Word[53];
			dt.tm_mday = g_InteralMemory.Word[55];
			dt.tm_hour = g_InteralMemory.Word[57];
			dt.tm_min = g_InteralMemory.Word[59];
			dt.tm_sec = g_InteralMemory.Word[61];

			dt.tm_wday = g_InteralMemory.Word[63];

			if(pcf8563_set_datetime(&dt))
			{
				if(UART_PrintStr && 0x23==rUBRDIV1) // 115200
					Uart_Printf("Set RTC failed!\n\r");
			}
		}
		
		g_InteralMemory.Bit[250]=0;	
	}

}


void Test_Read_DateTime()
{

	struct rtc_time dt;

	pcf8563_get_datetime(&dt);

	g_InteralMemory.Word[50] = dt.tm_year;
	g_InteralMemory.Word[52] = dt.tm_mon;
	g_InteralMemory.Word[54] = dt.tm_mday;
	g_InteralMemory.Word[56] = dt.tm_hour;
	g_InteralMemory.Word[58] = dt.tm_min;
	g_InteralMemory.Word[60] = dt.tm_sec;

	g_InteralMemory.Word[62] = dt.tm_wday;

}


//////////////
void Write_DateTime()
{
	struct rtc_time dt;

	if(g_InteralMemory.Bit[158]==1)	//修改RTC 显示时间
	{
		g_InteralMemory.Bit[158]=0;	

		if(!(g_InteralMemory.Word[320]==0 && g_InteralMemory.Word[321]==0 
			&& g_InteralMemory.Word[322]==0 && g_InteralMemory.Word[323]==0))
		{			
			dt.tm_year = g_InteralMemory.Word[320];
			dt.tm_mon = g_InteralMemory.Word[321];
			dt.tm_mday = g_InteralMemory.Word[322];
			dt.tm_hour = g_InteralMemory.Word[323];
			dt.tm_min = g_InteralMemory.Word[324];
			dt.tm_sec = g_InteralMemory.Word[325];

			//dt.tm_wday = g_InteralMemory.Word[326];

			if(pcf8563_set_datetime(&dt))
			{
				if(UART_PrintStr && 0x23==rUBRDIV1) // 115200
					Uart_Printf("Set RTC failed!\n\r");
			}

			g_InteralMemory.Word[PING_CTRL]=65;//回主选单
			
		}		
	}
}


void Read_DateTime()
{
	struct rtc_time dt;

	pcf8563_get_datetime(&dt);
	
	g_InteralMemory.Word[330] = dt.tm_year;
	g_InteralMemory.Word[331] = dt.tm_mon;
	g_InteralMemory.Word[332] = dt.tm_mday;
	g_InteralMemory.Word[333] = dt.tm_hour;
	g_InteralMemory.Word[334] = dt.tm_min;
	g_InteralMemory.Word[335] = dt.tm_sec;

	//g_InteralMemory.Word[336] = dt.tm_wday;
}




// 写pcf8563，设置日期与时间
int pcf8563_set_datetime(struct rtc_time *dt)
{

	unsigned char leap_yr;
	unsigned char buf[9];  
	//unsigned int i; 


	leap_yr = ((!(dt->tm_year % 4) && (dt->tm_year % 100))
	        || !(dt->tm_year % 400));
	        
	if ((dt->tm_mon < 1) || (dt->tm_mon > 12) || (dt->tm_mday == 0)) 
	{
		return -1;
	}

	if (dt->tm_mday > (days_in_mo[dt->tm_mon] + ((dt->tm_mon == 2)
	            && leap_yr))) 
	{
		return -2;
	}

	if ((dt->tm_hour >= 24) || (dt->tm_min >= 60) || (dt->tm_sec >= 60)) 
	{
		return -3;
	}

	if ((dt->tm_year -= epoch) > 255) 
	{
		// They are unsigned 
		return -4;
	}

	if (dt->tm_year >= 100) 
	{
		dt->tm_year -= 100;
	}

	buf[0] = BIN_TO_BCD(0x2);
	buf[1] = BIN_TO_BCD(dt->tm_sec);
	buf[2] = BIN_TO_BCD(dt->tm_min);
	buf[3] = BIN_TO_BCD(dt->tm_hour);
	buf[4] = BIN_TO_BCD(dt->tm_mday);
	buf[5] = BIN_TO_BCD(dt->tm_wday);
	buf[6] = BIN_TO_BCD(dt->tm_mon);
	buf[7] = BIN_TO_BCD(dt->tm_year);
	buf[8] = BIN_TO_BCD(0);

	i2c_write(PCF8563_WADDR, (unsigned char *)&buf, sizeof(buf));

	return 0;
}


//读取pcf8563，获得日期与时间
int pcf8563_get_datetime(struct rtc_time *dt)
{

	unsigned char addr[1] = { 0 };
	//struct rtc_regs rtc;
	struct pcf8563_regs pcf8563_regs1;

	memset(&pcf8563_regs1, 0, sizeof(pcf8563_regs1));

	i2c_write(PCF8563_WADDR, addr, 1);
	i2c_read(PCF8563_RADDR, (unsigned char *)&pcf8563_regs1, sizeof(pcf8563_regs1));


	 dt->tm_year = epoch;
	pcf8563_regs1.secs  &= 0x7f;	// seconds 
	pcf8563_regs1.mins  &= 0x7f;	// minutes 
	pcf8563_regs1.hours &= 0x3f;	// hours 
	pcf8563_regs1.wday  &= 0x07; 	// day-of-week 
	pcf8563_regs1.mday  &= 0x3f;	// day-of-month 
	pcf8563_regs1.mon   &= 0x1f;	// month 
	pcf8563_regs1.year  &= 0xff;	// year 

	dt->tm_sec     = BCD_TO_BIN(pcf8563_regs1.secs);
	dt->tm_min     = BCD_TO_BIN(pcf8563_regs1.mins);
	dt->tm_hour    = BCD_TO_BIN(pcf8563_regs1.hours);
	dt->tm_wday    = BCD_TO_BIN(pcf8563_regs1.wday);
	dt->tm_mday    = BCD_TO_BIN(pcf8563_regs1.mday);
	dt->tm_mon     = BCD_TO_BIN(pcf8563_regs1.mon);
	dt->tm_year    += BCD_TO_BIN(pcf8563_regs1.year);


	return 0;
}



//  I2C初始化
void i2c_init(void)
{

	rGPEUDP  |= 0xc000;       // 禁止内部上拉
	rGPECON &= 0x0fffffff;   
	rGPECON |= 0xa0000000;   // 选择引脚功能：GPE15:IICSDA, GPE14:IICSCL


	pISR_IIC=(unsigned)I2CIntHandle;
	rINTMSK1 &= ~(BIT_IIC);
	
	rIICCON = (1<<7) | (0<<6) | (1<<5) | (0xf);  // 0xaf

	// rIICADD  = 0xA2;     // S3C24xx slave address = [7:1]
	rIICSTAT = 0x10;     // I2C串行输出使能(Rx/Tx)
	
}

// 主机发送
// slvAddr : 从机地址，buf : 数据存放的缓冲区，len : 数据长度 
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len)
{

	g_tS3C24xx_I2C.Mode = WRDATA;   // 写操作
	g_tS3C24xx_I2C.Pt   = 0;        // 索引值初始为0
	g_tS3C24xx_I2C.pData = buf;     // 保存缓冲区地址
	g_tS3C24xx_I2C.DataCount = len; // 传输长度

	rIICDS   = slvAddr;
	rIICSTAT = 0xf0;         // 主机发送，启动

	//  等待直至数据传输完毕  
	while (g_tS3C24xx_I2C.DataCount != -1);      	   
    
}
        
// 主机接收
//slvAddr : 从机地址，buf : 数据存放的缓冲区，len : 数据长度 
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len)
{

	g_tS3C24xx_I2C.Mode = RDDATA;   // 读操作
	g_tS3C24xx_I2C.Pt   = -1;       // 索引值初始化为-1，表示第1个中断时不接收数据(地址中断)
	g_tS3C24xx_I2C.pData = buf;     // 保存缓冲区地址
	g_tS3C24xx_I2C.DataCount = len; // 传输长度

	rIICDS        = slvAddr;
	rIICSTAT      = 0xb0;    // 主机接收，启动

	// 等待直至数据传输完毕  
	while (g_tS3C24xx_I2C.DataCount != -1);
	
}

//I2C中断服务程序
// 根据剩余的数据长度选择继续传输或者结束
void __irq I2CIntHandle(void)
{

	unsigned int iicSt,i;


	iicSt  = rIICSTAT; 

	//if(iicSt & 0x8)
	//	printf("Bus arbitration failed\n\r");//printf(需要stdio.h 头文件) 会导致 __rt_lib_init()  无法执行
									//可能 __rt_lib_init()  运行时库跟stdio.h 冲突

	switch (g_tS3C24xx_I2C.Mode)
	{    
		case WRDATA:
		{			
			if((g_tS3C24xx_I2C.DataCount--) == 0)
			{
				// 下面两行用来恢复I2C操作，发出P信号
				rIICSTAT = 0xd0;
				rIICCON  = 0xaf;
				Delay(10000);  // 等待一段时间以便P信号已经发出10000
				break;    
			}

			rIICDS = g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++];
		    
			// 将数据写入IICDS后，需要一段时间才能出现在SDA线上
			for (i = 0; i < 10; i++);   

			rIICCON = 0xaf;      // 恢复I2C传输
			break;
		}
		case RDDATA:
		{
			if (g_tS3C24xx_I2C.Pt == -1)
			{
				// 这次中断是发送I2C设备地址后发生的，没有数据
				// 只接收一个数据时，不要发出ACK信号
				g_tS3C24xx_I2C.Pt = 0;
				if(g_tS3C24xx_I2C.DataCount == 1)
				   rIICCON = 0x2f;   // 恢复I2C传输，开始接收数据，接收到数据时不发出ACK
				else 
				   rIICCON = 0xaf;   // 恢复I2C传输，开始接收数据
				break;
			}
		    
			if ((g_tS3C24xx_I2C.DataCount--) == 0)
			{
				g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++] = rIICDS;

				// 下面两行恢复I2C操作，发出P信号
				rIICSTAT = 0x90;
				rIICCON  = 0xaf;
				Delay(10000);  // 等待一段时间以便P信号已经发出
				break;    
			}      
			   
			g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++] = rIICDS;

			// 接收最后一个数据时，不要发出ACK信号
			if(g_tS3C24xx_I2C.DataCount == 0)
				rIICCON = 0x2f;   // 恢复I2C传输，接收到下一数据时无ACK
			else 
				rIICCON = 0xaf;   // 恢复I2C传输，接收到下一数据时发出ACK
			break;
		}
		default:
			break;      
	}

	// 清中断
	ClearPending(BIT_IIC)
	
}





