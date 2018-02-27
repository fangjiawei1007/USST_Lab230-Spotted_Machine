
#include <string.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "2416slib.h"
#include "Nand.h"


BOOL bLARGEBLOCK;

volatile U8 NF8_Spare_Data[16];
volatile U8 Adv_NF8_Spare_Data[64];

/*static U8 se8Buf[16]={
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
};*/


static U8 Adv_se8Buf[64]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};


U8 InvalidBlk_Buf[528];


//////////////   090721  苟潼넋埼돕NandFlash  //////////////////////////
static U32 targetBlock;	    // Block number (0 ~ 4095)
static U32 targetSize;	    // Total byte size 
static U8 blockBuf[0x20000];   

static char RCVBUF[10];//by xh
static U32 imageSize;
volatile unsigned char *BufBase;

static void *function[][2]=
{
	(void *)K9f1g08_Program,		"K9f1g08 Program     ",
	(void *)K9f1g08_PrintBlock,	"K9f1g08 Pr BlkPage  ",
	(void *)1,			    		"Exit                ",
	0,0
};

#define BAD_CHECK	(0)
//#define ECC_CHECK	(0)



DWORD ReadFlashID(void)
{
	BYTE Mfg, Dev, i;
	volatile int ReadID;
	U32 nRet;

	bLARGEBLOCK = FALSE;
	nRet = TRUE;
	
	NF_nFCE_L();

	NF_CMD(CMD_READID);				// Send flash ID read command.

	NF_ADDR(0);	
	for (i=0; i<10; i++);

	Mfg	= NF_RDDATA8();		// Maker code
	Dev	= NF_RDDATA8();		// Devide code(K9S1208V:0x76), (K9K2G16U0M:0xca)

	NF_nFCE_H();

	if (Mfg == (U8)0xEC)
	{
		if (Dev >= (U8)0xA0)
			bLARGEBLOCK = TRUE;
	}
	else if (Mfg == (U8)0x98)
	{
	}
	else
	{
		nRet = FALSE;
	}

	ReadID = (DWORD)((Mfg<<8)+Dev);

	//return nRet;
	return ReadID;
}


void NF_Reset(void)
{
	volatile int i;
   
	NF_nFCE_L();

	NF_CLEAR_RB();
	NF_CMD(0xFF);	//reset command
	for(i=0;i<10;i++);  //tWB = 100ns. //??????
	NF_DETECT_RB();

	NF_nFCE_H();						// Deselect the flash chip.
}


