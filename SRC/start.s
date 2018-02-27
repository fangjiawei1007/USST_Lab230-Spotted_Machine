;=========================================
; NAME: 2410INIT.S
; DESC: C start up codes
;       Configure memory, ISR ,stacks
;	Initialize C-variables
; HISTORY:
; 2002.02.25:kwtark: ver 0.0
; 2002.03.20:purnnamu: Add some functions for testing STOP,POWER_OFF mode
;=========================================

	GET option.inc
	GET memcfg.inc
	;GET 2416addr.inc
    GET 2416.inc
    INCLUDE kxarm.h

    
BIT_SELFREFRESH EQU	(1<<22)

;------------------------add by xp-------------------------------------------
	OPT 1 
	OPT 128 
	
; Pre-defined constants.
;
USERMODE	EQU 	0x10
FIQMODE	 EQU 	0x11 
IRQMODE	 EQU 	0x12
SVCMODE	 EQU 	0x13
ABORTMODE   EQU 	0x17
UNDEFMODE   EQU 	0x1b
MODEMASK	EQU 	0x1f
NOINT	   EQU 	0xc0

; --- Amount of memory (in bytes) allocated for stacks

;========================= by zxs ===============================
;Len_FIQ_Stack    EQU     256
;Len_IRQ_Stack    EQU     256
;Len_ABT_Stack    EQU     256
;Len_UND_Stack    EQU     256
;Len_SVC_Stack    EQU     1024

Len_FIQ_Stack    EQU     0x1000
Len_IRQ_Stack    EQU     0x1000
Len_ABT_Stack    EQU     0x400		;0x400
Len_UND_Stack    EQU     0x400		;0x400
Len_SVC_Stack    EQU     0x1000

; Offsets will be loaded as immediate values.
; Offsets must be 8 byte aligned.

Offset_FIQ_Stack         EQU     0
Offset_IRQ_Stack         EQU     Offset_FIQ_Stack + Len_FIQ_Stack
Offset_ABT_Stack         EQU     Offset_IRQ_Stack + Len_IRQ_Stack
Offset_UND_Stack         EQU     Offset_ABT_Stack + Len_ABT_Stack
Offset_SVC_Stack         EQU     Offset_UND_Stack + Len_UND_Stack

;#define _RAM_STARTADDRESS 	0x30000000
;#define _ISR_STARTADDRESS 	0x33ffff00     
;#define _MMUTT_STARTADDRESS	0x33ff8000
;#define _STACK_BASEADDRESS	0x33ff8000
;#define HEAPEND		  	0x33ff0000
;top_of_stacks		EQU		_ISR_STARTADDRESS

; Stack locations.

;FIQStack	EQU	(top_of_stacks - 0x0)		; 0x33ffff00 ~ 
;IRQStack	EQU	(FIQStack   - Offset_FIQ_Stack)	  	; 0x33fffe00 ~
;AbortStack	EQU	(IRQStack   - Offset_IRQ_Stack) 	; 0x33fffd00 ~
;UndefStack	EQU	(AbortStack - Offset_ABT_Stack) 	; 0x33fffc00 ~
;SVCStack	EQU	(UndefStack - Offset_UND_Stack) 	; 0x33fffb00 ~
;UserStack	EQU	(SVCStack   - Offset_SVC_Stack)	  	; 0x33fff700 ~ 


FIQStack	EQU	(_STACK_BASEADDRESS - 0x0)		; 0x33ff8000 ~ 
IRQStack	EQU	(FIQStack   - Len_FIQ_Stack)	  	; 0x33ff7000 ~
AbortStack	EQU	(IRQStack   - Len_IRQ_Stack) 	; 0x33ff6000 ~
UndefStack	EQU	(AbortStack - Len_ABT_Stack) 	; 0x33ff5c00 ~
SVCStack	EQU	(UndefStack - Len_UND_Stack) 	; 0x33ff5800 ~
UserStack	EQU	(SVCStack   - Len_SVC_Stack)	  	; 0x33ff4800 ~ 
;========================= end zxs ===============================

;-----------------------------------------------------------------------------


;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.
	GBLL    THUMBCODE
	[ {CONFIG} = 16 
THUMBCODE SETL  {TRUE}
	    CODE32
    	|   
THUMBCODE SETL  {FALSE}
    	]

    	MACRO
	MOV_PC_LR
    	[ THUMBCODE
            bx lr
    	|
            mov	pc,lr
    	]
	MEND

    	MACRO
	MOVEQ_PC_LR
    	[ THUMBCODE
    	    bxeq lr
    	|
            moveq pc,lr
    	]
	MEND

    	MACRO
