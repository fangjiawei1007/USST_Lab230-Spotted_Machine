#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "2416addr.h"
#include "2416lib.h"
#include "..\inc\def.h"
#include "..\inc\option.h"
#include "..\inc\communication.h"
#include "..\inc\systemconf.h"

//#include <stdio.h>

extern char PlcType;
extern INTERALMEMORY g_InteralMemory;
extern int whichUart;

extern U8 com_frequence;
extern U8 g_com_Flag ;

extern TIMER2_V g_Timer2Uart;
extern TIMER2_V g_Timer2Virtual[MaxTimer];
extern U8 g_UartTimeOut;



INT8U ReadBit(INT16U DevNumber,char *DevName)
{ 
	INT8U bitValue;
	INT8U num;
	int count;

	if((strcmp(DevName,"P_KB")==0) ||(strcmp(DevName,"P_B")==0 ))
	{
		return Inn_ReadBit(DevNumber,DevName);
	}

	com_frequence++;
	if(com_frequence > 2)
	{
		com_frequence = 0;
		Led_on_off();
	}

	switch(PlcType)
	{
		case 0:
			   return FP_ReadBit(DevName[0],DevNumber);
			   break;
		case 1:
			   return NA_ReadBit(DevNumber);
			   break;
	   	case 2:
			   return FX_ReadBit(DevName[0],DevNumber);
			   break;
		case 3:
			   return DVP_ReadBit(DevName[0],DevNumber);
			   break;
		case 4:	//永宏PLC
			   return FATEK_ReadBit(DevName[0],DevNumber);
			   break;
		case 7:
			   return Scm_ReadBit(DevNumber);
			   break;
		case 8:
			   return UniTel_ReadBit(DevNumber);
			   break;
		case 9://080731 RTU
			{
				num = 0;
				do
				{
					for(count=0; count<DELAY_TIME; count++);	// 10000
					bitValue = RTU_ReadBit(DevNumber);
					num++;
				}
				while((0xff == bitValue) && (num<1)); // 2 通讯失败的时候多试几次	

				return bitValue;
			}
			  //return RTU_ReadBit(DevNumber);
			  break;
		case 10:
			   return VIGOR_ReadBit(DevName[0],DevNumber);
			   break;
			   
		default:   return 255;  
	}
}



INT8U WriteBit(INT16U DevNumber,char *DevName,U8 Value)
{ 
	INT8U boolSucceed;
	INT8U num;
	int count;

	if((strcmp(DevName,"P_KB")==0) ||(strcmp(DevName,"P_B")==0 ))
	{
		return Inn_WriteBit(DevNumber,DevName,Value);
	}
	
	com_frequence++;
	if(com_frequence > 2)
	{
		com_frequence = 0;
		Led_on_off();
	}

	switch(PlcType)
	{
		case 0:
			   boolSucceed =  FP_WriteBit(DevName[0],DevNumber,Value);
			   break;
		case 1:
			   boolSucceed =  NA_WriteBit(DevNumber,Value);
			   break;
	   	case 2:
			   boolSucceed = FX_WriteBit(DevName[0],DevNumber,Value);
			   break;
		case 3:
			  boolSucceed =  DVP_WriteBit(DevName[0],DevNumber,Value);
			   break;
		case 4:
			  boolSucceed =  FATEK_WriteBit(DevName[0],DevNumber,Value);
			   break;
		case 7:
			{
				num = 0;
				do
				{
					boolSucceed = Scm_WriteBit(DevNumber,Value);
					num++;
				}
				while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次		

				//return boolSucceed;
			}
			//return Scm_WriteBit(DevNumber,Value);
			break;
		case 8:
			   boolSucceed =  UniTel_WriteBit(DevNumber,Value);
			   break;
            case 9://080731 RTU
			{
				num = 0;
				do
				{
					for(count=0; count<DELAY_TIME; count++);	// 10000
					boolSucceed = RTU_WriteBit(DevNumber,Value);
					num++;
				}
				while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次3	
								
				//return boolSucceed;
			}
			  //return RTU_WriteBit(DevNumber,Value);
			  break;
		case 10:
			   boolSucceed =  VIGOR_WriteBit(DevName[0],DevNumber,Value);
			   break;

		default:   boolSucceed =  255;	  
	}

	return boolSucceed;
}



int ReadWord(INT16U DevNumber,char *DevName)
{ 
	int wordValue;
	INT8U num;
	int count;

	if((strcmp(DevName,"P_KW")==0) ||(strcmp(DevName,"P_W")==0 ))
	{
		return Inn_ReadWord(DevNumber,DevName);
	}

	com_frequence++;
	if(com_frequence > 2)
	{
		com_frequence = 0;
		Led_on_off();
	}

	g_com_Flag = TRUE;	// 通讯标志
	
	switch(PlcType)
	{
		case 0:
			   return FP_ReadWord(DevName,DevNumber);
			   break;
		case 1:
			   return NA_ReadWord(DevNumber);
			   break;
		case 2:
			   return FX_ReadWord(DevName,DevNumber);
			   break;
		case 3:
			   return DVP_ReadWord(DevName[0],DevNumber);
			   break;
		case 4:
			   return FATEK_ReadWord(DevName[0],DevNumber);
			   break;
		case 7:
			   return Scm_ReadWord(DevNumber);
			   break;
		case 8:
			   return UniTel_ReadWord(DevNumber);
			   break;
            case 9://080731 RTU
			{
				num = 0;
				do
				{
					for(count=0; count<DELAY_TIME; count++);	// 10000
					wordValue = RTU_ReadWord(DevNumber);
					num++;
				}
				while((-1 == wordValue) && (num<1)); // 2 通讯失败的时候多试几次		

				return wordValue;
			}
			 // return RTU_ReadWord(DevNumber);
			  break;
		case 10:
			   return VIGOR_ReadWord(DevName[0],DevNumber);
			   break;

		default:   return 255;
	}
}

