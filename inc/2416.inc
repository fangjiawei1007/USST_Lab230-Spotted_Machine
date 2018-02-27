;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;
;
; Use of this source code is subject to the terms of the Microsoft end-user
; license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
; If you did not accept the terms of the EULA, you are not authorized to use
; this source code. For a copy of the EULA, please see the LICENSE.RTF on your
; install media.
;
;-------------------------------------------------------------------------------
;
;  Header: s3c2450.inc
;
;  This header file defines only those registers required by the startup
;  code. All addresses are based off the physical addresses (PA) defined
;  in s3c2450_base_reg.h (s3c2450_base_reg.inc).
;
;-------------------------------------------------------------------------------

; Include the base register definitions

	INCLUDE 2416_base_regs.inc
	
	
BSP_SMDK2443 EQU 1
BSP_SMDK2450 EQU 2
BSP_TYPE EQU BSP_SMDK2450	
;------------------------------------------------------------------------------
; General CPU constants

Mode_USR         EQU        (0x10)
Mode_FIQ         EQU        (0x11)
Mode_IRQ         EQU        (0x12)
Mode_SVC         EQU        (0x13)
Mode_ABT         EQU        (0x17)
Mode_UND         EQU        (0x1B)
Mode_SYS         EQU        (0x1F)
I_Bit            EQU        (0x80)
F_Bit            EQU        (0x40)
R1_iA            EQU        (1<<31)
R1_nF            EQU        (1<<30)

;------------------------------------------------------------------------------
;  Miscellaneous defines

;WORD_SIZE       EQU         (4)
;DW8             EQU         (0x0)
;DW16            EQU         (0x1)
;DW32            EQU         (0x2)
;WAIT            EQU         (0x1<<2)
;UBLB            EQU         (0x1<<3)

;------------------------------------------------------------------------------
; MMU constants

MMU_CTL_MASK     EQU        (0x3FFF0000)
MMU_TTB_MASK     EQU        (0x00003FFF)
MMU_ID_MASK      EQU        (0xFFFFFFF0)

;------------------------------------------------------------------------------
;  Interrupt Control group 1 Registers
	[ BSP_TYPE = BSP_SMDK2443 	
INTMSK1          	EQU         (S3C2450_BASE_REG_PA_INTR        + 0x08)
INTSUBMSK       		EQU         (S3C2450_BASE_REG_PA_INTR        + 0x1C)
INTMOD1          	EQU         (S3C2450_BASE_REG_PA_INTR        + 0x04)
INTPND1						EQU	   		 	(S3C2450_BASE_REG_PA_INTR	     + 0x10)	
SRCPND1						EQU	    		(S3C2450_BASE_REG_PA_INTR	     + 0x00)	
;================== edit by zxs ====================================
INTOFFSET1  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x14)	;//Interrupt request source offset

;SRCPND     EQU        	(S3C2450_BASE_REG_PA_INTR	     + 0x00)	;//Interrupt request status
;INTMOD     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x04)	;//Interrupt mode control
;INTMSK     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x08)	;//Interrupt mask control
;PRIORITY   EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x0c)	;//IRQ priority control
;INTPND     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x10)	;//Interrupt request status
;INTOFFSET  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x14)	;//Interrupt request source offset
;SUBSRCPND  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x18)	;//Interrupt request status
;INTSUBMSK  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x1c)	;//Interrupt source mask
;===================================================================

vSRCPND		EQU	0xB0C00000	;Interrupt request status
vINTMOD		EQU	0xB0C00004	;Interrupt mode control
vINTMSK		EQU	0xB0C00008	;Interrupt mask control
vPRIORITY	EQU	0xB0C0000C	;IRQ priority control
vINTPND		EQU	0xB0C00010	;Interrupt request status
vINTOFFSET	EQU	0xB0C00014	;Interruot request source offset
vSUBSRCPND	EQU	0xB0C00018	;Sub source pending
vINTSUBMSK	EQU	0xB0C0001C	;Interrupt sub mask

vINTBASE	EQU	0xB0C00000	;Interrupt request status
oSRCPND		EQU	0x00			;Interrupt request status
oINTMSK		EQU	0x08			;Interrupt mask control
oINTPND		EQU	0x10			;Interrupt request status
oINTSUBMSK	EQU	0x1C			;Interrupt sub mask

	]
	[ BSP_TYPE = BSP_SMDK2450 
INTMSK1          	EQU         (S3C2450_BASE_REG_PA_INTR        + 0x08)
INTSUBMSK       		EQU         (S3C2450_BASE_REG_PA_INTR        + 0x1C)
INTMOD1          	EQU         (S3C2450_BASE_REG_PA_INTR        + 0x04)
INTPND1						EQU	   		 	(S3C2450_BASE_REG_PA_INTR	     + 0x10)	
SRCPND1						EQU	    		(S3C2450_BASE_REG_PA_INTR	     + 0x00)	
;================== edit by zxs ====================================
INTOFFSET1  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x14)	;//Interrupt request source offset

;SRCPND     EQU        	(S3C2450_BASE_REG_PA_INTR	     + 0x00)	;//Interrupt request status
;INTMOD     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x04)	;//Interrupt mode control
;INTMSK     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x08)	;//Interrupt mask control
;PRIORITY   EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x0c)	;//IRQ priority control
;INTPND     EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x10)	;//Interrupt request status
;INTOFFSET  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x14)	;//Interrupt request source offset
;SUBSRCPND  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x18)	;//Interrupt request status
;INTSUBMSK  EQU          (S3C2450_BASE_REG_PA_INTR	     + 0x1c)	;//Interrupt source mask
;===================================================================

vSRCPND1		EQU	0xB0C00000	;Interrupt request status
vINTMOD1		EQU	0xB0C00004	;Interrupt mode control
vINTMSK1		EQU	0xB0C00008	;Interrupt mask control
vPRIORITY_MODE1		EQU	0xB0C00030	;IRQ priority Mode 1
vPRIORITY_UPDATE1	EQU	0xB0C00034	;IRQ priority Update
vINTPND1		EQU	0xB0C00010	;Interrupt request status
vINTOFFSET1		EQU	0xB0C00014	;Interruot request source offset
vSUBSRCPND		EQU	0xB0C00018	;Sub source pending
vINTSUBMSK		EQU	0xB0C0001C	;Interrupt sub mask


vINTBASE1	 	EQU	0xB0C00000	;Interrupt request status
oSRCPND1		EQU	0x00			;Interrupt request status
oINTMSK1		EQU	0x08			;Interrupt mask control
oINTPND1		EQU	0x10			;Interrupt request status
oINTSUBMSK		EQU	0x1C			;Interrupt sub mask


;  Interrupt Control group 2 Registers


