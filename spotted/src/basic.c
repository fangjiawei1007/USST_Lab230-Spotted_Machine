#include "includes.h"
	INT16U	x_jinji_speed_t   = 100;
	INT16U	x_jianxie_speed_t = 100;
	INT16U	x_diandong_speed_t = 100;
	INT16U	x_huicheng_speed_t = 100;
		
	INT16U	w_jianxie_speed_t = 100;
	INT16U	w_lianxu_speed_t = 100;
	INT16U	w_diandong_speed_t = 100;
		
	INT16U	y_shangzhen_speed_t = 100;
	INT16U	y_xiazheng_speed_t = 100;
	INT16U	y_diandong_speed_t = 100;
		
		//手动测试
	INT16U	dingchang_x_t = 0;
	INT16U	dingchang_y_t = 0;
	INT16U	dingchang_w_t = 0;
		
		//单元初始化
	INT16U	danyuan_hangshu_t = 0;
	INT16U	danyuan_lieshu_t = 0;
	INT16U	danyuan_hangjianju_t = 0;
	INT16U	danyuan_liejianju_t = 0;
		
		//样品初始化
	INT16U	yangpin_hangshu_t = 0;
	INT16U	yangpin_lieshu_t = 0;
	INT16U	yangpin_bianhao_t = 0;
		
		//检测窗
	INT16U	jiance_hangshu_t = 0;
	INT16U	jiance_lieshu_t = 0;
	INT16U	jiance_hangjianju_t = 0;
	INT16U	jiance_liejianju_t = 0;
	INT16U	jiance_gaodu_t = 0;
	INT16U	jiance_kuandu_t = 0;
		
		//点阵
	INT16U	dianzhen_hangshu_t = 0;
	INT16U	dianzhen_lieshu_t = 0;
	INT16U	dianzhen_hangjianju_t = 0;
	INT16U	dianzhen_liejianju_t = 0;
	INT16U	dianzhen_bianhao_t = 0;
		
		//点样针参数
	INT16U	delaytime_t = 0;
	INT16U	pulsewidth_t = 0;
	INT16U	dianyang_voltage_t = 0;
	INT16U	dianyang_speed_t = 0;
	INT16U	xiye_speed_t = 0;
	INT16U	tuye_speed_t = 0;
	INT16U	clean_times_t = 0;
	INT16U	dianyang_times_t = 0;
	INT16U	pinshan_freq_t = 0;
		
		//补充点样
	INT16U	sup_dy_hang_t = 0;
	INT16U	sup_dy_lie_t = 0;
	INT16U	sup_jc_hang_t = 0;
	INT16U	sup_jc_lie_t = 0;
	INT16U	sup_yp_t = 0;
	INT16U	sup_dz_t = 0;
		
		//工作位置
	INT16U	gongzuo_x_t = 0;
	INT16U	gongzuo_y_t = 0;
	INT16U	gongzuo_w_t = 0;
		
	INT16U	clean_x_t = 0;
	INT16U	clean_y_t = 0;
	INT16U	clean_w_t = 0;
		
	INT16U	quyang_x_t = 0;
	INT16U	quyang_y_t = 0;
	INT16U	quyang_w_t = 0;
		
	INT16U	zhangan_x_t = 0;
	INT16U	zhangan_y_t = 0;
	INT16U	zhangan_w_t = 0;
		
	INT16U	pingshan_x_t = 0;
	INT16U	pingshan_y_t = 0;
	INT16U	pingshan_w_t = 0;
		
	INT16U	tuye_x_t = 0;
	INT16U	tuye_y_t = 0;
	INT16U	tuye_w_t = 0;
	
	unsigned int biaoding_status[3] = {0};//三轴标定移动位置
	
	unsigned int jog_status[3] = {0};
	unsigned int jog_monitor_status[3] = {0};
	unsigned int dingchang_status[3] = {0};
	unsigned int dingchang_monitor_status[3] = {0};
	unsigned int get_zero_status[3] = {0};
	unsigned int weitiao_status[3] = {0};	//三根轴微调的标志位
	unsigned int xitong_status[6] = {0};
	unsigned int confirm_status[6] = {0};
	
	unsigned int confirm_monitor_x_status[6] = {0};
	unsigned int confirm_monitor_y_status[6] = {0};
	unsigned int confirm_monitor_w_status[6] = {0};
	
	unsigned int xitong_c_status[6] = {0};	//确认是哪个轴转完之后进行回零
	
	unsigned int weitiao_x_status[6] = {0};	//微调status系统六个位置
	unsigned int weitiao_y_status[6] = {0};
	unsigned int weitiao_w_status[6] = {0};
	
	
	unsigned int gongzuo_flag_x = 0;
	unsigned int gongzuo_flag_y = 0;
	unsigned int gongzuo_flag_w = 0;
	
	unsigned int clean_flag_x = 0;
	unsigned int clean_flag_y = 0;
	unsigned int clean_flag_w = 0;
	
	unsigned int quyang_flag_x = 0;
	unsigned int quyang_flag_y = 0;
	unsigned int quyang_flag_w = 0;
	
	unsigned int zhangan_flag_x = 0;
	unsigned int zhangan_flag_y = 0;
	unsigned int zhangan_flag_w = 0;
	
	unsigned int pingshan_flag_x = 0;
	unsigned int pingshan_flag_y = 0;
	unsigned int pingshan_flag_w = 0;
	
	unsigned int tuye_flag_x = 0;
	unsigned int tuye_flag_y = 0;
	unsigned int tuye_flag_w = 0;

	int gongzuo_position_x = 0;
	int gongzuo_position_y = 0;
	int gongzuo_position_w = 0;
	
	int clean_position_x = 0;
	int clean_position_y = 0;
	int clean_position_w = 0;
	
	int quyang_position_x = 0;
	int quyang_position_y = 0;
	int quyang_position_w = 0;
	
	int zhangan_position_x = 0;
	int zhangan_position_y = 0;
	int zhangan_position_w = 0;
	
	int pingshan_position_x = 0;
	int pingshan_position_y = 0;
	int pingshan_position_w = 0;
	
	int tuye_position_x = 0;
	int tuye_position_y = 0;
	int tuye_position_w = 0;
	
	
	//微调监控按钮 数组中0---X轴 1----Y轴  2-----W轴
	int monitor_gongzuo_flag[3] = {0};
	int monitor_clean_flag[3] = {0};
	int monitor_quyang_flag[3] = {0};
	int monitor_zhangan_flag[3] = {0};
	int monitor_pingshan_flag[3] = {0};
	int monitor_tuye_flag[3] = {0};
	
	
	//运动按钮
	unsigned int start_status[6] = {0};
	
	//上电正常工作标志位
	unsigned int start_b_status = 0;
	
	unsigned int work_zero_x_status = 0;
	unsigned int work_zero_y_status = 0;
	unsigned int work_zero_w_status = 0;
	
	unsigned int work_zero_x_monitor_status = 0;
	unsigned int work_zero_y_monitor_status = 0;
	unsigned int work_zero_w_monitor_status = 0;
	//unsigned int work_zero_x_monitor_status = 0;
	unsigned int work_zero_x_finish = 0;
	unsigned int work_zero_y_finish = 0;
	unsigned int work_zero_w_finish = 0;

	

	unsigned int work_clean_x_status = 0;
	unsigned int work_clean_x_finish = 0;
	
	unsigned int work_clean_y_status = 0;
	unsigned int work_clean_y_finish = 0;
	
	unsigned int work_clean_x_monitor_status = 0;
	unsigned int work_clean_y_monitor_status = 0;
	unsigned int work_clean_back_monitor_status = 0;
	
	
	unsigned int work_cleaning_status = 0;
	unsigned int work_cleaning_finish = 0;
	unsigned int work_clean_back_status = 0;
	unsigned int work_clean_back_finish = 0;
	unsigned int work_clean_start_status = 0;

	
	unsigned int quyang_allow = 0;
	unsigned int quyang_status = 0;
	
	unsigned int work_quyang_x_status = 0;
	unsigned int work_quyang_x_finish = 0;
	
	unsigned int work_quyang_y_status = 0;
	unsigned int work_quyang_y_finish = 0;
	
	unsigned int work_quyang_x_monitor_status = 0;
	unsigned int work_quyang_y_monitor_status = 0;
	unsigned int work_quyang_back_monitor_status = 0;
	
	unsigned int work_quyanging_status = 0;
	unsigned int work_quyanging_finish = 0;
	unsigned int work_quyang_back_status = 0;
	unsigned int work_quyang_back_finish = 0;
	unsigned int work_quyang_start_status = 0;

	
	unsigned int pre_dianyang_allow = 0;
	unsigned int pre_dianyang_status = 0;
	
	unsigned int work_pre_dianyang_x_status = 0;
	unsigned int work_pre_dianyang_x_finish = 0;
	
	unsigned int work_pre_dianyang_y_status = 0;
	unsigned int work_pre_dianyang_y_finish = 0;
	
	unsigned int work_pre_dianyang_x_monitor_status = 0;
	unsigned int work_pre_dianyang_y_monitor_status = 0;
	unsigned int work_pre_dianyang_back_monitor_status = 0;
	
	unsigned int work_pre_dianyanging_status = 0;
	unsigned int work_pre_dianyanging_finish = 0;
	unsigned int work_pre_dianyang_back_status = 0;
	unsigned int work_pre_dianyang_back_finish = 0;
	unsigned int work_pre_dianyang_start_status = 0;
	
	unsigned int work_pre_dianyang_w_status = 0;
	unsigned int work_pre_dianyang_w_finish = 0;
	unsigned int work_pre_dianyang_w_monitor_status = 0;
	
	
	unsigned int zhangan_allow = 0;
	unsigned int zhangan_status = 0;
	
	unsigned int work_zhangan_x_status = 0;
	unsigned int work_zhangan_x_finish = 0;
	
	unsigned int work_zhangan_y_status = 0;
	unsigned int work_zhangan_y_finish = 0;
	
	unsigned int work_zhangan_x_monitor_status = 0;
	unsigned int work_zhangan_y_monitor_status = 0;
	unsigned int work_zhangan_back_monitor_status = 0;
	
	unsigned int work_zhanganing_status = 0;
	unsigned int work_zhanganing_finish = 0;
	unsigned int work_zhangan_back_status = 0;
	unsigned int work_zhangan_back_finish = 0;
	unsigned int work_zhangan_start_status = 0;
	
	unsigned int work_zhangan_w_status = 0;
	unsigned int work_zhangan_w_finish = 0;
	unsigned int work_zhangan_w_monitor_status = 0;
	
	unsigned int work_zhangan_w_back_status = 0;
	unsigned int work_zhangan_w_back_monitor_status = 0;
	unsigned int work_zhangan_w_back_finish = 0;
	
	unsigned int work_zhangan_x_back_status = 0;
	unsigned int work_zhangan_x_back_monitor_status = 0;
	unsigned int work_zhangan_x_back_finish = 0;
	
	unsigned int dianyang_allow = 0;
	
	unsigned int dianyang_status = 0;

	
	unsigned int dianyang_c_status = 0;
	
	unsigned int work_dianyang_x_s_status = 0;
	unsigned int work_dianyang_x_s_finish = 0;
	unsigned int work_dianyang_x_s_monitor_status = 0;
	
	unsigned int work_dianyang_x_m_status = 0;
	unsigned int work_dianyang_x_m_finish = 0;
	unsigned int work_dianyang_x_m_monitor_status = 0;

	unsigned int work_dianyang_x_l_status = 0;
	unsigned int work_dianyang_x_l_finish = 0;
	unsigned int work_dianyang_x_l_monitor_status = 0;
	
	unsigned int work_dianyang_w_s_lie_status = 0;
	unsigned int work_dianyang_w_s_lie_finish = 0;
	unsigned int work_dianyang_w_s_lie_monitor_status = 0;
		
	unsigned int work_dianyang_x_s_hang_status = 0;
	unsigned int work_dianyang_x_s_hang_finish = 0;
	unsigned int work_dianyang_x_s_hang_monitor_status = 0;		
		
	
	unsigned int work_dianyang_w_m_lie_status = 0;
	unsigned int work_dianyang_w_m_lie_finish = 0;
	unsigned int work_dianyang_w_m_lie_monitor_status = 0;

	unsigned int work_dianyang_w_l_lie_status = 0;
	unsigned int work_dianyang_w_l_lie_finish = 0;
	unsigned int work_dianyang_w_l_lie_monitor_status = 0;
	
	unsigned int work_dianyang_y_down_status = 0;
	unsigned int work_dianyang_y_down_finish = 0;	
	unsigned int work_dianyang_y_down_monitor_status = 0;
	
	unsigned int work_dianyang_y_up_status = 0;
	unsigned int work_dianyang_y_up_finish = 0;	
	unsigned int work_dianyang_y_up_monitor_status = 0;
	

	
	unsigned int work_dianyanging_s_status = 0;
	unsigned int work_dianyanging_s_finish = 0;
	unsigned int work_dianyanging_m_status = 0;
	unsigned int work_dianyanging_m_finish = 0;	
	unsigned int work_dianyanging_l_status = 0;
	unsigned int work_dianyanging_l_finish = 0;
	
	
	unsigned int work_dianyang_back_status = 0;
	unsigned int work_dianyang_back_finish = 0;
	unsigned int work_dianyang_start_status = 0;
	
	unsigned int work_dianyang_start_c_status = 0;
	
	unsigned int work_dianyang_w_s_status = 0;
	unsigned int work_dianyang_w_s_finish = 0;
	unsigned int work_dianyang_w_s_monitor_status = 0;
	
	unsigned int work_dianyang_w_m_status = 0;
	unsigned int work_dianyang_w_m_finish = 0;
	unsigned int work_dianyang_w_m_monitor_status = 0;
	
	unsigned int work_dianyang_w_l_status = 0;
	unsigned int work_dianyang_w_l_finish = 0;
	unsigned int work_dianyang_w_l_monitor_status = 0;
	
	
	unsigned int work_dianyang_w_back_status = 0;
	unsigned int work_dianyang_w_back_monitor_status = 0;
	unsigned int work_dianyang_w_back_finish = 0;
	
	unsigned int work_dianyang_x_back_status = 0;
	unsigned int work_dianyang_x_back_monitor_status = 0;
	unsigned int work_dianyang_x_back_finish = 0;
	
	unsigned int work_dianyang_allow = 0;
	
	unsigned int work_dianyang_x_l_dianyang_status = 0;
	unsigned int work_dianyang_x_l_lock_status = 0;
	
	unsigned int work_dianyang_again_status = 0;
	unsigned int work_dianyang_again_finish = 0;
	unsigned int work_dianyang_again_counter = 0;
	//上电回零工作标志位
	unsigned int start_once_status = 0;
	
	unsigned int work_dianyang_x_get_zero_status = 0;
	unsigned int work_dianyang_x_get_zero_finish = 0;
	unsigned int work_dianyang_x_get_zero_monitor_status = 0;

	unsigned int work_dianyang_w_get_zero_status = 0;
	unsigned int work_dianyang_w_get_zero_finish = 0;
	unsigned int work_dianyang_w_get_zero_monitor_status = 0;
	
	
	
	//连续点样标志位_c 即continuous
	unsigned int work_dianyang_c_x_s_status = 0;
	unsigned int work_dianyang_c_x_s_finish = 0;
	unsigned int work_dianyang_c_x_s_monitor_status = 0;
	
	unsigned int work_dianyang_c_x_s_hang_status = 0;
	unsigned int work_dianyang_c_x_s_hang_finish = 0;
	unsigned int work_dianyang_c_x_s_hang_monitor_status = 0;
	
	unsigned int work_dianyang_c_x_m_status = 0;
	unsigned int work_dianyang_c_x_m_finish = 0;
	unsigned int work_dianyang_c_x_m_monitor_status = 0;

	unsigned int work_dianyang_c_x_l_status = 0;
	unsigned int work_dianyang_c_x_l_finish = 0;
	unsigned int work_dianyang_c_x_l_monitor_status = 0;
	
	unsigned int work_dianyang_c_w_s_status = 0;
	unsigned int work_dianyang_c_w_s_finish = 0;
	unsigned int work_dianyang_c_w_s_monitor_status = 0;
	
	unsigned int work_dianyang_c_w_m_status = 0;
	unsigned int work_dianyang_c_w_m_finish = 0;
	unsigned int work_dianyang_c_w_m_monitor_status = 0;
	
	unsigned int work_dianyang_c_w_l_status = 0;
	unsigned int work_dianyang_c_w_l_finish = 0;
	unsigned int work_dianyang_c_w_l_monitor_status = 0;
	
	
	unsigned int work_dianyang_c_w_back_status = 0;
	unsigned int work_dianyang_c_w_back_monitor_status = 0;
	unsigned int work_dianyang_c_w_back_finish = 0;
	
	unsigned int work_dianyang_c_x_back_status = 0;
	unsigned int work_dianyang_c_x_back_monitor_status = 0;
	unsigned int work_dianyang_c_x_back_finish = 0;
	
	unsigned int work_dianyang_c_y_down_status = 0;
	unsigned int work_dianyang_c_y_down_finish = 0;	
	unsigned int work_dianyang_c_y_down_monitor_status = 0;
	
	unsigned int work_dianyang_c_y_up_status = 0;
	unsigned int work_dianyang_c_y_up_finish = 0;	
	unsigned int work_dianyang_c_y_up_monitor_status = 0;
	
	unsigned int work_dianyang_c_w_start_status = 0;
	unsigned int work_dianyang_c_w_start_monitor_status = 0;
	unsigned int work_dianyang_c_w_start_finish = 0;
	
	unsigned int work_dianyang_c_start = 0;
	unsigned int c_m_move_allow = 0;
	unsigned int work_dianyang_c_start_status = 0;
	
	unsigned int c_jiance_lieshu_status = 0;
	unsigned int c_l_move_allow = 0;

	unsigned int work_dianyang_c_working_finish = 0;
	
	unsigned int work_dianyang_c_w_stop_status = 0;
	unsigned int work_dianyang_c_w_stop_status_monitor = 0;
	unsigned int work_dianyang_c_w_stop_finish = 0;
	
	
	unsigned int tmp_dianyang_jiance_hangshu_last_status = 0;
	unsigned int c_s_move_allow = 0;
	
	unsigned int x_get_zero_status = 0;
	unsigned int y_get_zero_status = 0;
	unsigned int w_get_zero_status = 0;
	
	unsigned int x_get_zero_keep_status = 0;
	unsigned int y_get_zero_keep_status = 0;
	unsigned int w_get_zero_keep_status = 0;
	
	unsigned int x_get_zero_keep_monitor = 0;
	unsigned int y_get_zero_keep_monitor = 0;
	unsigned int w_get_zero_keep_monitor = 0;
	
	unsigned int Y1_PULSE_TOTAL = 0;
	unsigned int Y2_PULSE_TOTAL = 0;
	unsigned int Y3_PULSE_TOTAL = 0;
	unsigned int Y1_PULSE_TOTAL_BACK = 0;

	unsigned int Y1_BIAODING_PULSE_TOTAL = 0;
	unsigned int Y2_BIAODING_PULSE_TOTAL = 0;
	unsigned int Y3_BIAODING_PULSE_TOTAL = 0;
	unsigned int Y1_BIAODING_PULSE_TOTAL_BACK = 0;
	
	unsigned int Y1_BIAODING_LENGTH_TOTAL = 0;
	unsigned int Y2_BIAODING_LENGTH_TOTAL = 0;
	unsigned int Y3_BIAODING_LENGTH_TOTAL = 0;
	
