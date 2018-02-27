;//chapter2 SYSEM CONTROLLER
LOCKCON0    EQU           0x4C000000  		;//MPLL lock time conut
LOCKCON1    EQU           0x4C000004  		;//EPLL lock time count
OSCSET      EQU           0x4C000008  		;//OSC stabilization control
MPLLCON     EQU           0x4C000010  		;//MPLL configuration
EPLLCON     EQU           0x4C000018  		;//EPLL configuration
CLKSRC      EQU           0x4C000020  		;//Clock source control
CLKDIV0     EQU           0x4C000024  		;//Clock divider ratio control
CLKDIV1     EQU           0x4C000028  		;//Clock divider ratio control
HCLKCON     EQU           0x4C000030  		;//HCLK enable
PCLKCON     EQU           0x4C000034  		;//PCLK enable
SCLKCON     EQU           0x4C000038  		;//Special clock enable
PWRMODE     EQU           0x4C000040  		;//Power mode control
SWIRST      EQU           0x4C000044  		;//Software reset control
BUSPRI0     EQU           0x4C000050  		;//Bus priority control
BUSMISC     EQU           0x4C000058  		;//Bus miscellaneous bus control
SYSID       EQU           0x4C00005C  		;//System ID control
PWRCFG      EQU           0x4C000060  		;//Power management configuration control
RSTCON      EQU           0x4C000064  		;//Reset control 
RSTSTAT     EQU           0x4C000068  		;//Reset status
WKUPSTAT    EQU           0x4C00006c  		;//Wakeup status
INFORM0     EQU           0x4C000070  		;//Sleep mode information 0
INFORM1     EQU           0x4C000074  		;//Sleep mode information 1
INFORM2     EQU           0x4C000078  		;//Sleep mode information 2
INFORM3     EQU           0x4C00007C  		;//Sleep mode information 3
USB_PHYCTRL EQU           0x4C000080  		;//USB phy control
USB_PHYPWR  EQU           0x4C000084  		;//USB phy power control
USB_RSTCON  EQU           0x4C000088  		;//USB phy reset control
USB_CLKCON  EQU           0x4C00008C  		;//USB phy clock control
USB_TESTTI  EQU           0x4C000090  		;//USB phy test in
USB_TESTTO  EQU           0x4C000094  		;//USB phy test out
                
                
;//chapter3	EBI controller
BPRIORITY0	 EQU		0x4E800000	;//Matrix core 0 priority decision - edited by junon
BPRIORITY1	 EQU		0x4E800004	;//Matrix core 1 priority decision - added by junon
EBICON   	 EQU		0x4E800008	;//Bank Configuration - edited by junon


;//chapter5 SSMC
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


;//chapter6 MOBILE DRAM CONTROLLER
BANKCFG    EQU           0x48000000	;;//Mobile DRAM configuration
BANKCON1    EQU           0x48000004	;;//Mobile DRAM control 
BANKCON2    EQU           0x48000008	;;//Mobile DRAM timing control 
BANKCON3    EQU           0x4800000C	;;//Mobile DRAM (E)MRS 
REFRESH    EQU           0x48000010	;;//Mobile DRAM refresh control
TIMEOUT    EQU           0x48000014	;;//Write Buffer Time out control 


;//chapter7 Nand Flash
NFCONF		  EQU           0x4E000000		  ;//NAND Flash configuration
NFCONT		  EQU           0x4E000004      ;//NAND Flash control
NFCMD		  EQU           0x4E000008      ;//NAND Flash command 
NFADDR		  EQU           0x4E00000C      ;//NAND Flash address
NFDATA		  EQU           0x4E000010      ;//NAND Flash data                         
NFMECCD0	  EQU           0x4E000014      ;//NAND Flash ECC for Main 
NFMECCD1	  EQU           0x4E000018      ;//NAND Flash ECC for Main 
NFSECCD	  EQU           0x4E00001C	  	;//NAND Flash ECC for Spare Area
NFSBLK 	  EQU           0x4E000020		  ;//NAND Flash programmable start block address
NFEBLK 	  EQU           0x4E000024 	    ;//NAND Flash programmable end block address     
NFSTAT 	  EQU           0x4E000028      ;//NAND Flash operation status 
NFECCERR0	EQU           0x4E00002C      ;//NAND Flash ECC Error Status for I/O [7:0]
NFECCERR1	EQU           0x4E000030      ;//NAND Flash ECC Error Status for I/O [15:8]
NFMECC0		EQU           0x4E000034      ;//SLC or MLC NAND Flash ECC status
NFMECC1		EQU           0x4E000038	    ;//SLC or MLC NAND Flash ECC status	
NFSECC 		EQU           0x4E00003C  		;//NAND Flash ECC for I/O[15:0]
NFMLCBITPT	EQU           0x4E000040  		;//NAND Flash 4-bit ECC Error Pattern for data[7:0]


