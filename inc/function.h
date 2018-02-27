#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#define ADC_SAMPLE_NUM 6

typedef struct{

	int x[ADC_SAMPLE_NUM];
	int y[ADC_SAMPLE_NUM];
	int index;
	int enable;

} GOBLEADC;

void KeyBoard_Refresh(void);


int ReadAdc(int ch);
void Test_Adc(void);

void ScrSave_Init(void);
void ScrSave_Start(void);
void ScrSave_Stop(void);
void __irq Timer2_ScrSave(void);



void Ts_SampleStart(void);
void Ts_SampleStop(void);

void Ts_PowerOn(void);
void Ts_PowerOff(void);
void Ts_Init(void);

BOOL Ts_GetXY(int *px, int *py);
BOOL Touch_Pen_Filtering(int *px, int *py);
//void Sampling(void);
void __irq Timer4Sampling(void);
void __irq AdcTsAuto(void);
U8 Ts_Calibrate(void);

void KeyScan(void);
void UserScrKeyScan(void);
void DisplayKeyBoard(char *MaxValueStr,char *MinValueStr);
int KeyBoard(int Max,int Min,U8 DigitPos,U8 DigitNumb);
double KeyBoardScan(double MaxValue,double MinValue,U8 DigitNumb,char Flag);
void AddNum(char *Str,U8 DigitNum,char *Num);

INT8U Get_Element_Region(INT16U *Left,INT16U *Top,INT16U *Right,INT16U *Bottom);
INT8U Point_in_Region(INT16U PointX,INT16U PointY,INT16U Left,INT16U Top,INT16U Right,INT16U Bottom,INT8U RegionType);

int Max(int a,int b);
int Min(int a,int b);
void Check_Ctrl_State(void);


void Func_KeepButton(INT16U StarAddr);
void RefreshKeepButton(void);
void RefreshKeep2Button(void);
void RefreshButtonUp(void);
void Func_UpButton(int StarAddr);
void Func_SetValue(INT16 StarAddr);
void RefreshSetValue(void);
void Func_ShiftButton(int StarAddr);
void UpDown_NumDisp(INT16U plcWriteNum,INT32U CurrentPlcValue);
void RefreshWatchDispButton(void);
void RefreshNumDispButton(void);
void RefreshStickChart(void);
void Func_StickChart(INT16U which_chart);
void RefreshMultiState(void);





void Adc_XY_Init(void);
void Adc_SampleStart(void);
void CalibrateXY(void);




#endif