//点样仪函数功能模块实现
void parameter_init(void)
{
	if (init_flag != 1)
	{
		//运动参数设置
		x_jinji_speed = 3000;
		x_jianxie_speed = 3000;
		x_diandong_speed = 3000;
		x_huicheng_speed = 3000;
		
		w_jianxie_speed = 3000;
		w_lianxu_speed = 3000;
		w_diandong_speed = 3000;
		
		y_shangzhen_speed = 2000;
		y_xiazheng_speed = 2000;
		y_diandong_speed = 2000;
		
		//手动测试
		dingchang_x = 0;
		dingchang_w = 0;
		dingchang_w = 0;
		
		//单元初始化
		danyuan_hangshu = 0;
		danyuan_lieshu = 0;
		danyuan_hangjianju = 0;
		danyuan_liejianju = 0;
		
		//样品初始化
		yangpin_hangshu = 0;
		yangpin_lieshu = 0;
		yangpin_bianhao = 0;
		
		//检测窗
		jiance_hangshu = 0;
		jiance_lieshu = 0;
		jiance_hangjianju = 0;
		jiance_liejianju = 0;
		jiance_gaodu = 0;
		jiance_kuandu = 0;
		
		//点阵
		dianzhen_hangshu = 0;
		dianzhen_lieshu = 0;
		dianzhen_hangjianju = 0;
		dianzhen_liejianju = 0;
		dianzhen_bianhao = 0;
		
		//点样针参数
		delaytime = 0;
		pulsewidth = 0;
		dianyang_voltage = 0;
		dianyang_speed = 0;
		xiye_speed = 0;
		tuye_speed = 0;
		clean_times = 0;
		dianyang_times = 0;
		pinshan_freq = 0;
		
		//补充点样
		sup_dy_hang = 0;
		sup_dy_lie = 0;
		sup_jc_hang = 0;
		sup_jc_lie = 0;
		sup_yp = 0;
		sup_dz = 0;
		
		//工作位置
		gongzuo_x = 0;
		gongzuo_y = 0;
		gongzuo_w = 0;
		
		clean_x = 0;
		clean_y = 0;
		clean_w = 0;
		
		quyang_x = 0;
		quyang_y = 0;
		quyang_w = 0;
		
		zhangan_x = 0;
		zhangan_y = 0;
		zhangan_w = 0;
		
		pingshan_x = 0;
		pingshan_y = 0;
		pingshan_w = 0;
		
		tuye_x = 0;
		tuye_y = 0;
		tuye_w = 0;
		
		Y1_pulse_remember = 0;
		Y2_pulse_remember = 0;
		Y3_pulse_remember = 0;
		
		Y1_pulse_remember_add = 0;
		Y2_pulse_remember_add = 0;
		Y3_pulse_remember_add = 0;
		
		Y1_pulse_remember_back_add = 0;
		
		tmp_dianyang_danyuan_lieshu = 1;
		tmp_dianyang_danyuan_hangshu = 1;
	
	
		tmp_dianyang_jiance_lieshu = 1;
		tmp_dianyang_jiance_hangshu = 1;
	
		tmp_dianyang_dianzhen_no = 0;
		tmp_dianyang_yangpin_no = 0;
		
		monitor_x = 0;
		monitor_y = 0;
		monitor_w = 0;
		
		//标定
//		x_biaoding_pulse = 1;
	//	x_biaoding_length = 1;
		
//		y_biaoding_pulse = 1;
	//	y_biaoding_length = 1;
		
	//	w_biaoding_pulse = 1;
	//	w_biaoding_length = 1;
		
		monitor_x_biaoding = 0;
		monitor_y_biaoding = 0;
		monitor_w_biaoding = 0;
		
		init_flag = 1;
		
	}
}


