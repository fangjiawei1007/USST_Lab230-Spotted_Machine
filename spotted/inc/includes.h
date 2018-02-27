#ifndef _INCLUDES_H_
#define _INCLUDES_H_
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

#include "..\inc\app.h"
#include "..\inc\appBase.h"

#include "basic.h"



#define Y1_pulse_remember_add		(g_InteralMemory.KeepWord[490])
#define Y2_pulse_remember_add		(g_InteralMemory.KeepWord[491])
#define Y3_pulse_remember_add		(g_InteralMemory.KeepWord[492])

#define Y1_pulse_remember_back_add	(g_InteralMemory.KeepWord[493])

#define Y1_pulse_remember		(g_InteralMemory.KeepWord[501])
#define Y2_pulse_remember		(g_InteralMemory.KeepWord[502])
#define Y3_pulse_remember		(g_InteralMemory.KeepWord[503])

#define tmp_dianyang_danyuan_lieshu		(g_InteralMemory.KeepWord[504])
#define tmp_dianyang_danyuan_hangshu 	(g_InteralMemory.KeepWord[505])
	
#define tmp_dianyang_jiance_lieshu 		(g_InteralMemory.KeepWord[506])
#define tmp_dianyang_jiance_hangshu 	(g_InteralMemory.KeepWord[507])
	
#define tmp_dianyang_dianzhen_no		(g_InteralMemory.KeepWord[508])
#define tmp_dianyang_yangpin_no 		(g_InteralMemory.KeepWord[509])

#define Y1_dianyang_x_back_remember		(g_InteralMemory.KeepWord[510])
#define Y3_dianyang_w_remember			(g_InteralMemory.KeepWord[511])

#define tmp_dianyang_dianzhen_memory	(g_InteralMemory.KeepWord[512])	

#define spotted_c_remember				(g_InteralMemory.KeepWord[513])
#define tmp_dianyang_times				(g_InteralMemory.KeepWord[514])

#define c_s_distance					(g_InteralMemory.KeepWord[515])
#define c_m_distance					(g_InteralMemory.KeepWord[516])
#define c_l_distance					(g_InteralMemory.KeepWord[517])


extern INTERALMEMORY g_InteralMemory;

extern  unsigned int start_mode;

extern  unsigned int power_flag;
	
extern	unsigned int x_diandong_flag;
extern	unsigned int y_diandong_flag;
extern	unsigned int w_diandong_flag;
	
extern	unsigned int x_dingchang_flag;
extern	unsigned int y_dingchang_flag;
extern	unsigned int w_dingchang_flag;
	
extern	unsigned int weitiao_flag;
extern	unsigned int mode_flag;

extern  unsigned int status_record1;
extern  unsigned int status_record2;

extern  unsigned int jog_status[3];	//点动标志位
extern unsigned int jog_monitor_status[3];
extern  unsigned int dingchang_status[3];
extern  unsigned int dingchang_monitor_status[3];
extern  unsigned int get_zero_status[3];
extern 	unsigned int weitiao_status[3];
//extern  unsigned int condition_flag;//点动标志位

extern  INT16U	x_jinji_speed_t;
extern	INT16U	x_jianxie_speed_t;
extern	INT16U	x_diandong_speed_t;
extern	INT16U	x_huicheng_speed_t;
		
extern	INT16U	w_jianxie_speed_t;
extern	INT16U	w_lianxu_speed_t;
extern	INT16U	w_diandong_speed_t;
		
extern	INT16U	y_shangzhen_speed_t;
extern	INT16U	y_xiazheng_speed_t;
extern	INT16U	y_diandong_speed_t;
		
		//手动测试
extern	INT16U	dingchang_x_t;
extern	INT16U	dingchang_y_t;
extern	INT16U	dingchang_w_t;
		
		//单元初始化
extern	INT16U	danyuan_hangshu_t;
extern	INT16U	danyuan_lieshu_t;
extern	INT16U	danyuan_hangjianju_t;
extern	INT16U	danyuan_liejianju_t;
		
		//样品初始化
