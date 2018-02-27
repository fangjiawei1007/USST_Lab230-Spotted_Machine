#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"
#include "..\inc\wdt.h"	//WDT设置 by 方佳伟 2017/4/1


#include "..\inc\app.h"
#include "..\inc\appBase.h"

#include "includes.h"



//extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


extern unsigned int InPulseCount[7];		//记录6路编码器输入的脉冲数

extern unsigned int OutPulse_PreSet[4];	// 3路脉冲输出的  脉冲预设值
extern volatile unsigned int OutPulse_Counter[4];	// 3路脉冲输出的  当前输出脉冲数

extern INTERALMEMORY g_InteralMemory;




////从此开始添加用户变量//////////////////////////////////

//unsigned char model_work;
	unsigned int move_allow = 0;
	
	unsigned int start_mode = 0;
	unsigned int power_flag = 0;//用于上电时候选择模块
	
 	unsigned int x_diandong_flag = 0;
	unsigned int y_diandong_flag = 0;
	unsigned int w_diandong_flag = 0;
	
	unsigned int x_dingchang_flag = 0;
	unsigned int y_dingchang_flag = 0;
	unsigned int w_dingchang_flag = 0;
	
	unsigned int weitiao_flag = 0;
	unsigned int mode_flag = 0;
	unsigned int xitong_flag = 0;
	
	unsigned int gongzuo_flag = 0;
	unsigned int clean_flag = 0;
	unsigned int quyang_flag = 0;
	unsigned int zhangan_flag = 0;
	unsigned int pingshan_flag = 0;
	unsigned int tuye_flag = 0;
	
	
	unsigned int status_record1 = 0;//用于判断三键互斥
	unsigned int status_record2 = 0;//用于判断三键互斥
	
	unsigned int null = 0;
	static unsigned int condition_flag = 0;//condition_flag用于判断点动同时停和启动条件,condition_flag == 0的时候才能够正常启动
///////////////////////////////////////////////////////////////
//////	初始化     //////////////////////////////////////////
//////	每次上电App_Init 只执行一次	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i;
	
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	Pulse_Out_Init();
	parameter_init();
	xitong_position_init();
	wdt_feed_dog();

	//启动按键弹起
	start_b = 0;
	start_once = 0;
	
	
	//test:上点之后各个监控设置为0 最后需要屏蔽
	if(power_zero == 1)
	{
		tmp_dianyang_danyuan_lieshu = 1;
		tmp_dianyang_danyuan_hangshu = 1;
		
		tmp_dianyang_jiance_lieshu = 1;
		tmp_dianyang_jiance_hangshu = 1;
		
		tmp_dianyang_dianzhen_memory = 1;
			
		tmp_dianyang_dianzhen_no = dianzhen_bianhao;
		tmp_dianyang_yangpin_no = yangpin_bianhao;
		
		Y1_dianyang_x_back_remember = 0;
		Y1_pulse_remember_back_add = 0;
		
		spotted_c_remember = 0;
		tmp_dianyang_times_remember = 1;
		tmp_dianyang_times = 0;
		
		tmp_dianyang_c_x_s_hang_remember = 0;
		
		// Y1_pulse_remember = 0;
		// Y1_pulse_remember_add = 0;
		
		// Y2_pulse_remember = 0;
		// Y2_pulse_remember_add = 0;
		
		// Y3_pulse_remember = 0;
		// Y3_pulse_remember_add = 0;
		
		// Y1_dianyang_x_back_remember = 0;
		
		// x_biaoding_pulse = 1;
		// x_biaoding_length = 1;
		
		// y_biaoding_pulse = 1;
		// y_biaoding_length = 1;
		
		// w_biaoding_pulse = 1;
		// w_biaoding_length = 1;
		
		// monitor_x_biaoding = 0;
		// monitor_y_biaoding = 0;
		// monitor_w_biaoding = 0;
	}
	
	monitor_yangpin_no = 0;
	//	DA0_10_Init();

	// 脉冲输入初始化
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}
	//parameter_input();
	
}


///////////////////////////////////////////////////////////////
//////	应用主程序     //////////////////////////////////////////
//////	系统每个循环会调用一次App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{

//////********************************************////