void xitong_position_init(void)
{
	gongzuo_x_t = 0;
	gongzuo_y_t = 0;
	gongzuo_w_t = 0;
		
	clean_x_t = 0;
	clean_y_t = 0;
	clean_w_t = 0;
		
	quyang_x_t = 0;
	quyang_y_t = 0;
	quyang_w_t = 0;
		
	zhangan_x_t = 0;
	zhangan_y_t = 0;
	zhangan_w_t = 0;
		
	pingshan_x_t = 0;
	pingshan_y_t = 0;
	pingshan_w_t = 0;
		
	tuye_x_t = 0;
	tuye_y_t = 0;
	tuye_w_t = 0;
	

	
}
void button_huchi(unsigned char* button0, unsigned char* button1, unsigned int* status_record,  unsigned int condition_flag)
{
	
	if(*button0 == 1 || *button1 ==1)
	{
		if(*status_record == 0)
		{
				if(*button1 == 1)
				{
					*button0 =0;
					*status_record = 1;
				}
		}
		
		else 
		{
			if(*button0 == 1)
			{
				*button1 = 0;
				*status_record = 0;
			}
		}
		condition_flag = 1;		//condition_flag用于判断点动同时停和启动条件
	}
	
	else if((*button0 == 0) && (*button1 == 0))		//此处修改= 为== 之前有逻辑小错误。2017.2.7 下午进行测试
	{
		
		*status_record = 0;
		condition_flag = 0;
		
	}
	
	
}

 void button_3_huchi(unsigned char* button0,unsigned char* button1, unsigned char* button2,unsigned int* status_record1,unsigned int* status_record2)
{ 
	if(*button0 ==1 || *button1 == 1 || *button2 == 1)
	{
		if(*status_record1 == 0 && *status_record2 == 0)
		{
			if(*button0 == 1)
			{
				*button1 = 0;
				*button2 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
			
			else if(*button1 == 1 && *button0 ==0)
			{
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}
			
			else if(*button2 == 1 && *button1 == 0 && *button0 ==0)
			{
				*status_record1 = 1;
				*status_record2 = 1;
			}
			
		}
		
		else if(*status_record1 == 1 && *status_record2 == 0)
		{
			if (*button1 == 1)
			{
				*button0 = 0;
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}

			else if (*button2 == 1 && *button1 == 0)
			{
				*button0 = 0;
				*status_record1 = 1;
				*status_record2 = 1;
			}
		}	
		
		
		else if(*status_record1 == 0 && *status_record2 == 1)
		{
			if(*button2 == 1)
			{
				*button1 = 0;
				*button0 = 0;
				*status_record1 = 1;
				*status_record2 = 1;
			}
			
			
			else if(*button2 == 0 && *button0 == 1)
			{
				*button1 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
		}
		
		else if(*status_record1 == 1 && *status_record2 == 1)
		{
			if(*button0 == 1)
			{
				*button1 = 0;
				*button2 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
			
			else if(*button1 == 1 && *button0 == 0)
			{
				*button0 = 0;
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}
		}
		
	  }
	
	  else
	  {
		  *status_record1 = 0;
		  *status_record2 = 0;
	  }
	}


void set_parameter(INT16U setting, unsigned int original)//此函数有错误，应该用指针进行传参
{
	if (setting != original)
	{
		original = setting;
	}
}

void parameter_input()
{
	set_parameter(x_jinji_speed_t,x_jinji_speed);
	set_parameter(x_jianxie_speed_t,x_jianxie_speed);
	set_parameter(x_diandong_speed_t,x_diandong_speed);
	set_parameter(x_huicheng_speed_t,x_huicheng_speed);
	
	set_parameter(w_jianxie_speed_t,w_jianxie_speed);
	set_parameter(w_lianxu_speed_t,w_lianxu_speed);
	set_parameter(w_diandong_speed_t,w_diandong_speed);
	
	set_parameter(y_shangzhen_speed_t,y_shangzhen_speed);
	set_parameter(y_xiazheng_speed_t,y_xiazheng_speed);
	set_parameter(y_diandong_speed_t,y_diandong_speed);
	
	set_parameter(dingchang_x_t,dingchang_x);
}

 /* void fn_jog(unsigned char axis, unsigned int diandong_flag, unsigned int freq, unsigned int condition_flag)
{
 
	if((start_b == 0) && (start_once == 0))
	{
		
		Set_Y_Value(axis, diandong_flag);
		
		
		if((jog_status == 0) && (condition_flag == 1))
		{
			switch (axis)
			{
				case Y_1:
				PulseOut_1_Start(freq, -1);
				jog_status = 1;
				break;
				
				case Y_2:
				PulseOut_2_Start(freq, -1);
				jog_status = 1;
				break;
				
				case Y_3:
				PulseOut_3_Start(freq, -1);
				jog_status = 1;
				break;	
				
				default :
				break;
			}
		}
		if((jog_status == 1) && (condition_flag == 0))
		{
			switch(axis)
			{
				case Y_1:
				PulseOut_1_Stop();
				jog_status  = 0;
				break;
				case Y_2:
				PulseOut_2_Stop();
				jog_status  = 0;
				break;
				case Y_3:
				PulseOut_3_Stop();
				jog_status  = 0;
				break;
				
				default:
				break;
			}
		}
		
	}
}
 */
 void fn_jog_test(void)
{
	if(start_b == 0 && start_once == 0)
	{
		//X轴点动
		if ((diandong_x_p == 1) && (jog_status[0] == 0))
		{
			jog_status[0] = 1;
			jog_monitor_status[0] = 1;
			
			Set_Y_Value(Y_1,P);
			PulseOut_1_Start(x_diandong_speed*speed_ratio,-1);
			
			
			
		}
		else if ((diandong_x_n == 1) && (jog_status[0] == 0))
		{
			jog_status[0] = 1;
			jog_monitor_status[0] = 1;
			
			Set_Y_Value(Y_1,N);
			PulseOut_1_Start(x_diandong_speed*speed_ratio,-1);
			
			
		}
		
		//Y轴点动
		if ((diandong_y_p == 1) && (jog_status[1] == 0))
		{
			jog_status[1] = 1;
			jog_monitor_status[1] = 1;
			
			Set_Y_Value(Y_2,P);
			PulseOut_2_Start(y_diandong_speed*speed_ratio,-1);
			
			//Y2_pulse_remember += OutPulse_Counter[2];
			
		}
		else if ((diandong_y_n == 1) && (jog_status[1] == 0))
		{
			jog_status[1] = 1;
			jog_monitor_status[1] = 1;
			
			Set_Y_Value(Y_2,N);
			PulseOut_2_Start(y_diandong_speed*speed_ratio,-1);
			
			//Y2_pulse_remember -= OutPulse_Counter[2];
			
		}
		
		//W轴点动
		if ((diandong_w_p == 1) && (jog_status[2] == 0))
		{
			jog_status[2] = 1;
			jog_monitor_status[2] = 1;
			
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(w_diandong_speed*speed_ratio,-1);
			
			//Y3_pulse_remember += OutPulse_Counter[3];
			//jog_status[2] = 1;
		}
		else if ((diandong_w_n == 1) && (jog_status[2] == 0))
		{
			jog_status[2] = 1;
			jog_monitor_status[2] = 1;
			
			Set_Y_Value(Y_3,N);
			PulseOut_3_Start(w_diandong_speed*speed_ratio,-1);
			
			//Y3_pulse_remember -= OutPulse_Counter[3];
			//jog_status[2] = 1;
		}
	}
	
	if((jog_status[0]==1) && (diandong_x_p == 0)&&(diandong_x_n == 0))
	{
		
		jog_status[0] = 0;
		jog_monitor_status[0] = 0;
		PulseOut_1_Stop();
		OutPulse_Counter[1]=0;
		
	}
	
	if((jog_status[1]==1) && (diandong_y_p == 0)&&(diandong_y_n == 0))
	{
		jog_status[1] = 0;
		jog_monitor_status[1] = 0;
		PulseOut_2_Stop();
		OutPulse_Counter[2]=0;
	}
	
	if((jog_status[2]==1) && (diandong_w_p == 0)&&(diandong_w_n == 0))
	{
		jog_status[2] = 0;
		jog_monitor_status[2] = 0;
		PulseOut_3_Stop();
		OutPulse_Counter[3]=0;
	}
} 

void fn_dingchang(void)
{
	static dingchang_x_feed = 0;
	
	if (start_b == 0 && start_once == 0)
	{
		//x定长控制
		if( x_dingchang_pb == 1 && dingchang_status[0] == 0)
		{
			Set_Y_Value(Y_1,P);
			dingchang_x_feed++;
			if(dingchang_x_feed <2)
			PulseOut_1_Start(x_diandong_speed*speed_ratio,(Y1_BIAODING_PULSE_TOTAL-1));//*10是因为要有um单位换算
			
			else if(dingchang_x_feed >= 2)
			{
				PulseOut_1_Start(x_diandong_speed*speed_ratio,(Y1_BIAODING_PULSE_TOTAL+1));//*10是因为要有um单位换算
				dingchang_x_feed = 0;
				// Beep(1);
			}
			dingchang_status[0]=1;
			//dingchang_monitor_status[0]=1;
			//Y1_pulse_remember += OutPulse_Counter[1];		//必须在中断中完成
			//x_dingchang_pb = 0;						//必须在中断里面完成
		}
		
		else if (x_dingchang_nb == 1 && dingchang_status[0] == 0)
		{
			Set_Y_Value(Y_1,N);
			PulseOut_1_Start(x_diandong_speed*speed_ratio,(Y1_BIAODING_PULSE_TOTAL));//同上*10
			
			dingchang_status[0]=1;
			//dingchang_monitor_status[0]=1;
			//Y1_pulse_remember -= OutPulse_Counter[1];
			//x_dingchang_nb = 0;
		}
		
		//Y定长控制
		if( y_dingchang_pb == 1 && dingchang_status[1] == 0)
		{
			Set_Y_Value(Y_2,P);
			PulseOut_2_Start(y_diandong_speed*speed_ratio,(Y2_BIAODING_PULSE_TOTAL));
			
			dingchang_status[1]=1;
			//dingchang_monitor_status[1]=1;
			//Y2_pulse_remember += OutPulse_Counter[2];
			//y_dingchang_pb = 0;
		}
		
		else if (y_dingchang_nb == 1 && dingchang_status[1] == 0)
		{
			Set_Y_Value(Y_2,N);
			PulseOut_2_Start(x_diandong_speed*speed_ratio,(Y2_BIAODING_PULSE_TOTAL));
			
			//Y2_pulse_remember -= OutPulse_Counter[2];
			dingchang_status[1]=1;
			//dingchang_monitor_status[1]=1;

		}
		
		//w定长
		if( w_dingchang_pb == 1 && dingchang_status[2] == 0)
		{
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(w_diandong_speed*speed_ratio,(Y3_BIAODING_PULSE_TOTAL-1));
			
			//Y3_pulse_remember += OutPulse_Counter[3];
			dingchang_status[2] = 1;
		//	dingchang_monitor_status[2] = 1;
			//w_dingchang_pb = 0;
		}
		
		else if (w_dingchang_nb == 1 && dingchang_status[2] == 0)
		{
			Set_Y_Value(Y_3,N);
			PulseOut_3_Start(w_diandong_speed*speed_ratio,(Y3_BIAODING_PULSE_TOTAL));
			
			//Y3_pulse_remember -= OutPulse_Counter[3];
			dingchang_status[2]=1;
			//dingchang_monitor_status[2]=1;
			//w_dingchang_nb = 0;
		}
	}
	
	//定长停止 //在appbase中完成
	if(dingchang_status[0] == 1 && x_dingchang_pb == 0 && x_dingchang_nb == 0)
	{
		PulseOut_1_Stop();
		dingchang_status[0] = 0;
	}
	
	if(dingchang_status[1] == 1 && y_dingchang_pb == 0 && y_dingchang_nb == 0)
	{
		PulseOut_2_Stop();
		dingchang_status[1] = 0;
	}
	
	if(dingchang_status[2] == 1 && w_dingchang_pb == 0 && w_dingchang_nb == 0)
		{
		PulseOut_3_Stop();
		dingchang_status[2] = 0;
	}
}


void fn_get_zero(void)
{
	static unsigned int feedback = 0;
	static unsigned int feedback_z = 0;
	
	
	if (start_b == 0 && start_once == 0)
	{
		if (get_zero_status[0] == 0 && x_get_zero == 1)
		{
			get_zero_status[0] = 1;
			x_get_zero_status = 0;
			
			feedback++;
			
			Set_Y_Value(Y_1, N);//反转回零
			PulseOut_1_Start(x_diandong_speed*speed_ratio,(Y1_PULSE_TOTAL+500));
	
		}
		
		if(x_get_zero == 1 && x_get_zero_status == 1 && x_get_zero_keep_status == 0)
		{
			x_get_zero_keep_status = 1;
			x_get_zero_keep_monitor = 1;
			PulseOut_1_Stop();
			
			Set_Y_Value(Y_1,P);//反转回零
			
			if(feedback < 3)
			PulseOut_1_Start(500,500);//(unsigned int)(frequence_Y1/6)
			
			else if(feedback >= 3)
			{
				PulseOut_1_Start(500,501);//(unsigned int)(frequence_Y1/6)
				
				feedback = 0;
				
				//Beep(1);
			}
		}
		
		
		if (get_zero_status[1] == 0 && y_get_zero == 1)
		{
			get_zero_status[1] = 1;
			y_get_zero_status = 0;
			
			Set_Y_Value(Y_2, N);//反转回零
			PulseOut_2_Start(y_diandong_speed*speed_ratio,Y2_PULSE_TOTAL+500);

			//Beep(1);
		}
		
		if(y_get_zero == 1 && y_get_zero_status == 1 && y_get_zero_keep_status == 0)
		{
			y_get_zero_keep_status = 1;
			y_get_zero_keep_monitor = 1;
			//Beep(1);
			PulseOut_2_Stop();
			
			Set_Y_Value(Y_2,P);//反转回零
			
			PulseOut_2_Start(500,500);
		}
		
		
		if (get_zero_status[2] == 0 && w_get_zero == 1)
		{
			get_zero_status[2] = 1;
			w_get_zero_status = 0;
			
			feedback_z++;
			
			Set_Y_Value(Y_3, N);//反转回零
			PulseOut_3_Start(w_diandong_speed*speed_ratio,Y3_PULSE_TOTAL+500);
			//Y3_pulse_remember = 0;
			//Beep(1);
			//w_get_zero = 0;
			
		}
		
		if(w_get_zero == 1 && w_get_zero_status == 1 && w_get_zero_keep_status == 0)
		{
			w_get_zero_keep_status = 1;
			w_get_zero_keep_monitor = 1;
		//	Beep(1);
			PulseOut_3_Stop();
			
			Set_Y_Value(Y_3,P);//反转回零
			
			if(feedback_z < 2)
				PulseOut_3_Start(5000,500);
			else if (feedback_z >= 2)
			{
				PulseOut_3_Start(5000,501);
				feedback_z = 0;
				//Beep(1);
			}
			
		}
		
	}
	
	if (get_zero_status[0] == 1 && x_get_zero == 0)
	{
		PulseOut_1_Stop();
		get_zero_status[0] = 0;
		//Y1_pulse_remember = 0;
	}
	
	if (get_zero_status[1] == 1 && y_get_zero == 0)
	{
		PulseOut_2_Stop();
		get_zero_status[1] = 0;
		//Y2_pulse_remember = 0;
	}
	
	if (get_zero_status[2] == 1 && w_get_zero == 0)
	{
		PulseOut_3_Stop();
		get_zero_status[2] = 0;
		//Y3_pulse_remember = 0;
	}
}



void fn_pos_init(void)
{
	if(pos_init == 1)
	{
		gongzuo_x = 0;
		gongzuo_y = 0;
		gongzuo_w = 0;
		
		clean_x = 0;
		clean_y = 0;
		clean_w = 0;
		
		quyang_x = 0;
		quyang_y = 0;
		quyang_w = 0;
		
		zhangan_x = 0;
		zhangan_y = 0;
		zhangan_w = 0;
		
		pingshan_x = 0;
		pingshan_y = 0;
		pingshan_w = 0;
		
		tuye_x = 0;
		tuye_y = 0;
		tuye_w = 0;
		
		gongzuo_c = 0;
		clean_c = 0;
		quyang_c = 0;
		zhangan_c = 0;
		pingshan_c = 0;
		tuye_c = 0;
	}
}

void fn_weitiao(void)
{
	if(start_b == 0 && start_once == 0)
	{
		//X轴微调
		if ((weitiao_p == 1) && (weitiao_status[0] == 0) && (xitong_x == 1))
		{
			weitiao_status[0] = 1;
			Set_Y_Value(Y_1,P);
			PulseOut_1_Start(500,-1);			
		}
		
		if ((weitiao_n == 1) && (weitiao_status[0] == 0) && (xitong_x == 1))
		{
			weitiao_status[0] = 1;
			Set_Y_Value(Y_1,N);
			PulseOut_1_Start(500,-1);
			
			
		}
		
		//Y轴微调
		if ((weitiao_p == 1) && (weitiao_status[1] == 0) && (xitong_y == 1))
		{
			weitiao_status[1] = 1;
			Set_Y_Value(Y_2,P);
			PulseOut_2_Start(500,-1);
			
			//Y2_pulse_remember += OutPulse_Counter[2];
			
		}
		
		if ((weitiao_n == 1) && (weitiao_status[1] == 0) && (xitong_y == 1))
		{
			weitiao_status[1] = 1;
			
			Set_Y_Value(Y_2,N);
			PulseOut_2_Start(500,-1);
			
			//Y2_pulse_remember -= OutPulse_Counter[2];			
		}
		
		//W轴微调
		if ((weitiao_p == 1) && (weitiao_status[2] == 0) && (xitong_w == 1))
		{
			weitiao_status[2] = 1;
			
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(500,-1);
			
			//Y3_pulse_remember += OutPulse_Counter[3];			
		}
		
		if ((weitiao_n == 1) && (weitiao_status[2] == 0) && (xitong_w == 1))
		{
			weitiao_status[2] = 1;
			Set_Y_Value(Y_3,N);
			PulseOut_3_Start(500,-1);
			//Y3_pulse_remember -= OutPulse_Counter[3];
			
		}
	}
	
	if((weitiao_status[0]==1) && (weitiao_n == 0) && (weitiao_p == 0))
	{
		PulseOut_1_Stop();
		weitiao_status[0] = 0;
	}
	
	if((weitiao_status[1]==1) && (weitiao_n == 0) && (weitiao_p == 0))
	{
		PulseOut_2_Stop();
		weitiao_status[1] = 0;
	}
	
	if((weitiao_status[2]==1) && (weitiao_n == 0) && (weitiao_p == 0))
	{
		PulseOut_3_Stop();
		weitiao_status[2] = 0;
	}
	
}


void fn_gongzuo_position(void)
{
	if(start_b == 0 && start_once == 0)
	{
		
		if( gongzuo_b == 1 && xitong_status[0] == 0 )	//工作原点设置
		{
			xitong_status[0] = 1;
			
			//************X轴工作原点**********//
			if(gongzuo_x > gongzuo_x_t)
			{
				gongzuo_position_x = (gongzuo_x - gongzuo_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_gongzuo_flag[0] = 0;
				
				PulseOut_1_Start(20000,gongzuo_position_x*ratio);
				//gongzuo_x_t = gongzuo_x;
				//Beep(1);
			}
			
			else if (gongzuo_x < gongzuo_x_t)
			{
				gongzuo_position_x = (gongzuo_x_t - gongzuo_x) ;
				
				Set_Y_Value(Y_1,N);
				monitor_gongzuo_flag[0] = 1;
				
				PulseOut_1_Start(20000,gongzuo_position_x*ratio);
				//gongzuo_x_t = gongzuo_x;
			}
			
			else if (gongzuo_x == gongzuo_x_t)
			{
				
				gongzuo_flag_x = 1;
			}
			
			//************Y轴工作原点**********//
		 	if(gongzuo_y > gongzuo_y_t)
			{
				gongzuo_position_y = (gongzuo_y - gongzuo_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_gongzuo_flag[1] = P;
				
				PulseOut_2_Start(20000,gongzuo_position_y*ratio);
				//gongzuo_y_t = gongzuo_y;
			}
			
			else if (gongzuo_y < gongzuo_y_t)
			{
				gongzuo_position_y = (gongzuo_y_t - gongzuo_y);
				
				Set_Y_Value(Y_2,N);
				monitor_gongzuo_flag[1] = N;
				
				PulseOut_2_Start(20000,gongzuo_position_y*ratio);
				//gongzuo_y_t = gongzuo_y;
			} 
			
			else if (gongzuo_y == gongzuo_y_t)
			{
				gongzuo_flag_y = 1;
			}
			
			//************W轴工作原点**********//
		 	if(gongzuo_w > gongzuo_w_t)
			{
				gongzuo_position_w = (gongzuo_w - gongzuo_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_gongzuo_flag[2] = P;
				
				PulseOut_3_Start(20000,gongzuo_position_w*ratio);
				//gongzuo_w_t = gongzuo_w;
				
			}
			
			else if (gongzuo_w < gongzuo_w_t)
			{
				gongzuo_position_w = (gongzuo_w_t - gongzuo_w);
				
				Set_Y_Value(Y_3,N);
				monitor_gongzuo_flag[2] = N;
				
				PulseOut_3_Start(20000,gongzuo_position_w*ratio);
				//gongzuo_w_t = gongzuo_w;
			} 
			
			else if (gongzuo_w == gongzuo_w_t)
			{
				gongzuo_flag_w = 1;
			}
			
			
			gongzuo_x_t = gongzuo_x;
			gongzuo_y_t = gongzuo_y;		
			gongzuo_w_t = gongzuo_w; 
			
			//xitong_status[0] = 1;
			weitiao_x_status[0] = 1;
			weitiao_y_status[0] = 1;
			weitiao_w_status[0] = 1;
		}
		
	
	}
	
	if (xitong_status[0] == 1 && gongzuo_flag_x == 1 && gongzuo_flag_y == 1 && gongzuo_flag_w == 1)	//工作原点完成设置
			{
			PulseOut_1_Stop();
			PulseOut_2_Stop();
			PulseOut_3_Stop();
		
//			Beep(1);
			
			gongzuo_b = 0;	//按键弹起
			
			gongzuo_flag_x = 0;
			gongzuo_flag_y = 0;
			gongzuo_flag_w = 0;
			
			xitong_status[0] = 0;
			xitong_c_status[0] = 1;
			}

}

void fn_clean_position(void)
{
	
	if(start_b == 0 && start_once == 0)
	{
		
		if(clean_b == 1 && xitong_status[1] == 0)		//清洗位置设置
		{
			
			xitong_status[1] = 1;
			
			//************X轴清洗位置**********//
			if(clean_x > clean_x_t)
			{
				clean_position_x = (clean_x - clean_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_clean_flag[0] = 0;
				
				PulseOut_1_Start(20000,clean_position_x*ratio);

			}
			
			else if (clean_x < clean_x_t)
			{
				clean_position_x = (clean_x_t - clean_x);
				
				Set_Y_Value(Y_1,N);
				monitor_clean_flag[0] = 1;
				
				PulseOut_1_Start(20000,clean_position_x*ratio);
				
				//clean_flag_x = 1;
			}
			
			else if (clean_x == clean_x_t)
			{
				clean_flag_x = 1;
//				Beep(1);
			}
			
			//************Y轴clean_position**********//
		 	if(clean_y > clean_y_t)
			{
				clean_position_y = (clean_y - clean_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_clean_flag[1] = 0;
				
				PulseOut_2_Start(20000,clean_position_y*ratio);
				
				//clean_flag_y = 1;
			}
			
			else if (clean_y < clean_y_t)
			{
				clean_position_y = (clean_y_t - clean_y);
				
				Set_Y_Value(Y_2,N);
				monitor_clean_flag[1] = 1;
				
				PulseOut_2_Start(20000,clean_position_y*ratio);
				
				//clean_flag_y = 1;
			} 
			
			else if (clean_y == clean_y_t)
			{
				clean_flag_y = 1;
			}
			
			//************W轴clean原点**********//
		 	if(clean_w > clean_w_t)
			{
				clean_position_w = (clean_w - clean_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_clean_flag[2] = 0;
				
				PulseOut_3_Start(20000,clean_position_w*ratio);
				
			}
			
			else if (clean_w < clean_w_t)
			{
				clean_position_w = (clean_w_t - clean_w);
				
				Set_Y_Value(Y_3,N);
				monitor_clean_flag[2] = 1;
				
				PulseOut_3_Start(20000,clean_position_w*ratio);
				
				//clean_flag_w = 1;
			} 
			
			else if (clean_w == clean_w_t)
			{
				clean_flag_w = 1;
			}
			
			clean_x_t = clean_x;
			clean_y_t = clean_y;		
			clean_w_t = clean_w;
			
			weitiao_x_status[1] = 1;
			weitiao_y_status[1] = 1;
			weitiao_w_status[1] = 1;
		}
	}
	
	if (xitong_status[1] == 1 && clean_flag_x == 1 && clean_flag_y == 1 && clean_flag_w == 1)		//clean_position完成设置
	{
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		PulseOut_3_Stop();
		
		clean_b = 0;
		
		clean_flag_x = 0;
		clean_flag_y = 0;
		clean_flag_w = 0;
		
		xitong_status[1] = 0;
		xitong_c_status[1] = 1;
	}
	
}

void fn_quyang_position(void)
{
	if(start_b == 0 && start_once == 0)
	{
		
		
		if(quyang_b == 1 && xitong_status[2] == 0)		//取样位置设置
		{
			
			xitong_status[2] = 1;
			
			
			//************X轴取样位置**********//
			if(quyang_x > quyang_x_t)
			{
				quyang_position_x = (quyang_x - quyang_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_quyang_flag[0] = P;
				
				PulseOut_1_Start(20000,quyang_position_x*ratio);
				
			}
			
			else if (quyang_x < quyang_x_t)
			{
				quyang_position_x = (quyang_x_t - quyang_x);
				
				Set_Y_Value(Y_1,N);
				monitor_quyang_flag[0] = N;
				
				PulseOut_1_Start(20000,quyang_position_x*ratio);
				
				
			//	quyang_flag_x = 1;
			}
			
			else if (quyang_x == quyang_x_t)
			{
				quyang_flag_x = 1;
			}
			
			//************Y轴取样位置**********//
		 	if(quyang_y > quyang_y_t)
			{
				quyang_position_y = (quyang_y - quyang_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_quyang_flag[1] = P;
				
				PulseOut_2_Start(20000,quyang_position_y*ratio);
				
			}
			
			else if (quyang_y < quyang_y_t)
			{
				quyang_position_y = (quyang_y_t - quyang_y);
				
				Set_Y_Value(Y_2,N);
				monitor_quyang_flag[1] = N;
				
				PulseOut_2_Start(20000,quyang_position_y*ratio);
				
			//	quyang_flag_y = 1;
			} 
			
			else if (quyang_y == quyang_y_t)
			{
				quyang_flag_y = 1;
			}
			
			//************W轴取样位置**********//
		 	if(quyang_w > quyang_w_t)
			{
				
				quyang_position_w = (quyang_w - quyang_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_quyang_flag[2] = P;
				
				PulseOut_3_Start(20000,quyang_position_w*ratio);
				
			//	quyang_flag_w = 1;
			}
			
			else if (quyang_w < quyang_w_t)
			{
				quyang_position_w = (quyang_w_t - quyang_w);
				
				Set_Y_Value(Y_3,N);
				monitor_quyang_flag[2] = N;
				
				PulseOut_3_Start(20000,quyang_position_w*ratio);
				

			} 
			
			else if (quyang_w == quyang_w_t)
			{
				quyang_flag_w = 1;
			}
			
			quyang_x_t = quyang_x;
			quyang_y_t = quyang_y;		
			quyang_w_t = quyang_w;
			//xitong_status[2] = 1;
			
			
			weitiao_x_status[2] = 1;
			weitiao_y_status[2] = 1;
			weitiao_w_status[2] = 1;
			
		}
	}
	if (xitong_status[2] == 1 && quyang_flag_x == 1 && quyang_flag_y == 1 && quyang_flag_w == 1)		//取样完成设置
	{
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		PulseOut_3_Stop();
		
		quyang_b = 0;
		
		quyang_flag_x = 0;
		quyang_flag_y = 0;
		quyang_flag_w = 0;
		
		xitong_status[2] = 0;
		xitong_c_status[2] = 1;
	}
}

void fn_zhangan_position(void)
{
	if(start_b == 0 && start_once == 0)
	{
		if(zhangan_b == 1 && xitong_status[3] == 0)		//蘸干位置设置
		{
			
			xitong_status[3] = 1;
			//************X轴蘸干位置**********//
			if(zhangan_x > zhangan_x_t)
			{
				zhangan_position_x = (zhangan_x - zhangan_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_zhangan_flag[0] = P;
				
				PulseOut_1_Start(20000,zhangan_position_x*ratio);
				
		//		zhangan_flag_x = 1;
			}
			
			else if (zhangan_x < zhangan_x_t)
			{
				zhangan_position_x = (zhangan_x_t - zhangan_x);
				
				Set_Y_Value(Y_1,N);
				monitor_zhangan_flag[0] = N;
				
				PulseOut_1_Start(20000,zhangan_position_x*ratio);
				
			}
			
			else if (zhangan_x == zhangan_x_t)
			{
				zhangan_flag_x = 1;
			}
			
			//************Y轴蘸干位置**********//
		 	if(zhangan_y > zhangan_y_t)
			{
				zhangan_position_y = (zhangan_y - zhangan_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_zhangan_flag[1] = P;
				
				PulseOut_2_Start(20000,zhangan_position_y*ratio);
				
		//		zhangan_flag_y = 1;
				
			}
			
			else if (zhangan_y < zhangan_y_t)
			{
				zhangan_position_y = (zhangan_y_t - zhangan_y);
				
				Set_Y_Value(Y_2,N);
				monitor_zhangan_flag[1] = N;
				
				PulseOut_2_Start(20000,zhangan_position_y*ratio);
				
		//		zhangan_flag_y = 1;
			} 
			
			else if (zhangan_y == zhangan_y_t)
			{
				zhangan_flag_y = 1;
			} 
			
			
			//************W轴蘸干位置**********//
		 	if(zhangan_w > zhangan_w_t)
			{
				zhangan_position_w = (zhangan_w - zhangan_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_zhangan_flag[2] = P;
				
				PulseOut_3_Start(20000,zhangan_position_w*ratio);
				
		//		zhangan_flag_w = 1;
			}
			
			else if (zhangan_w < zhangan_w_t)
			{
				zhangan_position_w = (zhangan_w_t - zhangan_w);
				
				Set_Y_Value(Y_3,N);
				monitor_zhangan_flag[2] = N;
				
				PulseOut_3_Start(20000,zhangan_position_w*ratio);
				
		//		zhangan_flag_w = 1;
			} 
			
			else if (zhangan_w == zhangan_w_t)
			{
				zhangan_flag_w = 1;
			} 
			
			zhangan_x_t = zhangan_x;
			zhangan_y_t = zhangan_y;		
			zhangan_w_t = zhangan_w;
			//xitong_status[3] = 1;
			weitiao_x_status[3] = 1;
			weitiao_y_status[3] = 1;
			weitiao_w_status[3] = 1;
		}
	}
	
	if (xitong_status[3] == 1 && zhangan_flag_x == 1 && zhangan_flag_y == 1 && zhangan_flag_w == 1)	//蘸干完成设置
	{
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		PulseOut_3_Stop();
		
		zhangan_b = 0;
		
		zhangan_flag_x = 0;
		zhangan_flag_y = 0;
		zhangan_flag_w = 0;
		
		xitong_status[3] = 0;
		xitong_c_status[3] = 1;
		
	}
}

void fn_pingshan_position(void)
{
	if(start_b == 0 && start_once == 0)
	{
		
		if(pingshan_b == 1 && xitong_status[4] == 0)	//屏闪位置设置
		{
			xitong_status[4] = 1;
			
			//************X轴屏闪位置**********//
			if(pingshan_x > pingshan_x_t)
			{
				pingshan_position_x = (pingshan_x - pingshan_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_pingshan_flag[0] = P;
				
				PulseOut_1_Start(20000,pingshan_position_x*ratio);
				
		//		pingshan_flag_x = 1;
			}
			
			else if (pingshan_x < pingshan_x_t)
			{
				pingshan_position_x =(pingshan_x_t - pingshan_x);
				
				Set_Y_Value(Y_1,N);
				monitor_pingshan_flag[0] = N;
				
				PulseOut_1_Start(20000,pingshan_position_x*ratio);
				
		//		pingshan_flag_x = 1;
			}
			
			else if (pingshan_x == pingshan_x_t)
			{
				pingshan_flag_x = 1;
			}
			
			//************Y轴屏闪位置**********//
		 	if(pingshan_y > pingshan_y_t)
			{
				pingshan_position_y = (pingshan_y - pingshan_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_pingshan_flag[1] = P;
				
				PulseOut_2_Start(20000,pingshan_position_y*ratio);
				
			}
			
			else if (pingshan_y < pingshan_y_t)
			{
				pingshan_position_y = (pingshan_y_t - pingshan_y);
					
				Set_Y_Value(Y_2,N);
				monitor_pingshan_flag[1] = N;
				
				PulseOut_2_Start(20000,pingshan_position_y*ratio);
			
			} 
			
			else if (pingshan_y == pingshan_y_t)
			{
				pingshan_flag_y = 1;
			}
			
			//************W轴屏闪位置**********//
		 	if(pingshan_w > pingshan_w_t)
			{
				pingshan_position_w = (pingshan_w - pingshan_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_pingshan_flag[2] = P;
				
				PulseOut_3_Start(20000,pingshan_position_w*ratio);
				
		//		pingshan_flag_w = 1;
			}
			
			else if (pingshan_w < pingshan_w_t)
			{
				pingshan_position_w = (pingshan_w_t - pingshan_w);
			
				Set_Y_Value(Y_3,N);
				monitor_pingshan_flag[2] = N;
				
				PulseOut_3_Start(20000,pingshan_position_w*ratio);
				
			} 
			
			else if (pingshan_w == pingshan_w_t)
			{
				pingshan_flag_w = 1;
			}
			
			pingshan_x_t = pingshan_x;
			pingshan_y_t = pingshan_y;		
			pingshan_w_t = pingshan_w;
			//xitong_status[4] = 1;
			weitiao_x_status[4] = 1;
			weitiao_y_status[4] = 1;
			weitiao_w_status[4] = 1;
			
		}
	}
	
	
	if (xitong_status[4] == 1 && pingshan_flag_x == 1 && pingshan_flag_y == 1 && pingshan_flag_w == 1)	//屏闪完成设置
	{
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		PulseOut_3_Stop();
		
		pingshan_b = 0;
		
		pingshan_flag_x = 0;
		pingshan_flag_y = 0;
		pingshan_flag_w = 0;
		
		xitong_status[4] = 0;
		xitong_c_status[4] = 1;
	}

}

void fn_tuye_position(void)
{
	if(start_b == 0 && start_once == 0)
	{
		if(tuye_b == 1 && xitong_status[5] == 0)		//吐液位置设置
		{
			xitong_status[5] = 1;
			
			//************X轴吐液位置**********//
			if(tuye_x > tuye_x_t)
			{
				tuye_position_x = (tuye_x - tuye_x_t);
				
				Set_Y_Value(Y_1,P);
				monitor_tuye_flag[0] = P;
				
				PulseOut_1_Start(20000,tuye_position_x*ratio);

			}
			
			else if (tuye_x < tuye_x_t)
			{
				tuye_position_x = (tuye_x_t - tuye_x);
				
				Set_Y_Value(Y_1,N);
				monitor_tuye_flag[0] = N;
				
				PulseOut_1_Start(20000,tuye_position_x*ratio);

			}
			
			else if (tuye_x == tuye_x_t)
			{
				tuye_flag_x = 1;
			}
			
			//************Y轴吐液位置**********//
		 	if(tuye_y > tuye_y_t)
			{
				tuye_position_y = (tuye_y - tuye_y_t);
				
				Set_Y_Value(Y_2,P);
				monitor_tuye_flag[1] = P;
				
				PulseOut_2_Start(20000,tuye_position_y*ratio);
				
		//		tuye_flag_y = 1;
			}
			
			else if (tuye_y < tuye_y_t)
			{
				tuye_position_y = (tuye_y_t - tuye_y);
				
				Set_Y_Value(Y_2,N);
				monitor_tuye_flag[1] = N;
				
				PulseOut_2_Start(20000,tuye_position_y*ratio);

			} 
			
			else if (tuye_y == tuye_y_t)
			{
				tuye_flag_y = 1;
			}
			
			//************W轴吐液位置**********//
		 	if(tuye_w > tuye_w_t)
			{
				tuye_position_w = (tuye_w - tuye_w_t);
				
				Set_Y_Value(Y_3,P);
				monitor_tuye_flag[2] = P;
				
				PulseOut_3_Start(20000,tuye_position_w*ratio);
		//		tuye_flag_w = 1;
			}
			
			else if (tuye_w < tuye_w_t)
			{
				tuye_position_w = (tuye_w_t - tuye_w);
				
				Set_Y_Value(Y_3,N);
				monitor_tuye_flag[2] = N;
				
				PulseOut_3_Start(20000,tuye_position_w*ratio);
		//		tuye_flag_w = 1;
			} 
			
			else if (tuye_w == tuye_w_t)
			{
				tuye_flag_w = 1;
			}
			
			tuye_x_t = tuye_x;
			tuye_y_t = tuye_y;		
			tuye_w_t = tuye_w;
			//xitong_status[5] = 1;
			
			weitiao_x_status[5] = 1;
			weitiao_y_status[5] = 1;
			weitiao_w_status[5] = 1;
		}
	}
	
	if (xitong_status[5] == 1 && tuye_flag_x == 1 && tuye_flag_y == 1 && tuye_flag_w == 1)		//吐液完成设置
	{
		PulseOut_1_Stop();
		PulseOut_2_Stop();
		PulseOut_3_Stop();
		
		tuye_b = 0;
		
		tuye_flag_x = 0;
		tuye_flag_y = 0;
		tuye_flag_w = 0;
		
		xitong_status[5] = 0;
		xitong_c_status[5] = 1;
	}
}


void fn_gongzuo_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(gongzuo_c == 1 && confirm_status[0] == 0 && xitong_c_status[0] == 1) //
	{
		confirm_status[0] = 1;
		confirm_monitor_x_status[0] = 1;
		confirm_monitor_y_status[0] = 1;
		confirm_monitor_w_status[0] = 1;
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,gongzuo_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,gongzuo_y*ratio);
		
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,gongzuo_w*ratio);
		
		
		
		xitong_c_status[0] = 0;
		
/* 		weitiao_x_status[0] = 0;	//同步微调标志位,此段代码应该在中断中写
		weitiao_y_status[0] = 0;
		weitiao_w_status[0] = 0; */
		
	//	xitong_position_init();
		
		}
	

	if(gongzuo_c == 0 && confirm_status[0] == 1)
	{
		confirm_status[0] = 0;
	}
}

void fn_clean_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(clean_c == 1 && confirm_status[1] == 0 && xitong_c_status[1] == 1) //
	{
		
		confirm_status[1] = 1;
		confirm_monitor_x_status[1] = 1;
		confirm_monitor_y_status[1] = 1;
		confirm_monitor_w_status[1] = 1;
		
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,clean_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,clean_y*ratio);
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,clean_w*ratio);		
		
		xitong_c_status[1] = 0;
		
		
		//xitong_position_init();
		
	}
	

	if(clean_c == 0 && confirm_status[1] == 1)
	{
		confirm_status[1] = 0;
	}
}

void fn_quyang_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(quyang_c == 1 && confirm_status[2] == 0 && xitong_c_status[2] == 1) //
	{
		confirm_status[2] = 1;
		confirm_monitor_x_status[2] = 1;
		confirm_monitor_y_status[2] = 1;
		confirm_monitor_w_status[2] = 1;
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,quyang_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,quyang_y*ratio);
		
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,quyang_w*ratio);
		
		
		
		xitong_c_status[2] = 0;
		
		
		
		//xitong_position_init();
		
		}
	

	if(quyang_c == 0 && confirm_status[2] == 1)
	{
		confirm_status[2] = 0;
	}
}


void fn_zhangan_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(zhangan_c == 1 && confirm_status[3] == 0 && xitong_c_status[3] == 1) //
	{
		confirm_status[3] = 1;
		confirm_monitor_x_status[3] = 1;
		confirm_monitor_y_status[3] = 1;
		confirm_monitor_w_status[3] = 1;
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,zhangan_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,zhangan_y*ratio);
		
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,zhangan_w*ratio);		
		
		xitong_c_status[3] = 0;
		
		
		//xitong_position_init();
		
		}
	

	if(zhangan_c == 0 && confirm_status[3] == 1)
	{
		confirm_status[3] = 0;
	}
}

void fn_pingshan_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(pingshan_c == 1 && confirm_status[4] == 0 && xitong_c_status[4] == 1) //
	{
		confirm_status[4] = 1;
		confirm_monitor_x_status[4] = 1;
		confirm_monitor_y_status[4] = 1;
		confirm_monitor_w_status[4] = 1;
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,pingshan_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,pingshan_y*ratio);
		
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,pingshan_w);		
		
		xitong_c_status[4] = 0;
		
		
		//xitong_position_init();
		
		}
	

	if(pingshan_c == 0 && confirm_status[4] == 1)
	{
		confirm_status[4] = 0;
	}
}


void fn_tuye_confirm(void)//确认之后回零 必须是刚才动过的那个按键结束。应该在对应的position中进行设置 17/2/9 0:51
{
	if(tuye_c == 1 && confirm_status[5] == 0 && xitong_c_status[5] == 1) //
	{
		confirm_status[5] = 1;
		confirm_monitor_x_status[5] = 1;
		confirm_monitor_y_status[5] = 1;
		confirm_monitor_w_status[5] = 1;
		
		
		Set_Y_Value(Y_1, N);//反转回零
		PulseOut_1_Start(30000,tuye_x*ratio);
		
		Set_Y_Value(Y_2, N);//反转回零
		PulseOut_2_Start(30000,tuye_y*ratio);
		
		
		Set_Y_Value(Y_3, N);//反转回零
		PulseOut_3_Start(30000,tuye_w*ratio);		
		
		xitong_c_status[5] = 0;
	
		
		//xitong_position_init();
		
		}
	

	if(tuye_c == 0 && confirm_status[5] == 1)
	{
		confirm_status[5] = 0;
	}
}


void work_get_zero(void)
{
	if(work_zero_x_status == 0)
	{
		work_zero_x_status = 1;
		work_zero_x_monitor_status = 1;	
		
		Set_Y_Value(Y_1,N);
		PulseOut_1_Start(30000,Y1_PULSE_TOTAL);
		
	}
	
	if(work_zero_y_status == 0)
	{
		work_zero_y_status = 1;
		work_zero_y_monitor_status = 1;
		
		
		Set_Y_Value(Y_2,N);
		PulseOut_2_Start(30000,Y2_PULSE_TOTAL);
		
	}
	
	if(work_zero_w_status == 0)
	{
		work_zero_w_status = 1;
		work_zero_w_monitor_status = 1;
		
		
		Set_Y_Value(Y_3,N);
		PulseOut_3_Start(30000,Y3_PULSE_TOTAL);
		
	}
	
	
}


void work_clean(void)
{
	
	if(work_clean_x_status == 0 && work_clean_x_finish == 0)//在此之前已经添加判断条件
	{
		work_clean_x_status = 1;
		work_clean_x_monitor_status = 1;

		Set_Y_Value(Y_1,P);
		PulseOut_1_Start(x_jianxie_speed*speed_ratio,clean_x*ratio);
	}
	
	if(work_clean_y_status == 0 && work_clean_x_finish == 1 && work_clean_y_finish == 0)
	{
		//work_clean_x_finish = 0;
		
		work_clean_y_status = 1;
		work_clean_y_monitor_status = 1;
		
		Set_Y_Value(Y_2,P);
		PulseOut_2_Start(y_xiazheng_speed*speed_ratio,clean_y*ratio);
	}
	
	if(work_clean_y_finish == 1 && work_cleaning_status == 0 && work_cleaning_finish == 0)
	{
		//work_clean_y_finish = 0;
		
		work_cleaning_status = 1;
		
		Delay(clean_pre*clean_times);
		
		work_cleaning_finish = 1;
	}
	
	if(work_clean_back_status == 0 && work_cleaning_finish == 1 && work_clean_back_finish == 0)
	{
		work_clean_back_status = 1;
		//work_cleaning_finish = 0;
		
		work_clean_back_monitor_status = 1;	
		
		Set_Y_Value(Y_2,N);
		PulseOut_2_Start(y_shangzhen_speed*speed_ratio,clean_y*ratio);
	}
	
	
	//所有finish的标志位均尚未清零
}

void work_quyang(void)
{
	if(work_quyang_x_status == 0 && work_quyang_x_finish == 0)//在此之前已经添加判断条件
	{
		work_quyang_x_status = 1;
		work_quyang_x_monitor_status = 1;

		Set_Y_Value(Y_1,P);
		
		PulseOut_1_Start(x_jianxie_speed*speed_ratio,(quyang_x-clean_x)*ratio);
	}
	
	if(work_quyang_y_status == 0 && work_quyang_x_finish == 1 && work_quyang_y_finish == 0)
	{
		//work_clean_x_finish = 0;
		
		work_quyang_y_status = 1;
		work_quyang_y_monitor_status = 1;
		
		Set_Y_Value(Y_2,P);
		PulseOut_2_Start(y_xiazheng_speed*speed_ratio,quyang_y*ratio);
	}
	
	if(work_quyang_y_finish == 1 && work_quyanging_status == 0 && work_quyanging_finish == 0)
	{
		//work_clean_y_finish = 0;
		
		work_quyanging_status = 1;
		
		Delay(quyang_pre*dianyang_times);
		
		work_quyanging_finish = 1;
	}
	
	if(work_quyang_back_status == 0 && work_quyanging_finish == 1 && work_quyang_back_finish == 0)
	{
		work_quyang_back_status = 1;
		//work_cleaning_finish = 0;
		
		work_quyang_back_monitor_status = 1;	
		
		Set_Y_Value(Y_2,N);
		PulseOut_2_Start(y_shangzhen_speed*speed_ratio,quyang_y*ratio);
	}
	
	
	//所有finish的标志位均尚未清零
}

void work_pre_dianyang(void)
{
	if(work_pre_dianyang_x_status == 0 && work_pre_dianyang_x_finish == 0)//在此之前已经添加判断条件
	{
		work_pre_dianyang_x_status = 1;
		work_pre_dianyang_x_monitor_status = 1;

		Set_Y_Value(Y_1,P);
		
		PulseOut_1_Start(x_jianxie_speed*speed_ratio,(gongzuo_x-quyang_x)*ratio);
	}
	
	if(work_pre_dianyang_w_status == 0 && work_pre_dianyang_x_finish == 1 && work_pre_dianyang_w_finish == 0)
	{
		work_pre_dianyang_w_status = 1;
		work_pre_dianyang_w_monitor_status = 1;
		
		Set_Y_Value(Y_3,P);
		PulseOut_3_Start(w_jianxie_speed*speed_ratio,gongzuo_w);
		
	}
	
	if(work_pre_dianyang_y_status == 0 && work_pre_dianyang_w_finish == 1 && work_pre_dianyang_y_finish == 0)
	{
		//work_clean_x_finish = 0;
		
		work_pre_dianyang_y_status = 1;
		work_pre_dianyang_y_monitor_status = 1;
		
		Set_Y_Value(Y_2,P);
		PulseOut_2_Start(y_xiazheng_speed*speed_ratio,gongzuo_y*ratio);
	}
	
	
	if(work_pre_dianyang_y_finish == 1 && work_pre_dianyanging_status == 0 && work_pre_dianyanging_finish == 0)
	{
		//work_clean_y_finish = 0;
		
		work_pre_dianyanging_status = 1;
		
		//Delay(quyang_pre*dianyang_times);
		
		//work_fn_dianyang();	//点样函数API暂时未编写
		
		work_pre_dianyanging_finish = 1;
	}
	
	if(work_pre_dianyang_back_status == 0 && work_pre_dianyanging_finish == 1 && work_pre_dianyang_back_finish == 0)
	{
		work_pre_dianyang_back_status = 1;
		//work_cleaning_finish = 0;
		
		work_pre_dianyang_back_monitor_status = 1;	
		
		Set_Y_Value(Y_2,N);
		PulseOut_2_Start(y_shangzhen_speed*speed_ratio,gongzuo_y*ratio);
	}
}

void work_zhangan(void)
{
	if(work_zhangan_x_status == 0 && work_zhangan_x_finish == 0)//在此之前已经添加判断条件
	{
		work_zhangan_x_status = 1;
		work_zhangan_x_monitor_status = 1;

		Set_Y_Value(Y_1,N);
		
		PulseOut_1_Start(x_jianxie_speed*speed_ratio,(gongzuo_x-zhangan_x)*ratio);
	}
	
	if(work_zhangan_w_status == 0 && work_zhangan_x_finish == 1 && work_zhangan_w_finish == 0)
	{
		work_zhangan_w_status = 1;
		work_zhangan_w_monitor_status = 1;
		
		if(zhangan_w > gongzuo_w)
		{
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(w_jianxie_speed*speed_ratio,(zhangan_w - gongzuo_w)*ratio);	
		}
		
		if(zhangan_w < gongzuo_w)
		{
			Set_Y_Value(Y_3,N);
			PulseOut_3_Start(w_jianxie_speed*speed_ratio,(gongzuo_w - zhangan_w)*ratio);	
		}
		
		if(zhangan_w == gongzuo_w)
		{
			work_zhangan_w_status = 0;
			work_zhangan_w_finish = 1;
		}
		
	}
	
	if(work_zhangan_y_status == 0 && work_zhangan_w_finish == 1 && work_zhangan_y_finish == 0)
	{
		//work_clean_x_finish = 0;
		
		work_zhangan_y_status = 1;
		work_zhangan_y_monitor_status = 1;
		
		Set_Y_Value(Y_2,P);
		PulseOut_2_Start(y_xiazheng_speed*speed_ratio,zhangan_y*ratio);
	}
	
	
	if(work_zhangan_y_finish == 1 && work_zhanganing_status == 0 && work_zhanganing_finish == 0)
	{
		//work_clean_y_finish = 0;
		
		work_zhanganing_status = 1;
		
		//Delay(zhangan_time);
		
		work_fn_dianyang();	//点样函数API暂时未编写
		
		
	}
	
	if(work_zhangan_back_status == 0 && work_zhanganing_finish == 1 && work_zhangan_back_finish == 0)
	{
		work_zhangan_back_status = 1;
		//work_cleaning_finish = 0;
		
		work_zhangan_back_monitor_status = 1;	
		
		Set_Y_Value(Y_2,N);
		PulseOut_2_Start(y_shangzhen_speed*speed_ratio,zhangan_y*ratio);
	}
	
	
 	///////////////////////////////////蘸干工作结束后W轴回到工作原点/////////////////
	if(work_zhangan_w_back_status == 0 && work_zhangan_back_finish == 1 && work_zhangan_w_back_finish == 0)//
	{
		work_zhangan_w_back_status = 1;
		work_zhangan_w_back_monitor_status = 1;
		
		if(zhangan_w > gongzuo_w)
		{
			Set_Y_Value(Y_3,N);
			PulseOut_3_Start(w_jianxie_speed*speed_ratio,(zhangan_w - gongzuo_w)*ratio);	
		}
		
		if(zhangan_w <= gongzuo_w)
		{
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(w_jianxie_speed*speed_ratio,(gongzuo_w - zhangan_w)*ratio);	
		}
		
	}
	
	///////////////////////////////////蘸干工作结束后X轴回到工作原点/////////////////
	if(work_zhangan_x_back_status == 0  && work_zhangan_x_back_finish == 0  && work_zhangan_w_back_finish == 1)//
	{
		work_zhangan_x_back_status = 1;
		work_zhangan_x_back_monitor_status = 1;
		
		//Beep(1);
		
		Set_Y_Value(Y_1,P);	
		PulseOut_1_Start(x_jianxie_speed*speed_ratio,(gongzuo_x-zhangan_x)*ratio);
		
	} 
}

void work_dianyang(void)
{
 
 if(work_dianyang_allow == 0)
 {
	
	if (tmp_dianyang_danyuan_hangshu <= danyuan_hangshu)
	{
		//转一大下
		if(tmp_dianyang_jiance_hangshu == jiance_hangshu && tmp_dianyang_danyuan_hangshu < danyuan_hangshu && work_dianyang_again_finish == 1)//
		{
			if (work_dianyang_w_l_lie_status == 0 && work_dianyang_w_l_lie_finish == 0)
				{
					work_dianyang_w_l_lie_status = 1;
					work_dianyang_w_l_lie_monitor_status = 1;

					Set_Y_Value(Y_3,P);
					PulseOut_3_Start(w_jianxie_speed*speed_ratio,(danyuan_hangjianju*danyuan_ratio+jiance_gaodu*jiance_ratio));

				}
		}
		
		
		if (tmp_dianyang_jiance_hangshu <= jiance_hangshu)
		{

			if (tmp_dianyang_dianzhen_memory == 1 && tmp_dianyang_jiance_hangshu == 1 && tmp_dianyang_danyuan_hangshu == 1)//找到初始位置
			{
				if (work_dianyang_w_s_status == 0 && work_dianyang_w_s_finish == 0)//在此之前已经添加判断条件
				{
					work_dianyang_w_s_status = 1;
					work_dianyang_w_s_monitor_status = 1;


					switch (yangpin_bianhao)
					{
					case 1:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,(dianzhen_hangjianju*dianzhen_ratio*DZ_1_lie_Distance));
						break;

					case 2:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_2_lie_Distance);
						break;

					case 3:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_3_lie_Distance);
						break;

					case 4:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_4_lie_Distance);
						break;

					case 5:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_5_lie_Distance);
						break;

					case 6:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_6_lie_Distance);
						break;

					case 7:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_7_lie_Distance);
						break;

					case 8:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_8_lie_Distance);
						break;


					case 9:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_9_lie_Distance);
						break;

					case 10:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_10_lie_Distance);
						break;

					case 11:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_11_lie_Distance);
						break;

					case 12:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio*DZ_12_lie_Distance);
						break;

					default:
						break;

					}
				}

				if (work_dianyang_w_s_finish == 1 && work_dianyang_x_s_status == 0 && work_dianyang_x_s_finish == 0)
				{
					work_dianyang_x_s_status = 1;
					work_dianyang_x_s_monitor_status = 1;

					switch (yangpin_bianhao)
					{
					case 1:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_1_hang_Distance);
						break;

					case 2:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_2_hang_Distance);
						break;

					case 3:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_3_hang_Distance);
						break;

					case 4:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_4_hang_Distance);
						break;

					case 5:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_5_hang_Distance);
						break;

					case 6:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_6_hang_Distance);
						break;

					case 7:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_7_hang_Distance);
						break;

					case 8:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_8_hang_Distance);
						break;


					case 9:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_9_hang_Distance);
						break;

					case 10:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_10_hang_Distance);
						break;

					case 11:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_11_hang_Distance);
						break;

					case 12:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio*DZ_12_hang_Distance);
						break;

					default:
						break;

					}

				}
			}
			
			//转一小下(纵向)
			if (tmp_dianyang_dianzhen_memory != 1 && (tmp_dianyang_dianzhen_memory%2 == 0) && dianzhen_bianhao != 11
			        && dianzhen_bianhao != 12 && (tmp_dianyang_jiance_hangshu < jiance_hangshu || work_dianyang_again_status == 1))	
			{

				if (work_dianyang_w_s_lie_status == 0 && work_dianyang_w_s_lie_finish == 0)
				{
					work_dianyang_w_s_lie_status = 1;
					work_dianyang_w_s_lie_monitor_status = 1;

					Set_Y_Value(Y_3,P);
					PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*dianzhen_ratio);

				}
			}
			
			//横着走一小下(横向)
			if (tmp_dianyang_dianzhen_memory != 1 && (tmp_dianyang_dianzhen_memory%2 == 0) && (dianzhen_bianhao == 11
			        || dianzhen_bianhao == 12) && (tmp_dianyang_jiance_hangshu < jiance_hangshu ||work_dianyang_again_status == 1))	
			{
				if (work_dianyang_x_s_hang_status == 0 && work_dianyang_x_s_hang_finish == 0)
				{
					work_dianyang_x_s_hang_status = 1;
					work_dianyang_x_s_hang_monitor_status = 1;

					Set_Y_Value(Y_1,P);
					PulseOut_1_Start(x_jianxie_speed*speed_ratio,dianzhen_liejianju*dianzhen_ratio);
				}
			}

			//转一中下(纵向)&& dianzhen_bianhao != 11 && dianzhen_bianhao != 12
			if (tmp_dianyang_dianzhen_memory != 1 && (tmp_dianyang_dianzhen_memory%2 == 1) && (tmp_dianyang_jiance_hangshu < jiance_hangshu || work_dianyang_again_status == 1))	
			{
				if (work_dianyang_w_m_lie_status == 0 && work_dianyang_w_m_lie_finish == 0)
				{
					work_dianyang_w_m_lie_status = 1;
					work_dianyang_w_m_lie_monitor_status = 1;

					Set_Y_Value(Y_3,P);
					PulseOut_3_Start(w_jianxie_speed*speed_ratio,(jiance_hangjianju*jiance_ratio+jiance_gaodu*jiance_ratio));

				}
			}

	
			//找到位置之后点样开始
			if ((work_dianyang_x_s_finish == 1 || work_dianyang_w_s_lie_finish == 1 
				|| work_dianyang_x_s_hang_finish == 1 || work_dianyang_w_m_lie_finish == 1 
				|| work_dianyang_w_l_lie_finish == 1) && work_dianyang_y_down_status == 0 && work_dianyang_y_down_finish == 0)
			{
				work_dianyang_y_down_status = 1;
				work_dianyang_y_down_monitor_status = 1;

				Set_Y_Value(Y_2,P);
				PulseOut_2_Start(y_xiazheng_speed*speed_ratio,tuye_y*ratio);

			}

			if (work_dianyang_y_down_finish == 1 && work_dianyanging_s_status == 0 && work_dianyanging_s_finish == 0)
			{
				work_dianyanging_s_status = 1;

				work_fn_dianyang();
			}
			
			if (tmp_dianyang_jiance_lieshu < jiance_lieshu )//tmp <=如果没有=则当jiance_lieshu == 1 时，会出错
			{

				if(work_dianyang_x_l_dianyang_status == 1 && work_dianyang_x_l_lock_status == 0)
				{	
					work_dianyang_x_l_lock_status = 1;
					
					work_fn_dianyang();
				}		//单元行数加了以后，第一个没有点样

				if (work_dianyang_x_l_dianyang_status == 0 && work_dianyanging_s_finish == 1 && work_dianyang_x_m_status == 0 && work_dianyang_x_m_finish == 0 )
				{
					work_dianyang_x_m_status = 1;
					work_dianyang_x_m_monitor_status = 1;
				

					Set_Y_Value(Y_1,P);
					PulseOut_1_Start(x_jianxie_speed*speed_ratio,(jiance_liejianju*jiance_ratio+jiance_kuandu*jiance_ratio));
				}

				if (work_dianyang_x_m_finish == 1 && work_dianyanging_m_status == 0 && work_dianyanging_m_finish == 0)
				{
					work_dianyanging_m_status = 1;

					work_fn_dianyang();


				}
				
			}


			if (tmp_dianyang_jiance_lieshu == jiance_lieshu && tmp_dianyang_danyuan_lieshu < danyuan_lieshu
			        && work_dianyang_x_l_status == 0 )//&& work_dianyang_x_l_finish == 0 
			{

				work_dianyang_x_l_status = 1;
				work_dianyang_x_l_monitor_status = 1;


				Set_Y_Value(Y_1,P);
				PulseOut_1_Start(x_jianxie_speed*speed_ratio,(danyuan_liejianju*danyuan_ratio+jiance_liejianju*jiance_ratio+jiance_kuandu*jiance_ratio));//5000

			}


			if (tmp_dianyang_danyuan_lieshu == danyuan_lieshu && tmp_dianyang_jiance_lieshu == jiance_lieshu)
			{
				work_dianyang_x_l_finish = 1;
			}

			if (work_dianyang_x_l_finish == 1 && work_dianyang_y_up_status == 0 && work_dianyang_y_up_finish == 0)
			{
				work_dianyang_y_up_status = 1;
				work_dianyang_y_up_monitor_status = 1;

				Set_Y_Value(Y_2,N);
				PulseOut_2_Start(y_shangzhen_speed*speed_ratio,tuye_y*ratio);//5000

			}


			if (work_dianyang_y_up_finish == 1 && work_dianyang_x_back_status == 0 && work_dianyang_x_back_finish == 0)
			{
				work_dianyang_x_back_status = 1;
				work_dianyang_x_back_monitor_status = 1;

				Set_Y_Value(Y_1,N);
				PulseOut_1_Start(x_huicheng_speed*speed_ratio,Y1_PULSE_TOTAL_BACK);//200 5000

			}

		}
	}
 }


 else if(work_dianyang_allow == 1 && work_dianyang_again_finish == 1)
 {
	 if(work_dianyang_x_get_zero_status == 0 && work_dianyang_x_get_zero_finish == 0)
		{
			work_dianyang_x_get_zero_status = 1;
			work_dianyang_x_get_zero_monitor_status = 1;
		
			Set_Y_Value(Y_1,N);	
			PulseOut_1_Start(x_jianxie_speed*speed_ratio,Y1_PULSE_TOTAL);
		}
		
		if(work_dianyang_w_get_zero_status == 0 && work_dianyang_w_get_zero_finish == 0)
		{
		
			work_dianyang_w_get_zero_status = 1;
			work_dianyang_w_get_zero_monitor_status = 1;
		
			Set_Y_Value(Y_3,N);	
			PulseOut_3_Start(w_jianxie_speed*speed_ratio,Y3_PULSE_TOTAL);
		}
 }
}