extern	INT16U	yangpin_hangshu_t;
extern	INT16U	yangpin_lieshu_t;
extern	INT16U	yangpin_bianhao_t;
		
		//检测窗
extern	INT16U	jiance_hangshu_t;
extern	INT16U	jiance_lieshu_t;
extern	INT16U	jiance_hangjianju_t;
extern	INT16U	jiance_liejianju_t;
extern	INT16U	jiance_gaodu_t;
extern	INT16U	jiance_kuandu_t;
		
		//点阵
extern	INT16U	dianzhen_hangshu_t;
extern	INT16U	dianzhen_lieshu_t;
extern	INT16U	dianzhen_hangjianju_t;
extern	INT16U	dianzhen_liejianju_t;
extern	INT16U	dianzhen_bianhao_t;
		
		//点样针参数
extern	INT16U	delaytime_t;
extern	INT16U	pulsewidth_t;
extern	INT16U	dianyang_voltage_t;
extern	INT16U	dianyang_speed_t;
extern	INT16U	xiye_speed_t;
extern	INT16U	tuye_speed_t;
extern	INT16U	clean_times_t;
extern	INT16U	dianyang_times_t;
extern	INT16U	pinshan_freq_t;
		
		//补充点样
extern	INT16U	sup_dy_hang_t;
extern	INT16U	sup_dy_lie_t;
extern	INT16U	sup_jc_hang_t;
extern	INT16U	sup_jc_lie_t;
extern	INT16U	sup_yp_t;
extern	INT16U	sup_dz_t;
		
		//工作位置
extern	INT16U	gongzuo_x_t;
extern	INT16U	gongzuo_y_t;
extern	INT16U	gongzuo_w_t;
		
extern	INT16U	clean_x_t;
extern	INT16U	clean_y_t;
extern	INT16U	clean_w_t;
		
extern	INT16U	quyang_x_t;
extern	INT16U	quyang_y_t;
extern	INT16U	quyang_w_t;
		
extern	INT16U	zhangan_x_t;
extern	INT16U	zhangan_y_t;
extern	INT16U	zhangan_w_t;
		
extern	INT16U	pingshan_x_t;
extern	INT16U	pingshan_y_t;
extern	INT16U	pingshan_w_t;
		
extern	INT16U	tuye_x_t;
extern	INT16U	tuye_y_t;
extern	INT16U	tuye_w_t;

extern volatile unsigned int OutPulse_Counter[4];
extern unsigned int gongzuo_flag_x;
extern unsigned int gongzuo_flag_y;
extern unsigned int gongzuo_flag_w;

extern unsigned int clean_flag_x;
extern unsigned int clean_flag_y;
extern unsigned int clean_flag_w;

extern unsigned int quyang_flag_x;
extern unsigned int quyang_flag_y;
extern unsigned int quyang_flag_w;

extern unsigned int zhangan_flag_x;
extern unsigned int zhangan_flag_y;
extern unsigned int zhangan_flag_w;

extern unsigned int pingshan_flag_x;
extern unsigned int pingshan_flag_y;
extern unsigned int pingshan_flag_w;

extern unsigned int tuye_flag_x;
extern unsigned int tuye_flag_y;
extern unsigned int tuye_flag_w;
	
extern unsigned int xitong_c_status[6];
extern unsigned int xitong_status[6];
extern unsigned int weitiao_x_status[6];
extern unsigned int weitiao_y_status[6];
extern unsigned int weitiao_w_status[6];
extern unsigned int confirm_status[6];
extern unsigned int confirm_monitor_x_status[6];
extern unsigned int confirm_monitor_y_status[6];
extern unsigned int confirm_monitor_w_status[6];

extern int monitor_gongzuo_flag[3];
extern int monitor_clean_flag[3];
extern int monitor_quyang_flag[3];
extern int monitor_zhangan_flag[3];
extern int monitor_pingshan_flag[3];
extern int monitor_tuye_flag[3];

