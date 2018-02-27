/////////////////////////WDT设置 by 方佳伟 2017/4/1 ///////////////
#include "2416addr.h"
#include "wdt.h"

void wdt_init_reset()
{
	
	//第一步：设置分频器和预分频器
	
	rWTCON &= ~(0xff<<8);
	rWTCON |= (65<<8);		//1MHz
	
	rWTCON &= ~(0x3<<3);
	rWTCON |= (3<<3);		//1/128 MHz	T=128us
	
	// 第二步，设置中断和复位信号的使能或禁止
	rWTCON &= ~(1<<2);				// disable wdt interrupt
	rWTCON |= (1<<0);				// enable wdt reset
	
	//第三部设置定时时间
	// WDT定时计数个数，最终定时时间为这里的值×时钟周期
	rWTDAT = 30000;					// 定时3.84s
	rWTCNT = 30000;					// 定时3.84s
	
	
	//最后，所有寄存器设置好之后再打开看门狗
	rWTCON |= (1<<5);

}


void wdt_feed_dog()
{
	rWTCNT = 30000;	
}