void work_fn_dianyang(void)
{
	//Delay();
	int tmp = 0;
	for(tmp = 100000; tmp > 0; tmp--);
	// {

	// }

	monitor_yangpin_no ++;

	// if(start_b == 1)//静态点样
	// {

	if (work_dianyang_x_l_lock_status == 1)
	{
		work_dianyang_x_l_lock_status = 0;
		work_dianyang_x_l_dianyang_status = 0;

	}
	if (work_zhanganing_status == 1)
	{
		work_zhanganing_finish = 1;
	}

	if (work_dianyanging_s_status == 1)
	{
		work_dianyanging_s_status = 0;
		work_dianyanging_s_finish = 1;
	}

	if (work_dianyanging_m_status == 1)
	{
		work_dianyang_x_m_status = 0;
		work_dianyang_x_m_finish = 0;

		work_dianyanging_m_status = 0;//
		work_dianyanging_m_finish = 1;

		//if(tmp_dianyang_jiance_lieshu < jiance_lieshu)//点样结束之后进行++
		tmp_dianyang_jiance_lieshu++;
	}
	// }



	if (work_dianyang_c_start_status == 1)//连续动态点样标志位打开start_once == 1
	{

		if (dianzhen_bianhao != 11 && dianzhen_bianhao != 12)
		{

			tmp_dianyang_times_remember++;//作为点样计数器系数

			if ((tmp_dianyang_times_remember%2 == 1) && (tmp_dianyang_times_remember != 1))
			{

				if (tmp_dianyang_jiance_hangshu < jiance_hangshu)
					tmp_dianyang_jiance_hangshu++;
			}


			if (tmp_dianyang_jiance_hangshu == jiance_hangshu && ((tmp_dianyang_times_remember-1)%tmp_dianyang_jiance_hangshu == 0))
			{
				if (tmp_dianyang_danyuan_hangshu == danyuan_hangshu)
				{
					tmp_dianyang_danyuan_hangshu = 1;
					tmp_dianyang_jiance_hangshu = 1;
				}

				if (tmp_dianyang_danyuan_hangshu < danyuan_hangshu && tmp_dianyang_jiance_hangshu == jiance_hangshu)
				{
					tmp_dianyang_danyuan_hangshu ++;
					tmp_dianyang_jiance_hangshu = 1;

				}
				tmp_dianyang_times_remember = 1;

			}
		}

		if (dianzhen_bianhao == 11 || dianzhen_bianhao == 12)
		{
			if(tmp_dianyang_jiance_hangshu == jiance_hangshu)
			{
				tmp_dianyang_jiance_hangshu_last_status = 1;
			}
			
			if (tmp_dianyang_jiance_hangshu < jiance_hangshu)
				tmp_dianyang_jiance_hangshu++;

			if (tmp_dianyang_jiance_hangshu == jiance_hangshu && tmp_dianyang_jiance_hangshu_last_status == 1)
			{
				if (tmp_dianyang_danyuan_hangshu == danyuan_hangshu)
				{
					tmp_dianyang_danyuan_hangshu = 1;
					tmp_dianyang_jiance_hangshu = 1;
					//tmp_dianyang_c_x_s_hang_remember = 0;
				}

				if (tmp_dianyang_danyuan_hangshu < danyuan_hangshu && tmp_dianyang_jiance_hangshu == jiance_hangshu)
				{
					tmp_dianyang_danyuan_hangshu ++;
					tmp_dianyang_jiance_hangshu = 1;
				}
				
				tmp_dianyang_jiance_hangshu_last_status = 0;
			}
			
			
		}

	}
}