extern unsigned int start_b_status;
extern unsigned int start_once_status;

extern unsigned int work_zero_x_status;
extern unsigned int work_zero_y_status;
extern unsigned int work_zero_w_status;
extern unsigned int work_zero_x_monitor_status;
extern unsigned int work_zero_y_monitor_status;
extern unsigned int work_zero_w_monitor_status;


extern unsigned int work_clean_x_status;
extern unsigned int work_clean_x_finish;
	
extern unsigned int work_clean_y_status;
extern unsigned int work_clean_y_finish;
	
extern unsigned int work_cleaning_finish;
extern unsigned int work_clean_back_status;

extern unsigned int work_clean_x_monitor_status;
extern unsigned int work_clean_y_monitor_status;
extern unsigned int work_clean_back_monitor_status;
extern unsigned int work_clean_back_finish;


extern unsigned int work_zero_x_finish;
extern unsigned int work_zero_y_finish;
extern unsigned int work_zero_w_finish;

extern unsigned int work_zero_x_finish;
extern unsigned int work_zero_y_finish;
extern unsigned int work_zero_w_finish;

extern unsigned int work_clean_start_status;

extern unsigned int quyang_allow;
extern unsigned int quyang_status;

extern unsigned int work_quyang_x_status;
extern unsigned int work_quyang_x_finish;
	
extern unsigned int work_quyang_y_status;
extern unsigned int work_quyang_y_finish;
	
extern unsigned int work_quyang_x_monitor_status;
extern unsigned int work_quyang_y_monitor_status;
extern unsigned int work_quyang_back_monitor_status;
	
extern unsigned int work_quyanging_status;
extern unsigned int work_quyanging_finish;
extern unsigned int work_quyang_back_status;
extern unsigned int work_quyang_back_finish;
extern unsigned int work_quyang_start_status;


extern unsigned int work_pre_dianyang_x_status;
extern unsigned int work_pre_dianyang_x_finish;
	
extern unsigned int work_pre_dianyang_y_status;
extern unsigned int work_pre_dianyang_y_finish;
	
extern unsigned int work_pre_dianyang_x_monitor_status;
extern unsigned int work_pre_dianyang_y_monitor_status;
extern unsigned int work_pre_dianyang_back_monitor_status;
	
extern unsigned int work_pre_dianyanging_status;
extern unsigned int work_pre_dianyanging_finish;
extern unsigned int work_pre_dianyang_back_status;
extern unsigned int work_pre_dianyang_back_finish;
extern unsigned int work_pre_dianyang_start_status;
	
extern unsigned int work_pre_dianyang_w_status;
extern unsigned int work_pre_dianyang_w_finish;
extern unsigned int work_pre_dianyang_w_monitor_status;

extern unsigned int pre_dianyang_allow;
extern unsigned int pre_dianyang_status;

extern unsigned int zhangan_allow;
extern unsigned int zhangan_status;
	
extern unsigned int work_zhangan_x_status;
extern unsigned int work_zhangan_x_finish;
	
extern unsigned int work_zhangan_y_status;
extern unsigned int work_zhangan_y_finish;
	
extern unsigned int work_zhangan_x_monitor_status;
extern unsigned int work_zhangan_y_monitor_status;
extern unsigned int work_zhangan_back_monitor_status;
	
extern unsigned int work_zhanganing_status;
extern unsigned int work_zhanganing_finish;
extern unsigned int work_zhangan_back_status;
extern unsigned int work_zhangan_back_finish;
extern unsigned int work_zhangan_start_status;
	
extern unsigned int work_zhangan_w_status;
extern unsigned int work_zhangan_w_finish;
extern unsigned int work_zhangan_w_monitor_status;
	
extern unsigned int work_zhangan_w_back_status;
extern unsigned int work_zhangan_w_back_monitor_status;
extern unsigned int work_zhangan_w_back_finish;
	