INTMSK2          EQU         (S3C2450_BASE_REG_PA_INTR        + 0x48)
INTMOD2          EQU         (S3C2450_BASE_REG_PA_INTR        + 0x44)
INTPND2		EQU	    (S3C2450_BASE_REG_PA_INTR	     + 0x50)	
SRCPND2		EQU	    (S3C2450_BASE_REG_PA_INTR	     + 0x40)


vSRCPND2		EQU	0xB0C00040	;Interrupt request status
vINTMOD2		EQU	0xB0C00044	;Interrupt mode control
vINTMSK2		EQU	0xB0C00048	;Interrupt mask control
vPRIORITY_MODE2		EQU	0xB0C00070	;IRQ priority Mode 1
vPRIORITY_UPDATE2	EQU	0xB0C00074	;IRQ priority Update
vINTPND2		EQU	0xB0C00050	;Interrupt request status
vINTOFFSET2		EQU	0xB0C00054	;Interruot request source offset


vINTBASE2 		EQU	0xB0C00040	;Interrupt request status
oSRCPND2		EQU	0x00			;Interrupt request status
oINTMSK2		EQU	0x08			;Interrupt mask control
oINTPND2		EQU	0x10			;Interrupt request status
	]
;------------------------------------------------------------------------------
;  IOPort Control Registers

GPACDL		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x00)
GPACDH		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x04)

GPBCON		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x10)
GPBDAT		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x14)
GPBUDP		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x18)
	[ BSP_TYPE = BSP_SMDK2443 	
	]

	[ BSP_TYPE = BSP_SMDK2450 
GPBSEL		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x1C)	
	]


GPFCON		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x50)
GPFDAT		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x54)
GPFUDP		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x58)

	[ BSP_TYPE = BSP_SMDK2443 	
GPGCON		EQU		(S3C2450_BASE_REG_PA_IOPORT	 + 0x60)
GPGDAT		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x64)
GPGUDP		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x68)
	]
	[ BSP_TYPE = BSP_SMDK2450 
	]

GPHCON		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x70)
GPHDAT		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x74)
GPHUDP		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x78)

GPKCON		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0xE0)
GPKDAT		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0xE4)
GPKUDP		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0xE8)

MISCCR		EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0x80)

	[ BSP_TYPE = BSP_SMDK2443 
GSTATUS3	EQU		(S3C2450_BASE_REG_PA_IOPORT  + 0xB8)
	]
	[ BSP_TYPE = BSP_SMDK2450 
	]


vGPFCON		EQU	0xB2100050	;Port F data
vGPFDAT		EQU	0xB2100054	;Port F data
vGPFUDP		EQU	0xB2100058	;Port F data

vEINTMASK	EQU	0xB21000A4
vEINTPEND	EQU	0xB21000A8

vGPIOBASE	EQU	0xB2100000	;Port A control
oGSTATUS3       EQU     0xB8

oGPFCON		EQU	0x50			;Port F control
oGPFDAT		EQU	0x54			;Port F data
oGPFUDP		EQU	0x58			;Pull-Down control F

vMISCCR		EQU	0xB2100080	;Miscellaneous control
;------------------------------------------------------------------------------

;  Watch Dog Control Registers

WTCON           EQU         (S3C2450_BASE_REG_PA_WATCHDOG    + 0x00)
WTDAT		   EQU		 (S3C2450_BASE_REG_PA_WATCHDOG	+ 0x04)
WTCNT		   EQU		 (S3C2450_BASE_REG_PA_WATCHDOG	+ 0x08)

vWTCON		EQU	0xB1E00000	  ;Watch-dog timer mode
vWTDAT		EQU	0xB1E00004	  ;Watch-dog timer data
vWTCNT		EQU	0xB1E00008	  ;Eatch-dog timer count
;------------------------------------------------------------------------------
;  Memory configuration Control Registers

BANKCFG    EQU           0x48000000	;;//Mobile DRAM configuration
BANKCON1    EQU           0x48000004	;;//Mobile DRAM control 
BANKCON2    EQU           0x48000008	;;//Mobile DRAM timing control 
BANKCON3    EQU           0x4800000C	;;//Mobile DRAM (E)MRS 
REFRESH    EQU           0x48000010	;;//Mobile DRAM refresh control
TIMEOUT    EQU           0x48000014	;;//Write Buffer Time out control 
vREFRESH	EQU	0xB0800010	;DRAM/SDRAM refresh

;//chapter1	EBI controller
EBICON   EQU           0x4E800008	;//Bank Configuration 




;//chapter4 SSMC
SMBIDCYR0   EQU           0x4F000000	;//Bank0 idle cycle control 
SMBIDCYR1   EQU           0x4F000020	;//Bank1 idle cycle control 
SMBIDCYR2   EQU           0x4F000040	;//Bank2 idle cycle control 
SMBIDCYR3   EQU           0x4F000060	;//Bank3 idle cycle control 
SMBIDCYR4   EQU           0x4F000080	;//Bank0 idle cycle control 
SMBIDCYR5   EQU           0x4F0000A0	;//Bank5 idle cycle control 
SMBWSTRDR0  EQU           0x4F000004	;//Bank0 read wait state control 
SMBWSTRDR1  EQU           0x4F000024	;//Bank1 read wait state control
SMBWSTRDR2  EQU           0x4F000044	;//Bank2 read wait state control
SMBWSTRDR3  EQU           0x4F000064	;//Bank3 read wait state control 
SMBWSTRDR4  EQU           0x4F000084	;//Bank4 read wait state control 
SMBWSTRDR5  EQU           0x4F0000A4	;//Bank5 read wait state control 
SMBWSTWRR0  EQU           0x4F000008	;//Bank0 write wait state control 
SMBWSTWRR1  EQU           0x4F000028	;//Bank1 write wait state control 
SMBWSTWRR2  EQU           0x4F000048	;//Bank2 write wait state control 
SMBWSTWRR3  EQU           0x4F000068	;//Bank3 write wait state control 
SMBWSTWRR4  EQU           0x4F000088	;//Bank4 write wait state control 
SMBWSTWRR5  EQU           0x4F0000A8	;//Bank5 write wait state control 
SMBWSTOENR0 EQU           0x4F00000C	;//Bank0 output enable assertion delay control 
SMBWSTOENR1 EQU           0x4F00002C	;//Bank1 output enable assertion delay control 
SMBWSTOENR2 EQU           0x4F00004C	;//Bank2 output enable assertion delay control
SMBWSTOENR3 EQU           0x4F00006C	;//Bank3 output enable assertion delay control 
SMBWSTOENR4 EQU           0x4F00008C	;//Bank4 output enable assertion delay control
SMBWSTOENR5 EQU           0x4F0000AC	;//Bank5 output enable assertion delay control 
SMBWSTWENR0 EQU           0x4F000010	;//Bank0 write enable assertion delay control 
SMBWSTWENR1 EQU           0x4F000030	;//Bank1 write enable assertion delay control 
SMBWSTWENR2 EQU           0x4F000050	;//Bank2 write enable assertion delay control 
SMBWSTWENR3 EQU           0x4F000070	;//Bank3 write enable assertion delay control 
SMBWSTWENR4 EQU           0x4F000090	;//Bank4 write enable assertion delay control 
SMBWSTWENR5 EQU           0x4F0000B0	;//Bank5 write enable assertion delay control 
SMBCR0      EQU           0x4F000014	;//Bank0 control 
SMBCR1      EQU           0x4F000034	;//Bank1 control 
SMBCR2      EQU           0x4F000054	;//Bank2 control 
SMBCR3      EQU           0x4F000074	;//Bank3 control 
SMBCR4      EQU           0x4F000094	;//Bank4 control 
SMBCR5      EQU           0x4F0000B4	;//Bank5 control 
SMBSR0      EQU           0x4F000018	;//Bank0 status 
SMBSR1      EQU           0x4F000038	;//Bank1 status 
SMBSR2      EQU           0x4F000058	;//Bank2 status 
SMBSR3      EQU           0x4F000078	;//Bank3 status 
SMBSR4      EQU           0x4F000098	;//Bank4 status 
SMBSR5      EQU           0x4F0000B8	;//Bank5 status 
SMBWSTBRDR0 EQU           0x4F00001C	;//Bank0 burst read wait delay control 
SMBWSTBRDR1 EQU           0x4F00003C	;//Bank1 burst read wait delay control 
SMBWSTBRDR2 EQU           0x4F00005C	;//Bank2 burst read wait delay control 
SMBWSTBRDR3 EQU           0x4F00007C	;//Bank3 burst read wait delay control 
SMBWSTBRDR4 EQU           0x4F00009C	;//Bank4 burst read wait delay control 
SMBWSTBRDR5 EQU           0x4F0000BC	;//Bank5 burst read wait delay control 
SSMCSR      EQU           0x4F000200	;//SROMC status 
SSMCCR      EQU           0x4F000204	;//SROMC control 