void work_get_zero_finish()
{
	 work_clean_x_status = 0;
	 work_clean_x_finish = 0;
	
	 work_clean_y_status = 0;
	 work_clean_y_finish = 0;
	
	 work_clean_x_monitor_status = 0;
	 work_clean_y_monitor_status = 0;
	 work_clean_back_monitor_status = 0;
	
	
	  work_cleaning_status = 0;
	  work_cleaning_finish = 0;
	  work_clean_back_status = 0;
	  work_clean_back_finish = 0;
	  work_clean_start_status = 0;

	
	  quyang_allow = 0;
	  quyang_status = 0;
	
	  work_quyang_x_status = 0;
	  work_quyang_x_finish = 0;
	
	  work_quyang_y_status = 0;
	  work_quyang_y_finish = 0;
	
	  work_quyang_x_monitor_status = 0;
	  work_quyang_y_monitor_status = 0;
	  work_quyang_back_monitor_status = 0;
	
	  work_quyanging_status = 0;
	  work_quyanging_finish = 0;
	  work_quyang_back_status = 0;
	  work_quyang_back_finish = 0;
	  work_quyang_start_status = 0;

	
	  pre_dianyang_allow = 0;
	  pre_dianyang_status = 0;
	
	  work_pre_dianyang_x_status = 0;
	  work_pre_dianyang_x_finish = 0;
	
	  work_pre_dianyang_y_status = 0;
	  work_pre_dianyang_y_finish = 0;
	
	  work_pre_dianyang_x_monitor_status = 0;
	  work_pre_dianyang_y_monitor_status = 0;
	  work_pre_dianyang_back_monitor_status = 0;
	
	  work_pre_dianyanging_status = 0;
	  work_pre_dianyanging_finish = 0;
	  work_pre_dianyang_back_status = 0;
	  work_pre_dianyang_back_finish = 0;
	  work_pre_dianyang_start_status = 0;
	
	  work_pre_dianyang_w_status = 0;
	  work_pre_dianyang_w_finish = 0;
	  work_pre_dianyang_w_monitor_status = 0;
	
	
	  zhangan_allow = 0;
	  zhangan_status = 0;
	
	  work_zhangan_x_status = 0;
	  work_zhangan_x_finish = 0;
	
	  work_zhangan_y_status = 0;
	  work_zhangan_y_finish = 0;
	
	  work_zhangan_x_monitor_status = 0;
	  work_zhangan_y_monitor_status = 0;
	  work_zhangan_back_monitor_status = 0;
	
	  work_zhanganing_status = 0;
	  work_zhanganing_finish = 0;
	  work_zhangan_back_status = 0;
	  work_zhangan_back_finish = 0;
	  work_zhangan_start_status = 0;
	
	  work_zhangan_w_status = 0;
	  work_zhangan_w_finish = 0;
	  work_zhangan_w_monitor_status = 0;
	
	  work_zhangan_w_back_status = 0;
	  work_zhangan_w_back_monitor_status = 0;
	  work_zhangan_w_back_finish = 0;
	
	  work_zhangan_x_back_status = 0;
	  work_zhangan_x_back_monitor_status = 0;
	  work_zhangan_x_back_finish = 0;
	
	  dianyang_allow = 0;
	
	  dianyang_status = 0;
	
	  work_dianyang_x_s_status = 0;
	  work_dianyang_x_s_finish = 0;
	  work_dianyang_x_s_monitor_status = 0;
	
	  work_dianyang_x_m_status = 0;
	  work_dianyang_x_m_finish = 0;
	  work_dianyang_x_m_monitor_status = 0;

	  work_dianyang_x_l_status = 0;
	  work_dianyang_x_l_finish = 0;
	  work_dianyang_x_l_monitor_status = 0;
	
	  work_dianyang_w_s_lie_status = 0;
	  work_dianyang_w_s_lie_finish = 0;
	  work_dianyang_w_s_lie_monitor_status = 0;
		
	  work_dianyang_x_s_hang_status = 0;
	  work_dianyang_x_s_hang_finish = 0;
	  work_dianyang_x_s_hang_monitor_status = 0;		
		
	
	  work_dianyang_w_m_lie_status = 0;
	  work_dianyang_w_m_lie_finish = 0;
	  work_dianyang_w_m_lie_monitor_status = 0;

	  work_dianyang_w_l_lie_status = 0;
	  work_dianyang_w_l_lie_finish = 0;
	  work_dianyang_w_l_lie_monitor_status = 0;
	
	  work_dianyang_y_down_status = 0;
	  work_dianyang_y_down_finish = 0;	
	  work_dianyang_y_down_monitor_status = 0;
	
	  work_dianyang_y_up_status = 0;
	  work_dianyang_y_up_finish = 0;	
	  work_dianyang_y_up_monitor_status = 0;
	

	
	  work_dianyanging_s_status = 0;
	  work_dianyanging_s_finish = 0;
	  work_dianyanging_m_status = 0;
	  work_dianyanging_m_finish = 0;	
	  work_dianyanging_l_status = 0;
	  work_dianyanging_l_finish = 0;
	
	
	  work_dianyang_back_status = 0;
	  work_dianyang_back_finish = 0;
	  work_dianyang_start_status = 0;
	
	  work_dianyang_w_s_status = 0;
	  work_dianyang_w_s_finish = 0;
	  work_dianyang_w_s_monitor_status = 0;
	
	  work_dianyang_w_m_status = 0;
	  work_dianyang_w_m_finish = 0;
	  work_dianyang_w_m_monitor_status = 0;
	
	  work_dianyang_w_l_status = 0;
	  work_dianyang_w_l_finish = 0;
	  work_dianyang_w_l_monitor_status = 0;
	
	
	  work_dianyang_w_back_status = 0;
	  work_dianyang_w_back_monitor_status = 0;
	  work_dianyang_w_back_finish = 0;
	
	  work_dianyang_x_back_status = 0;
	  work_dianyang_x_back_monitor_status = 0;
	  work_dianyang_x_back_finish = 0;
	
	  work_dianyang_allow = 0;
	
	  work_dianyang_x_l_dianyang_status = 0;
	  work_dianyang_x_l_lock_status = 0;
	
	  work_dianyang_again_status = 0;
	  work_dianyang_again_finish = 0;
	  work_dianyang_again_counter = 0;
	  
	  work_dianyang_x_get_zero_status = 0;
	  work_dianyang_x_get_zero_finish = 0;
	  work_dianyang_x_get_zero_monitor_status = 0;

	 work_dianyang_w_get_zero_status = 0;
	 work_dianyang_w_get_zero_finish = 0;
	 work_dianyang_w_get_zero_monitor_status = 0;
	 
	 tmp_dianyang_dianzhen_memory = 1;
}

