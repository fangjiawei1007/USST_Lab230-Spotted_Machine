

void App_Init(void);



void App_Operation(void);

#define ratio 					10
#define speed_ratio				10

#define danyuan_ratio			1
#define jiance_ratio			1
#define dianzhen_ratio			1
//(g_InteralMemory.KeepWord[151]) 
//(g_InteralMemory.KeepWord[150]) 

//(g_InteralMemory.KeepWord[152])
//(g_InteralMemory.KeepWord[153])69100
//(g_InteralMemory.KeepWord[154])
//(g_InteralMemory.KeepWord[155])
//标定
#define x_biaoding_pulse		13
#define x_biaoding_length		52782000

#define y_biaoding_pulse		1
#define y_biaoding_length		1

#define w_biaoding_pulse		10
#define w_biaoding_length		30429093

#define x_biaoding_move			(g_InteralMemory.KeepBit[350])
#define y_biaoding_move			(g_InteralMemory.KeepBit[351])
#define w_biaoding_move			(g_InteralMemory.KeepBit[352])

#define x_biaoding_qingling		(g_InteralMemory.KeepBit[353])
#define y_biaoding_qingling		(g_InteralMemory.KeepBit[354])
#define w_biaoding_qingling		(g_InteralMemory.KeepBit[355])

#define monitor_x_biaoding		(g_InteralMemory.KeepWord[156])
#define monitor_y_biaoding		(g_InteralMemory.KeepWord[157])
#define monitor_w_biaoding		(g_InteralMemory.KeepWord[158])


//上电是否回到上次位置
#define power_previous			(g_InteralMemory.KeepBit[88])
#define power_zero				(g_InteralMemory.KeepBit[208])

//点样针运动参数
#define x_jinji_speed			((g_InteralMemory.KeepWord[10]))//*speed_ratio
#define x_jianxie_speed			((g_InteralMemory.KeepWord[11]))//*speed_ratio
#define x_diandong_speed		((g_InteralMemory.KeepWord[12]))//*speed_ratio
#define x_huicheng_speed		((g_InteralMemory.KeepWord[13]))//*speed_ratio
	
#define w_lianxu_speed			((g_InteralMemory.KeepWord[14]))//*speed_ratio
#define w_jianxie_speed			((g_InteralMemory.KeepWord[15]))//*speed_ratio
#define w_diandong_speed		((g_InteralMemory.KeepWord[16]))//*speed_ratio
//ps：17空余
#define y_shangzhen_speed		((g_InteralMemory.KeepWord[18]))//*speed_ratio
#define y_xiazheng_speed		((g_InteralMemory.KeepWord[19]))//*speed_ratio
#define y_diandong_speed		((g_InteralMemory.KeepWord[20]))//*speed_ratio

//点样参数
#define	danyuan_hangshu			(g_InteralMemory.KeepWord[21])
#define	danyuan_lieshu			(g_InteralMemory.KeepWord[22])
#define	danyuan_hangjianju		(g_InteralMemory.KeepWord[23])
#define	danyuan_liejianju		(g_InteralMemory.KeepWord[24])
#define danyuan_gaodu			(50)

#define	yangpin_hangshu			(g_InteralMemory.KeepWord[25])
#define	yangpin_lieshu			(g_InteralMemory.KeepWord[26])
#define yangpin_bianhao			(g_InteralMemory.KeepWord[27])

#define jiance_hangshu			(g_InteralMemory.KeepWord[28])
#define jiance_lieshu			(g_InteralMemory.KeepWord[29])
#define jiance_hangjianju		(g_InteralMemory.KeepWord[30])
#define jiance_liejianju		(g_InteralMemory.KeepWord[31])
#define jiance_gaodu			(g_InteralMemory.KeepWord[32])
#define jiance_kuandu			(g_InteralMemory.KeepWord[33])

#define dianzhen_hangshu		(g_InteralMemory.KeepWord[34])
#define dianzhen_lieshu			(g_InteralMemory.KeepWord[35])
#define dianzhen_hangjianju		(g_InteralMemory.KeepWord[36])
#define dianzhen_liejianju		(g_InteralMemory.KeepWord[37])
#define dianzhen_bianhao		(g_InteralMemory.KeepWord[38])