extern unsigned int work_zhangan_x_back_status;
extern unsigned int work_zhangan_x_back_monitor_status;
extern unsigned int work_zhangan_x_back_finish;
	
extern unsigned int dianyang_allow;

extern unsigned int dianyang_status;


	
	
extern unsigned int work_dianyang_x_s_status;
extern unsigned int work_dianyang_x_s_finish ;
extern unsigned int work_dianyang_x_s_monitor_status ;
	
extern unsigned int work_dianyang_x_m_status ;
extern unsigned int work_dianyang_x_m_finish ;
extern unsigned int work_dianyang_x_m_monitor_status ;

extern unsigned int work_dianyang_x_l_status;
extern unsigned int work_dianyang_x_l_finish ;
extern unsigned int work_dianyang_x_l_monitor_status ;
	
extern unsigned int work_dianyang_y_down_monitor_status;
extern unsigned int work_dianyang_y_down_status;
extern unsigned int work_dianyang_y_down_finish;

extern unsigned int work_dianyang_y_up_monitor_status;
extern unsigned int work_dianyang_y_up_status;
extern unsigned int work_dianyang_y_up_finish;

extern unsigned int work_dianyanging_s_status;
extern unsigned int work_dianyanging_s_finish;
extern unsigned int work_dianyanging_m_status;
extern unsigned int work_dianyanging_m_finish;	
extern unsigned int work_dianyanging_l_status;
extern unsigned int work_dianyanging_l_finish;
	
extern unsigned int work_dianyang_back_status;
extern unsigned int work_dianyang_back_finish;
extern unsigned int work_dianyang_start_status;
	
extern unsigned int work_dianyang_w_s_status;
extern unsigned int work_dianyang_w_s_finish;
extern unsigned int work_dianyang_w_s_monitor_status;
	
extern unsigned int work_dianyang_w_m_status ;
extern unsigned int work_dianyang_w_m_finish ;
extern unsigned int work_dianyang_w_m_monitor_status ;
	
extern unsigned int work_dianyang_w_l_status;
extern unsigned int work_dianyang_w_l_finish ;
extern unsigned int work_dianyang_w_l_monitor_status;
	
	
extern unsigned int work_dianyang_w_back_status;
extern unsigned int work_dianyang_w_back_monitor_status;
extern unsigned int work_dianyang_w_back_finish ;

extern unsigned int work_dianyang_x_back_status;
extern unsigned int work_dianyang_x_back_monitor_status;
extern unsigned int work_dianyang_x_back_finish ;

extern unsigned int work_dianyang_w_s_lie_status;
extern unsigned int work_dianyang_w_s_lie_finish;
extern unsigned int work_dianyang_w_s_lie_monitor_status;
		
extern unsigned int work_dianyang_x_s_hang_status;
extern unsigned int work_dianyang_x_s_hang_finish;
extern unsigned int work_dianyang_x_s_hang_monitor_status;		
		
	
extern unsigned int work_dianyang_w_m_lie_status;
extern unsigned int work_dianyang_w_m_lie_finish;
extern unsigned int work_dianyang_w_m_lie_monitor_status;

extern unsigned int work_dianyang_w_l_lie_status;
extern unsigned int work_dianyang_w_l_lie_finish;
extern unsigned int work_dianyang_w_l_lie_monitor_status;

extern unsigned int work_dianyang_allow;

extern unsigned int work_dianyang_x_l_dianyang_status;

extern unsigned int work_dianyang_x_get_zero_status;
extern unsigned int work_dianyang_x_get_zero_finish;
extern unsigned int work_dianyang_x_get_zero_monitor_status;

extern unsigned int work_dianyang_w_get_zero_status;
extern unsigned int work_dianyang_w_get_zero_finish;
extern unsigned int work_dianyang_w_get_zero_monitor_status;

extern unsigned int work_dianyang_again_status;
extern unsigned int work_dianyang_again_finish;
extern unsigned int work_dianyang_again_counter;

