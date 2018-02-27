#ifndef _NAND_H_
#define _NAND_H_

#define NF_CMD(cmd)		{rNFCMD=cmd;}
#define NF_ADDR(addr)	  {rNFADDR=addr;}	

#define NF_nFCE_L()			{rNFCONT &= ~(1 << 1);}
#define NF_nFCE_H()			{rNFCONT |=  (1 << 1);}

/*
#define NF_FCE	0	// 0:FCE0, 1:FCE1, In steploader, NAND cs has to be FCE0.

#if NF_FCE==0
#define NF_nFCE_L()			{rNFCONT &= ~(1 << 1);}
#define NF_nFCE_H()			{rNFCONT |=  (1 << 1);}
//#else if NF_FCE==1
#else
#define NF_nFCE_L()			{rNFCONT &= ~(1 << 2);}
#define NF_nFCE_H()			{rNFCONT |=  (1 << 2);}
#endif
*/

#define NF_RSTECC()			{rNFCONT |=  ((1<<5)|(1<<4));}

#define NF_MECC_UnLock()	{rNFCONT &= ~(1<<7);}
#define NF_MECC_Lock()		{rNFCONT |= (1<<7);}

#define NF_SECC_UnLock()	{rNFCONT &= ~(1<<6);}
#define NF_SECC_Lock()		{rNFCONT |= (1<<6);}

#define NF_CLEAR_RB()		{rNFSTAT |=  (1 << 4);}
//#define NF_DETECT_RB()		{while(!(rNFSTAT&(1<<4)));}
#define NF_DETECT_RB()		{ while((rNFSTAT&0x11)!=0x11);} // RnB_Transdetect & RnB
#define NF_WAITRB()		 {while (!(rNFSTAT & (1 << 0)));} 


#define NF_RDDATA() 	   (rNFDATA)
#define NF_RDDATA8() 	   (unsigned char)(rNFDATA8)
#define NF_RDDATA32() 	   (rNFDATA32)
#define NF_WRDATA(data) 	{rNFDATA=data;}

#define NF_RDMECC0()			(rNFMECC0)
#define NF_RDMECC1()			(rNFMECC1)

#define NF_RDMECCD0()			(rNFMECCD0)
#define NF_RDMECCD1()			(rNFMECCD1)

#define NF_WRMECCD0(data)			{rNFMECCD0 = (data);}
#define NF_WRMECCD1(data)			{rNFMECCD1 = (data);}


//#define ID_K9S1208V0M	  0xec76

// HCLK=100Mhz
#define TACLS		7	// 1-clk(0ns) 
#define TWRPH0		7	// 3-clk(25ns)
#define TWRPH1		7	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns



typedef enum {
    ECC_CORRECT_MAIN = 0,  // correct Main ECC
    ECC_CORRECT_SPARE1 = 1,  // correct Main ECC
    //ECC_CORRECT_SPARE2 = 2,  // correct Main ECC
    ECC_CORRECT_SPARE2 = 2  // correct Main ECC
} ECC_CORRECT_TYPE;


#define SB_NEED_EXT_ADDR				1
#define LB_NEED_EXT_ADDR				0


#define	EnNandFlash()	(rNFCONF |= 0x8000)
#define	DsNandFlash()	(rNFCONF &= ~0x8000)
#define	InitEcc()		(rNFCONF |= 0x1000)
#define	NoEcc()			(rNFCONF &= ~0x1000)
#define	NFChipEn()		(rNFCONF &= ~0x800)
#define	NFChipDs()		(rNFCONF |= 0x800)

#define	WrNFCmd(cmd)	(rNFCMD = (cmd))
#define	WrNFAddr(addr)	(rNFADDR = (addr))
#define	WrNFDat(dat)	(rNFDATA = (dat))
#define	WrNFDat8(dat)	(rNFDATA8 = (dat))
#define	RdNFDat()		(rNFDATA)
#define	RdNFStat()		(rNFSTAT)
#define	NFIsBusy()		(!(rNFSTAT&1))
#define	NFIsReady()		(rNFSTAT&1)