//点样针参数，不是太理解, ps:跳过了39
#define delaytime				(g_InteralMemory.KeepWord[40])
#define pulsewidth				(g_InteralMemory.KeepWord[41])
#define dianyang_voltage		(g_InteralMemory.KeepWord[42])
#define dianyang_speed			(g_InteralMemory.KeepWord[43])
#define xiye_speed				(g_InteralMemory.KeepWord[44])
#define tuye_speed				(g_InteralMemory.KeepWord[45])
#define clean_times				(g_InteralMemory.KeepWord[46])
#define dianyang_times			(g_InteralMemory.KeepWord[47])
#define pinshan_freq			(g_InteralMemory.KeepWord[48])

#define pinshan_func			(g_InteralMemory.KeepBit[50])

#define tmp_dianyang_times_remember 		(g_InteralMemory.KeepBit[51])
#define tmp_dianyang_c_x_s_hang_remember	(g_InteralMemory.KeepBit[52])

//监控
#define monitor_danyuan_hang	(g_InteralMemory.KeepWord[100])
#define monitor_danyuan_lie		(g_InteralMemory.KeepWord[101])
#define monitor_jiance_hang		(g_InteralMemory.KeepWord[102])
#define monitor_jiance_lie		(g_InteralMemory.KeepWord[103])
#define monitor_yangpin_no		(g_InteralMemory.KeepWord[104]) //样品编号
#define monitor_dianzhen_no		(g_InteralMemory.KeepWord[105])	//点阵编号

#define monitor_x				(g_InteralMemory.KeepWord[106])
#define monitor_y				(g_InteralMemory.KeepWord[107])
#define monitor_w				(g_InteralMemory.KeepWord[108])

#define start_b					(g_InteralMemory.KeepBit[300])
#define start_once				(g_InteralMemory.KeepBit[301])

//手动调试
#define diandong_x_p			(g_InteralMemory.KeepBit[60])
#define diandong_x_n			(g_InteralMemory.KeepBit[61])
#define diandong_y_p			(g_InteralMemory.KeepBit[62])
#define diandong_y_n			(g_InteralMemory.KeepBit[63])
#define diandong_w_p			(g_InteralMemory.KeepBit[64])
#define diandong_w_n			(g_InteralMemory.KeepBit[65])

#define dingchang_x				((g_InteralMemory.KeepWord[60]))//*ratio
#define dingchang_y				((g_InteralMemory.KeepWord[61]))//*ratio
#define dingchang_w				((g_InteralMemory.KeepWord[62]))//*ratio
#define x_dingchang_pb			(g_InteralMemory.KeepBit[70])
#define y_dingchang_pb			(g_InteralMemory.KeepBit[71])
#define w_dingchang_pb			(g_InteralMemory.KeepBit[72])
#define x_dingchang_nb			(g_InteralMemory.KeepBit[73])
#define y_dingchang_nb			(g_InteralMemory.KeepBit[74])
#define w_dingchang_nb			(g_InteralMemory.KeepBit[75])

#define x_get_zero				(g_InteralMemory.KeepBit[76])
#define y_get_zero				(g_InteralMemory.KeepBit[77])
#define w_get_zero				(g_InteralMemory.KeepBit[78])

#define clean					(g_InteralMemory.KeepBit[79])//打开清洗液
#define dry						(g_InteralMemory.KeepBit[80])//蘸干

//点样方式
#define single_mode				(g_InteralMemory.KeepBit[201])
#define multi_mode				(g_InteralMemory.KeepBit[202])

#define sup_dy_hang				(g_InteralMemory.KeepWord[70])
#define sup_dy_lie				(g_InteralMemory.KeepWord[71])
#define sup_jc_hang				(g_InteralMemory.KeepWord[72])
#define sup_jc_lie				(g_InteralMemory.KeepWord[73])
#define sup_yp					(g_InteralMemory.KeepWord[74])	//样品补充
#define sup_dz					(g_InteralMemory.KeepWord[75])	//点阵补充

//系统位置设置

#define pos_init				(g_InteralMemory.KeepBit[302])	//系统位置初始化


//选择轴
#define xitong_x				((g_InteralMemory.KeepBit[90]))
#define xitong_y				((g_InteralMemory.KeepBit[91]))
#define xitong_w				((g_InteralMemory.KeepBit[92]))

//微调
#define weitiao_p				((g_InteralMemory.KeepBit[93]))
#define weitiao_n				((g_InteralMemory.KeepBit[94]))

//工作原点
#define gongzuo_x				((g_InteralMemory.KeepWord[80]))//*ratio
#define gongzuo_y				((g_InteralMemory.KeepWord[81]))//*ratio
#define gongzuo_w				((g_InteralMemory.KeepWord[82]))//*ratio