;------------------------------------------------------------------------------
;  Clock and Power Control Registers

LOCKCON0        EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x00)
LOCKCON1        EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x04)
MPLLCON         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x10)
EPLLCON         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x18)
HCLKCON         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x30)
PCLKCON         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x34)
SCLKCON         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x38)
CLKDIV0         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x24)
CLKDIV1         EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x28)
CLKSRC          EQU         (S3C2450_BASE_REG_PA_CLOCK_POWER + 0x20)
PWRMODE			EQU			(S3C2450_BASE_REG_PA_CLOCK_POWER + 0x40)

RSTSTAT     EQU           0x4C000068  		;//Reset status
WKUPSTAT    EQU           0x4C00006c  		;//Wakeup status
INFORM0     EQU           0x4C000070  		;//Sleep mode information 0
INFORM1     EQU           0x4C000074  		;//Sleep mode information 1
INFORM2     EQU           0x4C000078  		;//Sleep mode information 2
INFORM3     EQU           0x4C00007C  		;//Sleep mode information 3
RSTCON      EQU     	  0x4C000064        ;//Reset Control

vRSTSTAT     EQU           0xb1000068  		;//Reset status
vWKUPSTAT    EQU           0xb100006c  		;//Wakeup status
vINFORM0     EQU           0xb1000070  		;//Sleep mode information 0
vINFORM1     EQU           0xb1000074  		;//Sleep mode information 1
vINFORM2     EQU           0xb1000078  		;//Sleep mode information 2
vINFORM3     EQU           0xb100007C  		;//Sleep mode information 3

vHCLKCON	EQU	0xb1000030	;HCLK enable control
vPCLKCON	EQU	0xb1000034	;PCLK enable control
vSCLKCON	EQU	0xb1000038	;Special colck enable control

vCLKDIV0	EQU	0xb1000024      ;Clock divider control
vCLKDIV1	EQU	0xb1000028      ;Clock divider control


vRSTCON        	EQU     0xb1000064      ;Reset Control
vOSCSET        	EQU     0xb1000008      ;Osc Control
vPWRCFG        	EQU     0xb1000060      ;Power config Control
vPWRMODE        EQU     0xb1000040      ;Power Mode Control


;------------------------------------------------------------------------------
;  RTC Control Registers
	[ BSP_TYPE = BSP_SMDK2443 	
RTCCON		EQU			0x57000040		
RTCALM		EQU			0x57000050
ALMSEC		EQU			0x57000054	;Alarm second
ALMMIN		EQU			0x57000058	;Alarm minute
ALMHOUR		EQU			0x5700005c	;Alarm Hour
ALMDATE		EQU			0x57000060	;Alarm date  // edited by junon
ALMMON		EQU			0x57000064	;Alarm month
ALMYEAR		EQU			0x57000068	;Alarm year
BCDSEC		EQU			0x57000070	;BCD second
BCDMIN		EQU			0x57000074	;BCD minute
BCDHOUR		EQU			0x57000078	;BCD hour
BCDDATE		EQU			0x5700007c	;BCD date  //edited by junon
BCDDAY		EQU			0x57000080	;BCD day   //edited by junon
BCDMON		EQU			0x57000084	;BCD month
BCDYEAR		EQU			0x57000088	;BCD year	
	]
	[ BSP_TYPE = BSP_SMDK2450 
	]
;---------------------------------------
; nGCS0 = AMD Flash = Bank0 Controller Parameter setting
;---------------------------------------
	GBLL	AMDBOOT		; bacnk0, 16-bit and flash
;	GBLL	NANDBOOT		; bacnk0, 8-bit nand
					
	IF :DEF: AMDBOOT

IDCY0				EQU		0x0			;	Idle or turnaround cycles IDCY*HCLK
WSTRD0				EQU		0xe			;	Read wait state = tacc
WSTWR0				EQU		0xe			;	wrie wait state
WSTOEN0				EQU		0			; 	output enable assertion delay from CS
WSTWEN0				EQU		0			; 	write enable assertion delay

