#ifndef _SYSTEMCONF_H_
#define _SYSTEMCONF_H_

typedef struct 
{

	int BaudRates;		//波特率
	char ComPort;		//通讯口：0=Uart0; 1=Uart1 
	char ParityMode;	//	0=NONE; 1=ODD; 2=EVEN
	char DataBits;		//	0=5-bits; 1=6-bits; 2=7-bits; 3=8-bit	
	char StopBits;		//	0=One stop bit; 1=Two stop bits	
	char ScrSaveTimeSet;	//	0,1,2,3,5,10,15,20	

	//double a1,b1,c1,d1;	//parameter of the arithmetic function to calculate the X coordinates
						//  X=a1*Rx + b1*Ry + c1
	//double a2,b2,c2,d2;	//parameter of the arithmetic function to calculate the Y coordinates
						//  Y=a2*Rx + b2*Ry + c2
	int XLc,YLc,Xc,Yc;
	double Kx,Ky;


}SYSTEMCONFIG;



typedef struct 
{
	unsigned char LCDType;
	unsigned char PLCType;
	unsigned char StarScr;

	unsigned char  PlcCtrStr[5];
	unsigned short PlcCtrlNum;
	unsigned char  PlcStaStr[5];
	unsigned short PlcStaNum;

	unsigned char  IsEnableBitMap;					//bit    map
	unsigned char  BitTimeInterval;
	unsigned short BitMapCount;
	unsigned char  ScreenBitMapStr[5];
	unsigned short ScreenBitMapNum;
	unsigned char  PLCBitMapStr[5];
	unsigned short PlcBitMapNum;

	unsigned char  IsEnableWordMap;					//word   map
	unsigned char  WordTimeInterval;
	unsigned short WordMapCount;
	unsigned char  ScreenWordMapStr[5];
	unsigned short ScreenWordMapNum;
	unsigned char  PLCWordMapStr[5];
	unsigned short PlcWordMapNum;

	unsigned char  TotalSrceenNum;

	

}SYSTEMINFO;



//********************************************
typedef struct 
{
	
	INT8U	Keep_flag;
	INT16U	Keep_StarAddr;
	INT8U 	Screen_Flag;	//允许切换画面的标志

	char		Keep_PlcWriteStr[5];
	INT16U	Keep_PlcWriteNum;

	INT16U	Timer2Count; 
	INT16U	ScrSaveTime;			  	  //屏保设定时间
	U8		ScrSaveFlag;

///////////定时器070208 ///////////////////
	INT8U	Minute;
	INT8U	Second;
	INT8U	TimerStartFlag;
	INT8U	TimerFinish;


////// 070516 加减值后跟着刷新//////
	INT16U	Which_NumDisp;
	INT32U	UpD_NumDisp[RefreshNum][2];

	char		PrePlcStr[5];

///////////////////////////////////////////////////////
	INT8U	TimeReach_1;
	INT8U	TimeReach_2;
	INT8U	TimeReach_3;

	INT8U 	Time_Alarm_One;
	INT8U 	Time_Alarm_State;

	INT8U	Stop_Communication;

}GLOBALVAR;
//********************************************



typedef struct 
{
	unsigned char KeepBit[1*2048];                          //掉电保护
	//short   KeepWord[1*2048/2];								//
	INT16U KeepWord[1*2048/2];								//

	unsigned char Bit[1*2048];
	//short Word[1*2048/2];
	INT16U Word[1*2048/2];

}INTERALMEMORY;



// Timer2
 typedef struct
  {
	//unsigned char  T10MS_Start;	// Elements Output Status
	//unsigned char  T10MS_Stop;		// Elements Last Scan Loop Value - only used for UI updating
	//unsigned char  T10MS_Pause;	// Last scanned value
	U8 			Timer_Status;
	U32			Timer_Set;   		// Preset Initial Value
	U32			Timer_Cur;			// Virtual memory, Amem, Actual Element Values
  }TIMER2_V;




void LoadSysConf(void);
void StorSysConf(void);

void NumToSaveScrTime(void);

int power2(int n);
void cal_cpu_bus_clk(void);

void HaltUndef(void);
void HaltSwi(void);
void HaltPabort(void);
void HaltDabort(void);
void Isr_Init(void);


void GlobalData_WriteZero(void);

void DataSave_Init (void);
void DataSave_Load (void);
void __irq DataSave_IntHandle(void);



void Led_on_off(void);


#endif