void work_dianyang_continuous(void)
{

 if(work_dianyang_allow == 0)
 {
	
	if (tmp_dianyang_danyuan_lieshu <= danyuan_lieshu)
	{		
		if (tmp_dianyang_jiance_lieshu <= jiance_lieshu)
		{

			if (tmp_dianyang_jiance_lieshu == 1 && tmp_dianyang_danyuan_lieshu == 1)//找到初始位置
			{
				if (work_dianyang_c_w_s_status == 0 && work_dianyang_c_w_s_finish == 0)//在此之前已经添加判断条件
				{
					work_dianyang_c_w_s_status = 1;
					work_dianyang_c_w_s_monitor_status = 1;


					switch (yangpin_bianhao)
					{
					case 1:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_1_lie_Distance);
						break;

					case 2:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_2_lie_Distance);
						break;

					case 3:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_3_lie_Distance);
						break;

					case 4:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_4_lie_Distance);
						break;

					case 5:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_5_lie_Distance);
						break;

					case 6:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_6_lie_Distance);
						break;

					case 7:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_7_lie_Distance);
						break;

					case 8:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_8_lie_Distance);
						break;


					case 9:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_9_lie_Distance);
						break;

					case 10:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_10_lie_Distance);
						break;

					case 11:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_11_lie_Distance);
						break;

					case 12:
						Set_Y_Value(Y_3,P);
						PulseOut_3_Start(w_jianxie_speed*speed_ratio,dianzhen_hangjianju*DZ_12_lie_Distance);
						break;

					default:
						break;

					}
				}

				if (work_dianyang_c_w_s_finish == 1 && work_dianyang_c_x_s_status == 0 && work_dianyang_c_x_s_finish == 0)
				{
					work_dianyang_c_x_s_status = 1;
					work_dianyang_c_x_s_monitor_status = 1;

					switch (yangpin_bianhao)
					{
					case 1:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_1_hang_Distance);
						break;

					case 2:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_2_hang_Distance);
						break;

					case 3:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_3_hang_Distance);
						break;

					case 4:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_4_hang_Distance);
						break;

					case 5:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_5_hang_Distance);
						break;

					case 6:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_6_hang_Distance);
						break;

					case 7:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_7_hang_Distance);
						break;

					case 8:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_8_hang_Distance);
						break;


					case 9:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_9_hang_Distance);
						break;

					case 10:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_10_hang_Distance);
						break;

					case 11:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_11_hang_Distance);
						break;

					case 12:
						Set_Y_Value(Y_1,P);
						PulseOut_1_Start(w_jianxie_speed*speed_ratio,dianzhen_liejianju*DZ_12_hang_Distance);
						break;

					default:
						break;

					}

				}
			}
			
	
			//连续点样找到位置之后点样Y轴开始下针
			if ((work_dianyang_c_x_s_finish == 1) && work_dianyang_c_y_down_status == 0 && work_dianyang_c_y_down_finish == 0)//|| work_dianyang_w_s_lie_finish == 1 || work_dianyang_x_s_hang_finish == 1 || work_dianyang_w_m_lie_finish == 1 || work_dianyang_w_l_lie_finish == 1
			{
				work_dianyang_c_y_down_status = 1;
				work_dianyang_c_y_down_monitor_status = 1;

				Set_Y_Value(Y_2,P);
				PulseOut_2_Start(y_xiazheng_speed*speed_ratio,tuye_y*ratio);

			}
			
			
			//连续点样找到位置之后点样w轴开始转动
			if (work_dianyang_c_y_down_finish == 1 && work_dianyang_c_w_start_status == 0 && work_dianyang_c_w_start_finish == 0)
			{
				work_dianyang_c_w_start_status = 1;		//防止重复进入
				work_dianyang_c_w_start_monitor_status = 1;

				Set_Y_Value(Y_3,P);
				PulseOut_3_Start(w_lianxu_speed*speed_ratio,-1);
				
				//work_dianyang_c_start = 1;	//开始标志位
			}

			
			if((dianzhen_bianhao == 11 || dianzhen_bianhao == 12) && tmp_dianyang_jiance_lieshu <= jiance_lieshu && c_jiance_lieshu_status == 0)
			{
				if((c_s_move_allow == 1) && (work_dianyang_c_x_s_hang_status == 0) && (work_dianyang_c_x_s_hang_finish == 0) && (tmp_dianyang_c_x_s_hang_remember%2 == 1))
				{
					work_dianyang_c_x_s_hang_status = 1;
					work_dianyang_c_x_s_hang_monitor_status = 1;
					//Beep(1);
					work_dianyang_c_start = 0;
					
					Set_Y_Value(Y_1,P);
					PulseOut_1_Start((x_jinji_speed*speed_ratio),dianzhen_liejianju);//5000
				}
				
				
				if((c_m_move_allow == 1) &&(work_dianyang_c_x_m_status == 0) && (work_dianyang_c_x_m_finish == 0) && (tmp_dianyang_c_x_s_hang_remember%2 == 0))
				{
					work_dianyang_c_x_m_status = 1;
					work_dianyang_c_x_m_monitor_status = 1;
					
					work_dianyang_c_start = 0;
					
					Set_Y_Value(Y_1,P);
					PulseOut_1_Start((x_jinji_speed*speed_ratio),(jiance_liejianju + jiance_kuandu));//5000
				}
			} 
			
				
			if(dianzhen_bianhao != 11 && dianzhen_bianhao != 12 && tmp_dianyang_jiance_lieshu <= jiance_lieshu && c_jiance_lieshu_status == 0)//检测列数<=设置数
			{

				if((c_m_move_allow == 1) && work_dianyang_c_x_m_status == 0 && work_dianyang_c_x_m_finish == 0)//
				{
					work_dianyang_c_x_m_status = 1;
					work_dianyang_c_x_m_monitor_status = 1;
					//Beep(1);
					work_dianyang_c_start = 0;
					
					Set_Y_Value(Y_1,P);
					PulseOut_1_Start((x_jinji_speed*speed_ratio),(jiance_liejianju + jiance_kuandu));//5000
				}
	
			}
			
			if(tmp_dianyang_danyuan_lieshu < danyuan_lieshu)//此处< 因为最后一个单元++完之后到了最后一个单元之后，m直接做完就可以结束，并不需要==（可能会是错误的）
			{
				if(c_l_move_allow == 1 && work_dianyang_c_x_l_status == 0 && work_dianyang_c_x_l_finish == 0)
				{
					//Beep(1);
					work_dianyang_c_x_l_status = 1;
					work_dianyang_c_x_l_monitor_status = 1;
					
					work_dianyang_c_start = 0;
					
					Set_Y_Value(Y_1,P);
					PulseOut_1_Start((x_jinji_speed*speed_ratio),(jiance_liejianju + jiance_kuandu+danyuan_liejianju));
				}
				
			}
			

			if( work_dianyang_c_working_finish  == 1 && work_dianyang_c_y_up_status == 0 && work_dianyang_c_y_up_finish == 0)
			{
				work_dianyang_c_y_up_status = 1;
				work_dianyang_c_y_up_monitor_status = 1;
				
				Set_Y_Value(Y_2,N);
				PulseOut_2_Start(y_shangzhen_speed*speed_ratio,tuye_y*ratio);
			}
				
				
				
			if (work_dianyang_c_y_up_finish == 1 && work_dianyang_c_x_back_status == 0 && work_dianyang_c_x_back_finish == 0)
			{
				work_dianyang_c_x_back_status = 1;
				work_dianyang_c_x_back_monitor_status = 1;
				
				
				Set_Y_Value(Y_1,N);
				
				PulseOut_1_Start(x_jianxie_speed*speed_ratio,Y1_PULSE_TOTAL);//200 5000

			}
			
			if(work_dianyang_c_x_back_finish == 1 && work_dianyang_c_w_stop_status == 0 && work_dianyang_c_w_stop_finish == 0)
			{
				work_dianyang_c_w_stop_status = 1;
				work_dianyang_c_w_stop_status_monitor = 1;
				
				PulseOut_3_Stop();
			//	Beep(1);
				OutPulse_Counter[3] = 0;//需要置零 不然无法回零和起初多做一次相同
				
				Set_Y_Value(Y_3,N);
					
				if(Y3_pulse_remember != 0)
				PulseOut_3_Start(w_lianxu_speed*speed_ratio,Y3_PULSE_TOTAL);
				
				else if(Y3_pulse_remember == 0)
				{
					work_dianyang_c_w_stop_status = 0;
					work_dianyang_c_w_stop_status_monitor = 0;
					work_dianyang_c_w_stop_finish = 1;
				}
				
				
			}

		}
	}
 }