BlWriteEn			EQU		1			;bit21-SMBAA signal control:0-1at all times, 1 active for sync
AddrValidWriteEn	EQU		1			;bit20-SMADDRVALD during write:0-always high,1-active for write 	
BurstLenWrite		EQU		0			;bit1819-burst transfer length:0-4,1-8,3-continu(sync only) 	
SyncWriteDev		EQU		0			;bit17-0:async, 1:sync 	
BMWrite				EQU		0			;bit16-burt mode write : 0-non-burst, 1-burst 	
WrapRead			EQU		0			;bit14-0-disable, 1 enable
BlReadEn			EQU		1			;bit13-SMBAA signal :0-1 at all time, 1-active for sync read
AddrValidReadEn		EQU		1			;bit12-SMADDRVALID signal: 0-always HIGH, 1-active for async & sync read
BurstLenRead		EQU		0			;bit1011-burst transfer length:0-4,1-8,2-16,3-cont(sync only)
SyncReadDev			EQU		0			;bit9-sync access :0-async, 1-sync
BMRead				EQU		0			;bit8-burst mode red and async page mode
SMBLSPOL			EQU		0			;bit6-polarit of signal nSMBLS
MW					EQU		1			;bit45-memory width : 00-8bit,01-16bit,10-32bit
WP					EQU		0			;bit3-write protect
WaitEn				EQU		0			;bit2-external wait signal enable
WaitPol				EQU		0			;bit1-polarity of the external wait input for actiation
RBLE				EQU		0			;bit0-read byte lane enable
SMBCR0_0			EQU		((BMRead<<8)+(SMBLSPOL<<6)+(MW<<4)+(WP<<3)+(WaitEn<<2)+(WaitPol<<1)+RBLE)
SMBCR0_1			EQU		((WrapRead<<14)+(BlReadEn<<13)+(AddrValidReadEn<<12)+(BurstLenRead<<10)+(SyncReadDev<<9))
SMBCR0_2			EQU		((BlWriteEn<<21)+(AddrValidWriteEn<<20)+(BurstLenWrite<<18)+(SyncWriteDev<<17)+(BMWrite<<16))

WaitTourErr0			EQU		0			; external wait timeout error flag

WSTBRD0				EQU		0x1f			; burst read wait state

MemClkRatio			EQU		1			; SMMEMCLK :0-HCLK,1-HCLK/2,2-HCLK/3
SMClockEn			EQU		1			; SMCLK enable 0-only active during mem access,1-always running	

	ELSE	; NAND Boot....
IDCY0				EQU		0x0			;	Idle or turnaround cycles IDCY*HCLK
WSTRD0				EQU		0xe			;	Read wait state = tacc
WSTWR0				EQU		0xe			;	wrie wait state
WSTOEN0				EQU		0			; 	output enable assertion delay from CS
WSTWEN0				EQU		0			; 	write enable assertion delay

BlWriteEn			EQU		1			;bit21-SMBAA signal control:0-1at all times, 1 active for sync
AddrValidWriteEn	EQU		1			;bit20-SMADDRVALD during write:0-always high,1-active for write 	
BurstLenWrite		EQU		0			;bit1819-burst transfer length:0-4,1-8,3-continu(sync only) 	
SyncWriteDev		EQU		0			;bit17-0:async, 1:sync 	
BMWrite				EQU		0			;bit16-burt mode write : 0-non-burst, 1-burst 	
WrapRead			EQU		0			;bit14-0-disable, 1 enable
BlReadEn			EQU		1			;bit13-SMBAA signal :0-1 at all time, 1-active for sync read
AddrValidReadEn		EQU		1			;bit12-SMADDRVALID signal: 0-always HIGH, 1-active for async & sync read
BurstLenRead		EQU		0			;bit1011-burst transfer length:0-4,1-8,2-16,3-cont(sync only)
SyncReadDev			EQU		0			;bit9-sync access :0-async, 1-sync
BMRead				EQU		0			;bit8-burst mode red and async page mode
SMBLSPOL			EQU		0			;bit6-polarit of signal nSMBLS
MW					EQU		0			;bit45-memory width : 00-8bit,01-16bit,10-32bit
WP					EQU		0			;bit3-write protect
WaitEn				EQU		0			;bit2-external wait signal enable
WaitPol				EQU		0			;bit1-polarity of the external wait input for actiation
RBLE				EQU		0			;bit0-read byte lane enable
SMBCR0_0			EQU		((BMRead<<8)+(SMBLSPOL<<6)+(MW<<4)+(WP<<3)+(WaitEn<<2)+(WaitPol<<1)+RBLE)
SMBCR0_1			EQU		((WrapRead<<14)+(BlReadEn<<13)+(AddrValidReadEn<<12)+(BurstLenRead<<10)+(SyncReadDev<<9))
SMBCR0_2			EQU		((BlWriteEn<<21)+(AddrValidWriteEn<<20)+(BurstLenWrite<<18)+(SyncWriteDev<<17)+(BMWrite<<16))

WaitTourErr0			EQU		0			; external wait timeout error flag

WSTBRD0				EQU		0x1f			; burst read wait state

MemClkRatio			EQU		1			; SMMEMCLK :0-HCLK,1-HCLK/2,2-HCLK/3
SMClockEn			EQU		1			; SMCLK enable 0-only active during mem access,1-always running	
    ENDIF ;	IF :DEF: AMDBOOT
;#########################################################################################




;#########################################################################################

_ISR_STARTADDRESS	EQU		0x33ffff00
top_of_stacks		EQU		_ISR_STARTADDRESS

;=====================================================================================
;	(42,1,1)=200Mhz, (47,1,1)=220Mhz, (72,2,1)=240Mhz, (57,1,1)=260Mhz, (125,4,1)=266Mhz
;   (43,1,1)=204Mhz,
;	(62,1,1)=280Mhz, (67,1,1)=300Mhz, (72,4,0)=320Mhz, (63,3,0)=340Mhz, (52,2,0)=360Mhz
;   (42,1,0)=400Mhz,

; ========================================================================
; Decide Re-setting the PLL value and mem setting in Kernel.
; If 1, In Kernel the PLL will set PLL and Memory setting again.
; If 0, The kernel will not change PLL and memory, 
;           so The setting in Eboot will be used.
	GBLA	CHANGE_CLK_EBOOT
	GBLA	CHANGE_CLK_OAL	
CHANGE_CLK_EBOOT	SETA	1	; 1:RE-Setting PLL value(refer startup.s in Eboot)
CHANGE_CLK_OAL		SETA	1	; 1:RE-Setting PLL value(refer startup.s in oal)