#define gongzuo_b				(g_InteralMemory.KeepBit[95])	//工作位置调整
#define gongzuo_c				(g_InteralMemory.KeepBit[96])	//工作位置确认

//清洗位置
#define clean_x					((g_InteralMemory.KeepWord[83]))//*ratio
#define clean_y					((g_InteralMemory.KeepWord[84]))//*ratio
#define clean_w					((g_InteralMemory.KeepWord[85]))//*ratio

#define clean_b					(g_InteralMemory.KeepBit[97])	//清洗位置调整
#define clean_c					(g_InteralMemory.KeepBit[98])	//清洗位置确认

//取样位置
#define quyang_x				((g_InteralMemory.KeepWord[86]))//*ratio
#define quyang_y				((g_InteralMemory.KeepWord[87]))//*ratio
#define quyang_w				((g_InteralMemory.KeepWord[88]))//*ratio

#define quyang_b				(g_InteralMemory.KeepBit[99])	
#define quyang_c				(g_InteralMemory.KeepBit[100])

//蘸干位置
#define zhangan_x				((g_InteralMemory.KeepWord[89]))//*ratio
#define zhangan_y				((g_InteralMemory.KeepWord[90]))//*ratio
#define zhangan_w				((g_InteralMemory.KeepWord[91]))//*ratio

#define zhangan_b				(g_InteralMemory.KeepBit[101])	
#define zhangan_c				(g_InteralMemory.KeepBit[102])


//屏闪位置
#define pingshan_x				((g_InteralMemory.KeepWord[92]))//*ratio
#define pingshan_y				((g_InteralMemory.KeepWord[93]))//*ratio
#define pingshan_w				((g_InteralMemory.KeepWord[94]))//*ratio

#define pingshan_b				(g_InteralMemory.KeepBit[103])	
#define pingshan_c				(g_InteralMemory.KeepBit[104])

//吐液位置
#define tuye_x					((g_InteralMemory.KeepWord[95]))//*ratio
#define tuye_y					((g_InteralMemory.KeepWord[96]))//*ratio
#define tuye_w					((g_InteralMemory.KeepWord[97]))//*ratio

#define tuye_b					(g_InteralMemory.KeepBit[105])	
#define tuye_c					(g_InteralMemory.KeepBit[106])

/* //系统位置初始化
#define xitong_init				(g_InteralMemory.KeepBit[200]) */	//感觉用不太到，将控件删除


//互斥锁用变量

#define start_normal	0
#define start_danbu		1

#define x_diandong_p	0
#define x_diandong_n	1

#define y_diandong_p	0
#define y_diandong_n	1

#define w_diandong_p	0
#define w_diandong_n	1

#define x_dingchang_p	0
#define x_dingchang_n	1

#define y_dingchang_p	0
#define y_dingchang_n	1

#define w_dingchang_p	0
#define w_dingchang_n	1

#define weitiao_p_flag	0
#define weitiao_n_flag	1

#define single_m		0
#define multi_m			1

//Set_Y_Value 用到的com口宏定义(点动)


#define Y_1  5
#define Y_2  6
#define Y_3  7


//正反转
#define N	0	//反转negative
#define P	1	//正转positive


//清洗时间基数
#define clean_pre	5000

//取样时间基数
#define quyang_pre	5000

//蘸干时间
#define zhangan_time 50000


//点样点阵距离
#define DZ_1_lie_Distance	1
#define DZ_2_lie_Distance	3
#define DZ_3_lie_Distance	3
#define DZ_4_lie_Distance	1
#define DZ_5_lie_Distance	1
#define DZ_6_lie_Distance	3
#define DZ_7_lie_Distance	3
#define DZ_8_lie_Distance	1
#define DZ_9_lie_Distance	1
#define DZ_10_lie_Distance	3
#define DZ_11_lie_Distance	0
#define DZ_12_lie_Distance	0

//点样点阵距离
#define DZ_1_hang_Distance	0
#define DZ_2_hang_Distance	0
#define DZ_3_hang_Distance	1
#define DZ_4_hang_Distance	1
#define DZ_5_hang_Distance	2
#define DZ_6_hang_Distance	2
#define DZ_7_hang_Distance	3
#define DZ_8_hang_Distance	3
#define DZ_9_hang_Distance	4
#define DZ_10_hang_Distance	4
#define DZ_11_hang_Distance	1
#define DZ_12_hang_Distance 3