$HandlerLabel HANDLER $HandleLabel

$HandlerLabel
	sub	sp,sp,#4        ;decrement sp(to store jump address)
	stmfd	sp!,{r0}        ;PUSH the work register to stack(lr does't push because it return to original address)
	ldr     r0,=$HandleLabel;load the address of HandleXXX to r0
	ldr     r0,[r0]         ;load the contents(service routine start address) of HandleXXX
	str     r0,[sp,#4]      ;store the contents(ISR) of HandleXXX to stack
	ldmfd   sp!,{r0,pc}     ;POP the work register and pc(jump to ISR)
	MEND
	
	IMPORT  |Image$$RO$$Base|	; Base of ROM code
	IMPORT  |Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
	IMPORT  |Image$$RW$$Base|   ; Base of RAM to initialise
	IMPORT  |Image$$ZI$$Base|   ; Base and limit of area
	IMPORT  |Image$$ZI$$Limit|  ; to zero initialise	
    
    ;EXPORT MMU_EnableICache
 	;EXPORT MMU_SetAsyncBusMode
	
	AREA    SelfBoot, CODE, READONLY

	ENTRY
	
	EXPORT	__ENTRY
__ENTRY	
ResetEntry
	;1)The code, which converts to Big-endian, should be in little endian code.
	;2)The following little endian code will be compiled in Big-Endian mode. 
	;  The code byte order should be changed as the memory bus width.
	;3)The pseudo instruction,DCD can't be used here because the linker generates error.
	ASSERT	:DEF:ENDIAN_CHANGE
	[ ENDIAN_CHANGE
	    ASSERT  :DEF:ENTRY_BUS_WIDTH
	    [ ENTRY_BUS_WIDTH=32
			b	ChangeBigEndian	    ;DCD 0xea000007 
	    ]
	    
	    [ ENTRY_BUS_WIDTH=16
		andeq	r14,r7,r0,lsl #20   ;DCD 0x0007ea00
	    ]
	    
	    [ ENTRY_BUS_WIDTH=8
		streq	r0,[r0,-r10,ror #1] ;DCD 0x070000ea
        ]
	|
	b	ResetHandler  
    ]
	b	HandlerUndef	;handler for Undefined mode
	b	HandlerSWI		;handler for SWI interrupt
	b	HandlerPabort	;handler for PAbort
	b	HandlerDabort	;handler for DAbort
	b	.				;reserved
	b	HandlerIRQ		;handler for IRQ interrupt 
	b	HandlerFIQ		;handler for FIQ interrupt

;@0x20
;	b	EnterPWDN
ChangeBigEndian
;@0x24
	[ ENTRY_BUS_WIDTH=32
	    DCD	0xee110f10	;0xee110f10 => mrc p15,0,r0,c1,c0,0
	    DCD	0xe3800080	;0xe3800080 => orr r0,r0,#0x80;  //Big-endian
	    DCD	0xee010f10	;0xee010f10 => mcr p15,0,r0,c1,c0,0
	]
	[ ENTRY_BUS_WIDTH=16
	    DCD 0x0f10ee11
	    DCD 0x0080e380	
	    DCD 0x0f10ee01	
	]
	[ ENTRY_BUS_WIDTH=8
	    DCD 0x100f11ee	
	    DCD 0x800080e3	
	    DCD 0x100f01ee	
    	]
	DCD 0xffffffff  ;swinv 0xffffff is similar with NOP and run well in both endian mode. 
	DCD 0xffffffff
	DCD 0xffffffff
	DCD 0xffffffff
	DCD 0xffffffff
	b ResetHandler
	
;Function for entering power down mode
; 1. SDRAM should be in self-refresh mode.
; 2. All interrupt should be maksked for SDRAM/DRAM self-refresh.
; 3. LCD controller should be disabled for SDRAM/DRAM self-refresh.
; 4. The I-cache may have to be turned on. 
; 5. The location of the following code may have not to be changed.

;void EnterPWDN(int CLKCON); 
;EnterPWDN			
	;mov r2,r0		;r2=rCLKCON
	;tst r0,#0x8		;POWER_OFF mode?
	;bne ENTER_POWER_OFF

;ENTER_STOP	
	;ldr r0,=REFRESH		
	;ldr r3,[r0]		;r3=rREFRESH	
	;mov r1, r3
	;orr r1, r1, #BIT_SELFREFRESH
	;str r1, [r0]		;Enable SDRAM self-refresh

	;mov r1,#16	   	;wait until self-refresh is issued. may not be needed.
;0	;subs r1,r1,#1
	;bne %B0

	;ldr r0,=CLKCON		;enter STOP mode.
	;str r2,[r0]    

	;mov r1,#32
;0	;subs r1,r1,#1	;1) wait until the STOP mode is in effect.
	;bne %B0		;2) Or wait here until the CPU&Peripherals will be turned-off
			;   Entering POWER_OFF mode, only the reset by wake-up is available.

	;ldr r0,=REFRESH ;exit from SDRAM self refresh mode.
	;str r3,[r0]
	
	;MOV_PC_LR