//////////////////////////////////////////////////////////////
/////////////////////从此开始添加用户程序/////////////////////
//////////////////////////////////////////////////////////////

	
//喂狗
	wdt_feed_dog();//WDT设置 by方佳伟
	
/*
g_InteralMemory.Bit[]
g_InteralMemory.KeepBit[]
g_InteralMemory.Word[]
g_InteralMemory.KeepWord[]
*/	
	if(dry == 1)
	{
		Y1_pulse_remember = 0;
		Y1_pulse_remember_add = 0;
		
		Y2_pulse_remember = 0;
		Y2_pulse_remember_add = 0;
		
		Y3_pulse_remember = 0;
		Y3_pulse_remember_add = 0;
		
		Y1_pulse_remember_back_add = 0;
	}
	pulse_cal();
	biaoding();
	//*********************监控**********************************//
	monitor_x_biaoding = (Y1_PULSE_TOTAL/10);//Y1_pulse_remember_add
	monitor_y_biaoding = (Y2_PULSE_TOTAL/10);
	monitor_w_biaoding = (Y3_PULSE_TOTAL/10);
	
	monitor_x = ((x_biaoding_pulse*Y1_PULSE_TOTAL*10.0)/x_biaoding_length*100000);// /10 Y1_pulse_remember_add,最后一组除以10是um转0.1um的精度转换监控值
	monitor_y = ((((Y2_PULSE_TOTAL/10)*y_biaoding_length)/(y_biaoding_pulse)));// /10Y2_BIAODING_PULSE_TOTAL
	monitor_w = ((w_biaoding_pulse*Y1_PULSE_TOTAL*10.0)/w_biaoding_length*100000);// /10
	
	/* if(Y3_pulse_remember == 5000)
	{
		Y3_pulse_remember = 0;
	} */
	
	monitor_jiance_hang = tmp_dianyang_jiance_hangshu;
	monitor_jiance_lie = tmp_dianyang_jiance_lieshu;
	
	monitor_danyuan_hang = tmp_dianyang_danyuan_hangshu;
	monitor_danyuan_lie = tmp_dianyang_danyuan_lieshu;
	
	monitor_dianzhen_no = dianzhen_bianhao;
	//monitor_yangpin_no = tmp_dianyang_dianzhen_memory;	// tmp_dianyang_yangpin_no
	
	
	c_s_distance = dianzhen_hangjianju;
	c_m_distance = (jiance_hangjianju +jiance_gaodu);
	c_l_distance = (jiance_hangshu * c_m_distance + danyuan_hangjianju);
	//**********************按键互斥****************************//
	
	button_huchi(&start_b,&start_once,&start_mode,null);
	button_huchi(&power_previous,&power_zero,&power_flag,null);//上电选择模式
	
	button_huchi(&diandong_x_p,&diandong_x_n,&x_diandong_flag,condition_flag);//此处condition_flag 没有用指针类型，所以导致一开始点动失败
	button_huchi(&diandong_y_p,&diandong_y_n,&y_diandong_flag,condition_flag);//后期如果有时间可以进行进一步调试，函数的参变量本质是
	button_huchi(&diandong_w_p,&diandong_w_n,&w_diandong_flag,condition_flag);//变量的拷贝和复制，只有在地址被拷贝的同时才能改变下面的值的变化
	
	button_huchi(&x_dingchang_pb,&x_dingchang_nb,&x_dingchang_flag,null);
	button_huchi(&y_dingchang_pb,&y_dingchang_nb,&y_dingchang_flag,null);
	button_huchi(&w_dingchang_pb,&w_dingchang_nb,&w_dingchang_flag,null);
	
	button_huchi(&weitiao_p,&weitiao_n,&weitiao_flag,null);//微调开关
	
	button_3_huchi(&xitong_x,&xitong_y,&xitong_w,&status_record1,&status_record2);//三个按键互斥
	/* button_huchi(&xitong_x,&xitong_y,&xitong_flag,null);
	button_huchi(&xitong_y,&xitong_w,&xitong_flag,null);
	button_huchi(&xitong_x,&xitong_w,&xitong_flag,null);
	 */
	button_huchi(&gongzuo_b,&gongzuo_c,&gongzuo_flag,null);
	button_huchi(&clean_b,&clean_c,&clean_flag,null);
	button_huchi(&quyang_b,&quyang_c,&quyang_flag,null);
	button_huchi(&zhangan_b,&zhangan_c,&zhangan_flag,null);
	button_huchi(&pingshan_b,&pingshan_c,&pingshan_flag,null);
	button_huchi(&tuye_b,&tuye_c,&tuye_flag,null);
	
	button_huchi(&single_mode,&multi_mode,&mode_flag,null);//单针多针模式选择
	
	
	//点动
	/* fn_jog(Y_1,x_diandong_n,x_diandong_speed_t,condition_flag);//Y_1 赋值5 电机dir
	fn_jog(Y_1,x_diandong_n,x_diandong_speed_t,condition_flag);
	
	fn_jog(Y_2,y_diandong_p,y_diandong_speed_t,condition_flag);
	fn_jog(Y_2,y_diandong_n,y_diandong_speed_t,condition_flag);
	
	fn_jog(Y_3,w_diandong_p,w_diandong_speed_t,condition_flag);
	fn_jog(Y_3,w_diandong_n,w_diandong_speed_t,condition_flag); */
	
	//TEST
	//PulseOut_1_Start(2000,-1);
	
	//确认按钮调试