void NF_Init(void)
{

	rNFCONF =	(TACLS  <<  12) | /* CLE & ALE = HCLK * (TACLS  + 1)   */
				(TWRPH0 <<  8) | /* TWRPH0	= HCLK * (TWRPH0 + 1)   */
				(TWRPH1 <<  4) |
				(0<<0);

	rNFCONT = (0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(0x3<<1)|(1<<0);
	
	rNFSTAT = (1<<4);
	
	NF_Reset();
}


BOOL ECC_CorrectData(unsigned int sectoraddr, UINT8* pData, UINT32 nRetEcc, ECC_CORRECT_TYPE nType)
{
	DWORD  nErrStatus;
	DWORD  nErrDataNo;
	DWORD  nErrBitNo;
	UINT32 nErrDataMask;
	UINT32 nErrBitMask = 0x7;
	BOOL bRet = TRUE;

	if (nType == ECC_CORRECT_MAIN ||nType == ECC_CORRECT_SPARE1 || nType == ECC_CORRECT_SPARE2)
	{
		nErrStatus   = 0;
		nErrDataNo   = 7;
		nErrBitNo    = 4;
		nErrDataMask = 0x7ff;
	}
	else
	{
		return FALSE;
	}

	switch((nRetEcc>>nErrStatus) & 0x3)
	{
		case 0:	// No Error
			bRet = TRUE;
			break;
		case 1:	// 1-bit Error(Correctable)
//			Uart_SendString("ECC 1bit error ");
			(pData)[(nRetEcc>>nErrDataNo)&nErrDataMask] ^= (1<<((nRetEcc>>nErrBitNo)&nErrBitMask));
			bRet = TRUE;		
			break;
		case 2:	// Multiple Error
//			Uart_SendString("ECC Multiple error ");
//			Uart_SendDWORD(sectoraddr,1);
			bRet = FALSE;			
			break;
		case 3:	// ECC area Error
//			Uart_SendString("ECC area error\n");
		default:
			bRet = FALSE;			
			break;
	}

	return bRet;
}


int NF_ReadPage(UINT32 sector,UINT8 *buffer)
{
	UINT32 block;
	UINT8 * bufPt=buffer;
	unsigned int RowAddr, ColAddr;
	DWORD MECCBuf, rddata1, rddata2;
	UINT32 nRetEcc;
	int nRet = FALSE;
	int nSectorLoop;

	block = sector>>6;

	NF_nFCE_L();	

	for (nSectorLoop = 0; nSectorLoop < (bLARGEBLOCK==TRUE?4:1); nSectorLoop++)
	{
		ColAddr = nSectorLoop * 512;

		NF_CMD(CMD_READ);   // Read command

		if (bLARGEBLOCK == TRUE)
		{
			RowAddr = (block<<6) + sector;
			
			NF_ADDR((ColAddr)   &0xff);    // 1st cycle
			NF_ADDR((ColAddr>>8)&0xff);    // 2nd cycle
			NF_ADDR((RowAddr)   &0xff);    // 3rd cycle
			NF_ADDR((RowAddr>>8)&0xff);    // 4th cycle
			if (LB_NEED_EXT_ADDR)
				NF_ADDR((RowAddr>>16)&0xff);    // 5th cycle
		}
		else
		{
			RowAddr = (block<<5) + sector;
			
			NF_ADDR((ColAddr)   &0xff);    // 1st cycle
			NF_ADDR((RowAddr)   &0xff);    // 2nd cycle
			NF_ADDR((RowAddr>>8)&0xff);    // 3rd cycle
			if (SB_NEED_EXT_ADDR)
				NF_ADDR((RowAddr>>16)&0xff);    // 4th cycle
		}

		NF_CLEAR_RB();
		NF_CMD(CMD_READ3);   // Read command
		NF_DETECT_RB();
		//NF_WAITRB();

		NF_RSTECC();	// Initialize ECC

		NF_MECC_UnLock();
		
		__RdPage512(bufPt+nSectorLoop*512);		// Read 512 bytes.

		NF_MECC_Lock();

		if (bLARGEBLOCK == TRUE)
		{
			ColAddr = PAGE_SIZE;

			NF_CMD(CMD_RDO);		// Random Data Output In a Page, 1st Cycle
			NF_ADDR((ColAddr)   &0xff);    // 1st cycle
			NF_ADDR((ColAddr>>8)&0xff);    // 2nd cycle
			NF_CMD(CMD_RDO2);		// Random Data Output In a Page. 2nd Cycle
		}

		rddata1 = NF_RDDATA32();  // check bad block
		rddata2 = NF_RDDATA32();

		if (bLARGEBLOCK == TRUE)
		{
			if (((rddata1 & 0xff) != 0xff) && (((rddata2>>8) & 0xff) != 0x03)) return nRet;  // bad block && !(OEM_BLOCK_RESERVED | OEM_BLOCK_READONLY)
		}
		else if ((((rddata2>>8) & 0xff) != 0xff) && ((rddata2 & 0xff) != 0x03)) return nRet;  // bad block && !(OEM_BLOCK_RESERVED | OEM_BLOCK_READONLY)

		if (bLARGEBLOCK == TRUE)
		{
			ColAddr = PAGE_SIZE + 8 + nSectorLoop*4;

			NF_CMD(CMD_RDO);		// Random Data Output In a Page, 1st Cycle
			NF_ADDR((ColAddr)   &0xff);    // 1st cycle
			NF_ADDR((ColAddr>>8)&0xff);    // 2nd cycle
			NF_CMD(CMD_RDO2);		// Random Data Output In a Page. 2nd Cycle
		}

		MECCBuf = NF_RDDATA32();

		NF_WRMECCD0( ((MECCBuf&0x0000ff00)<<8) | ((MECCBuf&0x000000ff)    ) );
		NF_WRMECCD1( ((MECCBuf&0xff000000)>>8) | ((MECCBuf&0x00ff0000)>>16) );

		nRetEcc = rNFECCERR0;

		nRet = ECC_CorrectData(RowAddr, bufPt+nSectorLoop*512, nRetEcc, ECC_CORRECT_MAIN);

		if (!nRet) return nRet;
	}

	NF_nFCE_H();	

	return nRet;
}




U32 ReadPage(U32 BlkIndex, U32 PageIndex, U8 *buf)
{
	//BOOL bRet = TRUE;
	int i;
	U32 Mecc, Secc;
	U32 BlkPageID;

	BlkPageID = (BlkIndex<<PAGES_PER_BLOCK)+PageIndex;

	for (i=0; i<100000; i++);	// 獨0.25ms

	// Initialize MECC Module
	NF_RSTECC();		// NFCONT [5:4]를 11로 셋팅. ; Init main ECC, Init spare ECC.
	NF_MECC_UnLock();	// NFCOnT [6]를 0으로 클리어. ; Unlock Spare ECC


	NF_nFCE_L();	// NFCONT의 [1]를 0으로 클리어 ... Force nFCE to low

	NF_CLEAR_RB();	// NFSTAT의 [4]를 1로 셋팅 ... RnB_TransDetect ... 

	NF_CMD(CMD_READ);	// Read data command

	// Set address to read data from Spare area
	//NF_ADDR((2048)&0xff);	
	//NF_ADDR((2048>>8)&0xff);
	NF_ADDR((0)&0xff);	
	NF_ADDR((0>>8)&0xff);
	NF_ADDR((BlkPageID)&0xff);
	NF_ADDR((BlkPageID>>8)&0xff);
	if (LB_NEED_EXT_ADDR)
		NF_ADDR((BlkPageID>>16)&0xff);  

	NF_CMD(CMD_READ3); // end of read command
	//for (i=0; i<10; i++);

	NF_DETECT_RB();		// NFSTAT의 [4],:0]이 11일 때까지 무한 루프. -> RnB가 low에서 high를 치면 1로 됨.


	for(i=0;i<PAGE_SIZE;i++) 
	{
		*buf++=NF_RDDATA8();	// Read one page
	}

	NF_MECC_Lock();		// NFCONT [6]를 1로 셋팅. ; Lock Spare ECC


	NF_SECC_UnLock();
	Adv_NF8_Spare_Data[0]=NF_RDDATA8();
	Mecc=NF_RDDATA();
	rNFMECCD0=((Mecc&0xff00)<<8)|(Mecc&0xff);
	rNFMECCD1=((Mecc&0xff000000)>>8)|((Mecc&0xff0000)>>16);

	Adv_NF8_Spare_Data[1]=Mecc&0xff;
	Adv_NF8_Spare_Data[2]=(Mecc&0xff00)>>8;
	Adv_NF8_Spare_Data[3]=(Mecc&0xff0000)>>16;
	Adv_NF8_Spare_Data[4]=(Mecc&0xff000000)>>24;

	Adv_NF8_Spare_Data[5]=NF_RDDATA8();  // read 5
	Adv_NF8_Spare_Data[6]=NF_RDDATA8();  // read 6
	Adv_NF8_Spare_Data[7]=NF_RDDATA8();  // read 7

	NF_SECC_Lock();
	Secc=NF_RDDATA();
	rNFSECCD=((Secc&0xff00)<<8)|(Secc&0xff);
	Adv_NF8_Spare_Data[8]=Secc&0xff;
	Adv_NF8_Spare_Data[9]=(Secc&0xff00)>>8;
	Adv_NF8_Spare_Data[10]=(Secc&0xff0000)>>16;
	Adv_NF8_Spare_Data[11]=(Secc&0xff000000)>>24;

	for(i=12;i<64;i++) 
	{
		Adv_NF8_Spare_Data[i]=NF_RDDATA8();	// Read spare array with 4byte width
	}

	NF_nFCE_H();		// NFCONT의 [1]를 1으로 셋팅 ... Force nFCE to high

	if ((rNFECCERR0&0xf) == 0x0)
	{
		if(UART_PrintStr && 0x23==rUBRDIV1)
			Uart_Printf("ECC OK!\n");
		return OK;
	}
	else 
	{
		if(UART_PrintStr && 0x23==rUBRDIV1)
			Uart_Printf("ECC FAIL!\n");
		return FAIL;
	}


}



U32 EraseBlock(U32 BlkIndex)
{
	BOOL    bRet = TRUE;
	U32 BlkPageID;
	int i;

	BlkPageID = (BlkIndex<<PAGES_PER_BLOCK);
	//BlkPageID &= ~0x3f;	

	for (i=0; i<100000; i++);	// 獨0.25ms

	//  Enable the chip
	NF_nFCE_L();						// Select the flash chip.

	NF_CLEAR_RB();

	//  Issue command
	NF_CMD(CMD_ERASE);

	//  Set up address
	NF_ADDR((BlkPageID) & 0xff);
	NF_ADDR((BlkPageID >> 8) & 0xff);
	if (LB_NEED_EXT_ADDR)
	{
		NF_ADDR((BlkPageID >> 16) & 0xff);
	}

	//  Complete erase operation
	NF_CMD(CMD_ERASE2);

	//  Wait for ready bit
	NF_DETECT_RB();	 // Wait tR(max 12us)


	// 결과 상태 체크
	if ( rNFSTAT & STATUS_ILLACC )
	{
        	//RETAILMSG(1, (TEXT("LB######## Error Erasing block (Illigar Access) %d!\n"), blockID));
		rNFSTAT =  STATUS_ILLACC;	// Write 1 to clear.
		bRet = FALSE;
	}
	else
	{
		//  Check the status
		NF_CMD(CMD_STATUS);

		if( NF_RDDATA8() & STATUS_ERROR) {
			//RETAILMSG(1, (TEXT("LB######## Error Erasing block %d!\n"), blockID));
			bRet = FALSE;
		}
	}

	NF_nFCE_H();						// Select the flash chip.


	return bRet;

}


U32 WritePage(U32 BlkIndex, U32 PageIndex, U8 *buf)
{
	//BOOL    bRet = TRUE;
	int i;
	U32 Mecc, Secc;
	U32 BlkPageID;

	BlkPageID = (BlkIndex<<PAGES_PER_BLOCK)+PageIndex;

	for (i=0; i<100000; i++);	// 獨0.25ms

	// Initialize MECC module
	NF_RSTECC();
	NF_MECC_UnLock();


   	//  Chip enable
	NF_nFCE_L();

	NF_CLEAR_RB();

   	//  Write the command
    	//  First, let's point to the spare area
	NF_CMD(CMD_WRITE);

   	//  Write the address
	//NF_ADDR((2048)&0xff);
	//NF_ADDR((2048>>8)&0xff);
	NF_ADDR((0)&0xff);
	NF_ADDR((0>>8)&0xff);
	NF_ADDR((BlkPageID)&0xff);
	NF_ADDR((BlkPageID>>8)&0xff);
	if (LB_NEED_EXT_ADDR)
	{
		NF_ADDR((BlkPageID>>16)&0xff);
	}

	for(i=0; i<PAGE_SIZE; i++)
		WrNFDat8(buf[i]);

	NF_MECC_Lock();



	// Get ECC data.
	// Spare data for 8bit
	// byte  0                  1    2     3     4          5               6      7            8         9
	// ecc  [Bad marking] [0]  [1]  [2]  [3]    x                       SECC0  SECC1
	Mecc = rNFMECC0;

	Adv_se8Buf[0]=0xff;
	Adv_se8Buf[1]=(U8)(Mecc&0xff);
	Adv_se8Buf[2]=(U8)((Mecc>>8) & 0xff);
	Adv_se8Buf[3]=(U8)((Mecc>>16) & 0xff);
	Adv_se8Buf[4]=(U8)((Mecc>>24) & 0xff);
			// Marking good block

	NF_SECC_UnLock();

	//Write extra data(ECC, bad marking)
	for(i=0;i<5;i++) 
	{
		WrNFDat8(Adv_se8Buf[i]);	// Write spare array(Main ECC)
		Adv_NF8_Spare_Data[i]=Adv_se8Buf[i];
    	}  
	NF_SECC_Lock(); 
	
	Secc=rNFSECC; 
	Adv_se8Buf[8]=(U8)(Secc&0xff);
	Adv_se8Buf[9]=(U8)((Secc>>8) & 0xff);
	
	for(i=5;i<64;i++) 
	{
		WrNFDat8(Adv_se8Buf[i]);  // Write spare array(Spare ECC and Mark)
		Adv_NF8_Spare_Data[i]=Adv_se8Buf[i];
	}  

 	NF_CLEAR_RB();
	NF_CMD(CMD_WRITE2);	 // Write 2nd command
	NF_DETECT_RB();
//	while(NFConDone==0);
//	 rNFCONT&=~(1<<9);
//	 rNFCONT&=~(1<<10); // Disable Illegal Access Interrupt
//	 if(rNFSTAT&0x20) return FAIL;

	NF_CMD(CMD_STATUS);   // Read status command   
    
	for(i=0;i<3;i++);  //twhr=60ns
    
	if (NF_RDDATA()&0x1) 
	{// Page write error
		NF_nFCE_H();
		if(UART_PrintStr && 0x23==rUBRDIV1)
			Uart_Printf("[PROGRAM_ERROR:block#=%d]\n", BlkPageID>>PAGES_PER_BLOCK);
		//NF8_MarkBadBlock(block);
		return FAIL;
	} 
	else 
	{
		NF_nFCE_H();
		return OK;
	}

}




void MarkBadBlk(U32 BlkIndex)
{
	BOOL    bRet = TRUE;

	U32 BlkPageID;

	BlkPageID = (BlkIndex<<PAGES_PER_BLOCK);
	//BlkPageID &= ~0x3f;

    //  Enable chip
	NF_nFCE_L();
	NF_CLEAR_RB();

    //  Issue command
    //  We are dealing with spare area
	NF_CMD(CMD_WRITE);

    //  Set up address
	NF_ADDR((PAGE_SIZE)&0xff);
	NF_ADDR(((PAGE_SIZE)>>8)&0xff);
	NF_ADDR((BlkPageID) & 0xff);
	NF_ADDR((BlkPageID >> 8) & 0xff);
	if (LB_NEED_EXT_ADDR)
		NF_ADDR((BlkPageID >> 16) & 0xff);

	//NF_WRDATA(BADBLOCKMARK);
	WrNFDat8(BADBLOCKMARK);

    //  Copmlete the write
	NF_CMD(CMD_WRITE2);

    //  Wait for RB
	NF_DETECT_RB();	 // Wait tR(max 12us)


	if ( rNFSTAT & STATUS_ILLACC )
	{
		//RETAILMSG(1, (TEXT("NAND_LB_WriteSectorInfo() ######## Error Programming page (Illigar Access) %d!\n")));
		rNFSTAT =  STATUS_ILLACC;	// Write 1 to clear.
       	bRet = FALSE;
	}
	else
	{
		//  Check the status of program
		NF_CMD(CMD_STATUS);

		if( NF_RDDATA8() & STATUS_ERROR) {
			//RETAILMSG(1, (TEXT("NAND_LB_WriteSectorInfo() ######## Error Programming page %d!\n")));
			bRet = FALSE;
		}
	}

	//  Disable chip select
	NF_nFCE_H();

	//return bRet;
	return;

}


//int CheckBadBlk(U32 dwPageID)
int IsBadBlk(U32 BlkIndex)
{
	BOOL bRet = FALSE;
	BYTE    wFlag;

	U32 BlkPageID;

	BlkPageID = (BlkIndex<<PAGES_PER_BLOCK);
	//BlkPageID &= ~0x3f;
	
	//  Enable the chip
	NF_nFCE_L();
	NF_CLEAR_RB();

	//  Issue the command
	NF_CMD(CMD_READ);

	//  Set up address
	// 2048+0x05 == 예비영역의 5번지 byte ==> Bad block marker. 0xff가 아니면 bad block
	// 1cycle + 2cycle address = column address -> page에서의 byte위치 표시(12bit)
	NF_ADDR((PAGE_SIZE+0)&0xff);
	NF_ADDR(((PAGE_SIZE+0)>>8)&0xff);
	// 3cycle + 4cycle + 5cycle = row address -> block의 위치 표시(2048개의 block, 11bit)
	NF_ADDR((BlkPageID) & 0xff);
	NF_ADDR((BlkPageID >> 8) & 0xff);
	if (LB_NEED_EXT_ADDR)
		NF_ADDR((BlkPageID >> 16) & 0xff);

	NF_CMD(CMD_READ3);

	//  Wait for Ready bit
	NF_DETECT_RB();	 // Wait tR(max 12us) -> NFSTAT의 4번bit와 1번bit가 1이 될 때까지 무한루프

	//  Now get the byte we want
	wFlag = (BYTE)(NF_RDDATA8()&0xff);

	//  Disable the chip
	NF_nFCE_H();
	
	
	if(wFlag != 0xff)
	{	//예비영역의 5번지가 0xff가 아니면 Bad block
		if(UART_PrintStr && 0x23==rUBRDIV1)
			Uart_Printf("[block %d has been marked as a bad block]\n", BlkPageID>>PAGES_PER_BLOCK);
		bRet = TRUE;
	}
	else
		bRet = FALSE;
	
	
	return bRet;
	
}




void Check_Invalid_Block(void)
{
	INT16U block;
	int i,count=1;
		
	for (i=0;i<528;i++)
	{
		InvalidBlk_Buf[i] = 0x0;
	}
	
	for (block=1;block<1024;block++)
	{
		if(IsBadBlk(block)) //젯崎뻐욥珂等척뇹잿?
		{
			InvalidBlk_Buf[count++]=((block>>8) & 0xff);
			InvalidBlk_Buf[count++]=(block & 0xff);
			continue;
		}
	
	}
	InvalidBlk_Buf[0] = (count-1)/2;

}




INT16U Logical_To_Physical_Block(INT16U LogicalBlock)
{
	INT16U i;
	INT8U InvalidCount;
	INT16U InvalidBlock;	

	InvalidCount = InvalidBlk_Buf[0];
	for (i=1;i<=InvalidCount*2;i=i+2)
	{
		InvalidBlock = InvalidBlk_Buf[i];
		InvalidBlock = (InvalidBlock<<8) | InvalidBlk_Buf[i+1];
		if (LogicalBlock<InvalidBlock)
		{			
			break;
		}
		else
		{		
			LogicalBlock++;
		}
	}
	
	return LogicalBlock;

}


INT16U Physical_To_Logical_Block(INT16U PhysicalBlock)
{
	INT16U i;
	INT8U InvalidCount;
	INT16U InvalidBlock;
	INT8U  count=0;

	InvalidCount = InvalidBlk_Buf[0];
	for (i=1;i<=InvalidCount*2;i=i+2)
	{
		InvalidBlock = InvalidBlk_Buf[i];
		InvalidBlock = (InvalidBlock<<8) | InvalidBlk_Buf[i+1];
		if(PhysicalBlock > InvalidBlock)
		{
			count++;
		}
		else 
		{
			break;
		}
	}
	return PhysicalBlock-count;

}







//////////////////////////////////////////////////////

INT8U Get_Invalid_Block(INT16U CurrentBlock,INT16U NextBlock,INT8U *ptr)
{
	INT16U BlockTemp;
	INT8U i,LoopCount;
	INT8U count;
	INT8U *TempPtr;

	TempPtr=ptr+1;
	count=0;

	LoopCount= InvalidBlk_Buf[0];
	for (i=1;i<=LoopCount*2;i=i+2)
	{
		BlockTemp=InvalidBlk_Buf[i];
		BlockTemp=(BlockTemp<<8) | InvalidBlk_Buf[i+1];;
		if(BlockTemp>CurrentBlock && BlockTemp<NextBlock)
		{
			count++;
			*(TempPtr++)=(BlockTemp>>8) & 0xff;
			*(TempPtr++)=BlockTemp & 0xff;	
		}
	}
	*ptr=count;
	return count;
 
}



//////////////////////////////////////////////////////////////////////////
//////////////   090721  苟潼넋埼돕NandFlash  //////////////////////////
//////////////////////////////////////////////////////////////////////////

U32 factorial(U8 num,U8 rank)
{
	U32 value = num ,i;
	
	if (rank==0) 
		return 1;
	else
	{
		for (i=1;i<rank;i++)
			value = value * num;
		
		return value;
	}    
}

U32 GETVALUE(char *buf)
{
	U32 value=0 ,i ,temp=0;
	
	for (i=0;i<10;i++)
		*(buf+i)=0;//clear buf
	 
	Uart_GetString(buf);
	
	for (i=0;i<10;i++)
		if (*(buf+i)!=0) temp++;//count nums
	 
	for(i=0;i<temp;i++)
		value = value + (*(buf+i)-48)*factorial(10,temp-1-i);//ascii-48=num, mul 10^rank
		
	return value;  
}  


void K9f1g08_Menu(void)
{
	int i;
	U16 id;

	Uart_Printf("\n[K9f1g08 NAND Flash JTAG Programmer]\n");
	//K9f1g08_JtagInit();
	NF_Init();

	id=ReadFlashID();

	if(id!=NandFlashID)	// K9F1G08Q0A Device Id = A1(0xeca1)
					// K9F1G08Q0B Device Id = F1(0xecf1)
	{
		Uart_Printf("ERROR: K9f1g08(0xecf1) is not detected. Detected ID=0x%x.\n",id);
		return;
	}
	else
	{
		Uart_Printf("K9f1g08(0xecf1) is detected. ID=0x%x\n",id);
	}

	while(1)
	{

		i=0;
		while(1)
		{   //display menu
			Uart_Printf("%2d:%s",i,function[i][1]);
			i++;
			if((int)(function[i][0])==0)
			{
				Uart_Printf("\n");
				break;
			}
			if((i%4)==0)
			Uart_Printf("\n");
		}

		Uart_Printf("Select the function to test :");
		//scanf("%d",&i);
		i=GETVALUE(RCVBUF);
		if( i>=0 && (i<((sizeof(function)/8)-2)) ) 
			( (void (*)(void)) (function[i][0]) )();  
		else
			break; //Exit menu
	}
}


void K9f1g08_Program(void)
{
	int i;
	int programError=0;
	U32 blockIndex;
	int noLoad=0;

	/*U8 spareBuf[64]=
	{
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	};*/

	U8 *srcPt;
	U32 progSize=0;

	Uart_Printf("\n[SMC(K9f1g08Q0M) NAND Flash Writing Program]\n");

	Uart_Printf("\nPlease input code size:");//by xhe
	imageSize = GETVALUE(RCVBUF);

	Uart_Printf("\nSource size:0h~%xh\n",imageSize-1);
	Uart_Printf("\nAvailable target block number: 0~1024\n");
	Uart_Printf("Input target block number:");
	//scanf("%d",&targetBlock);
	targetBlock = GETVALUE(RCVBUF);
	targetSize=((imageSize+0x20000-1)/0x20000)*0x20000;
	Uart_Printf("target start block number     =%d\n",targetBlock);
	Uart_Printf("target size        (0x20000*n) =0x%x\n",targetSize);
	Uart_Printf("STATUS:");
	blockIndex=targetBlock;
	BufBase = ((volatile unsigned char *)SJF_Ram_Base);
	
	while(1)
	{
		if(noLoad==0)
		{
		    LoadImageFile(blockBuf,0x20000);
		}
		noLoad=0;

#if BAD_CHECK       
		if(IsBadBlk(blockIndex) && blockIndex!=0 )	// 1:bad 0:good
		{
			blockIndex++;   // for next block
			noLoad=1;
			continue;
		}
#endif
		if(!EraseBlock(blockIndex))
		{
			MarkBadBlk(blockIndex);
			blockIndex++;   // for next block
			noLoad=1;
			continue;
		}

		Uart_Printf("E");
		srcPt=blockBuf;

		for(i=0;i<64;i++)    // if y download the testcode or OS image, y replace 4 to 64.
		{
			if(!WritePage(blockIndex,i,srcPt/*spareBuf*/))// block num, page num, buffer
			{
				programError=1;
				break;
			}

			// srcPt+=512;	// Increase buffer raddr one pase size
			srcPt+=2048;	// Increase buffer raddr one pase size
			Uart_Printf("p");
		}
		
		Uart_Printf("\n");

		if(programError==1)
		{
			blockIndex++;
			noLoad=1;
			programError=0;
			continue;
		}
		
		progSize+=0x20000;
		if(progSize>=imageSize)
			break;	// Exit while loop
		blockIndex++;
	}
}
 
int LoadImageFile(U8 *buf,int size)
{
	int i,readSize=size;
	
	for(i=0;i<size;i++)
	{
		buf[i] = *BufBase++;
	}
	return readSize;
}

void K9f1g08_PrintBlock(void)// Printf one page
{
	int i;
	U16 id;
	U32 block,page;
	U8	buffer[2048+64];

	Uart_Printf("\n[SMC(K9f1g08) NAND Flash block read]\n");	

	NF_Init();
	id=ReadFlashID();
	Uart_Printf("ID=%x(0xecf1)\n",id);
	if(id!=NandFlashID)
		return;

	Uart_Printf("Input target block number:");
	//scanf("%d",&block);
	block = GETVALUE(RCVBUF);
	Uart_Printf("Input target page number:");   
	//scanf("%d",&page);
	page = GETVALUE(RCVBUF);

	ReadPage(block,page,buffer);

	Uart_Printf("block=%d,page=%d:",block,page);
	for(i=0;i<2048;i++)
	{
		if(i%16==0)
			Uart_Printf("\n%3xh:",i);
		Uart_Printf("%02x ",buffer[i]);
	}
	Uart_Printf("\nS.A.:",i);

	for(i=2048;i<2048+64;i++)
	{
		Uart_Printf("%02x ",buffer[i]);
	}

	Uart_Printf("\n");    	
}