/*  else if(work_dianyang_allow == 1 && work_dianyang_again_finish == 1)
 {
	 if(work_dianyang_x_get_zero_status == 0 && work_dianyang_x_get_zero_finish == 0)
		{
			work_dianyang_x_get_zero_status = 1;
			work_dianyang_x_get_zero_monitor_status = 1;
		
			Set_Y_Value(Y_1,N);	
			PulseOut_1_Start(x_jianxie_speed,Y1_pulse_remember);
		}
		
		if(work_dianyang_w_get_zero_status == 0 && work_dianyang_w_get_zero_finish == 0)
		{
			
			work_dianyang_w_get_zero_status = 1;
			work_dianyang_w_get_zero_monitor_status = 1;
		
			Set_Y_Value(Y_3,N);	
			PulseOut_3_Start(w_jianxie_speed,Y3_pulse_remember);
		}
 } */

}


void work_get_zero_finish_c(void)
{
	  work_clean_x_status = 0;
	 work_clean_x_finish = 0;
	
	 work_clean_y_status = 0;
	 work_clean_y_finish = 0;
	
	 work_clean_x_monitor_status = 0;
	 work_clean_y_monitor_status = 0;
	 work_clean_back_monitor_status = 0;
	
	
	  work_cleaning_status = 0;
	  work_cleaning_finish = 0;
	  work_clean_back_status = 0;
	  work_clean_back_finish = 0;
	  work_clean_start_status = 0;

	
	  quyang_allow = 0;
	  quyang_status = 0;
	
	  work_quyang_x_status = 0;
	  work_quyang_x_finish = 0;
	
	  work_quyang_y_status = 0;
	  work_quyang_y_finish = 0;
	
	  work_quyang_x_monitor_status = 0;
	  work_quyang_y_monitor_status = 0;
	  work_quyang_back_monitor_status = 0;
	
	  work_quyanging_status = 0;
	  work_quyanging_finish = 0;
	  work_quyang_back_status = 0;
	  work_quyang_back_finish = 0;
	  work_quyang_start_status = 0;

	
	  pre_dianyang_allow = 0;
	  pre_dianyang_status = 0;
	
	  work_pre_dianyang_x_status = 0;
	  work_pre_dianyang_x_finish = 0;
	
	  work_pre_dianyang_y_status = 0;
	  work_pre_dianyang_y_finish = 0;
	
	  work_pre_dianyang_x_monitor_status = 0;
	  work_pre_dianyang_y_monitor_status = 0;
	  work_pre_dianyang_back_monitor_status = 0;
	
	  work_pre_dianyanging_status = 0;
	  work_pre_dianyanging_finish = 0;
	  work_pre_dianyang_back_status = 0;
	  work_pre_dianyang_back_finish = 0;
	  work_pre_dianyang_start_status = 0;
	
	  work_pre_dianyang_w_status = 0;
	  work_pre_dianyang_w_finish = 0;
	  work_pre_dianyang_w_monitor_status = 0;
	
	
	  zhangan_allow = 0;
	  zhangan_status = 0;
	
	  work_zhangan_x_status = 0;
	  work_zhangan_x_finish = 0;
	
	  work_zhangan_y_status = 0;
	  work_zhangan_y_finish = 0;
	
	  work_zhangan_x_monitor_status = 0;
	  work_zhangan_y_monitor_status = 0;
	  work_zhangan_back_monitor_status = 0;
	
	  work_zhanganing_status = 0;
	  work_zhanganing_finish = 0;
	  work_zhangan_back_status = 0;
	  work_zhangan_back_finish = 0;
	  work_zhangan_start_status = 0;
	
	  work_zhangan_w_status = 0;
	  work_zhangan_w_finish = 0;
	  work_zhangan_w_monitor_status = 0;
	
	  work_zhangan_w_back_status = 0;
	  work_zhangan_w_back_monitor_status = 0;
	  work_zhangan_w_back_finish = 0;
	
	  work_zhangan_x_back_status = 0;
	  work_zhangan_x_back_monitor_status = 0;
	  work_zhangan_x_back_finish = 0;
	
	  dianyang_allow = 0;
	
	  dianyang_c_status = 0;
	
	  work_dianyang_c_x_s_status = 0;
	  work_dianyang_c_x_s_finish = 0;
	  work_dianyang_c_x_s_monitor_status = 0;
	
	  work_dianyang_c_x_s_hang_status = 0;
	  work_dianyang_c_x_s_hang_finish = 0;
	  work_dianyang_c_x_s_hang_monitor_status = 0;
	
	  work_dianyang_c_x_m_status = 0;
	  work_dianyang_c_x_m_finish = 0;
	  work_dianyang_c_x_m_monitor_status = 0;

	  work_dianyang_c_x_l_status = 0;
	  work_dianyang_c_x_l_finish = 0;
	  work_dianyang_c_x_l_monitor_status = 0;
	
	  work_dianyang_c_w_s_status = 0;
	  work_dianyang_c_w_s_finish = 0;
	  work_dianyang_c_w_s_monitor_status = 0;
	
	  work_dianyang_c_w_m_status = 0;
	  work_dianyang_c_w_m_finish = 0;
	  work_dianyang_c_w_m_monitor_status = 0;
	
	  work_dianyang_c_w_l_status = 0;
	  work_dianyang_c_w_l_finish = 0;
	  work_dianyang_c_w_l_monitor_status = 0;
	
	
	  work_dianyang_c_w_back_status = 0;
	  work_dianyang_c_w_back_monitor_status = 0;
	  work_dianyang_c_w_back_finish = 0;
	
	  work_dianyang_c_x_back_status = 0;
	  work_dianyang_c_x_back_monitor_status = 0;
	  work_dianyang_c_x_back_finish = 0;
	
	  work_dianyang_c_y_down_status = 0;
	  work_dianyang_c_y_down_finish = 0;	
	  work_dianyang_c_y_down_monitor_status = 0;
	
	  work_dianyang_c_y_up_status = 0;
	  work_dianyang_c_y_up_finish = 0;	
	  work_dianyang_c_y_up_monitor_status = 0;
	
	  work_dianyang_c_w_start_status = 0;
	  work_dianyang_c_w_start_monitor_status = 0;
	  work_dianyang_c_w_start_finish = 0;
	
	  work_dianyang_c_start = 0;
	  c_m_move_allow = 0;
	  work_dianyang_c_start_status = 0;
	
	  c_jiance_lieshu_status = 0;
	  c_l_move_allow = 0;

	  work_dianyang_c_working_finish = 0;
	
	  work_dianyang_c_w_stop_status = 0;
	  work_dianyang_c_w_stop_status_monitor = 0;
	  work_dianyang_c_w_stop_finish = 0;
	  
	  tmp_dianyang_c_x_s_hang_remember = 0;
	  
	  tmp_dianyang_jiance_hangshu_last_status = 0;
	  c_s_move_allow = 0;
	
}

