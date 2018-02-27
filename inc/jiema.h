

void Get_CurNext_Block_Page(INT16U Specify_Scr_Num);   //Specify_Scr_Num: from 1 to 528
void GetScreenData(INT16U Specify_Scr_Num);
void DrawKeepButton(INT8U Statue,INT8U refreshflag);
void DrawUpButton(INT8U State);
void DrawShiftButton(INT8U State);
void DrawMultiState(U8 state,U8 FlagRefresh);
void DrawSetValueButton(U8 Statue);
void DrawWatchDispButton(INT16U CurrentPlcValue,INT8U FlagRefresh,INT16U PrePlcValue);
//void DrawNumDispButton(U8 FlagRefresh, U8 WhichNumDisp);
void DrawNumDispButton(U8 FlagRefresh);

void DrawStickChart(void);
void DrawStaticText(void);
void DisplayUserScr(INT16U ScrNum);
void Run(void);



//void TestTTT();