;ENTER_POWER_OFF	
	;NOTE.
	;1) rGSTATUS3 should have the return address after wake-up from POWER_OFF mode.
	
	;ldr r0,=REFRESH		
	;ldr r1,[r0]		;r1=rREFRESH	
	;orr r1, r1, #BIT_SELFREFRESH
	;str r1, [r0]		;Enable SDRAM self-refresh

	;mov r1,#16	   	;Wait until self-refresh is issued,which may not be needed.
;0	;subs r1,r1,#1
	;bne %B0

	;ldr 	r1,=MISCCR
	;ldr	r0,[r1]
	;orr	r0,r0,#(7<<17)  ;Make sure that SCLK0:SCLK->0, SCLK1:SCLK->0, SCKE=L during boot-up 
	;str	r0,[r1]

	;ldr r0,=CLKCON
	;str r2,[r0]    

	;b .			;CPU will die here.
	

;WAKEUP_POWER_OFF
	;Release SCLKn after wake-up from the POWER_OFF mode.

	;ldr r1,=MISCCR
	;ldr	r0,[r1]
	;bic	r0,r0,#(7<<17)  ;SCLK0:0->SCLK, SCLK1:0->SCLK, SCKE:L->H
	;str	r0,[r1]

	;Set memory control registers
	;ldr	r0,=SMRDATA
	;ldr	r1,=BWSCON	;BWSCON Address
	;add	r2, r0, #52	;End address of SMRDATA
;0       
	;ldr	r3, [r0], #4
	;str	r3, [r1], #4
	;cmp	r2, r0
	;bne	%B0

	;mov r1,#256
;0	;subs r1,r1,#1		;1) wait until the SelfRefresh is released.
	;bne %B0		
	
	;ldr r1,=GSTATUS3 	;GSTATUS3 has the start address just after POWER_OFF wake-up
	;ldr r0,[r1]
	;mov pc,r0

	LTORG   