INT8U WriteWord(INT16U DevNumber,char *DevName,int Value)
{ 
	INT8U boolSucceed;
	INT8U num;
	int count;

	if((strcmp(DevName,"P_KW")==0) ||(strcmp(DevName,"P_W")==0 ))
	{
		return Inn_WriteWord(DevNumber,DevName,Value);
	}

	com_frequence++;
	if(com_frequence > 2)
	{
		com_frequence = 0;
		Led_on_off();
	}

	switch(PlcType)
	{
		case 0:
			   return FP_WriteWord(DevName,DevNumber,Value);
			   break;
		case 1:
			   return NA_WriteWord(DevNumber,Value);
			   break;
		case 2:
			   return FX_WriteWord(DevName,DevNumber,Value);
			   break;
		case 3:
			   return DVP_WriteWord(DevName[0],DevNumber,Value);
			   break;
		case 4:
			   return FATEK_WriteWord(DevName[0],DevNumber,Value);
			   break;
		case 7:
			{
				num = 0;
				do
				{
					boolSucceed = Scm_WriteWord(DevNumber,Value);
					num++;
				}
				while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次		

				return boolSucceed;
			}
			//return Scm_WriteWord(DevNumber,Value);
			break;
		case 8:
			   return UniTel_WriteWord(DevNumber,Value);
			   break;
		case 9://080731 RTU
			{
				num = 0;
				do
				{
					for(count=0; count<DELAY_TIME; count++);	// 10000
					boolSucceed = RTU_WriteWord(DevNumber,Value);
					num++;
				}
				while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次3		

				return boolSucceed;
			}
			  //return RTU_WriteWord(DevNumber,Value);
			  break;
		case 10:
			   return VIGOR_WriteWord(DevName[0],DevNumber,Value);
			   break;

		default:   return 255;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
///////////                       斯奈德NERO___PLC                                 ///////////
////////////////////////////////////////////////////////////////////////////////////////
INT8U NA_ReadBit(INT16U DevNumber)
{
    U8 auchMsg[7],SendArray[16],RecArray[16];  
    U8 Result=FALSE;
	U8 Count;
	int i;

	auchMsg[0]=1;
	auchMsg[1]=1;
	auchMsg[2]=DevNumber/256;
	auchMsg[3]=DevNumber % 256;
	auchMsg[4]=0;
	auchMsg[5]=1;
				
	auchMsg[6]=LRC(auchMsg,6);		//校验码

	SendArray[0]=0x3a;

	for(Count=0;Count<7;Count++) 
	{
		SendArray[Count*2+1]=DataToChar(auchMsg[Count]/16);
 		SendArray[Count*2+2]=DataToChar(auchMsg[Count] % 16);
	}

	SendArray[15]=0x0a;


	//rINTMSK |=BIT_GLOBAL;

	for(Count=0;Count<16;Count++)         //发字符串
	{
		Uart_SendByte(SendArray[Count]);
	}
	
	for(i=0;i<1000;i++);

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;

	if(RecArray[0]==':')
	{
		RecArray[1]=Uart_Getch();
		if((RecArray[2]=Uart_Getch())!=0)
		{

			for(Count=3;Count<15;Count++)          //收字符串
			{
				RecArray[Count]=Uart_Getch();
			}
		}
		else
 			Result=3;	
	}
	else
 		Result=3;
	
	if(Result!=3)
	{	
		if(RecArray[4]=='1' && RecArray[3]=='0' && RecArray[7]=='0' && RecArray[8]=='1')
		{
			Result=TRUE;
		}
	}
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
}




U8 NA_WriteBit(INT16U DevNumber,U8 Value)
{
	U8 auchMsg[7],SendArray[16],RecArray[17];  
        U8 Count;
	U8 Result=FALSE;
	int i; 

	auchMsg[0]=1;
	auchMsg[1]=5;
	auchMsg[2]=DevNumber/256;
	auchMsg[3]=DevNumber % 256;
	if(Value==0)
	{
		auchMsg[4]=0;
		auchMsg[5]=0;
	}
	else
	{
		auchMsg[4]=255;
		auchMsg[5]=0;
	}
				
	auchMsg[6]=LRC(auchMsg,6);		//校验码
	
	SendArray[0]=0x3a;

	for(Count=0;Count<7;Count++) 
	{
		SendArray[Count*2+1]=DataToChar(auchMsg[Count]/16);
		SendArray[Count*2+2]=DataToChar(auchMsg[Count] % 16);
	}

	SendArray[15]=0x0A;

	//rINTMSK|=BIT_GLOBAL;

	for(Count=0;Count<16;Count++)         //发字符串
	{
		Uart_SendByte(SendArray[Count]);
	}

	for(i=0;i<1000;i++);

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;

	if(RecArray[0]==':')
	{

		RecArray[1]=Uart_Getch();
		if((RecArray[2]=Uart_Getch())!=0)
		{

			for(Count=3;Count<17;Count++)          //收字符串
			{
				RecArray[Count]=Uart_Getch();
			}
		}
		else
 			Result=3;	
	}
	else
 		Result=3;
	
	if(Result!=3)
	{	
		if(RecArray[4]=='5' && RecArray[3]=='0' && RecArray[9]=='F' && RecArray[10]=='F')
		{
			Result=TRUE;
		}
	}
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
 
}



int NA_ReadWord(INT16U DevNumber)
{
   
	U8 auchMsg[7],SendArray[16],RecArray[15];  
    int Result=0;
	U8 Count; 
	int i;


	auchMsg[0]=1;
	auchMsg[1]=3;
	auchMsg[2]=DevNumber/256;
	auchMsg[3]=DevNumber % 256;
	auchMsg[4]=0;
	auchMsg[5]=1;
			
	auchMsg[6]=LRC(auchMsg,6);		//校验码
	
	SendArray[0]=0x3a;
	
	for(Count=0;Count<7;Count++) 
	{
		SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
		SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
	}

	SendArray[15]=0x0a;

	//rINTMSK |=BIT_GLOBAL;
	
	for(Count=0;Count<16;Count++)         
	{
		Uart_SendByte(SendArray[Count]);
	}

	for(i=0;i<1000;i++);

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;

	if(RecArray[0]==':')
	{
		RecArray[1]=Uart_Getch();
		if((RecArray[2]=Uart_Getch())!=0)
		{

			for(Count=3;Count<15;Count++)          
			{
				RecArray[Count]=Uart_Getch();
			}
		}
		else
 			Result=-1;	
	}
	else
 		Result=-1;
	
	if(Result!=-1)
	{
		if(RecArray[4]=='3' && RecArray[3]=='0' )
		{
			for(i=7;i<11;i++)
			{
				Result=Result*16+CharToData(RecArray[i]);
			}
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
}



U8 NA_WriteWord(INT16U DevNumber,int Value)
{
	
	U8 auchMsg[7],SendArray[16],RecArray[17];  
        int Result=0;
	U8 Count; 
	int i;
	
	auchMsg[0]=1;
	auchMsg[1]=6;
	auchMsg[2]=DevNumber/256;
	auchMsg[3]=DevNumber % 256;
	auchMsg[4]=Value/256;
	auchMsg[5]=Value % 256;

			
	auchMsg[6]=LRC(auchMsg,6);		//校验码

	SendArray[0]=0x3a;

	for(Count=0;Count<7;Count++) 
	{
		SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
		SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
	}

	SendArray[15]=0x0a;

	//rINTMSK |=BIT_GLOBAL;
	
	for(Count=0;Count<16;Count++)         //发字符串
	{
		Uart_SendByte(SendArray[Count]);
	}


	for(i=0;i<1000;i++);

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;

	if(RecArray[0]==':')
	{
		RecArray[1]=Uart_Getch();
		if((RecArray[2]=Uart_Getch())!=0)
		{

			for(Count=3;Count<17;Count++)          //收字符串
			{
				RecArray[Count]=Uart_Getch();
			}
		}
		else
 			Result=-1;	
	}
	else
 		Result=-1;
	
	if(Result!=-1)
	{
		if(RecArray[4]=='6' && RecArray[3]=='0')
		{
			Result=TRUE;
		}
	}
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return Result;
}


/*******************************************************************************************/
/************************  斯奈德UniTelWay___PLC 	*****************************/
/*******************************************************************************************/  
INT8U UniTel_ReadBit(INT16U DevNumber)
{
    U8 auchMsg[20],RecArray[14]={0};  
    U8 Result=3;
	U8 Count,n,Shiftnum;
	//int i;

	n=0;
	Shiftnum=DevNumber%8;
	DevNumber=DevNumber/8*8;
	
	auchMsg[0]=0x10;
	auchMsg[1]=0x02;
//	auchMsg[2]=0x02;

	auchMsg[3]=0x0E;

	auchMsg[4]=0x20;
	auchMsg[5]=0x00;
	auchMsg[6]=0xFE;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=0x00;
	auchMsg[10]=0x36;
	auchMsg[11]=0x07;
	auchMsg[12]=0x64;
	auchMsg[13]=0x05;

	auchMsg[14]=DevNumber%256;
	auchMsg[15]=DevNumber/256;

	auchMsg[16]=0x08;
	auchMsg[17]=0x00;

	         //0xE7; 

	auchMsg[19]=0x06;

	//rINTMSK |=BIT_GLOBAL;

	do
	{
		Count = 0;
		n++;
		do
		{
			Count++;
			RecArray[0]=Uart_Getch();
		}while( RecArray[0]!=0x01&& RecArray[0]!=0x02 && RecArray[0]!=0x03 && RecArray[0]!=0x04 && Count<=120);

		if(Count<=120)
		{
			auchMsg[2]=RecArray[0];
			auchMsg[18]=UniTelWay_LRC(auchMsg,18); 
		//	for(i=0;i<5000;i++);
			for(Count=0;Count<20;Count++)         //发字符串
			{
				  Unitel_SendByte(auchMsg[Count]);
			}
		
			Count = 0;
			while((RecArray[0]=Uart_Getch())!=0x10 && Count<=80) Count++;

			if(RecArray[0]==0x10)
			{
				for(Count=1;Count<14;Count++)          //收字符串
					RecArray[Count]=Uart_Getch();
				if(RecArray[10]==0x66 )
				{
					Result=(RecArray[12]>>Shiftnum)&0x01;
				}
			}
		}
	}while(n<4 && Result==3);

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
}


U8 UniTel_WriteBit(INT16U DevNumber,U8 Value)
{
	U8 auchMsg[21],RecArray[12]={0};  
    U8 Result=FALSE;
	U8 Count,n;
	//int i;

	n=0;
	auchMsg[0]=0x10;
	auchMsg[1]=0x02;
//	auchMsg[2]=0x02;
	auchMsg[3]=0x0f;

	auchMsg[4]=0x20;
	auchMsg[5]=0x00;
	auchMsg[6]=0xFE;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=0x00;

	auchMsg[10]=0x37;
	auchMsg[11]=0x07;
	auchMsg[12]=0x64;
	auchMsg[13]=0x05;

	auchMsg[14]=DevNumber%256;
	auchMsg[15]=DevNumber/256; 

	auchMsg[16]=0x01;
	auchMsg[17]= 0x00;

	auchMsg[18]=Value;
		
	auchMsg[19]=UniTelWay_LRC(auchMsg,19);  

	auchMsg[20]=0x06;      //  %m0=1

	//rINTMSK |=BIT_GLOBAL;

	do
	{
		Count = 0;
		n++;
		do
		{
			Count++;
			RecArray[0]=Uart_Getch();
		}while(RecArray[0]!=0x01&&  RecArray[0]!=0x02 && RecArray[0]!=0x03 && RecArray[0]!=0x04 && Count<=120);

		if(Count<=120)
		{
			auchMsg[2]=RecArray[0];
			auchMsg[19]=UniTelWay_LRC(auchMsg,19); 
		//	for(i=0;i<5000;i++);
			for(Count=0;Count<21;Count++)         //发字符串
			{
				Unitel_SendByte(auchMsg[Count]);
			}
		
			Count = 0;
			while((RecArray[0]=Uart_Getch())!=0x10 && Count<=80) Count++;

			if(RecArray[0]==0x10)
			{
				for(Count=1;Count<12;Count++)          //收字符串
					RecArray[Count]=Uart_Getch();
				if(RecArray[10]==0xFE )
					Result=TRUE;
			}
		}
	}while(n<4 && Result==FALSE);
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
 
}

int UniTel_ReadWord(INT16U DevNumber)
{
   
	U8 auchMsg[20],RecArray[15];  
    int Result=-1;
	U8 Count,Count1,n; 
	//int i;

	n=0;
	auchMsg[0]=0x10;
	auchMsg[1]=0x02;
//	auchMsg[2]=0x02;

	auchMsg[3]=0x0E;

	auchMsg[4]=0x20;
	auchMsg[5]=0x00;
	auchMsg[6]=0xFE;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=0x00;

	auchMsg[10]=0x36;
	auchMsg[11]=0x07;
	auchMsg[12]=0x68;
	auchMsg[13]=0x07;

	auchMsg[14]=DevNumber%256;
	auchMsg[15]=DevNumber/256;

	auchMsg[16]=0x01;
	auchMsg[17]=0x00;

//	auchMsg[18]=UniTelWay_LRC(auchMsg,18);
	auchMsg[19]=0x06;

	//rINTMSK |=BIT_GLOBAL;

	do
	{
		n++;
		Count = 0;
		do
		{
			Count++;
			RecArray[0]=Uart_Getch();
		}while(RecArray[0]!=0x01&& RecArray[0]!=0x02 && RecArray[0]!=0x03 && RecArray[0]!=0x04 && Count<=120);

		if(Count<=120)
		{
			auchMsg[2]=RecArray[0];
			auchMsg[18]=UniTelWay_LRC(auchMsg,18); 
			for(Count1=0;Count1<20;Count1++)         //发字符串
			{
				Unitel_SendByte(auchMsg[Count1]);
			}
		
			Count1 = 0;
			while((RecArray[0]=Uart_Getch())!=0x10 && Count1<=80) Count1++;

			if(RecArray[0]==0x10)
			{
				for(Count1=1;Count1<15;Count1++)          //收字符串
					RecArray[Count1]=Uart_Getch();
				if(RecArray[10]==0x66 )
				{
					Result=RecArray[12]+RecArray[13]*256;
				}
			}
		}
	}while(n<4 && Result==-1);

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
    return Result ;
}


U8 UniTel_WriteWord(INT16U DevNumber,int Value)
{
	U8 auchMsg[18],RecArray[12]={0};  
    int Result=0;
	U8 Count,n; 
	//int i;

	n=0;

	auchMsg[0]=0x10;
	auchMsg[1]=0x02;
	

	auchMsg[3]=0x0C;

	auchMsg[4]=0x20;
	auchMsg[5]=0x00;
	auchMsg[6]=0xFE;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=0x00;

	auchMsg[10]=0x14;
	auchMsg[11]=0x07;

	auchMsg[12]=DevNumber%256;
	auchMsg[13]=DevNumber/256;

	auchMsg[14]=Value%256;  
	auchMsg[15]=Value/256;

//	auchMsg[16]=UniTelWay_LRC(auchMsg,16);        

	auchMsg[17]=0x06;

	//rINTMSK |=BIT_GLOBAL;

	do
	{
		Count = 0;
		n++;
		do
		{
			Count++;
			RecArray[0]=Uart_Getch();
		}while(RecArray[0]!=0x01&&  RecArray[0]!=0x02 && RecArray[0]!=0x03 && RecArray[0]!=0x04 && Count<=120);

		if(Count<=120)
		{
			auchMsg[2]=RecArray[0];
			auchMsg[16]=UniTelWay_LRC(auchMsg,16); 
			for(Count=0;Count<18;Count++)         //发字符串
			{
					Unitel_SendByte(auchMsg[Count]);
			}
		
			Count = 0;
			while((RecArray[0]=Uart_Getch())!=0x10 && Count<=80) Count++;

			if(RecArray[0]==0x10)
			{
				for(Count=1;Count<12;Count++)          //收字符串
					RecArray[Count]=Uart_Getch();
				if(RecArray[10]==0xFE )
					Result=TRUE;
			}
			
		}
	}while(n<4 && Result==0);
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return Result;
}
 
/*******************************************************************************************/
/************************  	台达DVP___PLC 	*****************************/
/*******************************************************************************************/  
int DVP_ReadWord(char DevChar,INT16U DevNumber)
{
		unsigned short num;
		U8 auchMsg[7],SendArray[16],RecArray[15];  
        int Result=0;
		U8 Count; 
		int i;
	
		auchMsg[0]=1;   
		auchMsg[1]=3;  
		
		num=DVP_Device(DevChar,DevNumber,0);

		if(num==0xffff)
			return(Result);
		
		auchMsg[2]=num/256;
		auchMsg[3]=num%256;
		auchMsg[4]=0;
		auchMsg[5]=1;

		auchMsg[6]=LRC(auchMsg,6);		//

		
		SendArray[0]=0x3a; 
		for(Count=0;Count<7;Count++) 
		{
			SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
			SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
		}
		SendArray[15]=0x0A;
		
		
		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<16;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		for(i=0;i<1000;i++);

		Count = 0;
		while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;
		
		if(RecArray[0]==':')
		{
			for(Count=1;Count<15;Count++)          
			{
				RecArray[Count]=Uart_Getch();
			}

			if(RecArray[1]=='0'&&RecArray[2]=='1'&&RecArray[3]=='0'&&RecArray[4]=='3')
			{
				if((RecArray[6])=='2')                      //16bit
				{
					for(i=7;i<11;i++)
						Result=Result*16+CharToData(RecArray[i]);
				}
				else if((RecArray[6])=='4')    //32bit
				{
					for(i=7;i<15;i++)
						Result=Result*16+CharToData(RecArray[i]);
				}

			}
			else
 				Result=-1;	
		}
		else
 			Result=-1;

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return Result ;
	
}

U8 DVP_WriteWord(char DevChar,INT16U DevNumber,int Value)
{
		unsigned short num;
		U8 auchMsg[9],SendArray[18],RecArray[15];  
		U8 BoolSucceed=0;
   		U8 Count; 
		int i;

		auchMsg[0]=1;
		auchMsg[1]=6;

		num=DVP_Device(DevChar,DevNumber,0);

		if(num==0xffff)
			return(BoolSucceed);
		
		auchMsg[2]=num/256;
		auchMsg[3]=num % 256;
		
		if(DevChar=='c' ||DevChar=='C')                       //  写C232---C255时
		{
			if(DevNumber<232 && DevNumber<=255)
			{
				//auchMsg[4]=(Value/65536)/256;
				//auchMsg[5]=(Value/65536)%256;
				//auchMsg[6]=(Value%65536)/256;
				//auchMsg[7]=(Value%65536)%256;

				auchMsg[4]=(Value/65536)/256;
				auchMsg[5]=(Value/65536)%256;
				auchMsg[6]=(Value%65536)/256;
				auchMsg[7]=((Value%65536)%256);

				auchMsg[8]=LRC(auchMsg,8);

				SendArray[0]=0x3a; 
				for(Count=0;Count<9;Count++) 
				{
					SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
					SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
				}
				SendArray[17]=0x0A;
				
				//rINTMSK |=BIT_GLOBAL;
			
				for(Count=0;Count<18;Count++)         
				{
					Uart_SendByte(SendArray[Count]);
				}

				for(i=0;i<1000;i++);

				Count = 0;
				while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;
				
				if(RecArray[0]==':')
				{
					for(Count=1;Count<15;Count++)          
					{
						RecArray[Count]=Uart_Getch();
					}

					if(RecArray[1]=='0'&&RecArray[2]=='1'&&RecArray[3]=='0'&&RecArray[4]=='6')
					{
						BoolSucceed=1;	
					}
				}
				//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
				return BoolSucceed;
			}
		}
		
		auchMsg[4]=Value/256;
		auchMsg[5]=Value % 256;

		auchMsg[6]=LRC(auchMsg,6);
		
		SendArray[0]=0x3a; 
		for(Count=0;Count<7;Count++) 
		{
			SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
			SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
		}
		SendArray[15]=0x0A;
		
		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<16;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		for(i=0;i<1000;i++);

		Count = 0;
		while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;
		
		if(RecArray[0]==':')
		{
			for(Count=1;Count<15;Count++)          
			{
				RecArray[Count]=Uart_Getch();
			}

			if(RecArray[1]=='0'&&RecArray[2]=='1'&&RecArray[3]=='0'&&RecArray[4]=='6')
			{
				BoolSucceed=1;	
			}
		}
		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolSucceed;

}

U8 DVP_ReadBit(char DevChar,INT16U DevNumber)
{
		unsigned short num;
		U8 auchMsg[7],SendArray[16],RecArray[15];  
		U8 BoolSucceed=3;
   		U8 Count; 
		int i;
	
		auchMsg[0]=1;   
		auchMsg[1]=2;  
		
		num=DVP_Device(DevChar,DevNumber,1);
		if(num==0xffff)
			return(BoolSucceed);
		
		auchMsg[2]=num/256;
		auchMsg[3]=num%256;
		auchMsg[4]=0;
		auchMsg[5]=8;

		auchMsg[6]=LRC(auchMsg,6);		//???

		SendArray[0]=0x3a; 
		for(Count=0;Count<7;Count++) 
		{
			SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
			SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
		}
		SendArray[15]=0x0A;
		
		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<16;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		for(i=0;i<1000;i++);

		Count = 0;
		while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;
		
		if(RecArray[0]==':')
		{
			for(Count=1;Count<11;Count++)          
			{
				RecArray[Count]=Uart_Getch();
			}

			if(RecArray[1]=='0'&&RecArray[2]=='1'&&RecArray[3]=='0'&&RecArray[4]=='2'&&RecArray[5]=='0'&&RecArray[6]=='1')
			{
				BoolSucceed=Fx_BitValue(DevChar,DevNumber,RecArray[7],RecArray[8]);	
			}
		}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolSucceed;

}

U8 DVP_WriteBit(char DevChar,INT16U DevNumber,U8 Value)
{
		unsigned short num;
		U8 auchMsg[7],SendArray[16],RecArray[15];  
		U8 BoolSucceed=0;
   		U8 Count; 
		int i;
		
		auchMsg[0]=1;
		auchMsg[1]=5;

		num=DVP_Device(DevChar,DevNumber,0);
		if(num==0xffff)
			return(BoolSucceed);
		
		auchMsg[2]=num/256;
		auchMsg[3]=num % 256;
		if(Value==0)
		{
			auchMsg[4]=0;
			auchMsg[5]=0;
		}
		else
		{
			auchMsg[4]=255;
			auchMsg[5]=0;
		}
				
		auchMsg[6]=LRC(auchMsg,6);		//???


		SendArray[0]=0x3a; 
		for(Count=0;Count<7;Count++) 
		{
			SendArray[Count*2+1]=(DataToChar(auchMsg[Count]/16));
			SendArray[Count*2+2]=(DataToChar(auchMsg[Count] % 16));
		}
		SendArray[15]=0x0A;
		
		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<16;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		for(i=0;i<1000;i++);

		Count = 0;
		while((RecArray[0]=Uart_Getch())!=':' && Count<=20) Count++;
		
		if(RecArray[0]==':')
		{
			for(Count=1;Count<15;Count++)          
			{
				RecArray[Count]=Uart_Getch();
			}

			if(RecArray[3]=='0'&&RecArray[4]=='5')
			{
				BoolSucceed=1;
			}
		}
	
		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolSucceed;

}


/////////////////////////////////////////////////////////
////           DVP_PLC                        ////////////////
/////////////////////////////////////////////////////////
unsigned short DVP_Device(char DevChar,short DevNumber,U8 flag)  
{
	unsigned short num,error;
	char str[5]={'\0'};
	error=0;

	num=DevNumber;

	if(DevChar>=0x61&&DevChar<=0x7a)
		DevChar-=0x20;
	if(DevChar<0x41||DevChar>0x5a)
		return(0xffff);
   
	
	if(DevChar=='X'||DevChar=='Y')
	{
		sprintf(str,"%d",DevNumber);
		sscanf(str,"%o",&num);
		 
	}

	if(flag==1)                     //flag==1 ??ReadBit??? ???
		num/=8; 
  
	switch(DevChar)
	{
		case 'S':
			if(num>127)
				error=0xffff;
			else if(flag==1)
				num=0x0000+num*8;
			else
				num+=0x0000;
			break;
		case 'X':
			if(DevNumber>177)
				error=0xffff;
			else if(flag==1)
				num=0x0400+num*8;
			else
				num+=0x0400;
			break;
		case 'Y':
			if(DevNumber>177)
				error=0xffff;
			else if(flag==1)
				num=0x0500+num*8;
			else
				num+=0x0500;
			break;
		case 'T':
			if(num>127)
				error=0xffff;
			else if(flag==1)
				num=0x0600+num*8;
			else
				num+=0x0600;
			break;
		case 'M':
			if(num>1279)
				error=0xffff;
			else if(flag==1)
				num=0x0800+num*8;
			else
				num+=0x0800;
			break;
		case 'C':
			if(num>=256||(num>=128&&num<=231))
				error=0xffff;
			else if(flag==1)
				num=0x0e00+num*8;
			else
				num+=0x0800;
			break;
		default:         //  D元件
			if(num>=1144||(num>=600&&num<=999))
			error=0xffff;
			num+=0x1000;
	}

	if(error==0xffff)
		return(0xffff);
	else
		return(num);
}


/*******************************************************************************************/
/************************  	丰伟___PLC 	*****************************/
/*******************************************************************************************/  
U8 VIGOR_ReadBit(char DevChar,INT16U DevNumber)
{
	unsigned short num;
	U8 auchMsg[14],SendArray[14],RecArray[12];  
	U8 BoolSucceed=3;
	U8 count; 
	int i;

	num=VIGOR_Device(DevChar,DevNumber,0);
	if(num==0xffff)
		return(BoolSucceed);

	auchMsg[0]=0x02;
	auchMsg[1]=0x30;
	auchMsg[2]=0x30;
	auchMsg[3]=0x35;
	auchMsg[4]=0x31;
	auchMsg[5]=DataToChar((num/0x100)/0x10);
	auchMsg[6]=DataToChar((num/0x100)%0x10);
	auchMsg[7]=DataToChar((num % 0x100)/0x10);
	auchMsg[8]=DataToChar((num % 0x100)%0x10);
	auchMsg[9]=0x30;
	auchMsg[10]=0x31;
	auchMsg[11]=0x03;
	auchMsg[12]=DataToChar(VIGOR_LRC((auchMsg+1),11)/16);
	auchMsg[13]=DataToChar(VIGOR_LRC((auchMsg+1),11)%16);
	
	
	for (count=0;count<14;count++)
		SendArray[count]=auchMsg[count];

	
	//rINTMSK |=BIT_GLOBAL;

	for(count=0;count<14;count++)         
	{
		Uart_SendByte(SendArray[count]);
	}

	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x06 && count<=20) count++;
	
	if(RecArray[0]==0x06)
	{
		for(count=1;count<12;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[5]=='0'&&RecArray[6]=='0')
		{
			BoolSucceed=Fx_BitValue(DevChar,DevNumber,RecArray[7],RecArray[8]);	
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}


U8 VIGOR_WriteBit(char DevChar,INT16U DevNumber,U8 Value)
{
	unsigned short num;
	U8 auchMsg[12],SendArray[12],RecArray[10];  
	U8 BoolSucceed=0;
	U8 count; 
	int i;
	
	num=VIGOR_Device(DevChar,DevNumber,1);	// force 
	if(num==0xffff)
		return(BoolSucceed);

	auchMsg[0]=0x02;
	auchMsg[1]=0x30;
	auchMsg[2]=0x30;
	if(Value==1)
	{
		auchMsg[3]=0x37;
		auchMsg[4]=0x30;
	}
	else
	{
		auchMsg[3]=0x37;
		auchMsg[4]=0x31;
	}
	auchMsg[5]=DataToChar((num/0x100)/0x10);
	auchMsg[6]=DataToChar((num/0x100)%0x10);
	auchMsg[7]=DataToChar((num % 0x100)/0x10);
	auchMsg[8]=DataToChar((num % 0x100)%0x10);
	auchMsg[9]=0x03;
	auchMsg[10]=DataToChar(VIGOR_LRC((auchMsg+1),9)/16);
	auchMsg[11]=DataToChar(VIGOR_LRC((auchMsg+1),9)%16);
	
	
	for (count=0;count<12;count++)
		SendArray[count]=auchMsg[count];
	
	//rINTMSK |=BIT_GLOBAL;

	for(count=0;count<16;count++)         
	{
		Uart_SendByte(SendArray[count]);
	}

	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x06 && count<=20) count++;
	
	if(RecArray[0]==0x06)
	{
		for(count=1;count<10;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[5]=='0'&&RecArray[6]=='0')
		{
			BoolSucceed=1;
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}


int VIGOR_ReadWord(char DevChar,INT16U DevNumber)
{
	unsigned short num;
	U8 auchMsg[14],RecArray[14];
	short shortVar=-1;
	U8 count;
	int i;

	num=VIGOR_Device(DevChar,DevNumber,0);
	if(num==0xffff)
		return(-1);

	auchMsg[0]=0x02;
	auchMsg[1]=0x30;
	auchMsg[2]=0x30;
	auchMsg[3]=0x35;
	auchMsg[4]=0x31;
	auchMsg[5]=DataToChar((num/0x100)/0x10);
	auchMsg[6]=DataToChar((num/0x100)%0x10);
	auchMsg[7]=DataToChar((num % 0x100)/0x10);
	auchMsg[8]=DataToChar((num % 0x100)%0x10);
	auchMsg[9]=0x30;
	auchMsg[10]=0x32;
	auchMsg[11]=0x03;
	auchMsg[12]=DataToChar(VIGOR_LRC((auchMsg+1),11)/16);
	auchMsg[13]=DataToChar(VIGOR_LRC((auchMsg+1),11)%16);


	//rINTMSK |=BIT_GLOBAL;

	
	for(i=0;i<14;i++)
		Uart_SendByte(auchMsg[i]);


	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x06 && count<=20) count++;
	
	if(RecArray[0]==0x06)
	{
		for(count=1;count<14;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[5]=='0'&&RecArray[6]=='0')
		{
			shortVar=CharToData(RecArray[8])+CharToData(RecArray[7])*16+CharToData(RecArray[10])*16*16+CharToData(RecArray[9])*16*16*16;
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return shortVar;

}


INT8U VIGOR_WriteWord(char DevChar,INT16U DevNumber,int Value)
{
	unsigned short num;
	U8 auchMsg[18],RecArray[10];
	char BoolSucceed=FALSE;
	U8 count;
	int i;

	num=VIGOR_Device(DevChar,DevNumber,0);
	if(num==0xffff)
		return((INT8U)-1);

	auchMsg[0]=0x02;
	auchMsg[1]=0x30;
	auchMsg[2]=0x30;
	auchMsg[3]=0x36;
	auchMsg[4]=0x31;
	auchMsg[5]=DataToChar((num/0x100)/0x10);
	auchMsg[6]=DataToChar((num/0x100)%0x10);
	auchMsg[7]=DataToChar((num % 0x100)/0x10);
	auchMsg[8]=DataToChar((num % 0x100)%0x10);
	auchMsg[9]=0x30;
	auchMsg[10]=0x32;

	auchMsg[11]=DataToChar((Value%256)/16);
	auchMsg[12]=DataToChar((Value%256)%16);
	auchMsg[13]=DataToChar((Value/256)/16);
	auchMsg[14]=DataToChar((Value/256)%16);
	
	auchMsg[15]=0x03;
	auchMsg[16]=DataToChar(VIGOR_LRC((auchMsg+1),15)/16);
	auchMsg[17]=DataToChar(VIGOR_LRC((auchMsg+1),15)%16);


	//rINTMSK |=BIT_GLOBAL;

	
	for(i=0;i<18;i++)
		Uart_SendByte(auchMsg[i]);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x06 && count<=20) count++;
	
	if(RecArray[0]==0x06)
	{
		for(count=1;count<10;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[5]=='0'&&RecArray[6]=='0')
		{
			BoolSucceed=1;
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;
	
}


INT8U VIGOR_LRC(U8 *auchMsg,INT16U usDataLen)
{
	INT16U uchLRC = 0 ;                  /* LRC U8 initialized */
	while (usDataLen--)
	{
		uchLRC += *auchMsg++;    /* add buffer byte without carry */
	}
	uchLRC=uchLRC & 0xff;
	return ((INT8U)uchLRC) ;            /* return twos complement */
}



/////////////////////////////////////////////////////////
////           VIGOR_PLC                        ////////////////
/////////////////////////////////////////////////////////
unsigned short VIGOR_Device(char DevChar,INT16U DevNumber,U8 force_flag)  
{
	unsigned short num,error;
	char str[5]={'\0'};
	error=0;

	num=DevNumber;

	if(DevChar>=0x61&&DevChar<=0x7a)	//a~z  小写到大写
		DevChar-=0x20;
	if(DevChar<0x41||DevChar>0x5a)		//确保在A~Z 之间
		return(0xffff);
   
	
	if(DevChar=='X'||DevChar=='Y')
	{
		sprintf(str,"%d",DevNumber);
		sscanf(str,"%o",&num);
		 
	}

//	if(force_flag==1)                     //flag==1 ??ReadBit??? ???
//		num/=8; 
  
	switch(DevChar)
	{
		case 'X':
			if(DevNumber>777)
				error=0xffff;
			else if(force_flag==1)
				num=0x0000*8+num;
			else
				num=0x0000+num/8;
			break;
		case 'Y':
			if(DevNumber>777)
				error=0xffff;
			else if(force_flag==1)
				num=0x0040*8+num;
			else
				num=0x0040+num/8;
			break;
		case 'M':
			if(num>9255||(num>5119&&num<9000))
				error=0xffff;
			else if(force_flag==1)
				{
					if(num<=5119)
						num=0x0080*8+num;
					else
						num=0x03E0*8+num;
				}
			else
				{
					if(num<=5119)
						num=0x0080+num/8;
					else
						num=0x03E0+num/8;
				}
			break;
		case 'S':
			if(num>999)
				error=0xffff;
			else if(force_flag==1)
				num=0x0300*8+num;
			else
				num=0x0300+num/8;
			break;
		case 'T':
			if(num>255)
				error=0xffff;
			num=0x0380+num/8;
			break;
		case 'C':
			if(num>255)
				error=0xffff;
			num=0x03A0+num/8;
			break;
		case 'D':        //  D元件
			if(num>9255||(num>8191&&num<9000))
				error=0xffff;
			else if(num<=8191)
				num=0x1C00+num*2;
			else
				num=0x1600+(num-9000)*2;				
	}

	if(error==0xffff)
		return(0xffff);
	else
		return(num);
}



/*******************************************************************************************/
/************************  	松下Panasonic  FP PLC 	*****************************/
/*******************************************************************************************/  
int FP_ReadWord(char* DevChar,INT16U DevNumber)
{
    	U8 auchMsg[21],SendArray[21],RecArray[21];
	U8 Count;
	int shortVar=-1;

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='R';
		auchMsg[5]='D';
		auchMsg[6]=DevChar[0];
//		auchMsg[7]=48;
		auchMsg[7]=DataToChar(DevNumber/10000);
		auchMsg[8]=DataToChar((DevNumber/1000)%10);
		auchMsg[9]=DataToChar((DevNumber/100)%10);
		auchMsg[10]=DataToChar((DevNumber % 100)/10);
		auchMsg[11]=DataToChar((DevNumber % 100)%10);
//		auchMsg[12]=48;
		auchMsg[12]=DataToChar(DevNumber/10000);
		auchMsg[13]=DataToChar((DevNumber/1000)%10);
		auchMsg[14]=DataToChar((DevNumber/100)%10);
		auchMsg[15]=DataToChar((DevNumber % 100)/10);
		auchMsg[16]=DataToChar((DevNumber % 100)%10);

		auchMsg[17]=DataToChar(BCC(auchMsg,17)/16);	
		auchMsg[18]=DataToChar(BCC(auchMsg,17)%16);
		auchMsg[19]='\n';

		
		for (Count=0;Count<20;Count++)
			SendArray[Count]=auchMsg[Count];


		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<20;Count++)         //send
		{
			Uart_SendByte(SendArray[Count]);
		}

		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		
		for(Count=1;Count<10;Count++)		//recieve
		RecArray[Count]=Uart_Getch();
	
		if(RecArray[3]=='$')
		{
			shortVar=CharToData(RecArray[6])*16+CharToData(RecArray[7])+
				CharToData(RecArray[8])*16*16*16+CharToData(RecArray[9])*16*16;
		}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

	return shortVar;
}

U8 FP_ReadBit(char DevChar,INT16U DevNumber)
{
    	U8 auchMsg[20],SendArray[20],RecArray[20];
		U8 Count;
		U8 BoolValue=0;

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='R';
		auchMsg[5]='C';
		auchMsg[6]='S';
		auchMsg[7]=DevChar;

		if((DevChar=='X') ||(DevChar=='Y') ||(DevChar=='R'))
		{
			INT16U tempNum=DevNumber/16;                 //十进制部分
			auchMsg[8]=DataToChar(tempNum/100);
			auchMsg[9]=DataToChar((tempNum%100)/10);
			auchMsg[10]=DataToChar((tempNum%100)%10);
			auchMsg[11]=DataToChar(DevNumber%16);               //最后一位16进制

		}
		else 
		{
			auchMsg[8]=DataToChar((DevNumber/100)/10);
			auchMsg[9]=DataToChar((DevNumber/100)%10);
			auchMsg[10]=DataToChar((DevNumber % 100)/10);
			auchMsg[11]=DataToChar((DevNumber % 100)%10);
		}


		auchMsg[12]=DataToChar(BCC(auchMsg,12)/16);	
		auchMsg[13]=DataToChar(BCC(auchMsg,12)%16);
		auchMsg[14]='\n';

		for (Count=0;Count<15;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<15;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		for(Count=1;Count<7;Count++)
			RecArray[Count]=Uart_Getch();
		
			if(RecArray[3]=='$')
			{
				BoolValue=CharToData(RecArray[6]);
			}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

    return BoolValue ;
}

U8 FP_WriteBit(char DevChar,INT16U DevNumber,U8 Value)
{

    	U8 auchMsg[20],SendArray[20],RecArray[20];
	U8 Count;
	U8 BoolSucceed=0;
		
	for(Count=0;Count<20;Count++)
		RecArray[Count]=0;


		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='W';
		auchMsg[5]='C';
		auchMsg[6]='S';
		auchMsg[7]=DevChar;
		if((DevChar=='X') ||(DevChar=='Y') ||(DevChar=='R'))
		{
			INT16U tempNum=DevNumber/16;                 //十进制部分
			auchMsg[8]=DataToChar(tempNum/100);
			auchMsg[9]=DataToChar((tempNum%100)/10);
			auchMsg[10]=DataToChar((tempNum%100)%10);
			auchMsg[11]=DataToChar(DevNumber%16);               //最后一位16进制

		}
		else
		{
			auchMsg[8]=DataToChar((DevNumber/100)/10);
			auchMsg[9]=DataToChar((DevNumber/100)%10);
			auchMsg[10]=DataToChar((DevNumber % 100)/10);
			auchMsg[11]=DataToChar((DevNumber % 100)%10);
		}

		auchMsg[12]=DataToChar(Value);

		auchMsg[13]=DataToChar(BCC(auchMsg,13)/16);	
		auchMsg[14]=DataToChar(BCC(auchMsg,13)%16);
		auchMsg[15]='\n';


		for (Count=0;Count<16;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<16;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}
		
		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20) Count++;
		for(Count=1;Count<4;Count++)
			RecArray[Count]=Uart_Getch();
		
		if(RecArray[3]=='$')
		{
			BoolSucceed=1;
		}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

	return BoolSucceed;
}

U8 FP_WriteWord(char* DevChar,INT16U DevNumber,int Value)
{

    	U8 auchMsg[25],SendArray[25],RecArray[20];
		U8 Count;
		U8 BoolSucceed=0;
		

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='W';
		auchMsg[5]='D';
		auchMsg[6]=DevChar[0];
//		auchMsg[7]=48;
		auchMsg[7]=DataToChar(DevNumber/10000);
		auchMsg[8]=DataToChar((DevNumber/1000)%10);
		auchMsg[9]=DataToChar((DevNumber/100)%10);
		auchMsg[10]=DataToChar((DevNumber % 100)/10);
		auchMsg[11]=DataToChar((DevNumber % 100)%10);
//		auchMsg[12]=48;
		auchMsg[12]=DataToChar(DevNumber/10000);
		auchMsg[13]=DataToChar((DevNumber/1000)%10);
		auchMsg[14]=DataToChar((DevNumber/100)%10);
		auchMsg[15]=DataToChar((DevNumber % 100)/10);
		auchMsg[16]=DataToChar((DevNumber % 100)%10);

		auchMsg[17]=DataToChar((Value % 256)/16);
		auchMsg[18]=DataToChar((Value % 256)%16);
		auchMsg[19]=DataToChar((Value/256)/16);
		auchMsg[20]=DataToChar((Value/256)%16);
	
		auchMsg[21]=DataToChar(BCC(auchMsg,21)/16);	
		auchMsg[22]=DataToChar(BCC(auchMsg,21)%16);
		auchMsg[23]='\n';

		for (Count=0;Count<24;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<24;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}
		
		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		for(Count=1;Count<4;Count++)
			RecArray[Count]=Uart_Getch();
		
			if(RecArray[3]=='$')
			{
				BoolSucceed=1;
			}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

		
	return BoolSucceed;
}

/////////////////////////////////////////////////////////
///////// 松下 块通讯  //////////////////////////////////
////////////////////////////////////////////////////////
U8 FP_ReadBlockBit(char DevChar,INT16U DevNumber,char CountNum,char RetArray[8])
{
    	U8 auchMsg[60],SendArray[60],RecArray[20];
		U8 Count;
		U8 BoolValue=0;
		int i;

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='R';
		auchMsg[5]='C';
		auchMsg[6]='P';
		auchMsg[7]=DataToChar(CountNum);
	
		for(i=0;i<CountNum;i++)
		{
			auchMsg[8+i*5]=DevChar;

			if((DevChar=='X') ||(DevChar=='Y') ||(DevChar=='R'))
			{
				INT16U tempNum=(DevNumber+i)/16;                 //十进制部分
				auchMsg[9+i*5]=DataToChar(tempNum/100);
				auchMsg[10+i*5]=DataToChar((tempNum%100)/10);
				auchMsg[11+i*5]=DataToChar((tempNum%100)%10);
				auchMsg[12+i*5]=DataToChar((DevNumber+i)%16);               //最后一位16进制

			}
			else 
			{
				auchMsg[9+i*5]=DataToChar(((DevNumber+i)/100)/10);
				auchMsg[10+i*5]=DataToChar(((DevNumber+i)/100)%10);
				auchMsg[11+i*5]=DataToChar(((DevNumber+i) % 100)/10);
				auchMsg[12+i*5]=DataToChar(((DevNumber+i) % 100)%10);
			}

		}
		auchMsg[8+i*5]=DataToChar(BCC(auchMsg,8+i*5)/16);	
		auchMsg[9+i*5]=DataToChar(BCC(auchMsg,8+i*5)%16);
		auchMsg[10+i*5]='\n';

		for (Count=0;Count<11+i*5;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<11+i*5;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}

		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		for(Count=1;Count<6+CountNum;Count++)
			RecArray[Count]=Uart_Getch();
		
		if(RecArray[3]=='$')
		{
			for(i=6;i<6+CountNum;i++)
				RetArray[i-6]=CharToData(RecArray[i]);

			BoolValue=1;
		}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

    return BoolValue ;
}


U8 FP_WriteBlockBit(char DevChar,INT16U DevNumber,char CountNum,U8 Value[])
{

   	U8 auchMsg[70],SendArray[70],RecArray[10];
	U8 Count;
	U8 BoolSucceed=0;
	int i;
	//U8  temp=Value[2];

	for(Count=0;Count<10;Count++)
		RecArray[Count]=0;


		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='W';
		auchMsg[5]='C';
		auchMsg[6]='P';
		auchMsg[7]=DataToChar(CountNum);
	
		for(i=0;i<CountNum;i++)
		{
			auchMsg[8+i*6]=DevChar;

			if((DevChar=='X') ||(DevChar=='Y') ||(DevChar=='R'))
			{
				INT16U tempNum=(DevNumber+i)/16;                 //十进制部分
				auchMsg[9+i*6]=DataToChar(tempNum/100);
				auchMsg[10+i*6]=DataToChar((tempNum%100)/10);
				auchMsg[11+i*6]=DataToChar((tempNum%100)%10);
				auchMsg[12+i*6]=DataToChar((DevNumber+i)%16);               //最后一位16进制
				auchMsg[13+i*6]=DataToChar(Value[i]);
			}
			else 
			{
				auchMsg[9+i*6]=DataToChar(((DevNumber+i)/100)/10);
				auchMsg[10+i*6]=DataToChar(((DevNumber+i)/100)%10);
				auchMsg[11+i*6]=DataToChar(((DevNumber+i)% 100)/10);
				auchMsg[12+i*6]=DataToChar(((DevNumber+i) % 100)%10);
				auchMsg[13+i*6]=DataToChar(Value[i]);

			}

		}
		auchMsg[8+i*6]=DataToChar(BCC(auchMsg,8+i*6)/16);	
		auchMsg[9+i*6]=DataToChar(BCC(auchMsg,8+i*6)%16);
		auchMsg[10+i*6]='\n';

		for (Count=0;Count<11+i*6;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<11+i*6;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}
		
		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20) Count++;
		for(Count=1;Count<4;Count++)
			RecArray[Count]=Uart_Getch();
		
		if(RecArray[3]=='$')
		{
			BoolSucceed=1;
		}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

	return BoolSucceed;
}

U8 FP_WriteBlockWord(char* DevChar,INT16U DevNumber,int CountNum,int Value[])
{

    	U8 auchMsg[4050],SendArray[4050],RecArray[20];
		U8 Count;
		U8 BoolSucceed=0;
		U8 i;

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='W';
		auchMsg[5]='D';
		auchMsg[6]=DevChar[0];
		auchMsg[7]=48;
		auchMsg[8]=DataToChar((DevNumber/100)/10);
		auchMsg[9]=DataToChar((DevNumber/100)%10);
		auchMsg[10]=DataToChar((DevNumber % 100)/10);
		auchMsg[11]=DataToChar((DevNumber % 100)%10);
		auchMsg[12]=48;
		auchMsg[13]=DataToChar(((DevNumber+CountNum-1)/100)/10);
		auchMsg[14]=DataToChar(((DevNumber+CountNum-1)/100)%10);
		auchMsg[15]=DataToChar(((DevNumber+CountNum-1) % 100)/10);
		auchMsg[16]=DataToChar(((DevNumber+CountNum-1) % 100)%10);

		for(i=0;i<CountNum;i++)
		{
			auchMsg[17+i*4]=DataToChar((Value[i] % 256)/16);
			auchMsg[18+i*4]=DataToChar((Value[i] % 256)%16);
			auchMsg[19+i*4]=DataToChar((Value[i]/256)/16);
			auchMsg[20+i*4]=DataToChar((Value[i]/256)%16);
		}

		auchMsg[17+i*4]=DataToChar(BCC(auchMsg,17+i*4)/16);	
		auchMsg[18+i*4]=DataToChar(BCC(auchMsg,17+i*4)%16);
		auchMsg[19+i*4]='\n';

		for (Count=0;Count<20+i*4;Count++)
			SendArray[Count]=auchMsg[Count];

		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<20+i*4;Count++)         
		{
			Uart_SendByte(SendArray[Count]);
		}
		
		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		for(Count=1;Count<4;Count++)
			RecArray[Count]=Uart_Getch();
		
			if(RecArray[3]=='$')
			{
				BoolSucceed=1;
			}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

		
	return BoolSucceed;
}


char FP_ReadBlockWord(char* DevChar,INT16U DevNumber,int CountNum,int Value[])
{
    	U8 auchMsg[21],SendArray[21],RecArray[4050];
		U8 Count;
		char shortVar=0;
		int i;

		auchMsg[0]='%';
		auchMsg[1]=48;
		auchMsg[2]=49;
		auchMsg[3]='#';
		auchMsg[4]='R';
		auchMsg[5]='D';
		auchMsg[6]=DevChar[0];
		auchMsg[7]=48;
		auchMsg[8]=DataToChar((DevNumber/100)/10);
		auchMsg[9]=DataToChar((DevNumber/100)%10);
		auchMsg[10]=DataToChar((DevNumber % 100)/10);
		auchMsg[11]=DataToChar((DevNumber % 100)%10);
		auchMsg[12]=48;
		auchMsg[13]=DataToChar(((DevNumber+CountNum-1)/100)/10);
		auchMsg[14]=DataToChar(((DevNumber+CountNum-1)/100)%10);
		auchMsg[15]=DataToChar(((DevNumber+CountNum-1) % 100)/10);
		auchMsg[16]=DataToChar(((DevNumber+CountNum-1) % 100)%10);

		auchMsg[17]=DataToChar(BCC(auchMsg,17)/16);	
		auchMsg[18]=DataToChar(BCC(auchMsg,17)%16);
		auchMsg[19]='\n';

		
		for (Count=0;Count<20;Count++)
			SendArray[Count]=auchMsg[Count];


		//rINTMSK |=BIT_GLOBAL;
	
		for(Count=0;Count<20;Count++)         //send
		{
			Uart_SendByte(SendArray[Count]);
		}

		Count = 0;
		while((RecArray[0]=Uart_Getch())!='%' && Count<=20)Count++;
		
		for(Count=1;Count<7+CountNum*5;Count++)		//recieve
			RecArray[Count]=Uart_Getch();
	
		if(RecArray[3]=='$')
		{
			for(i=0;i<CountNum;i++)
			{
				Value[i]=CharToData(RecArray[6+i*4])*16+CharToData(RecArray[7+i*4])+
					CharToData(RecArray[8+i*4])*16*16*16+CharToData(RecArray[9+i*4])*16*16;
			}
			shortVar=1;
		}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);

	return shortVar;
}






/*******************************************************************************************/
/************************  	三菱FX PLC 	*****************************/
/*******************************************************************************************/  
char FX_ReadBit(char DevChar,short DevNumber)
{
	   	unsigned char auchMsg[20],RecArray[20];
		char BoolValue=2;

		unsigned short num;
		int i;

		auchMsg[0]=0x02;
		auchMsg[1]=0x30;
		auchMsg[6]=0x30;
		auchMsg[7]=0x31;
		auchMsg[8]=0x03;
		
		num=Fx_Addr(&DevChar,DevNumber);
		if(num==0xffff)
			return(BoolValue);
		auchMsg[2]=DataToChar(num/0x1000);
		auchMsg[3]=DataToChar(num/0x100-num/0x1000*0x10);
		auchMsg[4]=DataToChar(num/0x10-num/0x100*0x10);
		auchMsg[5]=DataToChar(num%0x10);

		for(num=0,i=1;i<9;i++)
			num+=auchMsg[i];                 //校验码
		num=num%0x100;                  //校验码
		auchMsg[9]=DataToChar(num/0x10);
		auchMsg[10]=DataToChar(num%0x10);

		//rINTMSK |=BIT_GLOBAL;
	
		for(i=0;i<11;i++)         
		{
			Uart_SendByte(auchMsg[i]);
		}

		i=0;
		while((RecArray[0]=Uart_Getch())!=0x02 && i<=20) i++;
		for(i=1;i<11;i++)
			RecArray[i]=Uart_Getch();
		
		if(RecArray[3]==0x03)
		{
			BoolValue=Fx_BitValue(DevChar,DevNumber,RecArray[1],RecArray[2]);
		}

		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolValue;
}


short FX_ReadWord(char *DevChar,short DevNumber)
{
    	unsigned char auchMsg[21],RecArray[20];
		short shortVar=-1;

		unsigned short num;
		int i;
	
		auchMsg[0]=0x02;
		auchMsg[1]='0';
		auchMsg[6]='0';
		auchMsg[7]='2';
		auchMsg[8]=0x03;
	 	num=Fx_Addr(DevChar,DevNumber);
		if(num==0xffff)
			return(-1);
		
		auchMsg[2]=DataToChar(num/0x1000);
		auchMsg[3]=DataToChar(num/0x100-num/0x1000*0x10);
		auchMsg[4]=DataToChar(num/0x10-num/0x100*0x10);
		auchMsg[5]=DataToChar(num%0x10);

		for(num=0,i=1;i<9;i++)
			num+=auchMsg[i];
		num=num%0x100;
		auchMsg[9]=DataToChar(num/0x10);
		auchMsg[10]=DataToChar(num%0x10);

		//rINTMSK |=BIT_GLOBAL;
		if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write
		for(i=0;i<1000;i++);

		for(i=0;i<11;i++)
			Uart_SendByte(auchMsg[i]);
			
		if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	+Write
		for(i=0;i<1000;i++);

	
		i=0;
		while((RecArray[0]=Uart_Getch())!=0x02 && i<=20) i++;
		for(i=1;i<8;i++)
			RecArray[i]=Uart_Getch();
		
		if(RecArray[5]==0x03)
		{   
				shortVar=CharToData(RecArray[2])+CharToData(RecArray[1])*16+CharToData(RecArray[4])*16*16+CharToData(RecArray[3])*16*16*16;
		}
		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return shortVar;
}

char FX_WriteBit(char DevChar,short DevNumber,char Value)
{
   		unsigned char auchMsg[9],RecArray[20];
		char BoolSucceed=0;

		unsigned short num;
		int i,j;

		for(j=0;j<20;j++)
			RecArray[j]=3;

		auchMsg[0]=0x02;
		auchMsg[6]=0x03;
		if(Value==0)
			auchMsg[1]=0x38;
		else
			auchMsg[1]=0x37;
		num=Fx_Device(DevChar,DevNumber);
		if(num==0xffff)
			return(BoolSucceed);
		auchMsg[3]=DataToChar(num%0x10);
		auchMsg[2]=DataToChar(num/0x10-num/0x100*0x10);
		auchMsg[4]=DataToChar(num/0x1000);
		auchMsg[5]=DataToChar(num/0x100-num/0x1000*0x10);

		for(num=0,i=1;i<7;i++)
	    	num+=auchMsg[i];
		num=num%0x100;                  //校验码
		auchMsg[7]=DataToChar(num/0x10);
		auchMsg[8]=DataToChar(num%0x10);
		
		//rINTMSK |=BIT_GLOBAL;
		for(i=0;i<9;i++)
			Uart_SendByte(auchMsg[i]);
	
	//	for(i=0;i<60;i++)
	//	for(i=0;i<20;i++)
	//		RecArray[i]=Uart_Getch();
		i=0;
		while((RecArray[0]=Uart_Getch())!=0x06 && i<=40) i++;
		
		if(RecArray[0]==0x06)
		{
			BoolSucceed=TRUE;
		}
		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolSucceed;
	
}


char FX_WriteWord(char *DevChar,short DevNumber,int Value)
{
      	unsigned char auchMsg[25],RecArray[20];
		char BoolSucceed=FALSE;

		int i;
		unsigned short num;

		auchMsg[0]=0x02;
		auchMsg[1]=0x31;
		auchMsg[6]=0x30;
		auchMsg[7]=0x32;
		auchMsg[12]=0x03;
		
		num=Fx_Addr(DevChar,DevNumber);
		if(num==0xffff)
			return(BoolSucceed);
		auchMsg[2]=DataToChar(num/0x1000);
		auchMsg[3]=DataToChar(num/0x100-num/0x1000*0x10);
		auchMsg[4]=DataToChar(num/0x10-num/0x100*0x10);
		auchMsg[5]=DataToChar(num%0x10);

		auchMsg[10]=DataToChar(Value/0x1000);
		auchMsg[11]=DataToChar(Value/0x100-Value/0x1000*0x10);
		auchMsg[8]=DataToChar(Value/0x10-Value/0x100*0x10);
		auchMsg[9]=DataToChar(Value%0x10);

		for(num=0,i=1;i<13;i++)
			num+=auchMsg[i];
		num=num%0x100;
		auchMsg[13]=DataToChar(num/0x10);
		auchMsg[14]=DataToChar(num%0x10);

		//rINTMSK |=BIT_GLOBAL;
		for(i=0;i<15;i++)
			Uart_SendByte(auchMsg[i]);
	
		i=0;
		while((RecArray[0]=Uart_Getch())!=0x06 && i<=20) i++;

		if(RecArray[0]==0x06)
		{   
			BoolSucceed=TRUE;
		}
		//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
		return BoolSucceed;
		
}

/////////////////////////////////////////////////////////
////     以下函数求FX中位元件的地址      ////////////////
/////////////////////////////////////////////////////////
unsigned short Fx_Device(char DevChar,short DevNumber)
{
	unsigned short num,error;
	char str[6];
	error=0;

	if(DevChar>=0x61&&DevChar<=0x7a)
		DevChar-=0x20;
	if(DevChar<0x41||DevChar>0x5a)
		error=0xffff;
    	num=DevNumber;

	if(DevChar=='X'||DevChar=='Y')
	{
		sprintf(str,"%d",DevNumber);
		sscanf(str,"%o",&num);
	}

	switch(DevChar)
	{
		case 'S':
			if(num>=1000)
				error=0xffff;
			num+=0x0000;
			break;
		case 'X':
			if(num>=178)
				error=0xffff;
			num+=0x0400;
			break;
		case 'Y':
			if(num>=178)
				error=0xffff;
			num+=0x0500;
			break;
		case 'T':
			if(num>=256)
				error=0xffff;
			num+=0x0600;
			break;
		case 'M':
			if(num>=1024&&num<=7999||num>=8256)
				error=0xffff;
			if(num<1024)
				num+=0x0800;
			if(num>=8000&&num<=8255)
				num=num-8000+0x0f00;
			break;
		case 'C':
			if(num>=256)
				error=0xffff;
			num+=0x0e00;
			break;
		default:
			error=0xffff;
	}
	if(error==0xffff)
		return(0xffff);
	else
		return(num);
}


unsigned short Fx_Addr(char *DevChar,short DevNumber)
{
	int i;
	unsigned short num;
	unsigned char word;
    char str[3]={0};

	if(DevChar[0]<='A'||(DevChar[0]>='Z'&&DevChar[0]<='a')||DevChar[0]>='z')
		return(0xffff);
	num=DevNumber;
	
	word=DevChar[0];
	if(DevChar[0]>='a'&&DevChar[0]<='z')
		word-=0x20;
	if(word=='X'||word=='Y')
	{
		sprintf(str,"%d",DevNumber);
		sscanf(str,"%o",&num);
	}
	if(word!='D')
	num/=8;
	
	if(DevChar[1]>='A'&&DevChar[1]<='Z'||DevChar[1]>='a'&&DevChar[1]<='z')
	{
		if(strcmp(DevChar,"PY")==0)
			word='W';
		if(strcmp(DevChar,"OT")==0)
			word='N';
		if(strcmp(DevChar,"PM")==0)
			word='O';
		if(strcmp(DevChar,"OC")==0)
			word='P';
		if(strcmp(DevChar,"RT")==0)
			word='Q';
		if(strcmp(DevChar,"RC")==0)
			word='R';
		if(strcmp(DevChar,"DT")==0)
			word='U';
		if(strcmp(DevChar,"DC")==0)
			word='V';
		if(word!='U'&&word!='V')
			num/=8;
	}
	if(word==0)
		return(0xffff);

	switch(word)
	{
		case 'C':
			num+=0x01c0;
			break;
		case 'D':
			if(num<=511)
			i=0x1000+num*2;
			if(num>=8000&&num<=8255)
			i=0x0e00+(num-8000)*2;
			num=i;
			break;
		case 'M':
			num+=0x0100;
			break;
		case 'N':
			num+=0x02c0;
			break;
		case 'O':
			num+=0x0300;
			break;
		case 'P':
			num+=0x03c0;
			break;
		case 'Q':
			num+=0x04c0;
			break;
		case 'R':
			num+=0x05c0;
			break;
		case 'S':
			num+=0x0000;
			break;
		case 'T':
			num+=0x00c0;
			break;
		case 'U':
			num=0x0800+num*2;
			break;
		case 'V':
			if(num<=199)
			i=0x0a00+num*2;
			if(num>=200&&num<=255)
			i=0x0c00+(num-200)*4;
			num=i;
			break;
		case 'W':
			num+=0x02a0;
			break;
		case 'X':
			num+=0x0080;
			break;
		case 'Y':
			num+=0x00a0;
			break;
	}
	return(num);
}

char Fx_BitValue(char DevChar,short DevNumber,char Data1,char Data2)
{
	char str[5];
	short shiftnum,value,num;
	char Result;
	num=DevNumber;

	if(DevChar=='X'||DevChar=='Y')
	{
		sprintf(str,"%d",DevNumber);
		sscanf(str,"%o",&num);
	}

	shiftnum=num-(num/8)*8;
	value=CharToData(Data1)*16+CharToData(Data2);
	Result=(value>>shiftnum)&0x01;
	return Result;
}

/******************************************
*用于MODBUS(RTU mode)通信的CRC检验函数
*
******************************************/
INT16U CRC(U8 *auchMsg,INT8U usDataLen)
{
   INT16U j;
   INT16U reg_crc=0xFFFF;
   while(usDataLen--)
   	{
   	  reg_crc^=*auchMsg++;
	  for(j=0;j<8;j++)
	  	{
	  	  if(reg_crc&0x01)
		  	reg_crc=(reg_crc>>1)^0xA001;
		  else
		  	reg_crc=(reg_crc>>1);
	  	}
   	}
   return reg_crc;
}


INT8U LRC_ADD(U8 *auchMsg,INT16U usDataLen)
{
	INT16U uchLRC = 0 ;                  /* LRC U8 initialized */
	while (usDataLen--)
	{
		uchLRC += *auchMsg++;    /* add buffer byte without carry */
	}
	uchLRC=uchLRC & 0xff;
	return ((INT8U)uchLRC) ;            /* return twos complement */
}



U8 LRC(U8 *auchMsg,INT16U usDataLen)
{
	U8 uchLRC = 0 ;                  /* LRC U8 initialized */
	while (usDataLen--)
	{
		uchLRC += *auchMsg++;    /* add buffer byte without carry */
	}
	uchLRC=256-uchLRC;
	return ((U8)uchLRC) ;            /* return twos complement */
}

U8 BCC(U8 *auchMsg,INT16U usDataLen)
{
	unsigned char uchBCC = 0 ;			/* BCC char initialized */
	while (usDataLen--)
	{
		uchBCC = uchBCC^(*(auchMsg++)); 	/* add buffer byte without carry */
	}

	return ((char)uchBCC) ; 		/* return twos complement */
}

U8 UniTelWay_LRC(U8 *auchMsg,INT16U usDataLen)
{
	U8 uchLRC = 0 ;                  /* LRC U8 initialized */
	while (usDataLen--)
	{
		uchLRC += *auchMsg++;    /* add buffer byte without carry */
	}
	uchLRC=uchLRC % 256;
	return ((U8)uchLRC) ;            /* return twos complement */
}


U8 DataToChar(U8 Data)
{
	U8 charVar;
	if(Data<=9)
	{
		charVar=Data+48;
	}
	else
	{
		charVar=Data+55;
	}
	return charVar;
}



U8 CharToData(U8 Data)
{
	U8 DataVar;
	if(Data>=48 && Data<=57)
	{
		DataVar=Data-48;
	}
	else
	{
		DataVar=Data-55;
	}
	return DataVar;
}

/////////////////////////////////////////////////////////////////
///       内部操作    ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
INT8U Inn_ReadBit(INT16U DevNumber,char *DevName)
{
	if(strcmp(DevName,"P_KB")==0) 
		return g_InteralMemory.KeepBit[DevNumber];
	else if(strcmp(DevName,"P_B")==0) 
		return g_InteralMemory.Bit[DevNumber];
	else
		return 3;

}

INT8U Inn_WriteBit(INT16U DevNumber,char *DevName,U8 Value)
{
	if(strcmp(DevName,"P_KB")==0) 
	{
		g_InteralMemory.KeepBit[DevNumber]=Value;
//		Inn_Store();
		return 1;
	}
	else if((strcmp(DevName,"P_B")==0 ))
	{
		g_InteralMemory.Bit[DevNumber]=Value;
		return 1;
	}
	else 
		return 0;

}


int Inn_ReadWord(INT16U DevNumber,char *DevName)
{
	if(strcmp(DevName,"P_KW")==0) 
		return g_InteralMemory.KeepWord[DevNumber];
	else if(strcmp(DevName,"P_W")==0) 
		return g_InteralMemory.Word[DevNumber];
	else
		return -1;
}


INT8U Inn_WriteWord(INT16U DevNumber,char *DevName,int Value)
{
	if(strcmp(DevName,"P_KW")==0) 
	{
		g_InteralMemory.KeepWord[DevNumber]=Value;
//		Inn_Store();
		return 1;
	}
	else if((strcmp(DevName,"P_W")==0 ))
	{
		g_InteralMemory.Word[DevNumber]=Value;
		return 1;
	}
	else 
		return 0;
}

/*
void Inn_Store(void)
{
	INT8U *PageBuff;
	int Block,i;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(3);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_InteralMemory;

	for(i=0;i<4;i++)
	{
		WritePage(Block,i,PageBuff);
		PageBuff+=PAGE_SIZE;
	}
}

void Inn_Load(void)
{
	INT8U PageBuff[PAGE_SIZE];
	int Block,i,j;
	INT16U temp;

	Block = Logical_To_Physical_Block(3);

	for(i=0;i<1;i++)
	{
		ReadPage(Block,i,PageBuff);

		for(j=0;j<PAGE_SIZE;j++)
				g_InteralMemory.KeepBit[i*PAGE_SIZE+j]=PageBuff[j];

	}

	for(i=1;i<2;i++)
	{
		ReadPage(Block,i,PageBuff);

		for(j=0;j<PAGE_SIZE;j=j+2)
		{
			temp=PageBuff[j+1];
			temp =(temp <<8)|PageBuff[j];
			
			g_InteralMemory.KeepWord[(i-1)*PAGE_SIZE/2+j/2]=temp;
		}
	}

}
*/




////////////////////////////////////////////////////////////////////////////////////////
///////////                       单片机   070208                              ///////////
////////////////////////////////////////////////////////////////////////////////////////
U8 LRC_Check(U8 *auchMsg,INT16U usDataLen)
{
	U8 uchLRC = 0 ;                  /* LRC U8 initialized */
	while (usDataLen--)
	{
		uchLRC += *auchMsg++;    /* add buffer byte without carry */
	}
	// uchLRC=256-uchLRC;
	return ((U8)uchLRC) ;            /* return twos complement */
}




INT8U Scm_ReadBit(INT16U DevNumber)
{
       unsigned char auchMsg[11]={0};
       //unsigned char recArray[11]={0};
       unsigned char recArray[8]={0};
	char Count;
	int i=0;
	int i_count=0;
	U8 Result=0xff;

	DevNumber=(INT8U) DevNumber;
	
	
       auchMsg[0]=0x3a;      // start 
	auchMsg[1]=0x01;      //station number
	auchMsg[2]=0xb3;      //command:  a3:读字节      a6:写字节      b3: 读位    b6: 写位		
	auchMsg[3]=0x01;		//length
	auchMsg[4]=DevNumber;
	auchMsg[5]=0x00;
	auchMsg[6]=0x00;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=LRC_Check(auchMsg,9);
	auchMsg[10]=0x13;


	//rINTMSK |=BIT_GLOBAL;


	for (Count=0;Count<11;Count++)
	{	
		 // Uart_SendByte(auchMsg[Count]);   //  由于Uart_SendByte()  函数发送字符时遇到0x0a时，会发送0x0a  0x0d，故改用函数Unitel_SendByte()
		 Unitel_SendByte(auchMsg[Count]);
	}

	for(i=0;i<1000;i++);
	    	
	i_count=0;
	 while ((recArray[0]=Uart_Getch())!=0x3b && i_count<20)
	{
		i_count++;
	}

	if(recArray[0]==0x3b)
	{		     	
		for(Count=1;Count<8;Count++)
		{
			recArray[Count]=Uart_Getch();
		}

		if((recArray[0]==0x3b) && (recArray[1]==0x01) && (LRC_Check(recArray,6)==recArray[6]))
	  		Result=recArray[5]; 
		else
			Result = 0xff;	// 080927    原来1  通讯不稳定时会跳1
	}
	else
		Result = 0xff;	// 080927    原来1  通讯不稳定时会跳1
	

	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	
	return Result ;
	
}


INT8U Scm_WriteBit(INT16U DevNumber,U8 value)
{
       unsigned char auchMsg[8]={0};
       unsigned char recArray[8]={0};
	char Count;
	int i=0;
	int i_count=0;
	U8 BoolSucceed = 0;

	DevNumber=(INT8U) DevNumber;
	
	
       auchMsg[0]=0x3a;      // start 
	auchMsg[1]=0x01;      //station number
	auchMsg[2]=0xb6;      //command:  a3:读字节      a6:写字节      b3: 读位    b6: 写位		
	auchMsg[3]=0x01;		//length
	auchMsg[4]=DevNumber;
	auchMsg[5]=value;
	auchMsg[6]=LRC_Check(auchMsg,6);
	auchMsg[7]=0x13;


	//rINTMSK |=BIT_GLOBAL;


	for (Count=0;Count<8;Count++)
	{	
		 // Uart_SendByte(auchMsg[Count]);   //  由于Uart_SendByte()  函数发送字符时遇到0x0a时，会发送0x0a  0x0d，故改用函数Unitel_SendByte()
		 Unitel_SendByte(auchMsg[Count]);
	}

	 for(i=0;i<1000;i++);

	 
	 
	    	
	i_count=0;
	 while ((recArray[0]=Uart_Getch())!=0x3b && i_count<20)
	{
		i_count++;
	}

	if(recArray[0]==0x3b)
	{		     	
		for(Count=1;Count<8;Count++)
		{
			recArray[Count]=Uart_Getch();
		}
	}
	
	if((recArray[0]==0x3b) && (recArray[1]==0x01) && (LRC_Check(recArray,6)==recArray[6]))
  		BoolSucceed=TRUE; 

	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	
	return BoolSucceed ;
	
}




int Scm_ReadWord(INT16U DevNumber)
{
       unsigned char auchMsg[11]={0};
       unsigned char recArray[11]={0};
	char Count;
	int i=0;
	int i_count=0;
	int Result=-1;


	//rINTMSK |=BIT_GLOBAL;


	DevNumber=(INT8U) DevNumber;
		
       auchMsg[0]=0x3a;      // start 
	auchMsg[1]=0x01;      //station number
	auchMsg[2]=0xa3;      //command:  a3:读字节      a6:写字节      b3: 读位    b6: 写位		
	auchMsg[3]=0x01;		//length
	auchMsg[4]=DevNumber;
	auchMsg[5]=0x00;
	auchMsg[6]=0x00;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=LRC_Check(auchMsg,9);
	auchMsg[10]=0x13;




	for (Count=0;Count<11;Count++)
	{	
		 // Uart_SendByte(auchMsg[Count]);   //  由于Uart_SendByte()  函数发送字符时遇到0x0a时，会发送0x0a  0x0d，故改用函数Unitel_SendByte()
		 Unitel_SendByte(auchMsg[Count]);
	}

	 for(i=0;i<1000;i++);
	    	
	i_count=0;
	 while ((recArray[0]=Uart_Getch())!=0x3b && i_count<20)
	{
		i_count++;
	}

	if(recArray[0]==0x3b)
	{		     	
		for(Count=1;Count<11;Count++)
		{
			recArray[Count]=Uart_Getch();
		}
		
		if((recArray[0]==0x3b) && (recArray[1]==0x01) && (LRC_Check(recArray,9)==recArray[9]))
		{
			if((recArray[3])==0x01)                      //8abit
			{
					Result=recArray[5];
			}
			else if((recArray[3])==0x02)    //16bit
			{
				for(i=6;i>4;i--)
					Result=(Result<<8)+recArray[i];
			}

		}
		else
			Result=-1;	

	}
	else
		Result=-1;	
	
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	
	return Result ;
	

}






INT8U Scm_WriteWord(INT16U DevNumber,int value)
{
       unsigned char auchMsg[11]={0};
       unsigned char recArray[11]={0};
	char Count;
	int i=0;
	int i_count=0;
	U8 BoolSucceed=0;


	//rINTMSK |=BIT_GLOBAL;

	DevNumber=(INT8U) DevNumber;
	
       auchMsg[0]=0x3a;      // start 
	auchMsg[1]=0x01;      //station number
	auchMsg[2]=0xa6;      //command:  a3:读字节      a6:写字节      b3: 读位    b6: 写位		
	auchMsg[3]=0x01;		//length
	auchMsg[4]=DevNumber;
	auchMsg[5]=value & 0xff;
	auchMsg[6]=(value & 0xff00)>>8;
	auchMsg[7]=(value & 0xff0000)>>16;
	auchMsg[8]=(value & 0xff000000)>>24;
	auchMsg[9]=LRC_Check(auchMsg,9);
	auchMsg[10]=0x13;



	for (Count=0;Count<11;Count++)
	{	
		 // Uart_SendByte(auchMsg[Count]);   //  由于Uart_SendByte()  函数发送字符时遇到0x0a时，会发送0x0a  0x0d，故改用函数Unitel_SendByte()
		 Unitel_SendByte(auchMsg[Count]);
	}

	 for(i=0;i<1000;i++);
	    	
	i_count=0;
	 while ((recArray[0]=Uart_Getch())!=0x3b && i_count<20)
	{
		i_count++;
	}

	if(recArray[0]==0x3b)
	{		     	
		for(Count=1;Count<11;Count++)
		{
			recArray[Count]=Uart_Getch();
		}
		
		if((recArray[0]==0x3b) && (recArray[1]==0x01) && (LRC_Check(recArray,9)==recArray[9]))
		{
			BoolSucceed=1;
		}
	}
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	
	return BoolSucceed ;
	
}







INT8U Scm_ReadString(INT16U DevNumber,INT16U CurveDataLen,INT8U *CurveValue)
{
	unsigned char auchMsg[11]={0};
//   unsigned char recArray[11]={0};
	char Count;
	int i=0;
	int i_count=0;
	int Result=0;


	//rINTMSK |=BIT_GLOBAL;


	DevNumber=(INT8U) DevNumber;
		
       auchMsg[0]=0x3a;      // start 
	auchMsg[1]=0x01;      //station number
	auchMsg[2]=0xa1;      //command: a1:读一串字节  a3:读字节  a6:写字节   b3: 读位  b6: 写位		
	auchMsg[3]=(CurveDataLen>>8);		//length
	auchMsg[4]=(CurveDataLen & 0xff);
		
	auchMsg[5]=DevNumber;
	auchMsg[6]=0x00;
	auchMsg[7]=0x00;
	auchMsg[8]=0x00;
	auchMsg[9]=LRC_Check(auchMsg,9);
	auchMsg[10]=0x13;




	for (Count=0;Count<11;Count++)
	{	
		 Unitel_SendByte(auchMsg[Count]);
	}


	 for(i=0;i<1000;i++);
	    	

		
	i_count=0;
	 while ((CurveValue[0]=Uart_Getch())!=0x3b && i_count<20)
	{
		i_count++;
	}


	if(CurveValue[0]==0x3b)
	{		     	
		for(Count=1;Count<(8+CurveDataLen);Count++)
		{
			CurveValue[Count]=Uart_Getch();
		}
		
		if((CurveValue[0]==0x3b) && (CurveValue[1]==0x01) && (CurveValue[7+CurveDataLen]==0x13))
		{
			Result=1;	
		}
		else
			Result=0;	

	}
	else
		Result=0;	
	
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	
	return Result ;
	

}



unsigned char RTU_ReadBit(INT16U DevAddress)
{
	//unsigned short num;
	U8 auchMsg[8],SendArray[8],RecArray[6];  
	unsigned char BoolValue=0xff;
	U8 Count; 
	int i;

	auchMsg[0]=0x01;   
	auchMsg[1]=0x01;  
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	auchMsg[4]=0x00;
	auchMsg[5]=0x01;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	for(Count=0;Count<8;Count++) 
	{
		SendArray[Count]=auchMsg[Count];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);


	UartTimeOut_Start();
	
	Count = 0;
	while((RecArray[0]=Uart_Getch())!=0x01 && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==0x01)
	{
		for(Count=1;Count<6;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if((RecArray[0]==0x01) && 
		(RecArray[4]==(CRC(RecArray,4) & 0xff)) 
		/*&& (RecArray[5]==((CRC(RecArray,4)>>8) & 0xff))*/) // 最后一个字节不判断
		{

			BoolValue=RecArray[3];
		}
		else
			BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1
	}
	else
		BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1


	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolValue;


}


	

INT8U RTU_WriteBit(INT16U DevAddress,U8 Value)
{
	//unsigned short num;
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 BoolSucceed=0;
		U8 Count; 
	int i;

	auchMsg[0]=0x01;
	auchMsg[1]=0x05;
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	if(Value==0)
	{
		auchMsg[4]=0x00;
		auchMsg[5]=0x00;
	}
	else
	{
		auchMsg[4]=0xFF;
		auchMsg[5]=0x01;
	}
			
	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	for(Count=0;Count<8;Count++) 
	{
		SendArray[Count]=auchMsg[Count];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);


	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=0x01 && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==0x01)
	{
		for(Count=1;Count<8;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if(RecArray[0]==0x01 &&RecArray[1]==0x05)
		{
			BoolSucceed=1;
		}
	}

		
	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}



int RTU_ReadWord(INT16U DevAddress)
{
	//unsigned short num;
	U8 auchMsg[8] = {0};
	U8 RecArray[7] = {0};
	//U8 RecArray[8] = {0};
   	//short Result=-1;
   	 int Result=-1;
	U8 Count; 
	int i;
	
	auchMsg[0]=0x01;   
	auchMsg[1]=0x03;  
	
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	auchMsg[4]=0x00;
	auchMsg[5]=0x01;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(auchMsg[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);	//1000


	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=0x01 && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==0x01)
	{
		for(Count=1;Count<7;Count++)          
		//for(Count=1;Count<8;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		//if(RecArray[0]==0x01&&RecArray[1]==0x03&&RecArray[2]==0x02)
		if((RecArray[0]==0x01) && 
			(RecArray[5]==(CRC(RecArray,5) & 0xff)) 
			/*&& (RecArray[6]==((CRC(RecArray,5)>>8) & 0xff))*/) // 最后一个字节不判断
		{
			Result=(RecArray[3]<<8)+RecArray[4];
			Result = Result & 0xffff;
		}
		else
		{
			Result=-1;
		}
	}
	else
			Result=-1;

	g_Timer2Uart.Timer_Status = TIMER_STOP;

	
	Delay(400000);  //根据不同设备更改不同延时   增加 2012_10_22
	
	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return Result ;
	

}



U8 RTU_WriteWord(INT16U DevAddress,int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 BoolSucceed=0;
		U8 Count; 
	int i;

	auchMsg[0]=0x01;
	auchMsg[1]=0x06;
	
	auchMsg[2]=DevAddress>>8;
	auchMsg[3]=DevAddress & 0xff;

	auchMsg[4]=Value>>8;
	auchMsg[5]=Value & 0xff;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;
	
	for(i=0;i<8;i++)
	{
		  SendArray[i]=auchMsg[i];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);


	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=0x01 && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==0x01)
	{
		for(Count=1;Count<8;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

	if((RecArray[0]==0x01) && 
		(RecArray[6]==(CRC(RecArray,6) & 0xff)) 
		/*&& (RecArray[7]==((CRC(RecArray,6)>>8) & 0xff))*/) // 最后一个字节不判断
		{
			BoolSucceed=1;	
		}
	}
	
	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;


}



//永宏PLC  080827    ASCII模式
U8 FATEK_ReadBit(char DevChar,INT16U DevNumber)
{
	U8 SendArray[15],RecArray[10];  
	U8 BoolSucceed=3;
	U8 count; 
	int i;

	SendArray[0]=0x02;
	SendArray[1]=0x30;
	SendArray[2]=0x31;	//栈号
	SendArray[3]=0x34;
	SendArray[4]=0x38; //命令
	SendArray[5]=0x30;
	SendArray[6]=0x31;	//个数
	//SendArray[7]=DataToChar(DevChar);
	SendArray[7]=DevChar;
	SendArray[8]=DataToChar(DevNumber/1000);
	SendArray[9]=DataToChar(DevNumber/100%10);
	SendArray[10]=DataToChar(DevNumber/10%10);
	SendArray[11]=DataToChar(DevNumber%10);
	SendArray[12]=DataToChar(LRC_ADD(SendArray,12)/16);
	SendArray[13]=DataToChar(LRC_ADD(SendArray,12)%16);
	SendArray[14]=0x03;
	
	
	
	//rINTMSK |=BIT_GLOBAL;

	for(count=0;count<15;count++)         
	{
		Uart_SendByte(SendArray[count]);
	}

	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x02 && count<=20) count++;
	
	if(RecArray[0]==0x02)
	{
		for(count=1;count<10;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[7]==DataToChar(LRC_ADD(RecArray,7)/16) 
			&& RecArray[8]==DataToChar((LRC_ADD(RecArray,7)%16)))
		{
			BoolSucceed = CharToData(RecArray[6]);
		}	
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}





U8 FATEK_WriteBit(char DevChar,INT16U DevNumber,U8 Value)
{
	U8 SendArray[16],RecArray[9];  
	U8 BoolSucceed=0;
	U8 count; 
	int i;
	
	SendArray[0]=0x02;
	SendArray[1]=0x30;
	SendArray[2]=0x31;	//栈号
	SendArray[3]=0x34;
	SendArray[4]=0x39; //命令
	SendArray[5]=0x30;
	SendArray[6]=0x31;	//个数
	//SendArray[7]=DataToChar(DevChar);
	SendArray[7]=DevChar;
	SendArray[8]=DataToChar(DevNumber/1000);
	SendArray[9]=DataToChar(DevNumber/100%10);
	SendArray[10]=DataToChar(DevNumber/10%10);
	SendArray[11]=DataToChar(DevNumber%10);
	SendArray[12]=DataToChar(Value);
	SendArray[13]=DataToChar(LRC_ADD(SendArray,13)/16);
	SendArray[14]=DataToChar(LRC_ADD(SendArray,13)%16);
	SendArray[15]=0x03;

	

	//rINTMSK |=BIT_GLOBAL;

	for(count=0;count<16;count++)         
	{
		Uart_SendByte(SendArray[count]);
	}

	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x02 && count<=20) count++;
	
	if(RecArray[0]==0x02)
	{
		for(count=1;count<9;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[6]==DataToChar(LRC_ADD(RecArray,6)/16) 
			&& RecArray[7]==DataToChar((LRC_ADD(RecArray,6)%16)))
		{
			BoolSucceed = 1;
		}	
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}



short FATEK_ReadWord(char DevChar,INT16U DevNumber)
{
	U8 SendArray[16],RecArray[13];
	short shortVar=-1;
	U8 count;
	int i;

	SendArray[0]=0x02;
	SendArray[1]=0x30;
	SendArray[2]=0x31;	//栈号
	SendArray[3]=0x34;
	SendArray[4]=0x38; //命令
	SendArray[5]=0x30;
	SendArray[6]=0x31;	//个数
	//SendArray[7]=DataToChar(DevChar);
	SendArray[7]=DevChar;
	SendArray[8]=DataToChar(DevNumber/10000);
	SendArray[9]=DataToChar(DevNumber/1000%10);
	SendArray[10]=DataToChar(DevNumber/100%10);
	SendArray[11]=DataToChar(DevNumber/10%10);
	SendArray[12]=DataToChar(DevNumber%10);
	SendArray[13]=DataToChar(LRC_ADD(SendArray,13)/16);
	SendArray[14]=DataToChar(LRC_ADD(SendArray,13)%16);
	SendArray[15]=0x03;


	//rINTMSK |=BIT_GLOBAL;

	
	for(i=0;i<16;i++)
		Uart_SendByte(SendArray[i]);


	for(i=0;i<1000;i++);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x02 && count<=20) count++;
	
	if(RecArray[0]==0x02)
	{
		for(count=1;count<13;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[10]==DataToChar(LRC_ADD(RecArray,10)/16) 
			&& RecArray[11]==DataToChar((LRC_ADD(RecArray,10)%16)))
		{
			shortVar=CharToData(RecArray[6])*16*16*16 + CharToData(RecArray[7])*16*16 
				+ CharToData(RecArray[8])*16 + CharToData(RecArray[9]);
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return shortVar;

}




INT8U FATEK_WriteWord(char DevChar,INT16U DevNumber,int Value)
{
	U8 SendArray[20],RecArray[9];
	char BoolSucceed=0;
	U8 count;
	int i;

	int tempValue;

	SendArray[0]=0x02;
	SendArray[1]=0x30;
	SendArray[2]=0x31;	//栈号
	SendArray[3]=0x34;
	SendArray[4]=0x39; //命令
	SendArray[5]=0x30;
	SendArray[6]=0x31;	//个数
	//SendArray[7]=DataToChar(DevChar);
	SendArray[7]=DevChar;
	SendArray[8]=DataToChar(DevNumber/10000);
	SendArray[9]=DataToChar(DevNumber/1000%10);
	SendArray[10]=DataToChar(DevNumber/100%10);
	SendArray[11]=DataToChar(DevNumber/10%10);
	SendArray[12]=DataToChar(DevNumber%10);

	tempValue = Value;
	SendArray[16]=DataToChar(tempValue%16);	
	tempValue=tempValue/16;                
	SendArray[15]=DataToChar(tempValue%16);
	tempValue=tempValue/16;                
	SendArray[14]=DataToChar(tempValue%16);
	tempValue=tempValue/16;                
	SendArray[13]=DataToChar(tempValue%16);
	
	SendArray[17]=DataToChar(LRC_ADD(SendArray,17)/16);
	SendArray[18]=DataToChar(LRC_ADD(SendArray,17)%16);
	SendArray[19]=0x03;



	//rINTMSK |=BIT_GLOBAL;

	
	for(i=0;i<20;i++)
		Uart_SendByte(SendArray[i]);

	count = 0;
	while((RecArray[0]=Uart_Getch())!=0x02 && count<=20) count++;
	
	if(RecArray[0]==0x02)
	{
		for(count=1;count<9;count++)          
		{
			RecArray[count]=Uart_Getch();
		}

		if(RecArray[6]==DataToChar(LRC_ADD(RecArray,6)/16) 
			&& RecArray[7]==DataToChar((LRC_ADD(RecArray,6)%16)))
		{
			BoolSucceed=1;
		}
	}

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;
	
}








unsigned char RS485_422_Test(INT16U DevAddress)
{
	//unsigned short num;
	U8 auchMsg[8],SendArray[8],RecArray[16];  
	unsigned char BoolValue=0xff;
	U8 Count; 
	int i;

	auchMsg[0]=0x01;   
	auchMsg[1]=0x01;  
	auchMsg[2]=DevAddress/256;
	auchMsg[3]=DevAddress%256;
	auchMsg[4]=0x00;
	auchMsg[5]=0x01;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	for(Count=0;Count<8;Count++) 
	{
		SendArray[Count]=auchMsg[Count];
	}
	
	//rINTMSK |=BIT_GLOBAL;


	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	for(i=0;i<1000;i++);

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	/*
	if(whichUart==0)
		RdURXH0();
	else if(whichUart==1)
		RdURXH1();
	else if(whichUart==2)
		RdURXH2();
	else if(whichUart==3)
		RdURXH3();
	*/	
	
	for(Count=0;Count<16;Count++)          
	{
		RecArray[Count]=Uart_Getch();
	}

/*
	Count = 0;
	while((RecArray[0]=Uart_Getch())!=0x01 && Count<=20) Count++;
	
	if(RecArray[0]==0x01)
	{
		for(Count=1;Count<6;Count++)          
		{
			RecArray[Count]=Uart_Getch();
		}

		//if(RecArray[1]==0x01 && RecArray[0]==0x01)
		if((RecArray[0]==0x01) && 
		(RecArray[4]==(CRC(RecArray,4) & 0xff)) 
		&& (RecArray[5]==((CRC(RecArray,4)>>8) & 0xff)))
		{

			BoolValue=RecArray[3];
		}
		else
			BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1
	}
	else
		BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1
*/

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolValue;

}




U8 Modus_ReadBit(U8 T_Number, INT16U DevAddress)
{
	INT8U bitValue;
	INT8U num;
	int count;

	num = 0;
	do
	{
		for(count=0; count<DELAY_TIME; count++);	// 10000
		bitValue = Modus_ReadBit_Operation(T_Number, DevAddress);
		num++;
	}
	while((0xff == bitValue) && (num<1)); // 2 通讯失败的时候多试几次	

	return bitValue;
	
}


U8 Modus_ReadBit_Operation(U8 T_Number, INT16U DevAddress)
{
	//unsigned short num;
	U8 auchMsg[8],SendArray[8],RecArray[6];  
	unsigned char BoolValue=0xff;
	U8 Count; 
	int i;

	auchMsg[0]=T_Number;   
	auchMsg[1]=0x01;  
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	auchMsg[4]=0x00;
	auchMsg[5]=0x01;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	for(Count=0;Count<8;Count++) 
	{
		SendArray[Count]=auchMsg[Count];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);

	UartTimeOut_Start();
	
	Count = 0;
	while((RecArray[0]=Uart_Getch())!=T_Number && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==T_Number)
	{
		for(Count=1;Count<6;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if((RecArray[0]==T_Number) && 
		(RecArray[4]==(CRC(RecArray,4) & 0xff)) 
		/*&& (RecArray[5]==((CRC(RecArray,4)>>8) & 0xff))*/) // 最后一个字节不判断
		{

			BoolValue=RecArray[3];
		}
		else
			BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1
	}
	else
		BoolValue = 0xff;	// 080927    原来1  通讯不稳定时会跳1

	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolValue;
	
}


U8 Modus_WriteBit(U8 T_Number, INT16U DevAddress, U8 Value)
{
	INT8U boolSucceed;
	INT8U num;
	int count;

	num = 0;
	do
	{
		for(count=0; count<DELAY_TIME_TRAN; count++);	// 100000
		boolSucceed = Modus_WriteBit_Operation(T_Number, DevAddress, Value);
		num++;
	}
	while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次		

	return boolSucceed;

}

	

U8 Modus_WriteBit_Operation(U8 T_Number, INT16U DevAddress, U8 Value)
{
	//unsigned short num;
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 BoolSucceed=0;
		U8 Count; 
	int i;

	auchMsg[0]=T_Number;
	auchMsg[1]=0x05;
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	if(Value==0)
	{
		auchMsg[4]=0x00;
		auchMsg[5]=0x00;
	}
	else
	{
		auchMsg[4]=0xFF;
		auchMsg[5]=0x01;
	}
			
	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	for(Count=0;Count<8;Count++) 
	{
		SendArray[Count]=auchMsg[Count];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	
	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);

	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=T_Number && Count<=20)
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==T_Number)
	{
		for(Count=1;Count<8;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if(RecArray[0]==T_Number &&RecArray[1]==0x05)
		{
			BoolSucceed=1;
		}
	}

	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}




// 读取变频器输出地址: 0x2103
int Modus_ReadWord(U8 T_Number, INT16U DevAddress)
{
	int wordValue;
	INT8U num;
	int count;

	num = 0;
	do
	{
		for(count=0; count<DELAY_TIME_TRAN; count++);	// 100000
		wordValue = Modus_ReadWord_Operation(T_Number, DevAddress);
		num++;
	}
	while((-1 == wordValue) && (num<1)); // 通讯失败的时候多试几次		

	return wordValue;

}




// 读取变频器输出地址: 0x2103
int Modus_ReadWord_Operation(U8 T_Number, INT16U DevAddress)
{
	//unsigned short num;
	U8 auchMsg[8] = {0};
	U8 RecArray[7] = {0};
   	 short Result=-1;
	U8 Count; 
	int i;
	
	auchMsg[0]=T_Number;   //变频器地址
	auchMsg[1]=0x03;  
	
	auchMsg[2]=DevAddress >> 8;
	auchMsg[3]=DevAddress & 0xff;
	auchMsg[4]=0x00;
	auchMsg[5]=0x01;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;

	
	//rINTMSK |=BIT_GLOBAL;

	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(auchMsg[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);	//1000

	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=T_Number && Count<=20)
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==T_Number)  //变频器地址
	{
		for(Count=1;Count<7;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if((RecArray[0]==T_Number) && 
			(RecArray[5]==(CRC(RecArray,5) & 0xff)) 
			/*&& (RecArray[6]==((CRC(RecArray,5)>>8) & 0xff))*/)  // 最后一个字节不处理
		{
			Result=(RecArray[3]<<8)+RecArray[4];
			Result = Result & 0xffff;
		}
		else
		{
			Result=-1;
		}
	}
	else
			Result=-1;

	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return Result ;
	
}





// 设置变频器自动频率地址: 0x2001
// 设置变频器点动频率地址: 0x0115
U8 Modus_WriteWord(U8 T_Number, INT16U DevAddress, int Value)
{
	INT8U boolSucceed;
	INT8U num;
	int count;

	num = 0;
	do
	{
		for(count=0; count<DELAY_TIME_TRAN; count++);	// 100000
		boolSucceed = Modus_WriteWord_Operation(T_Number, DevAddress, Value);
		num++;
	}
	while((FALSE == boolSucceed) && (num<3)); // 通讯失败的时候多试几次		

	return boolSucceed;

}


// 设置变频器自动频率地址: 0x2001
// 设置变频器点动频率地址: 0x0115
U8 Modus_WriteWord_Operation(U8 T_Number, INT16U DevAddress, int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 BoolSucceed=0;
		U8 Count; 
	int i;

	auchMsg[0]=T_Number;	//变频器地址(RTU PLC 为0x01)
	auchMsg[1]=0x06;
	
	auchMsg[2]=DevAddress>>8;
	auchMsg[3]=DevAddress & 0xff;

	auchMsg[4]=Value>>8;
	auchMsg[5]=Value & 0xff;

	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;
	
	for(i=0;i<8;i++)
	{
		  SendArray[i]=auchMsg[i];
	}
	
	//rINTMSK |=BIT_GLOBAL;

	if(whichUart==3)
		rGPHDAT |= 0x1000;	//GPH12	+Write

	for(i=0; i<DELAY_TIME_RTU; i++);

	for(Count=0;Count<8;Count++)         
	{
		Unitel_SendByte(SendArray[Count]);
	}

	if(whichUart==3)
		rGPHDAT &= 0xefff;	//GPH12	-Read

	for(i=0;i<1000;i++);

	UartTimeOut_Start();

	Count = 0;
	while((RecArray[0]=Uart_Getch())!=T_Number && Count<=20) 
	{
		Count++;
		if(TRUE == g_UartTimeOut)
			break;
	}
	
	if(RecArray[0]==T_Number)	//变频器地址
	{
		for(Count=1;Count<8;Count++)          
		{
			if(TRUE == g_UartTimeOut)
				break;
			RecArray[Count]=Uart_Getch();
		}

		if((RecArray[0]==T_Number) && 
			(RecArray[6]==(CRC(RecArray,6) & 0xff)) 
		/*&& (RecArray[7]==((CRC(RecArray,6)>>8) & 0xff))*/)  // 最后一个字节不处理
		{
			BoolSucceed=1;	
		}
	}
	
	g_Timer2Uart.Timer_Status = TIMER_STOP;

	//rINTMSK &=~(BIT_GLOBAL|BIT_EINT1);
	return BoolSucceed;

}