; ========================================================================
; PLL Value setting
; EBOOT and Kernel refers this value.
;=====================================================================================
;	Fin = 12MHz,
;
;	MPLLout = (2m x Fin)/(p x 2**s), m=MDIV+8, p=PDIV, s=SDIV, Fin=10~30MHz
;	(17,1,1)=300Mhz, (92,3,1)=400Mhz, (67,2,1)=450Mhz, (81,2,1)=534Mhz, 
;	(17,1,0)=600Mhz, (92,3,0)=800Mhz
;
;	EPLLout = (m x Fin)/(p x 2**s), m=MDIV+8, p=PDIV+2, s=SDIV, Fin=10~100MHz
;	(28,1,2)=36Mhz, (40,1,2)=48Mhz, (22,1,1)=60Mhz, (28,1,1)=72Mhz, (34,1,1)=84Mhz
;	(40,1,1)=96Mhz
;=====================================================================================
	GBLA	CLKVAL
	GBLA	DVSON		
	GBLA	HCLKVAL
	
	[ BSP_TYPE = BSP_SMDK2443 	
CLKVAL	SETA	533
	]
	[ BSP_TYPE = BSP_SMDK2450 
;CLKVAL	SETA	533
CLKVAL	SETA	400133
;CLKVAL SETA	266
;CLKVAL	SETA	400
	]


	[ BSP_TYPE = BSP_SMDK2443 
	[ CLKVAL = 300
DVSON	SETA	0	
HCLKVAL	SETA	100
Startup_MPLL		EQU	300000000
Startup_Mdiv		EQU 	17
Startup_Pdiv		EQU 	1
Startup_Sdiv		EQU 	0
Startup_ARMCLKdiv	EQU		8			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x2			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]
	
	[ CLKVAL = 400
DVSON	SETA	0	
HCLKVAL	SETA	100
Startup_MPLL		EQU	400000000
Startup_Mdiv		EQU 	92
Startup_Pdiv		EQU 	3
Startup_Sdiv		EQU 	0

Startup_ARMCLKdiv	EQU		8			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x3			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]
	
	[ CLKVAL = 400133
DVSON	SETA	0	
HCLKVAL	SETA	133
Startup_MPLL		EQU	400000000
Startup_Mdiv		EQU 	92
Startup_Pdiv		EQU 	3
Startup_Sdiv		EQU 	0

Startup_ARMCLKdiv	EQU		8			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x2			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x2 : HCLK  = PREDIV_CLK/3
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]		
	
	[ CLKVAL = 533
DVSON	SETA	0	
HCLKVAL	SETA	133
Startup_MPLL		EQU	533000000
Startup_Mdiv		EQU 	81
Startup_Pdiv		EQU 	2 
Startup_Sdiv		EQU 	1

Startup_ARMCLKdiv	EQU		0			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x1			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]
	
	[ CLKVAL = 500
DVSON	SETA	0	
HCLKVAL	SETA	120
Startup_MPLL		EQU	500000000
Startup_Mdiv		EQU 	34
Startup_Pdiv		EQU 	1
Startup_Sdiv		EQU 	0

Startup_ARMCLKdiv	EQU		8			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x3			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]	
Startup_EMdiv		EQU 	40			; 96Mhz
Startup_EPdiv		EQU     1	
Startup_ESdiv		EQU 	1
	]
	[ BSP_TYPE = BSP_SMDK2450 
	[ CLKVAL = 36
DVSON	SETA	0	
HCLKVAL	SETA	18
Startup_MPLL		EQU	36000000
Startup_Mdiv		EQU 	24
Startup_Pdiv		EQU 	4
Startup_Sdiv		EQU 	1
Startup_ARMCLKdiv	EQU		0			;	0	 :	ARMCLK	= MPLL/1
								;	1	 :	ARMCLK	= MPLL/2
								;	2	 :	ARMCLK	= MPLL/3
								;	3	 :	ARMCLK	= MPLL/4
								;	5	 :	ARMCLK	= MPLL/6
								;	7	 :	ARMCLK	= MPLL/8
								;	13	 :	ARMCLK	= MPLL/12
								;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HALFHCLKdiv	EQU		1			;	0	: HCLKx1_2(SSMC) = HCLK
								;	1	: HCLKx1_2(SSMC) = HCLK/2
Startup_PCLKdiv		EQU		0			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2								
Startup_HCLKdiv		EQU		1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4
	]
	[ CLKVAL = 400133 ;800
DVSON	SETA	0	
HCLKVAL	SETA	133
Startup_MPLL		EQU	800000000		;	800000000
Startup_Mdiv		EQU 	400
Startup_Pdiv		EQU 	3
Startup_Sdiv		EQU 	1			;	1


Startup_ARMCLKdiv	EQU	1			;	0	 :	ARMCLK	= MPLL/1
							;	1	 :	ARMCLK	= MPLL/2
							;	2	 :	ARMCLK	= MPLL/3
							;	3	 :	ARMCLK	= MPLL/4
							;	5	 :	ARMCLK	= MPLL/6
							;	7	 :	ARMCLK	= MPLL/8
							;	13	 :	ARMCLK	= MPLL/12
							;	15	 :	ARMCLK	= MPLL/16

Startup_PREdiv		EQU	2			;	0  :	PREDIV_CLK	= MPLL
							;	1 :	PREDIV_CLK	= MPLL/2
							;	2 :	PREDIV_CLK	= MPLL/3
							;	3 :	PREDIV_CLK	= MPLL/4
										
							;  	PREdiv   HCLKdiv
Startup_HCLKdiv		EQU	1			;	0  :	HCLK	= PREDIV_CLK
							;	0x1 :	HCLK	= PREDIV_CLK/2
							;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU	1			;	0	 :	PCLK	= HCLK
							;	1	 :	PCLK	= HCLK/2
	]
	[ CLKVAL = 400 ;400/100
DVSON	SETA	0	
HCLKVAL	SETA	100
Startup_MPLL		EQU	800000000
Startup_Mdiv		EQU 	400
Startup_Pdiv		EQU 	3
Startup_Sdiv		EQU 	1


Startup_ARMCLKdiv	EQU	1			;	0	 :	ARMCLK	= MPLL/1
							;	1	 :	ARMCLK	= MPLL/2
							;	2	 :	ARMCLK	= MPLL/3
							;	3	 :	ARMCLK	= MPLL/4
							;	5	 :	ARMCLK	= MPLL/6
							;	7	 :	ARMCLK	= MPLL/8
							;	13	 :	ARMCLK	= MPLL/12
							;	15	 :	ARMCLK	= MPLL/16

Startup_PREdiv		EQU	3			;	0  :	PREDIV_CLK	= MPLL
							;	1 :	PREDIV_CLK	= MPLL/2
							;	2 :	PREDIV_CLK	= MPLL/3
							;	3 :	PREDIV_CLK	= MPLL/4
										
							;  	PREdiv   HCLKdiv
Startup_HCLKdiv		EQU	1			;	0  :	HCLK	= PREDIV_CLK
							;	0x1 :	HCLK	= PREDIV_CLK/2
							;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU	1			;	0	 :	PCLK	= HCLK
							;	1	 :	PCLK	= HCLK/2
	]
	[ CLKVAL = 266 ;266/133
DVSON	SETA	0	
HCLKVAL	SETA	133
Startup_MPLL		EQU	533000000
Startup_Mdiv		EQU 	355
Startup_Pdiv		EQU 	4
Startup_Sdiv		EQU 	1


Startup_ARMCLKdiv	EQU	1			;	0	 :	ARMCLK	= MPLL/1
							;	1	 :	ARMCLK	= MPLL/2
							;	2	 :	ARMCLK	= MPLL/3
							;	3	 :	ARMCLK	= MPLL/4
							;	5	 :	ARMCLK	= MPLL/6
							;	7	 :	ARMCLK	= MPLL/8
							;	13	 :	ARMCLK	= MPLL/12
							;	15	 :	ARMCLK	= MPLL/16

Startup_PREdiv		EQU	1			;	0  :	PREDIV_CLK	= MPLL
							;	1 :	PREDIV_CLK	= MPLL/2
							;	2 :	PREDIV_CLK	= MPLL/3
							;	3 :	PREDIV_CLK	= MPLL/4
										
							;  	PREdiv   HCLKdiv
Startup_HCLKdiv		EQU	1			;	0  :	HCLK	= PREDIV_CLK
							;	0x1 :	HCLK	= PREDIV_CLK/2
							;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU	1			;	0	 :	PCLK	= HCLK
							;	1	 :	PCLK	= HCLK/2
	]	
	[ CLKVAL = 533
DVSON	SETA	0	
HCLKVAL	SETA	133
Startup_MPLL		EQU	533000000
Startup_Mdiv		EQU 	355
Startup_Pdiv		EQU 	4 
Startup_Sdiv		EQU 	1

Startup_ARMCLKdiv	EQU		0			;	0	 :	ARMCLK	= MPLL/1
										;	8	 :	ARMCLK	= MPLL/2
										;	2	 :	ARMCLK	= MPLL/3
										;	9	 :	ARMCLK	= MPLL/4
										;	10	 :	ARMCLK	= MPLL/6
										;	11	 :	ARMCLK	= MPLL/8
										;	13	 :	ARMCLK	= MPLL/12
										;	15	 :	ARMCLK	= MPLL/16
Startup_PREdiv		EQU		0x1			;	0x0  :	PREDIV_CLK	= MPLL
										;	0x1 :	PREDIV_CLK	= MPLL/2
										;	0x2 :	PREDIV_CLK	= MPLL/3
										;	0x3 :	PREDIV_CLK	= MPLL/4
Startup_HCLKdiv		EQU		0x1			;	0x0  :	HCLK	= PREDIV_CLK
										;	0x1 :	HCLK	= PREDIV_CLK/2
										;	0x3 :	HCLK	= PREDIV_CLK/4

Startup_PCLKdiv		EQU		1			;	0	 :	PCLK	= HCLK
										;	1	 :	PCLK	= HCLK/2
	]
	
Startup_EMdiv		EQU 	32			; EPLL -> 96Mhz
Startup_EPdiv		EQU   1
Startup_ESdiv		EQU 	2			
	]