HandlerFIQ      HANDLER HandleFIQ
HandlerIRQ      HANDLER HandleIRQ
HandlerUndef    HANDLER HandleUndef
;HandlerUndef
;	sub	sp, sp, #4			;decrement sp(to store jump address)
;	stmfd	sp!, {r14}		;PUSH the work register to stack(lr does't push because it return to original address)
;	ldr	r0, =HandleUndef	;load the address of HandleXXX to r0
;	ldr	r0, [r0]         	;load the contents(service routine start address) of HandleXXX
;	str	r0, [sp, #4]		;store the contents(ISR) of HandleXXX to stack
;	ldmfd	sp!, {r0, pc}	
HandlerSWI      HANDLER HandleSWI
HandlerDabort   HANDLER HandleDabort
HandlerPabort   HANDLER HandlePabort

IsrIRQ  
	sub	sp, sp, #4       ;reserved for PC
	stmfd	sp!, {r8-r9}
	
	ldr	r9, =INTOFFSET1
	ldr	r9, [r9]
	ldr	r8, =HandleEINT0
	add	r8, r8,r9,lsl #2
	ldr	r8, [r8]
	str	r8, [sp,#8]
	ldmfd	sp!,{r8-r9,pc}

;=======
; ENTRY  
;=======
ResetHandler

	ldr	r0,=WTCON       ;watch dog disable 
	ldr	r1,=0x0         
	str	r1,[r0]

;-------------------------add by xp----------------------------------------------

	;ldr pc,=0x30038000

	;by zxs 100412
	;ldr	r0, =SRCPND1      ; clear all SRCPND 1.   set 1 to clear
	;ldr	r1, =0xffffffff
	;str	r1, [r0]

	ldr	r0, =INTMSK1      ; mask all first-level interrupts.
	ldr	r1, =0xffffffff
	str	r1, [r0]

	ldr	r0, =INTMSK2      ; mask all first-level interrupts.
	ldr	r1, =0xffffffff
	str	r1, [r0]

	ldr	r0, =INTSUBMSK   ; mask all second-level interrupts.
	ldr	r1, =0x1fffffff
	str	r1, [r0]

	ldr r0, = INTMOD1
	mov r1, #0x0			; set all interrupt as IRQ
	str r1, [r0]

	ldr r0, = INTMOD2
    mov r1, #0x0			; set all interrupt as IRQ
    str r1, [r0]
 ;------------------------------------------------------------------------------
    
	[ {FALSE}
	; rGPFDAT = (rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);    
	; Led_Display
	ldr	r0,=GPFCON
	ldr	r1,=0x5500		
	str	r1,[r0]
	ldr	r0,=GPFDAT
	ldr	r1,=0x10
	str	r1,[r0]
	]
	
;----------------------------add by xp----------------------------------------	
	ldr		r0,=CLKDIV0			;	Set Clock Divider
	ldr		r1,[r0]
	bic		r1,r1,#0x37		; clear HCLKDIV, PREDIV, PCLKDIV
	bic		r1,r1,#(0xf<<9) ; clear ARMCLKDIV
	ldr		r2,=((Startup_ARMCLKdiv<<9)+(Startup_PREdiv<<4)+(Startup_PCLKdiv<<2)+(Startup_HCLKdiv)) 
	orr		r1,r1,r2
	;ldr     r1,=0x22d
	str		r1,[r0]			

	ldr		r0,=LOCKCON0		;	Set lock time of MPLL. added by junon
	mov		r1,#0xe10			;	Fin = 12MHz - 0x800, 16.9844MHz - 0xA00
	str		r1,[r0]	

	ldr		r0,=LOCKCON1		;	Set lock time of EPLL. added by junon
	mov		r1,#0x800			;	Fin = 12MHz - 0x800, 16.9844MHz - 0xA00
	str		r1,[r0]	

	ldr		r0,=MPLLCON			;	Set MPLL
	ldr		r1,=((0<<24)+(Startup_Mdiv<<14)+(Startup_Pdiv<<5)+(Startup_Sdiv))
	str		r1,[r0]			

  	ldr		r0,=EPLLCON			;	Set EPLL
	ldr		r1,=((0<<24)+(Startup_EMdiv<<16)+(Startup_EPdiv<<8)+(Startup_ESdiv))
	str		r1,[r0]			

	ldr		r0,=CLKSRC			;	Select MPLL clock out for SYSCLK
	ldr		r1,[r0]
	orr		r1,r1,#0x50
	str		r1,[r0]	
	
	;----------------------------
	; MMU set Asynchonous Bus Mode

	;bl		MMU_SetAsyncBusMode

 	;----------------------------
	; Memory Controller initialize

	bl		InitMEM

	
  ;----------------------------
	; SMC initialize

	bl		InitSSMC




;------------------------------------------------------------------------------		
	
	;To reduce PLL lock time, adjust the LOCKTIME register. 
	;ldr	r0,=LOCKTIME
	;ldr	r1,=0xffffff
	;str	r1,[r0]
        
    ;[ PLL_ON_START
	;Configure MPLL
	;ldr	r0,=MPLLCON
	;ldr	r1,=((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV)  ;Fin=12MHz,Fout=50MHz
	;str	r1,[r0]
	;]



	EXPORT StartPointAfterPowerOffWakeUp
StartPointAfterPowerOffWakeUp

	;Set memory control registers
	;adr	r0, SMRDATA	;can't use ldr r0, =xxxx important!!!
	;ldr	r1, =BWSCON	;BWSCON Address
	;add	r2, r0, #52	;End address of SMRDATA
;0       
	;ldr	r3, [r0], #4    
	;str	r3, [r1], #4    
	;cmp	r2, r0		
	;bne	%B0
	
;--------------------------add by xp---------------------------------------	
	;Set GPK port when using x32 bus width.
	ldr		r0,=GPKCON
	ldr		r1,=0xaaaaaaaa	; set Sdata[31:16]
	str		r1, [r0]
				
	;Set SDR Memory parameter control registers
 	ldr		r0,=MEMDATA
	ldr		r1,=BANKCFG	;
	add		r2, r0, #12	;End address of MEMDATA
1
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	cmp		r2, r0
	bne		%B1	


	  			
;---------------------------------------------------------------------------	

    ;Initialize stacks
	;bl	InitStacks
	
  	; Setup IRQ handler
	ldr	r0,=HandleIRQ       ;This routine is needed
	ldr	r1,=IsrIRQ          ;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
	str	r1,[r0]

   ; b      copy_proc_beg
    
	;ldr	r0, =BWSCON
	;ldr	r0, [r0]
	;ands	r0, r0, #6		;OM[1:0] != 0, NOR FLash boot
	;bne	copy_proc_beg		;don't read nand flash
	;adr	r0, ResetEntry		;OM[1:0] == 0, NAND FLash boot
	;cmp	r0, #0				;if use Multi-ice, 
	;bne	copy_proc_beg		;don't read nand flash for boot
;===========================================================
nand_boot_beg

copy_proc_beg

	 	[ {TRUE}	; DonGo
	;bne		%B20
	]

	
	; Initialize stacks.
	;

	bl	InitStacks
	
	

    [ :LNOT:THUMBCODE
    	ldr pc, GotoMain	;bl	Main        ;Don't use main() because ......
    	b	.                       
    ]

   ; [ THUMBCODE         ;for start-up code for Thumb mode
    	;orr	lr,pc,#1
    	;bx	lr
    	;CODE16
    	;bl	Main        ;Don't use main() because ......
    	;b	.
    	;CODE32
   ; ]


;function initializing stacks
InitStacks
	;Don't use DRAM,such as stmfd,ldmfd......
	;SVCstack is initialized before
	;Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'

	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		;UndefMode
	ldr	sp,=UndefStack
	
	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		;AbortMode
	ldr	sp,=AbortStack

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		;IRQMode
	ldr	sp,=IRQStack
    
	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		;FIQMode
	ldr	sp,=FIQStack

	; by zxs  100412
	bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		;SVCMode
	ldr	sp,=SVCStack
	;bic	r0,r0,#MODEMASK
	;orr	r1,r0,#SVCMODE|NOINT
	;msr	cpsr_cxsf,r1		;SVCMode
	;ldr	sp,=SVCStack

	;USER mode has not be initialized.
	
	mov	pc,lr 
	;The LR register won't be valid if the current mode is not SVC mode.
	

;===========================================================
	EXPORT	disable_irq
disable_irq
	mrs	r0, cpsr				;enter svc mode and disable irq,fiq
	orr	r0, r0, #0xc0
	msr	cpsr_c, r0
	mov	pc, lr
	
ReadNandID
	mov		 r7,#NFCONF	
	ldr      r0,[r7,#0]		;NFChipEn();
	bic      r0,r0,#0x800
	str      r0,[r7,#0]	
	mov      r0,#0x90		;WrNFCmd(RdIDCMD);
	strb     r0,[r7,#4]	
	mov      r4,#0			;WrNFAddr(0);
	strb     r4,[r7,#8]	
1							;while(NFIsBusy());
	ldr      r0,[r7,#0x10]	
	tst      r0,#1
	beq      %B1
	ldrb     r0,[r7,#0xc]	;id  = RdNFDat()<<8;
	mov      r0,r0,lsl #8	
	ldrb     r1,[r7,#0xc]	;id |= RdNFDat();
	orr      r5,r1,r0	
	ldr      r0,[r7,#0]		;NFChipDs();
	orr      r0,r0,#0x800
	str      r0,[r7,#0]	
	mov		 pc,lr	
	
ReadNandStatus
	mov		 r7,#NFCONF
	ldr      r0,[r7,#0]		;NFChipEn();
	bic      r0,r0,#0x800
	str      r0,[r7,#0]
	mov      r0,#0x70		;WrNFCmd(QUERYCMD);
	strb     r0,[r7,#4]	
	ldrb     r1,[r7,#0xc]	;r1 = RdNFDat();
	ldr      r0,[r7,#0]		;NFChipDs();
	orr      r0,r0,#0x800
	str      r0,[r7,#0]
	mov		 pc,lr

WaitNandBusy
	mov      r0,#0x70		;WrNFCmd(QUERYCMD);
	mov      r1,#NFCONF
	strb     r0,[r1,#4]
1							;while(!(RdNFDat()&0x40));	
	ldrb     r0,[r1,#0xc]
	tst      r0,#0x40
	beq		 %B1
	mov      r0,#0			;WrNFCmd(READCMD0);
	strb     r0,[r1,#4]
	mov      pc,lr

CheckBadBlk
	mov		r7, lr
	mov		r5, #NFCONF
	
	bic		r0, r0, #0x1f	;addr &= ~0x1f;
	ldr      r1,[r5,#0]		;NFChipEn()
	bic      r1,r1,#0x800
	str      r1,[r5,#0]	

	mov      r1,#0x50		;WrNFCmd(READCMD2)
	strb     r1,[r5,#4]	
	mov      r1, #5
	strb     r1,[r5,#8]		;WrNFAddr(5)
	strb     r0,[r5,#8]		;WrNFAddr(addr)
	mov      r1,r0,lsr #8	;WrNFAddr(addr>>8)
	strb     r1,[r5,#8]	
	cmp      r6,#0			;if(NandAddr)		
	movne    r0,r0,lsr #16	;WrNFAddr(addr>>16)
	strneb   r0,[r5,#8]
	
	bl		WaitNandBusy	;WaitNFBusy()

	ldrb	r0, [r5,#0xc]	;RdNFDat()
	sub		r0, r0, #0xff
	
	mov      r1,#0			;WrNFCmd(READCMD0)
	strb     r1,[r5,#4]	
	
	ldr      r1,[r5,#0]		;NFChipDs()
	orr      r1,r1,#0x800
	str      r1,[r5,#0]
	
	mov		pc, r7
	
ReadNandPage
	mov		 r7,lr
	mov      r4,r1
	mov      r5,#NFCONF

	ldr      r1,[r5,#0]		;NFChipEn()
	bic      r1,r1,#0x800
	str      r1,[r5,#0]	

	mov      r1,#0			;WrNFCmd(READCMD0)
	strb     r1,[r5,#4]	
	strb     r1,[r5,#8]		;WrNFAddr(0)
	strb     r0,[r5,#8]		;WrNFAddr(addr)
	mov      r1,r0,lsr #8	;WrNFAddr(addr>>8)
	strb     r1,[r5,#8]	
	cmp      r6,#0			;if(NandAddr)		
	movne    r0,r0,lsr #16	;WrNFAddr(addr>>16)
	strneb   r0,[r5,#8]
	
	ldr      r0,[r5,#0]		;InitEcc()
	orr      r0,r0,#0x1000
	str      r0,[r5,#0]	
	
	bl       WaitNandBusy	;WaitNFBusy()
	
	mov      r0,#0			;for(i=0; i<512; i++)
1
	ldrb     r1,[r5,#0xc]	;buf[i] = RdNFDat()
	strb     r1,[r4,r0]
	add      r0,r0,#1
	bic      r0,r0,#0x10000
	cmp      r0,#0x200
	bcc      %B1
	
	ldr      r0,[r5,#0]		;NFChipDs()
	orr      r0,r0,#0x800
	str      r0,[r5,#0]
		
	mov		 pc,r7

;===========================================================

InitMEM
				;DDR2
				
				;Set GPK port when using x32 bus width.
				ldr		r0,=GPKCON
				ldr		r1,=0xaaaaaaaa	; set Sdata[31:16]
				str		r1, [r0]
				
				;Set DDR2 Memory parameter control registers
				ldr		r0,=BANKCFG
				ldr		r1,=BANKCFGVAL 	; set Sdata[31:16]
				str		r1, [r0]

				ldr		r0,=BANKCON1
				ldr		r1,=BANKCON1VAL  	; set Sdata[31:16]
				str		r1, [r0]


				ldr		r0,=BANKCON2
				ldr		r1,=BANKCON2VAL   	; set Sdata[31:16]
				str		r1, [r0]
				
				
;				ldr		r2,=RSTSTAT			;
;				ldr		r1,[r2]
;				cmp 	r1, #0x1	
;				bne		GOM			

				ldr		r2,=BANKCON1			;	4nd	:	Issue a PALL command
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x1<<0)			
				str		r1,[r2]	

				ldr		r2,=BANKCON3			;	5th	:	Issue a EMRS2 command
				ldr		r3,=0xffff0000
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,#(BA_EMRS2<<30)
				str		r1,[r2]	

				ldr		r2,=BANKCON1
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x3<<0)			
				str		r1,[r2]

				ldr		r2,=BANKCON3			;	6th	:	Issue a EMRS3 command
				ldr		r3,=0xffff0000
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,#(BA_EMRS3<<30)
				str		r1,[r2]

				ldr		r2,=BANKCON1
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x3<<0)			
				str		r1,[r2]

				ldr		r2,=BANKCON3			;	7th	:	Issue a EMRS1 command
				ldr		r3,=0xffff0000
				ldr		r4,=((BA_EMRS1<<30)+(RDQS_DIS<<27)+(nDQS_DIS<<26)+(OCD_MODE_EXIT<<23)+(DLL_EN<<16)) 
				                      ; (0x1<<30)|(0x0<<27)|(0x1<<26)|(0x0<<23)|(0x0<<16)
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,r4
				str		r1,[r2]

				ldr		r2,=BANKCON1
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x3<<0)			
				str		r1,[r2]

				ldr		r2,=BANKCON3			;	8th	:	Issue a MRS command
				ldr		r3,=0xffff
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,#((BA_MRS<<14)+(DLL_RESET_HIGH<<8)+(TM<<7)+(CL_MRS<<4))
				str		r1,[r2]
				
				ldr		r2,=BANKCON1				
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x2<<0)			
				str		r1,[r2]

				ldr		r2,=BANKCON1			;	9nd	:	Issue a PALL command
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x1<<0)			
				str		r1,[r2]

				ldr		r4,=REFRESH				;	10th : wait 2 auto - clk
				ldr		r0,=0x20
				str		r0,[r4]					
				
				ldr		r2,=BANKCON3			;	11th	:	Issue a MRS command
				ldr		r3,=0xffff
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,#((BA_MRS<<14)+(DLL_RESET_LOW<<8)+(TM<<7)+(CL_MRS<<4))
				str		r1,[r2]
				
				ldr		r2,=BANKCON1				
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x2<<0)			
				str		r1,[r2]

				mov	r0, #0x100					;	Wait 200 clock
2				subs	r0, r0,#1;
				bne	%B2	
				
				ldr		r2,=BANKCON3			;	12th	:	Issue a EMRS1 command For OCD Mode Set to default
				ldr		r3,=0xffff0000
				ldr		r4,=((BA_EMRS1<<30)+(RDQS_DIS<<27)+(nDQS_DIS<<26)+(OCD_MODE_DEFAULT<<23)+(DLL_EN<<16))
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,r4
				str		r1,[r2]

				ldr		r2,=BANKCON1
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x3<<0)			
				str		r1,[r2]

				ldr		r2,=BANKCON3
				ldr		r3,=0xffff0000
				ldr		r4,=((BA_EMRS1<<30)+(RDQS_DIS<<27)+(nDQS_DIS<<26)+(OCD_MODE_EXIT<<23)+(DLL_EN<<16))
				ldr		r1,[r2]
				bic		r1,r1,r3
				orr		r1,r1,r4
				str		r1,[r2]

				ldr		r2,=BANKCON1
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				orr		r1,r1,#(0x3<<0)			
				str		r1,[r2]
				
				ldr		r4,=REFRESH			;	13fh : refresh  normal
				ldr		r0,=REFCYC
				str		r0,[r4]					

				ldr		r2,=BANKCON1		;	14th	:	Issue a Normal mode
				ldr		r1,[r2]
				bic		r1,r1,#(0x3<<0)
				str		r1,[r2]





InitSSMC

	;Set SSMC Memory parameter control registers : AMD Flash
	ldr		r0,=SMBIDCYR0
	ldr		r1,=IDCY0
	str		r1,[r0]
	
	ldr		r0,=SMBWSTRDR0
	ldr		r1,=WSTRD0
	str		r1,[r0]
	
	ldr		r0,=SMBWSTWRR0
	ldr		r1,=WSTWR0
	str		r1,[r0]
	
	ldr		r0,=SMBWSTOENR0
	ldr		r1,=WSTOEN0
	str		r1,[r0]
	
	ldr		r0,=SMBWSTWENR0
	ldr		r1,=WSTWEN0
	str		r1,[r0]
	
	ldr		r0,=SMBCR0
	ldr		r1,=(SMBCR0_2+SMBCR0_1+SMBCR0_0)
	str		r1,[r0]
	
	ldr		r0,=SMBWSTBRDR0
	ldr		r1,=WSTBRD0
	str		r1,[r0]

	
	ldr		r0,=SMBWSTBRDR0
	ldr		r1,=WSTBRD0
	str		r1,[r0]

	ldr		r0,=SSMCCR
	ldr		r1,=((MemClkRatio<<1)+(SMClockEn<<0))
	str		r1,[r0]
	
	ldr		r0,=SMBWSTRDR5
	ldr		r1,=0xe
	str		r1,[r0]
	
	mov pc, lr
	
;-----------------------------------------------------------------------------

	EXPORT	__RdPage512
__RdPage512
	;input:a1(r0)=pPage
	stmfd	sp!,{r1-r11}

	ldr		r1,=0x4e000010  ;NFDATA
	mov		r2,#0x200
10	
	ldr 		r4,[r1]
	ldr 		r5,[r1]
	ldr 		r6,[r1]
	ldr 		r7,[r1]
	ldr 		r8,[r1]
	ldr 		r9,[r1]
	ldr 		r10,[r1]
	ldr 		r11,[r1]
	stmia	r0!,{r4-r11}
	subs		r2,r2,#32
	bne		%B10

	ldmfd	sp!,{r1-r11}
	mov		pc,lr


;------------------------------------
; MMU Cache/TLB/etc on/off functions
;------------------------------------
R1_I	EQU	(1<<12)
R1_C	EQU	(1<<2)
R1_A	EQU	(1<<1)
R1_M  	EQU	(1)
;R1_iA	EQU	(1<<31)
;R1_nF 	EQU	(1<<30)

; void MMU_EnableICache(void);
;
	;LEAF_ENTRY MMU_EnableICache	
	
	;mrc 	p15, 0, r0, c1, c0, 0
	;orr 	r0, r0, #R1_I
	;mcr 	p15, 0, r0, c1, c0, 0
	;mov 	pc, lr

; void MMU_SetAsyncBusMode(void);
; FCLK:HCLK= 1:2
;
	;LEAF_ENTRY MMU_SetAsyncBusMode
	;mrc 	p15, 0, r0, c1, c0, 0
	;orr 	r0, r0, #R1_nF:OR:R1_iA
	;mcr 	p15, 0, r0, c1, c0, 0
	;mov 	pc, lr
			
	

	LTORG
	

SMRDATA DATA
; Memory configuration should be optimized for best performance 
; The following parameter is not optimized.                     
; Memory access cycle parameter strategy
; 1) The memory settings is  safe parameters even at HCLK=75Mhz.
; 2) SDRAM refresh period is for HCLK=75Mhz. 

;--------------------------------add by xp---------------------------------------

MEMDATA			DATA
				DCD		((RASBW0<<17)+(RASBW1<<14)+(CASBW0<<11)+(CASBW1<<8)+(ADDRCFG0<<6)+(ADDRCFG1<<4)+(MEMCFG<<1)+(BW<<0))
				DCD		((DQSDelay<<28)+(1<<26)+(BStop<<7)+(WBUF<<6)+(AP<<5)+(PWRDN<<4))
				DCD		((tRAS<<20)+(tARFC<<16)+(CL<<4)+(tRCD<<2)+(tRP<<0))

       ; DCD (0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))
    	;DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))   ;GCS0
    	;DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))   ;GCS1 
    	;DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))   ;GCS2
    	;DCD 0x1f7c;((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))   ;GCS3
    	;DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))   ;GCS4
    	;DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))   ;GCS5
    	;DCD ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))    ;GCS6
    	;DCD ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))    ;GCS7
    	;DCD ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)    



	;DCD 0x32            ;SCLK power saving mode, BANKSIZE 128M/128M

    	;DCD 0x30            ;MRSR6 CL=3clk
    	;DCD 0x30            ;MRSR7
;    	DCD 0x20            ;MRSR6 CL=2clk
;    	DCD 0x20            ;MRSR7

BaseOfROM	DCD	|Image$$RO$$Base|
TopOfROM	DCD	|Image$$RO$$Limit|
BaseOfBSS	DCD	|Image$$RW$$Base|
BaseOfZero	DCD	|Image$$ZI$$Base|
EndOfBSS	DCD	|Image$$ZI$$Limit|

	GBLS	main_entry
main_entry	SETS	"Main"	
	IMPORT	$main_entry
GotoMain	DCD	$main_entry		

    	ALIGN


    	AREA RamData, DATA, READWRITE

        ^   _ISR_STARTADDRESS
HandleReset 	#   4
HandleUndef 	#   4
HandleSWI   	#   4
HandlePabort    #   4
HandleDabort    #   4
HandleReserved  #   4
HandleIRQ   	#   4
HandleFIQ   	#   4

;Don't use the label 'IntVectorTable',
;The value of IntVectorTable is different with the address you think it may be.
;IntVectorTable
HandleEINT0   	#   4
HandleEINT1   	#   4
HandleEINT2   	#   4
HandleEINT3   	#   4
HandleEINT4_7	#   4
HandleEINT8_23	#   4
HandleRSV6	#   4
HandleBATFLT   	#   4
HandleTICK   	#   4
HandleWDT	#   4
HandleTIMER0 	#   4
HandleTIMER1 	#   4
HandleTIMER2 	#   4
HandleTIMER3 	#   4
HandleTIMER4 	#   4
HandleUART2  	#   4
HandleLCD 	#   4
HandleDMA0	#   4
HandleDMA1	#   4
HandleDMA2	#   4
HandleDMA3	#   4
HandleMMC	#   4
HandleSPI0	#   4
HandleUART1	#   4
HandleRSV24	#   4
HandleUSBD	#   4
HandleUSBH	#   4
HandleIIC   	#   4
HandleUART0 	#   4
HandleSPI1 	#   4
HandleRTC 	#   4
HandleADC 	#   4


        END
