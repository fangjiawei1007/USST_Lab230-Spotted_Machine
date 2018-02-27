INT8U NA_ReadBit(INT16U DevNumber);
U8 NA_WriteBit(INT16U DevNumber,U8 Value);
int NA_ReadWord(INT16U DevNumber);
U8 NA_WriteWord(INT16U DevNumber,int Value);

U8 FP_ReadBit(char DevChar,INT16U DevNumber);
int FP_ReadWord(char* DevChar,INT16U DevNumber);
U8 FP_WriteBit(char DevChar,INT16U DevNumber,U8 Value);
U8 FP_WriteWord(char* DevChar,INT16U DevNumber,int Value);
char FP_ReadBlockWord(char* DevChar,INT16U DevNumber,int CountNum,int Value[]);
U8 FP_WriteBlockWord(char* DevChar,INT16U DevNumber,int CountNum,int Value[]);
U8 FP_WriteBlockBit(char DevChar,INT16U DevNumber,char CountNum,U8 Value[]);
U8 FP_ReadBlockBit(char DevChar,INT16U DevNumber,char CountNum,char RetArray[8]);

INT16U CRC(U8 *auchMsg,INT8U usDataLen);
INT8U LRC_ADD(U8 *auchMsg,INT16U usDataLen);
U8 LRC(U8 *auchMsg,INT16U usDataLen);
U8 BCC(U8 *auchMsg,INT16U usDataLen);
U8 DataToChar(U8 Data);
U8 CharToData(U8 Data);

INT8U ReadBit(INT16U DevNumber,char *DevName);
INT8U WriteBit(INT16U DevNumber,char *DevName,U8 Value);
int ReadWord(INT16U DevNumber,char *DevName);
INT8U WriteWord(INT16U DevNumber,char *DevName,int Value);

char FX_ReadBit(char DevChar,short DevNumber);
char FX_WriteBit(char DevChar,short DevNumber,char Value);
short FX_ReadWord(char *DevChar,short DevNumber);
char FX_WriteWord(char *DevChar,short DevNumber,int Value);
unsigned short Fx_Addr(char *DevChar,short DevNumber);
unsigned short Fx_Device(char DevChar,short DevNumber);
char Fx_BitValue(char DevChar,short DevNumber,char Data1,char Data2);

////////////////////////////////////////////////////////////////
int DVP_ReadWord(char DevChar,INT16U DevNumber);
U8 DVP_WriteWord(char DevChar,INT16U DevNumber,int Value);
U8 DVP_ReadBit(char DevChar,INT16U DevNumber);
U8 DVP_WriteBit(char DevChar,INT16U DevNumber,U8 Value);
unsigned short DVP_Device(char DevChar,short DevNumber,U8 flag);

//////////////////060706//////////////////////////////
U8 VIGOR_ReadBit(char DevChar,INT16U DevNumber);
U8 VIGOR_WriteBit(char DevChar,INT16U DevNumber,U8 Value);
int VIGOR_ReadWord(char DevChar,INT16U DevNumber);
INT8U VIGOR_WriteWord(char DevChar,INT16U DevNumber,int Value);
INT8U VIGOR_LRC(U8 *auchMsg,INT16U usDataLen);
unsigned short VIGOR_Device(char DevChar,INT16U DevNumber,U8 force_flag);


//U8 NA_WriteBit_Test(INT16U DevNumber,U8 Value);
///////////////////////////////////////////////////
U8 UniTelWay_LRC(U8 *auchMsg,INT16U usDataLen);
INT8U UniTel_ReadBit(INT16U DevNumber);
U8 UniTel_WriteBit(INT16U DevNumber,U8 Value);
int UniTel_ReadWord(INT16U DevNumber);
U8 UniTel_WriteWord(INT16U DevNumber,int Value);

/////////////////////////////////////////////////
INT8U Inn_ReadBit(INT16U DevNumber,char *DevName);
INT8U Inn_WriteBit(INT16U DevNumber,char *DevName,U8 Value);
int Inn_ReadWord(INT16U DevNumber,char *DevName);
INT8U Inn_WriteWord(INT16U DevNumber,char *DevName,int Value);
//void Inn_Store(void);
//void Inn_load(void);
//void __irq Eint2Isr();
//void Int2_init(void);




////////////////µ¥Æ¬»ú////////////////////
U8 LRC_Check(U8 *auchMsg,INT16U usDataLen);
INT8U Scm_ReadBit(INT16U DevNumber);
INT8U Scm_WriteBit(INT16U DevNumber,U8 value);
int Scm_ReadWord(INT16U DevNumber);
INT8U Scm_WriteWord(INT16U DevNumber,int value);



unsigned char RTU_ReadBit(INT16U DevAddress);
INT8U RTU_WriteBit(INT16U DevAddress,U8 Value);
int RTU_ReadWord(INT16U DevAddress);
U8 RTU_WriteWord(INT16U DevAddress,int Value);



U8 FATEK_ReadBit(char DevChar,INT16U DevNumber);
U8 FATEK_WriteBit(char DevChar,INT16U DevNumber,U8 Value);
short FATEK_ReadWord(char DevChar,INT16U DevNumber);
INT8U FATEK_WriteWord(char DevChar,INT16U DevNumber,int Value);


unsigned char RS485_422_Test(INT16U DevAddress);

U8 Modus_ReadBit(U8 T_Number, INT16U DevAddress);
U8 Modus_ReadBit_Operation(U8 T_Number, INT16U DevAddress);
U8 Modus_WriteBit(U8 T_Number, INT16U DevAddress, U8 Value);
U8 Modus_WriteBit_Operation(U8 T_Number, INT16U DevAddress, U8 Value);
int Modus_ReadWord(U8 T_Number, INT16U DevAddress);
int Modus_ReadWord_Operation(U8 T_Number, INT16U DevAddress);
U8 Modus_WriteWord(U8 T_Number, INT16U DevAddress, int Value);
U8 Modus_WriteWord_Operation(U8 T_Number, INT16U DevAddress, int Value);