;//chapter11 I/O PORT 
GPACON   	EQU           0x56000000	;//Configure the pins of port A
GPADAT   	EQU           0x56000004	;//The data for port A
GPBCON  	EQU           0x56000010	;//Configure the pins of port B
GPBDAT   	EQU           0x56000014	;//The data for port B
GPBUDP    EQU           0x56000018	;//Pull-down disable for port 
GPCCON   	EQU           0x56000020	;//Configure the pins of port C
GPCDAT   	EQU           0x56000024	;//The data for port C
GPCUDP    EQU           0x56000028	;//Pull-down disable for port C
GPDCON   	EQU           0x56000030	;//Configure the pins of port D
GPDDAT   	EQU           0x56000034	;//The data for port D
GPDUDP    EQU           0x56000038	;//Pull-down disable for port D
GPECON   	EQU           0x56000040	;//Configure the pins of port E
GPEDAT   	EQU           0x56000044	;//The data for port E
GPEUDP    EQU           0x56000048	;//Pull-down disable for port E
GPFCON   	EQU           0x56000050	;//Configure the pins of port F
GPFDAT   	EQU           0x56000054	;//The data for port F
GPFUDP    EQU           0x56000058	;//Pull-down disable for port F
GPGCON   	EQU           0x56000060	;//Configure the pins of portt G 
GPGDAT   	EQU           0x56000064	;//The data for port G 
GPGUDP    EQU           0x56000068	;//Pull-down disable for port G
GPHCON		EQU           0x56000070	;//Configure the pins of porttt H 
GPHDAT		EQU           0x56000074	;//The data for port H 
GPHUDP		EQU           0x56000078	;//Pull-down disable for port H
GPJCON		EQU           0x560000D0	;//Configure the pins of portttt J
GPJDAT		EQU           0x560000D4	;//The data for port J 
GPJUDP		EQU           0x560000D8	;//Pull-down disable for porl J
GPKCON		EQU           0x560000E0	;//Configure the pins of portttt K
GPKDAT		EQU           0x560000E4	;//The data for port K 
GPKUDP		EQU           0x560000E8	;//Pull-down disable for porl K
GPLCON		EQU           0x560000F0	;//Configure the pins of portttt F
GPLDAT		EQU           0x560000F4	;//The data for port F 
GPLUDP		EQU           0x560000F8	;//Pull-down disable for porl F
GPMCON		EQU           0x56000100	;//Configure the pins of portttt M
GPMDAT		EQU           0x56000104	;//The data for port M 
GPMUDP		EQU           0x56000108	;//Pull-down disable for porl M
MISCCR		EQU           0x56000080	;//Miscellaneous control
DCLKCON	  EQU           0x56000084	;//DCLK0/1 control
EXTINT0	  EQU           0x56000088	;//External interrupt control register 0
EXTINT1	  EQU           0x5600008C	;//External interrupt control register 1
EXTINT2	  EQU           0x56000090	;//External interrupt control register 2
EINTFLT0	EQU           0x56000094	;//Reserved
EINTFLT1	EQU           0x56000098	;//Reserved
EINTFLT2	EQU           0x5600009c	;//External interrupt filter control register 2
EINTFLT3	EQU           0x560000A0	;//External interrupt filter control register 3
EINTMASK	EQU           0x560000A4	;//External interrupt mask
EINTPEND	EQU           0x560000A8	;//External interrupt pending
GSTATUS0	EQU           0x560000AC	;//External pin status
GSTATUS1	EQU           0x560000B0	;//Chip ID(0x32443000)
GSTATUS2	EQU           0x560000B4	;//Infrom
GSTATUS3	EQU           0x560000B8	;//Infrom
GSTATUS4	EQU           0x560000BC	;//Infrom
MSLCON		EQU           0x560000CC	;//memory stop control register
                      
                      
;//chapter10 INTERRUPT 
SRCPND     EQU          0x4a000000	;//Interrupt request status
INTMOD     EQU          0x4a000004	;//Interrupt mode control
INTMSK     EQU          0x4a000008	;//Interrupt mask control
PRIORITY   EQU          0x4a00000c	;//IRQ priority control
INTPND     EQU          0x4a000010	;//Interrupt request status
INTOFFSET  EQU          0x4a000014	;//Interrupt request source offset
SUBSRCPND  EQU          0x4a000018	;//Interrupt request status
INTSUBMSK  EQU          0x4a00001c	;//Interrupt source mask
          

;// chapter12 WATCH DOG TIMER
WTCON   EQU          0x53000000	;//Watch-dog timer mode
WTDAT   EQU          0x53000004	;//Watch-dog timer data
WTCNT   EQU          0x53000008	;//Watch-dog timer count

	END