/* 	
	if(gongzuo_c == 1 || clean_c == 1 || quyang_c == 1 || zhangan_c == 1 || pingshan_c == 1 || tuye_c == 1)
	{
		Beep(1);
	} */
	//*****手动设置*****//
	//*****点动*****//
	fn_jog_test();
	
	//*****定长******//
	fn_dingchang();
	
	
	//*****回零******//
	fn_get_zero();//回零函数问题：中断函数中的 Y1_pulse_remember值无法传入
	
	//************************************//
	
	
	//****系统位置设置*****//
	//********微调*********//
	fn_weitiao();
	
	//****************//
	
	fn_pos_init();
	
	//fn_position();//此种程序有bug：只能第一个动，后面的没有办法移动
	fn_gongzuo_position();
	fn_gongzuo_confirm();
	
	
	fn_clean_position();
	fn_clean_confirm();
	
	
	fn_quyang_position();
	fn_quyang_confirm();
	
	
	fn_zhangan_position();
	fn_zhangan_confirm();
	
	
	fn_pingshan_position();
	fn_pingshan_confirm();
	
	
	fn_tuye_position();
	fn_tuye_confirm();
	
	//系统位置按键弹起解决方案：给出3个flag
	
	
	//只有所有位置信息都确认了之后，才能够进行点样
	move_allow = (gongzuo_c && clean_c && quyang_c && zhangan_c && pingshan_c && tuye_c);	

	//方案一：静态制备方案，回零处理	
	if(start_b == 1 && move_allow == 1 && power_zero == 1 )//&& start_b_status == 0
	{
		start_b_status = 1;
		
		//Beep(1);
		//上电回零
		work_get_zero();
		
		
		//回零上电1st进行清洗工作
		if(work_zero_x_finish == 1 && work_zero_y_finish == 1 && work_zero_w_finish == 1 && work_clean_start_status == 0)
		{
			work_clean();
		}
		
		if(work_clean_x_finish == 1 && work_clean_y_finish == 1 && work_cleaning_finish == 1 && work_clean_back_finish == 1)
		{
			work_clean_x_finish = 0;
			work_clean_y_finish = 0;
			work_cleaning_finish = 0;
			work_clean_back_finish = 0;
			
			work_clean_start_status = 1;	//点样结束完成之后将此置为1
			
			quyang_allow = 1;
		}
		
		
		//clean工作完成后，进行取样工作
		if(quyang_allow == 1 && quyang_status == 0 && work_quyang_start_status == 0)//取样功能
		{
			work_quyang();
		}
		
		if(work_quyang_x_finish == 1 && work_quyang_y_finish == 1 && work_quyanging_finish == 1 && work_quyang_back_finish == 1)
		{
			quyang_allow = 0;
			
			work_quyang_x_finish = 0;
			work_quyang_y_finish = 0;
			work_quyanging_finish = 0;
			work_quyang_back_finish = 0;
			
			work_quyang_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			pre_dianyang_allow = 1;
			
		}
		
		//取样工作完成后进行预点样工作
	 	if(pre_dianyang_allow == 1 && pre_dianyang_status == 0 && work_pre_dianyang_start_status == 0)
		{
			work_pre_dianyang();
		}
		
		if(work_pre_dianyang_x_finish == 1 && work_pre_dianyang_y_finish == 1 && work_pre_dianyanging_finish == 1 
		   && work_pre_dianyang_w_finish == 1 && work_pre_dianyang_back_finish == 1)//
		{
			pre_dianyang_allow = 0;
			
			work_pre_dianyang_x_finish = 0;
			work_pre_dianyang_y_finish = 0;
			
			work_pre_dianyanging_finish = 0;
			
			work_pre_dianyang_w_finish = 0;
			work_pre_dianyang_back_finish = 0;
			
			work_pre_dianyang_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			zhangan_allow = 1;
		} 
		
		//预点样工作完成后进行蘸干工作
	 	
 		if(zhangan_allow == 1 && zhangan_status == 0 && work_zhangan_start_status == 0)
		{
			work_zhangan();
			
			
		}
		
		 if(work_zhangan_x_finish == 1 && work_zhangan_y_finish == 1 && work_zhanganing_finish == 1 
		   && work_zhangan_w_finish == 1 && work_zhangan_back_finish == 1 && work_zhangan_x_back_finish == 1)//
		{
			zhangan_allow = 0;
			
			work_zhangan_x_finish = 0;
			work_zhangan_y_finish = 0;
			
			work_zhanganing_finish = 0;
			
			work_zhangan_w_finish = 0;
			work_zhangan_back_finish = 0;
			
			work_zhangan_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			dianyang_allow = 1;
		}   
		
		if(dianyang_allow == 1 && dianyang_status == 0 && work_dianyang_start_status == 0)
		{
			work_dianyang();
			//Beep(1);
		}
		
		
		
	}
	
	
	else if(start_b == 0 && start_b_status == 1)
	{
 		
		start_b_status = 0;
		
		if (work_zero_x_status == 1 || work_zero_y_status == 1 || work_zero_w_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
		/* 	work_zero_x_status = 0;
			work_zero_y_status = 0;
			work_zero_w_status = 0; */
			
		}
		
		
		if (work_clean_x_status == 1 || work_clean_y_status == 1 || work_clean_back_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_clean_x_status = 0;
			work_clean_y_status = 0;
			work_clean_back_status = 0;	

		} 
		
		
		if (work_quyang_x_status == 1 || work_quyang_y_status == 1 || work_quyang_back_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_quyang_x_status = 0;
			work_quyang_y_status = 0;
			work_quyang_back_status = 0;	

		} 
		
		if (work_pre_dianyang_x_status == 1 || work_pre_dianyang_y_status == 1 || work_pre_dianyang_back_status == 1 
		   || work_pre_dianyang_w_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_pre_dianyang_x_status = 0;
			work_pre_dianyang_y_status = 0;
			work_pre_dianyang_w_status = 0;
			
			work_pre_dianyang_back_status = 0;	

		}
		
		if(work_zhangan_x_status == 1 || work_zhangan_w_status == 1 || work_zhangan_y_status == 1  
		|| work_zhangan_back_status == 1 || work_zhangan_w_back_status == 1 || work_zhangan_x_back_status == 1)//|| work_zhanganing_status == 1
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_zhangan_x_status = 0;
			work_zhangan_w_status = 0;
			work_zhangan_y_status = 0;
			
			work_zhanganing_status = 0;
			work_zhangan_back_status = 0;
			work_zhangan_w_back_status = 0;
			work_zhangan_x_back_status = 0;
		
		}
		
		if(work_dianyang_w_l_lie_status == 1 || work_dianyang_w_s_status == 1 || work_dianyang_x_s_status == 1
		  || work_dianyang_w_s_lie_status == 1 || work_dianyang_x_s_hang_status == 1 || work_dianyang_w_m_lie_status == 1 
		  || work_dianyang_y_down_status == 1 || work_dianyanging_s_status == 1 || work_dianyang_x_m_status == 1 
		  || work_dianyang_x_l_status == 1 || work_dianyang_y_up_status == 1 || work_dianyang_x_back_status == 1 
		  || work_dianyang_x_get_zero_status == 1 || work_dianyang_w_get_zero_status == 1)
		  {
				PulseOut_1_Stop();
				PulseOut_2_Stop();
				PulseOut_3_Stop();
			  
				work_dianyang_w_l_lie_status = 0;
				work_dianyang_w_s_status = 0;
				work_dianyang_x_s_status = 0;
				work_dianyang_w_s_lie_status = 0;
				work_dianyang_x_s_hang_status = 0;
				work_dianyang_w_m_lie_status = 0;

				work_dianyang_y_down_status = 0;
				work_dianyanging_s_status = 0;
				work_dianyang_x_m_status = 0;
				work_dianyang_x_l_status= 0;

				work_dianyang_y_up_status = 0;

				work_dianyang_x_back_status = 0;
				
				work_dianyang_x_get_zero_status = 0;
				work_dianyang_w_get_zero_status = 0;
				
		  }
		
	}
	
	//方案一：静态制备方案，不回零处理		
	if(start_b == 1 && move_allow == 1 && power_previous == 1 && start_b_status == 0)
		{
			start_b_status = 1;
			
			
		}



	//方案二：动态制备方案，回零处理
	if(start_once == 1 && move_allow == 1 && power_zero == 1 )//&& start_b_status == 0
	{
		start_once_status = 1;
		
		//上电回零
		work_get_zero();
		
		
		//回零上电1st进行清洗工作
		if(work_zero_x_finish == 1 && work_zero_y_finish == 1 && work_zero_w_finish == 1 && work_clean_start_status == 0)
		{
			work_clean();
		}
		
		if(work_clean_x_finish == 1 && work_clean_y_finish == 1 && work_cleaning_finish == 1 && work_clean_back_finish == 1)
		{
			work_clean_x_finish = 0;
			work_clean_y_finish = 0;
			work_cleaning_finish = 0;
			work_clean_back_finish = 0;
			
			work_clean_start_status = 1;	//点样结束完成之后将此置为1
			
			quyang_allow = 1;
		}
		
		
		//clean工作完成后，进行取样工作
		if(quyang_allow == 1 && quyang_status == 0 && work_quyang_start_status == 0)//取样功能
		{
			work_quyang();
		}
		
		if(work_quyang_x_finish == 1 && work_quyang_y_finish == 1 && work_quyanging_finish == 1 && work_quyang_back_finish == 1)
		{
			quyang_allow = 0;
			
			work_quyang_x_finish = 0;
			work_quyang_y_finish = 0;
			work_quyanging_finish = 0;
			work_quyang_back_finish = 0;
			
			work_quyang_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			pre_dianyang_allow = 1;
			
		}
		
		//取样工作完成后进行预点样工作
	 	if(pre_dianyang_allow == 1 && pre_dianyang_status == 0 && work_pre_dianyang_start_status == 0)
		{
			work_pre_dianyang();
		}
		
		if(work_pre_dianyang_x_finish == 1 && work_pre_dianyang_y_finish == 1 && work_pre_dianyanging_finish == 1 
		   && work_pre_dianyang_w_finish == 1 && work_pre_dianyang_back_finish == 1)//
		{
			pre_dianyang_allow = 0;
			
			work_pre_dianyang_x_finish = 0;
			work_pre_dianyang_y_finish = 0;
			
			work_pre_dianyanging_finish = 0;
			
			work_pre_dianyang_w_finish = 0;
			work_pre_dianyang_back_finish = 0;
			
			work_pre_dianyang_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			zhangan_allow = 1;
		} 
		
		//预点样工作完成后进行蘸干工作
	 	
 		if(zhangan_allow == 1 && zhangan_status == 0 && work_zhangan_start_status == 0)
		{
			work_zhangan();
			
			
		}
		
		 if(work_zhangan_x_finish == 1 && work_zhangan_y_finish == 1 && work_zhanganing_finish == 1 
		   && work_zhangan_w_finish == 1 && work_zhangan_back_finish == 1 && work_zhangan_x_back_finish == 1)//
		{
			zhangan_allow = 0;
			
			work_zhangan_x_finish = 0;
			work_zhangan_y_finish = 0;
			
			work_zhanganing_finish = 0;
			
			work_zhangan_w_finish = 0;
			work_zhangan_back_finish = 0;
			
			work_zhangan_start_status = 1;	//点样结束完成之后将此置为0,进行循环
			
			dianyang_allow = 1;
		}   
		
		
		if(dianyang_allow == 1 && dianyang_c_status == 0 )//&& work_dianyang_c_start_status == 0
		{
			
			work_dianyang_continuous();
			
		}
		
		
		
	}
	
	
	else if(start_once == 0 && start_once_status == 1)
	{
 		
		start_once_status = 0;
		
		if (work_zero_x_status == 1 || work_zero_y_status == 1 || work_zero_w_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
		/* 	work_zero_x_status = 0;
			work_zero_y_status = 0;
			work_zero_w_status = 0; */
			
		}
		
		
		if (work_clean_x_status == 1 || work_clean_y_status == 1 || work_clean_back_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_clean_x_status = 0;
			work_clean_y_status = 0;
			work_clean_back_status = 0;	

		} 
		
		
		//取样位置按停
		if (work_quyang_x_status == 1 || work_quyang_y_status == 1 || work_quyang_back_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_quyang_x_status = 0;
			work_quyang_y_status = 0;
			work_quyang_back_status = 0;	

		} 
		
		
		//预点样位置按停
		if (work_pre_dianyang_x_status == 1 || work_pre_dianyang_y_status == 1 || work_pre_dianyang_back_status == 1 
		   || work_pre_dianyang_w_status == 1)
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_pre_dianyang_x_status = 0;
			work_pre_dianyang_y_status = 0;
			work_pre_dianyang_w_status = 0;
			
			work_pre_dianyang_back_status = 0;	

		}
		
		
		//蘸干位置按停
		if(work_zhangan_x_status == 1 || work_zhangan_w_status == 1 || work_zhangan_y_status == 1  
		|| work_zhangan_back_status == 1 || work_zhangan_w_back_status == 1 || work_zhangan_x_back_status == 1)//|| work_zhanganing_status == 1
		{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
			
			work_zhangan_x_status = 0;
			work_zhangan_w_status = 0;
			work_zhangan_y_status = 0;
			
			work_zhanganing_status = 0;
			work_zhangan_back_status = 0;
			work_zhangan_w_back_status = 0;
			work_zhangan_x_back_status = 0;
		
		}
		
		
		//点样位置按停
	/* 	if(work_dianyang_w_l_lie_status == 1 || work_dianyang_w_s_status == 1 || work_dianyang_x_s_status == 1
		  || work_dianyang_w_s_lie_status == 1 || work_dianyang_x_s_hang_status == 1 || work_dianyang_w_m_lie_status == 1 
		  || work_dianyang_y_down_status == 1 || work_dianyanging_s_status == 1 || work_dianyang_x_m_status == 1 
		  || work_dianyang_x_l_status == 1 || work_dianyang_y_up_status == 1 || work_dianyang_x_back_status == 1 
		  || work_dianyang_x_get_zero_status == 1 || work_dianyang_w_get_zero_status == 1)
		  {
				PulseOut_1_Stop();
				PulseOut_2_Stop();
				PulseOut_3_Stop();
			  
				work_dianyang_w_l_lie_status = 0;
				work_dianyang_w_s_status = 0;
				work_dianyang_x_s_status = 0;
				work_dianyang_w_s_lie_status = 0;
				work_dianyang_x_s_hang_status = 0;
				work_dianyang_w_m_lie_status = 0;

				work_dianyang_y_down_status = 0;
				work_dianyanging_s_status = 0;
				work_dianyang_x_m_status = 0;
				work_dianyang_x_l_status= 0;

				work_dianyang_y_up_status = 0;

				work_dianyang_x_back_status = 0;
				
				work_dianyang_x_get_zero_status = 0;
				work_dianyang_w_get_zero_status = 0;
				
		  } */
		
	}		
		
	//wdt_test by 方佳伟
		/* if(start_once == 1)
		{
			while(1)
			{
				
			}
			
		} */
}





