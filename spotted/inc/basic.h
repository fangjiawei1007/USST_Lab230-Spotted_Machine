#ifndef _BASIC_H_
#define _BASIC_H_

#define init_flag				(g_InteralMemory.KeepBit[500])


//点样仪工作模块函数定义
void parameter_init(void);
void xitong_position_init(void);
//void fn_jog(unsigned char axis, unsigned int diandong_flag, unsigned int freq, unsigned int condition_flag);
void fn_jog_test(void);
void set_parameter(INT16U setting, unsigned int original);
void parameter_input(void);
void button_huchi(unsigned char* button0, unsigned char* button1, unsigned int* status_record,  unsigned int condition_flag);
void button_3_huchi(unsigned char* button0,unsigned char* button1, unsigned char* button2,unsigned int* status_record1,unsigned int* status_record2);
void fn_dingchang(void);
void fn_get_zero(void);

void fn_pos_init(void);
void fn_gongzuo_confirm(void);
void fn_clean_confirm(void);
void fn_quyang_confirm(void);
void fn_zhangan_confirm(void);
void fn_pingshan_confirm(void);
void fn_tuye_confirm(void);

void fn_weitiao(void);
void fn_position(void);
void fn_gongzuo_position(void);
void fn_clean_position(void);
void fn_quyang_position(void);
void fn_zhangan_position(void);
void fn_pingshan_position(void);
void fn_tuye_position(void);

void work_clean(void);
void work_get_zero(void);
void work_quyang(void);
void work_pre_dianyang(void);
void work_zhangan(void);
void work_dianyang(void);
void work_get_zero_finish(void);
void work_dianyang_continuous(void);
void work_get_zero_finish_c(void);

void work_fn_dianyang(void);

void pulse_cal(void);
void biaoding(void);
void accelerate(unsigned int OutPulse_PreSet, int frequence);
#endif