//#define	WIAT_BUSY_HARD	1
//#define	ER_BAD_BLK_TEST
//#define	WR_BAD_BLK_TEST

/*
#define CMD_READ                0x00        //  Read
#define CMD_READ1               0x01        //  Read1
#define CMD_READ2               0x50        //  Read2
#define CMD_READ3               0x30        //  Read3
#define CMD_WRITE0              0x80
#define CMD_WRITE1              0x10
#define CMD_RDO		0x05        //  Random Data Output
#define CMD_RDO2	0xE0        //  Random Data Output
#define CMD_ERASE0  0x60
#define CMD_ERASE1  0xD0
#define CMD_QUERY   0x70
*/

#define BADBLOCKMARK                0x00

#define CMD_READID			0x90		//  ReadID
#define CMD_READ			0x00		//  Read
#define CMD_READ2			0x50		//  Read2
#define CMD_READ3			0x30        //  Read3
#define CMD_RESET			0xff		//  Reset
#define CMD_ERASE			0x60		//  Erase phase 1
#define CMD_ERASE2			0xd0		//  Erase phase 2
#define CMD_WRITE			0x80		//  Write phase 1
#define CMD_WRITE2			0x10		//  Write phase 2
#define CMD_STATUS			0x70		//  STATUS
#define CMD_RDI				0x85        //  Random Data Input
#define CMD_RDO				0x05        //  Random Data Output
#define CMD_RDO2			0xE0        //  Random Data Output


#define	STATUS_ILLACC			(1<<5)		//	Illigar Access


//  Status bit pattern
#define STATUS_READY		0x40		//  Ready
#define STATUS_ERROR		0x01		//  Error


#define PAGES_PER_BLOCK	6			// Used to avoid multiplications
#define PAGES_IN_BLK	64


/////////////////////////////////////////////////////////////////
void __RdPage512(UINT8 *bufPt); 

/*
extern "C" void RdPage512(unsigned char *bufPt);
extern "C" void RdPage2048(unsigned char *bufPt);
extern "C" void RdPage512Unalign(unsigned char *bufPt);
extern "C" void WrPage512(unsigned char *bufPt); 
extern "C" void WrPage512Unalign(unsigned char *bufPt); 
extern "C" void WrPageInfo(PBYTE pBuff);
extern "C" void RdPageInfo(PBYTE pBuff);
extern "C" void RdPageSpare(PBYTE pBuff);
*/

DWORD ReadFlashID(void);
void NF_Reset(void);
void NF_Init(void);

BOOL ECC_CorrectData(unsigned int sectoraddr, UINT8* pData, UINT32 nRetEcc, ECC_CORRECT_TYPE nType);
//int NF_ReadPage(UINT32 block,UINT32 sector,UINT8 *buffer);
int NF_ReadPage(UINT32 sector,UINT8 *buffer);


U32 ReadPage(U32 BlkIndex, U32 PageIndex, U8 *buf);
//U32 WaitNFBusy(void);	// R/B 未接好?
U32 EraseBlock(U32 BlkIndex);
//static U32 WritePage(U32 addr, U8 *buf, U16 blk_idx);
U32 WritePage(U32 BlkIndex, U32 PageIndex, U8 *buf);

void MarkBadBlk(U32 BlkIndex);
int IsBadBlk(U32 BlkIndex);

void Check_Invalid_Block(void);
INT16U Logical_To_Physical_Block(INT16U LogicalBlock);
INT16U Physical_To_Logical_Block(INT16U PhysicalBlock);

INT8U Get_Invalid_Block(INT16U CurrentBlock,INT16U NextBlock,INT8U *ptr);


//////////////   090721  下载程序到NandFlash  //////////////////////////
U32 factorial(U8 num,U8 rank);
U32 GETVALUE(char *buf);
void K9f1g08_Menu(void);
void K9f1g08_Program(void);
int LoadImageFile(U8 *buf,int size);
void K9f1g08_PrintBlock(void);// Printf one page

#endif