; EBICON(0x4E80_0008) setting
;[31:11] Reserved
;[10] BANK3 0:SROM, 1:CF
;[9] BANK2 0:SROM, 1:CF
;[8] BANK1 0:SROM, 1:NAND
	GBLA    EBICON_VAL
;EBICON_VAL	SETA	((1<<7)+(1<<6)+(1<<5)+(0<<4)+(0<<3)+(1<<2)+(0<<1)+(0<<0))	; BANK[7:2]=SDRAM/SDRAM/RAM/CF/NAND/SRAM
EBICON_VAL	SETA	((0<<10)+(0<<9)+(1<<8)+(1<<2)+(0<<1)+(0<<0))	; BANK[7:2]=SDRAM/SDRAM/RAM/CF/NAND/SRAM
;======================================================================================
;	MEM config	[SDR, DDR]
;======================================================================================

	[ BSP_TYPE = BSP_SMDK2443 	
	;GBLL	mDDR		; 256Mbit x16 x2ea = 512Mbit mem size
	GBLL	mSDR		; 256Mbit x16 x2ea = 512Mbit mem size	
	]
	[ BSP_TYPE = BSP_SMDK2450 
	;GBLL	mSDR		; 256Mbit x16 x2ea = 512Mbit mem size
	;GBLL	mDDR		; 256Mbit x16 x2ea = 512Mbit mem size
	GBLL	DDR2		; 256Mbit x16 x2ea = 512Mbit mem size
	]


	[ BSP_TYPE = BSP_SMDK2443 	
	IF :DEF: mSDR
;---------------------------------------
; BANKCFG register  : DRAM configure
;---------------------------------------
RASBW0				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
RASBW1				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
CASBW0				EQU		1			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
CASBW1				EQU		1			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
ADDRCFG0			EQU		1			; 	addre configure
          		                        ;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
ADDRCFG1			EQU		0			; 	addre configure
          		                        ;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
MEMCFG				EQU		1			; 	Ext.Mem 00=SDR,01=MSDR,10=DDR, 11=MDDR
BW					EQU		0			; 	Bus width 00=32bit,01=16bit

;---------------------------------------
; BANKCON1 register : DRAM timing control
;---------------------------------------
DQS					EQU		0			;	[30:28] 000b is the best. 
Reserved0		EQU		1			;	[27:26]	01b (should be set)
BStop				EQU		0			;	read burst stop control
WBUF				EQU		1			;	write buffer control
AP					EQU		1			;	auto precharge control
PWRDN				EQU		0			;	power down mode
BANKINIT			EQU		0    ;3	 		;	DRAM initialize control

;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
;tRAS				EQU		5			;	Row active time
;tRC					EQU		7			;	Row cycle time
;CL					EQU		3			;	CAS latency control
;tRCD				EQU		2			;	RAS to CAS delay
;tRP					EQU		2			; 	Row pre-charge time

;---------------------------------------
; BANKCON3 register : MRS/EMRS register
;---------------------------------------
BA_EMRS				EQU		2			;	BA : EMRS
DS					EQU		0   ;2			;	Driver strength
PASR				EQU		0			;	PASR
BA_MRS				EQU		0			;	BA : MRS
TM					EQU		0			; 	Test Mode - mode register set
CL_MRS				EQU		3			; 	CAS Latency

;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
;REFCYC				EQU		787			; 	refresh cycle
										;  	100MHz=787, 133MHz=1053


	ELSE


;---------------------------------------
; BANKCFG register  : DRAM configure
;---------------------------------------
RASBW0				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
RASBW1				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
CASBW0				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
CASBW1				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
ADDRCFG0			EQU		1			; 	addre configure
          		                        ;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
ADDRCFG1			EQU		0			; 	addre configure
          		                        ;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
MEMCFG				EQU		3			; 	Ext.Mem 00=SDR,01=MSDR,10=DDR, 11=MDDR
BW					EQU		1			; 	Bus width 00=32bit,01=16bit

;---------------------------------------
; BANKCON1 register : DRAM timing control
;---------------------------------------
DQS					EQU		0		;4	;	[30:28] 000b is the best. 
Reserved0		EQU		1			;	[27:26]	01b (should be set)
BStop				EQU		0			;	read burst stop control
WBUF				EQU		1			;	write buffer control
AP					EQU		1			;	auto precharge control
PWRDN				EQU		0			;	power down mode
BANKINIT			EQU		0    ; 3			;	DRAM initialize control

;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
;tRAS				EQU		5			;	Row active time
;tRC					EQU		7			;	Row cycle time
;CL					EQU		3			;	CAS latency control
;tRCD				EQU		2			;	RAS to CAS delay
;tRP					EQU		2			; 	Row pre-charge time

;---------------------------------------
; BANKCON3 register : MRS/EMRS register
;---------------------------------------
BA_EMRS				EQU		2			;	BA : EMRS
DS					EQU		0   ;3			;	Driver strength
PASR				EQU		0			;	PASR
BA_MRS				EQU		0			;	BA : MRS
TM					EQU		0			; 	Test Mode - mode register set
CL_MRS				EQU		3			; 	CAS Latency

;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
;REFCYC				EQU		787			; 	refresh cycle
										;  	100MHz=787, 133MHz=1053
;---------------------------------------------------------------------------
					
	ENDIF	
	[ HCLKVAL = 133
;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
tRAS				EQU		6			;	Row active time
tRC					EQU		9			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
REFCYC				EQU		1037			; 	refresh cycle
	|
	[ HCLKVAL = 120
;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
tRAS				EQU		5			;	Row active time
tRC					EQU		8			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
REFCYC				EQU		936			; 	refresh cycle
	|
	[ HCLKVAL = 110
;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
tRAS				EQU		5			;	Row active time
tRC					EQU		7			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
REFCYC				EQU		858			; 	refresh cycle	
	|
	[ HCLKVAL = 100
;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
tRAS				EQU		4			;	Row active time
tRC					EQU		7			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
REFCYC				EQU		780			; 	refresh cycle
	|
;---------------------------------------
; BANKCON2 register : DRAM timing control
;---------------------------------------
tRAS				EQU		4			;	Row active time
tRC					EQU		7			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
;---------------------------------------
; REFRESH register : refresh register
;---------------------------------------
REFCYC				EQU		514			; 	refresh cycle	
	]
	]
	]	
	]
	
	]

	[ BSP_TYPE = BSP_SMDK2450 
	IF :DEF: mSDR

;---------------------------------------
; BANKCFG register  : SDRAM configure
;---------------------------------------
RASBW0				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
RASBW1				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
CASBW0				EQU		1			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
CASBW1				EQU		1			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
ADDRCFG0			EQU		1			; 	addre configure
												;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
ADDRCFG1			EQU		1			; 	addre configure
												;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
MEMCFG				EQU		2			; 	Ext.Mem 000=SDR,010=MSDR,100=DDRz,110=MDDR,001=DDR2
BW				EQU		0			; 	Bus width 00=32bit,01=16bit

;---------------------------------------
; BANKCON1 register : SDRAM timing control
;---------------------------------------
BStop				EQU		0			;	read burst stop control
WBUF				EQU		1			;	write buffer control
;;AP				EQU		1			;	auto precharge control
;;PWRDN				EQU		0			;	power down mode
AP				EQU		0			;	auto precharge control
PWRDN				EQU		1			;	power down mode
DQSDelay			EQU		4			;	DQS delay

;-----------------------[SDRAM]-------------------------------
;    HCLK		tRAS		tARFC		CAS		tRCD		RP		REFCYC
;---------------------------------------------------------------
;18MHz			0		1		3		0		0		140
;36MHz			1		2		3		0		0		280
; 100Mhz		5		7		3		1		1		780
; 110Mhz		5		7		3		2		2		858
; 120Mhz		6		7		3		2		2		936
; 133Mhz		6		10		3		2		2		1037
; 133.5Mhz		6		10		3		2		2		1041
; 135Mhz		6		10		3		2		2		1053
; 136Mhz		6		10		3		2		2		1060
; 136.5Mhz		6		10		3		2		2		1064
; 137.3Mhz		6		10		3		2		2		1070
; 138Mhz		6		10		3		2		2		1076
; 138.67Mhz		6		10		3		2		2		1081
; 140Mhz		6		11		3		3		3		1092 (out of spec)
; 141Mhz		6		11		3		3		3		1099
; 142Mhz		7		11		3		3		3		1107 (out of spec)
; 143.6Mhz		7		11		3		3		3		1120 (out of spec)		
;---------------------------------------------------------------
;---------------------------------------
; BANKCON2 register : SDRAM timing control 
;---------------------------------------

	[ HCLKVAL = 18
tRAS					EQU		4			;	Row active time
tARFC					EQU		6			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD					EQU		1			;	RAS to CAS delay
tRP					EQU		1			; 	Row pre-charge time
	]
	[ HCLKVAL = 133
tRAS					EQU		6			;	Row active time
tARFC					EQU		10			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD					EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
	]
	[ HCLKVAL = 100
tRAS					EQU		5			;	Row active time
tARFC					EQU		7			;	Row cycle time
CL					EQU		3			;	CAS latency control
tRCD					EQU		2			;	RAS to CAS delay
tRP					EQU		2			; 	Row pre-charge time
	]
;---------------------------------------
; BANKCON3 register : SDRAM MRS/EMRS register
;---------------------------------------
BA_EMRS				EQU		2			;	BA : EMRS
DS				EQU		0			;	Driver strength
PASR				EQU		0			;	PASR
BA_MRS				EQU		0			;	BA : MRS
TM				EQU		0			; 	Test Mode - mode register set
CL_MRS				EQU		3			; 	CAS Latency

;---------------------------------------
; REFRESH register : SDRAM refresh register

	[ HCLKVAL = 18
REFCYC				EQU		140		; 	refresh cycle
	]
	[ HCLKVAL = 133
REFCYC				EQU		1037		; 	refresh cycle
	]
	[ HCLKVAL = 100
REFCYC				EQU		780		; 	refresh cycle
	]	
	ENDIF

	IF :DEF: mDDR

;---------------------------------------
; BANKCFG register  : mDDR SDRAM configure
;---------------------------------------
RASBW0				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
RASBW1				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
CASBW0				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
CASBW1				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
ADDRCFG0			EQU		1			; 	addre configure
									;   	00={BA,RAS,CAS}, 01={RAS,BA,CAS}
ADDRCFG1			EQU		1			; 	addre configure
												;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
MEMCFG				EQU		6			; 	Ext.Mem 000=SDR,010=MSDR,100=DDRz,110=MDDR,001=DDR2
BW				EQU		1			; 	Bus width 00=32bit,01=16bit

;---------------------------------------
; BANKCON1 register : mDDR SDRAM timing control
;---------------------------------------
BStop				EQU		0			;	read burst stop control
WBUF				EQU		1			;	write buffer control
;;AP				EQU		1			;	auto precharge control
;;PWRDN				EQU		1			;	power down mode
AP				EQU		0			;	auto precharge control
PWRDN				EQU		1			;	power down mode
DQSDelay			EQU		4			;	DQS delay

;-----------------------[mDDR SDRAM]--------------------------
;    HCLK		tRAS		tARFC		CAS		tRCD		tRP		REFCYC
;---------------------------------------------------------------
;18MHz			0		1		3		0		0		140
;36MHz			1		2		3		0		0		280
;100Mhz			4		7		3		2		2		780
;110Mhz			4		8		3		2		2		858
;120Mhz			5		9		3		2		2		936
;133Mhz			5		10		3		2		2		1037		
;---------------------------------------------------------------

;---------------------------------------
; BANKCON2 register : mDDR SDRAM timing control
;---------------------------------------
	[ HCLKVAL = 18
tRAS				EQU		0			;	Row active time
tARFC				EQU		1			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		0			;	RAS to CAS delay
tRP				EQU		0			; 	Row pre-charge time
	]
	[ HCLKVAL = 133
tRAS				EQU		5			;	Row active time
tARFC				EQU		10			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP				EQU		2			; 	Row pre-charge time
	]
	[ HCLKVAL = 100
tRAS				EQU		4			;	Row active time
tARFC				EQU		7			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		2			;	RAS to CAS delay
tRP				EQU		2			; 	Row pre-charge time
	]	
;---------------------------------------
; BANKCON3 register : mDDR SDRAM MRS/EMRS register
;---------------------------------------
BA_EMRS				EQU		2			;	BA : EMRS
DS				EQU		0			;	Driver strength
PASR				EQU		0			;	PASR
BA_MRS				EQU		0			;	BA : MRS
TM				EQU		0			; 	Test Mode - mode register set
CL_MRS				EQU		3			; 	CAS Latency

;---------------------------------------
; REFRESH register : mDDR SDRAM refresh register
;--------------------------------------
	[ HCLKVAL = 18
REFCYC				EQU		140			; 	refresh cycle
	]
	[ HCLKVAL = 133
REFCYC				EQU		1037			; 	refresh cycle
	]
	[ HCLKVAL = 100
REFCYC				EQU		780			; 	refresh cycle
	]	
;---------------------------------------------------------------------------

	ENDIF
	IF :DEF: DDR2

;---------------------------------------
; BANKCFG register  : DDR2 SDRAM configure
;---------------------------------------
RASBW0				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
RASBW1				EQU		2			;	RAS addr 00=11bit,01-12bit,10=13bit, 11=14bit
CASBW0				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
CASBW1				EQU		2			;	CAS addr 00=8bit,01-9bit,10=10bit, 11=11bit
ADDRCFG0			EQU		1			; 	addre configure
												;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
ADDRCFG1			EQU		1			; 	addre configure
												;   00={BA,RAS,CAS}, 01={RAS,BA,CAS}
MEMCFG				EQU		1			; 	Ext.Mem 000=SDR,010=MSDR,100=DDRz,110=MDDR,001=DDR2
BW				EQU		1			; 	Bus width 00=32bit,01=16bit


;---------------------------------------
; BANKCON1 register : DDR2 SDRAM timing control
;---------------------------------------
PADLOAD				EQU		1
BStop				EQU		0			;	read burst stop control
WBUF				EQU		1			;	write buffer control
AP				EQU		0			;	auto precharge control
;;PWRDN				EQU		0			;	power down mode
PWRDN				EQU		1			;	power down mode
DQSDelay			EQU		4			;	DQS delay

;-----------------------[DDR2 SDRAM]--------------------------
;    HCLK		tRAS		tARFC		CAS		tRCD		tRP		REFCYC
;---------------------------------------------------------------
;18MHz			0		1		3		0		0		140
;36MHz			1		3		3		1		1		280
;100Mhz			4		10		3		1		1		780
;110Mhz			4		11		3		1		1		858
;120Mhz			4		12		3		1		1		936
;133Mhz			5		13		3		1		1		1037		
;---------------------------------------------------------------

;---------------------------------------
; BANKCON2 register : DDR2 SDRAM timing control
;---------------------------------------
	[ HCLKVAL = 18
tRAS				EQU		0			;	Row active time
tARFC				EQU		1			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		0			;	RAS to CAS delay
tRP				EQU		0			; 	Row pre-charge time
	]
	[ HCLKVAL = 133
tRAS				EQU		5			;	Row active time
tARFC				EQU		13			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		1			;	RAS to CAS delay
tRP				EQU		1			; 	Row pre-charge time
	]
	[ HCLKVAL = 100
tRAS				EQU		4			;	Row active time
tARFC				EQU		10			;	Row cycle time
CL				EQU		3			;	CAS latency control
tRCD				EQU		1			;	RAS to CAS delay
tRP				EQU		1			; 	Row pre-charge time
	]
;---------------------------------------
; BANKCON3 register : DDR2 SDRAM MRS/EMRS register
;---------------------------------------
BA_EMRS1			EQU		1			;	BA : EMRS
DLL_DIS				EQU		1
DLL_EN				EQU		0
nDQS_DIS			EQU		1
RDQS_DIS			EQU		0
OCD_MODE_EXIT			EQU		0
OCD_MODE_DEFAULT		EQU		7
BA_EMRS2			EQU		2			;	BA : EMRS
BA_EMRS3			EQU		3			;	BA : EMRS
DS				EQU		0			;	Driver strength
PASR				EQU		0			;	PASR
BA_MRS				EQU		0			;	BA : MRS
TM				EQU		0			; 	Test Mode - mode register set
CL_MRS				EQU		3			; 	CAS Latency
DLL_RESET_HIGH			EQU		1
DLL_RESET_LOW			EQU		0

;---------------------------------------
; REFRESH register : DDR2 SDRAM refresh register
;---------------------------------------
	[ HCLKVAL = 18
REFCYC				EQU		140			; 	refresh cycle
	]
	[ HCLKVAL = 133
REFCYC				EQU		1037			; 	refresh cycle
	]
	[ HCLKVAL = 100
REFCYC				EQU		780			; 	refresh cycle
	]	
;---------------------------------------------------------------------------
					
	ENDIF	
	]	

	[ BSP_TYPE = BSP_SMDK2450 
BANKCFGVAL   EQU		((RASBW0<<17)+(RASBW1<<14)+(CASBW0<<11)+(CASBW1<<8)+(ADDRCFG0<<6)+(ADDRCFG1<<4)+(MEMCFG<<1)+(BW<<0))
BANKCON1VAL  EQU		((DQSDelay<<28)+(1<<26)+(BStop<<7)+(WBUF<<6)+(AP<<5)+(PWRDN<<4))
BANKCON2VAL  EQU		((tRAS<<20)+(tARFC<<16)+(CL<<4)+(tRCD<<2)+(tRP<<0))
	IF :DEF: mSDR
BANKCON3VAL  EQU		((BA_EMRS<<30)+(DS<<21)+(PASR<<16)+(BA_MRS<<14)+(TM<<7)+(CL_MRS<<4))	
	ENDIF
	IF :DEF: mDDR
BANKCON3VAL  EQU		((BA_EMRS<<30)+(DS<<21)+(PASR<<16)+(BA_MRS<<14)+(TM<<7)+(CL_MRS<<4))	
	ENDIF
	]
	
;--------------------------------------------------------------
;nand--
NFCONF		  EQU           0x4E000000		  ;//NAND Flash configuration


	END