extern unsigned int work_dianyang_c_x_s_status;
extern unsigned int work_dianyang_c_x_s_finish;
extern unsigned int work_dianyang_c_x_s_monitor_status;
	
extern unsigned int work_dianyang_c_x_s_hang_status;
extern unsigned int work_dianyang_c_x_s_hang_finish;
extern unsigned int work_dianyang_c_x_s_hang_monitor_status;
 
extern unsigned int work_dianyang_c_x_m_status;
extern unsigned int work_dianyang_c_x_m_finish;
extern unsigned int work_dianyang_c_x_m_monitor_status;

extern unsigned int work_dianyang_c_x_l_status;
extern unsigned int work_dianyang_c_x_l_finish;
extern unsigned int work_dianyang_c_x_l_monitor_status;
 
extern unsigned int work_dianyang_c_w_s_status;
extern unsigned int work_dianyang_c_w_s_finish;
extern unsigned int work_dianyang_c_w_s_monitor_status;
 
extern unsigned int work_dianyang_c_w_m_status;
extern unsigned int work_dianyang_c_w_m_finish;
extern unsigned int work_dianyang_c_w_m_monitor_status;
 
extern unsigned int work_dianyang_c_w_l_status;
extern unsigned int work_dianyang_c_w_l_finish;
extern unsigned int work_dianyang_c_w_l_monitor_status;

extern unsigned int work_dianyang_c_w_back_status;
extern unsigned int work_dianyang_c_w_back_monitor_status;
extern unsigned int work_dianyang_c_w_back_finish;

extern unsigned int work_dianyang_c_x_back_status;
extern unsigned int work_dianyang_c_x_back_monitor_status;
extern unsigned int work_dianyang_c_x_back_finish;
 
extern unsigned int work_dianyang_c_y_down_status;
extern unsigned int work_dianyang_c_y_down_finish;
extern unsigned int work_dianyang_c_y_down_monitor_status;

extern unsigned int work_dianyang_c_y_up_status;
extern unsigned int work_dianyang_c_y_up_finish;
extern unsigned int work_dianyang_c_y_up_monitor_status;

extern unsigned int work_dianyang_c_w_start_status;
extern unsigned int work_dianyang_c_w_start_monitor_status;
extern unsigned int work_dianyang_c_w_start_finish;
 
extern unsigned int work_dianyang_c_start;
extern unsigned int c_m_move_allow;
extern unsigned int work_dianyang_c_start_status;
 
extern unsigned int c_jiance_lieshu_status;
extern unsigned int c_l_move_allow;

extern unsigned int work_dianyang_c_working_finish;
extern unsigned int dianyang_c_status;

extern unsigned int work_dianyang_c_w_stop_status;
extern unsigned int work_dianyang_c_w_stop_status_monitor;
extern unsigned int work_dianyang_c_w_stop_finish;

extern unsigned int tmp_dianyang_jiance_hangshu_last_status;
extern unsigned int c_s_move_allow;

extern unsigned int x_get_zero_status;
extern unsigned int y_get_zero_status;
extern unsigned int w_get_zero_status;

extern unsigned int x_get_zero_keep_status;
extern unsigned int y_get_zero_keep_status;
extern unsigned int w_get_zero_keep_status;
	
extern unsigned int x_get_zero_keep_monitor;
extern unsigned int y_get_zero_keep_monitor;
extern unsigned int w_get_zero_keep_monitor;

extern unsigned int Y1_PULSE_TOTAL;
extern unsigned int Y2_PULSE_TOTAL;
extern unsigned int Y3_PULSE_TOTAL;

extern unsigned int Y1_PULSE_TOTAL_BACK;


extern unsigned int biaoding_status[3];

extern unsigned int frequence_Y1;
extern unsigned int frequence_Y2;
extern unsigned int frequence_Y3;

extern unsigned int Y1_BIAODING_PULSE_TOTAL;
extern unsigned int Y2_BIAODING_PULSE_TOTAL;
extern unsigned int Y3_BIAODING_PULSE_TOTAL;
#endif