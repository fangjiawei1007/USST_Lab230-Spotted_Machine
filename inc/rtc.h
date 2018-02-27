#ifndef _RTC_H_
#define _RTC_H_
// FILE: rtc.h
// 提供设置、读取RTC芯片pcf8563的函数
 
#define PCF8563_WADDR 0xa2  
#define PCF8563_RADDR 0xa3

#define BCD_TO_BIN(val) (((val)&15) + ((val)>>4)*10)
#define BIN_TO_BCD(val) ((((val)/10)<<4) + (val)%10)                     



struct rtc_time 
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_wday;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int cs;
};

struct pcf8563_regs
{
	unsigned char   ctl1;
	unsigned char   ctl2;
	
	unsigned char   secs;
	unsigned char   mins;
	unsigned char   hours;
	unsigned char   mday;
	unsigned char   wday;
	unsigned char   mon;
	unsigned char   year;
	
	//unsigned char   cs;	
	
	unsigned char   mins_alrm;
	unsigned char   hours_alrm;
	unsigned char   mday_alrm;
	unsigned char   wday_alrm;
	unsigned char   clkout;
	unsigned char   timer_ctl;
	unsigned char   timer_val;	 
};



void RTC_Init(void);
void Test_RTC_SetTime(void);
void Test_RTC_ReadTime(void);
void RTC_SetTime(void);
void RTC_ReadTime(void);




void Test_Write_DateTime(void);
void Test_Read_DateTime(void);




void Write_DateTime(void);
void Read_DateTime(void);



	
int pcf8563_get_datetime(struct rtc_time *dt);
int pcf8563_set_datetime(struct rtc_time *dt);



void i2c_init(void);
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len);
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len);
void __irq I2CIntHandle(void);


//void Delay(int time);

#endif