void pulse_cal(void)
{
	Y1_PULSE_TOTAL = Y1_pulse_remember_add*65535 + Y1_pulse_remember;
	Y2_PULSE_TOTAL = Y2_pulse_remember_add*65535 + Y2_pulse_remember;
	Y3_PULSE_TOTAL = Y3_pulse_remember_add*65535 + Y3_pulse_remember;
	
	Y1_PULSE_TOTAL_BACK = Y1_pulse_remember_back_add*65535 + Y1_dianyang_x_back_remember;
	
	
	Y1_BIAODING_PULSE_TOTAL = ((x_biaoding_pulse*dingchang_x*100.0)/x_biaoding_length*100000);//   
	Y2_BIAODING_PULSE_TOTAL = ((y_biaoding_pulse*dingchang_y*10.0)/y_biaoding_length*1);//0   00000
	Y3_BIAODING_PULSE_TOTAL = ((w_biaoding_pulse*dingchang_w*100.0)/w_biaoding_length*100000);//   
	
}

void biaoding(void)
{
	if (start_b == 0 && start_once == 0)
	{
		//清零按钮
		
		if(x_biaoding_qingling == 1)
		{
			monitor_x_biaoding = 0;
			Y1_pulse_remember_add = 0;
			Y1_pulse_remember = 0;
			
		}
		
		if(y_biaoding_qingling == 1)
		{
			monitor_y_biaoding = 0;
			Y2_pulse_remember_add = 0;
			Y2_pulse_remember = 0;
		}
		
		if(w_biaoding_qingling == 1)
		{
			monitor_w_biaoding = 0;
			Y3_pulse_remember_add = 0;
			Y3_pulse_remember = 0;
		}
		
		
		//x定长控制
		if(x_biaoding_move == 1 && biaoding_status[0] == 0)
		{
			Set_Y_Value(Y_1,P);
			PulseOut_1_Start(x_diandong_speed*speed_ratio,(x_biaoding_pulse*ratio));
			//Beep(1);
			biaoding_status[0]=1;
			
		}
		
		
		//Y定长控制
		if( y_biaoding_move == 1 && biaoding_status[1] == 0)
		{
			Set_Y_Value(Y_2,P);
			PulseOut_2_Start(y_diandong_speed*speed_ratio,(y_biaoding_pulse*ratio));
			
			biaoding_status[1]=1;
			
		}
		
		
		//w定长
		if( w_biaoding_move == 1 && biaoding_status[2] == 0)
		{
			Set_Y_Value(Y_3,P);
			PulseOut_3_Start(w_diandong_speed*speed_ratio,(w_biaoding_pulse*ratio));
			
			biaoding_status[2] = 1;
		}
		
	}
	
	//定长停止 //在appbase中完成
	if(biaoding_status[0] == 1 && x_biaoding_move == 0 )
	{
		PulseOut_1_Stop();
		biaoding_status[0] = 0;
	}
	
	if(biaoding_status[1] == 1 && y_biaoding_move == 0)
	{
		PulseOut_2_Stop();
		biaoding_status[1] = 0;
	}
	
	if(biaoding_status[2] == 1 && w_biaoding_move == 0)
		{
		PulseOut_3_Stop();
		biaoding_status[2] = 0;
	}
}

void accelerate(unsigned int OutPulse_PreSet, int frequence)
{
	//加速
	//Beep(1);
		if((OutPulse_PreSet/2) == 0)
		{
			rTCNTB1= 300300/(unsigned int)(frequence/5);
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/100)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/4);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/90)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/3);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/80)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/2);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/70)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.8);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/60)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.5);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/50)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.3);	
		}
		
		if((OutPulse_PreSet/2) == ((OutPulse_PreSet/40)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1);	
		}
		
		
		// if((OutPulse_PreSet/2) == ((OutPulse_PreSet/3)))
		// {
			// rTCNTB1= 300300/(frequence);	
		// }
		
		
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/3)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.3);	
		}
		
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/4)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.5);	
		}
		
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/5)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/1.8);	
		}
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/6)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/2);	
		}
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/7)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/3);	
		}
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/8)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/4);	
		}
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/9)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/5);	
		}
		
		if((OutPulse_PreSet/2) == (OutPulse_PreSet-(OutPulse_PreSet/10)))
		{
			rTCNTB1= 300300/(unsigned int)(frequence/6);	
		}
}