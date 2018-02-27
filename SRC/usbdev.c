/*------------------------------------------------------------------------------
 *
 * Filename: usbd_h.cpp
 *
 * Contents:
 *
 * Notes:
 * 		29.Nov.2005		By Hyunsun.Ahn	1st Made
 *		27.Dec.2006		modified By gyu hwan cha	
 *
 * Contributors : Hyunsun.ahn	
 *
 * Copyright (c) 2005 SAMSUNG Electronics.
 *
 *------------------------------------------------------------------------------
 */
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "2416addr.h"
#include "2416lib.h"

#include "..\inc\Nand.h"
#include "..\inc\usbdev.h"

#include "..\inc\communication.h"
#include "..\inc\download.h"


extern void Uart_SendString(char *pt);
//#define DBG_USBD20
//#define STALL_CHECK
#ifndef		__NANDBL

#ifdef DBG_USBD20
    //#define Dbg printf
	#define DbgUsb(x) Dbg x
#else 	
	#define DbgUsb(x) 0;
	//#define DbgUsb(x) Uart_Printf(x)
#endif

//	#define USBUARTTEST 1

//extern U32 downloadAddress;
//extern download_run;
//extern volatile U8 dma_done;
//extern tempDownloadAddress;

U32 loop_count;

#define USBDEV_BASE 0x49800000


#define Outp32(addr, data) (*(volatile U32 *)(addr) = (data))
#define Outp16(addr, data) (*(volatile U16 *)(addr) = (data))
#define Outp8(addr, data)  (*(volatile U8 *)(addr) = (data))
#define Inp32(addr, data) (data = (*(volatile U32 *)(addr)))
#define Inp16(addr, data) (data = (*(volatile U16 *)(addr)))
#define Inp8(addr, data)  (data = (*(volatile U16 *)(addr)))
#define Input32(addr) (*(volatile U32 *)(addr))



#define	    USBD20_REGS		         U32
#define		INDEX_REG                (USBDEV_BASE+0x00) // Index register
#define		EP_INT_REG               (USBDEV_BASE+0x04) // EP Interrupt pending and clear
#define		EP_INT_EN_REG            (USBDEV_BASE+0x08) // EP Interrupt enable
#define		FUNC_ADDR_REG            (USBDEV_BASE+0x0c) // Function address
#define		FRAME_NUg_REG            (USBDEV_BASE+0x10) // Frame number
#define		EP_DIR_REG               (USBDEV_BASE+0x14) // Endpoint direction
#define		TEST_REG                 (USBDEV_BASE+0x18) // Test register
#define		SYS_STATUS_REG           (USBDEV_BASE+0x1c) // System status
#define		SYS_CON_REG              (USBDEV_BASE+0x20) // System control
#define		EP0_STATUS_REG           (USBDEV_BASE+0x24) // Endpoint 0 status
#define		EP0_CON_REG              (USBDEV_BASE+0x28) // Endpoint 0 control
#define		EP_STATUS_REG            (USBDEV_BASE+0x2c) // Endpoints status
#define		EP_CON_REG               (USBDEV_BASE+0x30) // Endpoints control
#define		BYTE_READ_CNT_REG        (USBDEV_BASE+0x34) // read count
#define		BYTE_WRITE_CNT_REG       (USBDEV_BASE+0x38) // write count
#define		MAX_PKT_REG              (USBDEV_BASE+0x3c) // Max packet size
#define		DMA_CON_REG              (USBDEV_BASE+0x40) // DMA control
#define		DMA_CNT_REG              (USBDEV_BASE+0x44) // DMA count
#define		DMA_FIFO_CNT_REG         (USBDEV_BASE+0x48) // DMA FIFO count
#define		DMA_TOTAL_CNT1_REG       (USBDEV_BASE+0x4c) // DMA Total count1
#define		DMA_TOTAL_CNT2_REG       (USBDEV_BASE+0x50) // DMA Total count2
#define		DMA_IF_CON_REG           (USBDEV_BASE+0x84) // DMA interface Control
#define		DMA_MEg_BASE_ADDR        (USBDEV_BASE+0x88) // Mem Base Addr
#define		DMA_MEg_CURRENT_ADDR     (USBDEV_BASE+0x8c) // Mem current Addr
#define		EP0_FIFO                 (USBDEV_BASE+0x60) // Endpoint 0 FIFO
#define		EP1_FIFO                 (USBDEV_BASE+0x64) // Endpoint 1 FIFO
#define		EP2_FIFO                 (USBDEV_BASE+0x68) // Endpoint 2 FIFO
#define		EP3_FIFO                 (USBDEV_BASE+0x6c) // Endpoint 3 FIFO
#define		EP4_FIFO                 (USBDEV_BASE+0x70) // Endpoint 4 FIFO
#define		FCON                     (USBDEV_BASE+0x100) // Burst Fifo Control


// Descriptor Types
#define DESC_TYPE	unsigned char

#define	DEVICE_TYPE  0x1
#define	CONFIGURATION_TYPE 	2
#define		STRING_TYPE		3
#define		INTERFACE_TYPE	4
#define		ENDPOINT_TYPE	5


// configuration descriptor: bmAttributes
#define DESC_CONF		unsigned char

#define	CONF_ATTR_DEFAULT        0x80 // Spec 1.0 it was BUSPOWERED bit.
#define	CONF_ATTR_REMOTE_WAKEUP  0x20
#define	CONF_ATTR_SELFPOWERED    0x40


// endpoint descriptor
#define DESC_ENDPT		unsigned char

#define		EP_ADDR_IN               0x80
#define		EP_ADDR_OUT            0x00

#define		EP_ATTR_CONTROL         0x0
#define		EP_ATTR_ISOCHRONOUS  0x1
#define		EP_ATTR_BULK            0x2
#define		EP_ATTR_INTERRUPT      0x3


#define EP_INDEX		unsigned char

#define	 EP0	0
#define	 EP1	1
#define	 EP2	2
#define	 EP3	3
#define	 EP4	4



typedef		struct __USB_DEVICE_DESCRIPTOR
{
	U8 bLength;
	U8 bDescriptorType;
	U8 bcdUSBL;
	U8 bcdUSBH;
	U8 bDeviceClass;
	U8 bDeviceSubClass;
	U8 bDeviceProtocol;
	U8 bMaxPacketSize0;
	U8 idVendorL;
	U8 idVendorH;
	U8 idProductL;
	U8 idProductH;
	U8 bcdDeviceL;
	U8 bcdDeviceH;
	U8 iManufacturer;
	U8 iProduct;
	U8 iSerialNumber;
	U8 bNumConfigurations;
}USB_DEVICE_DESCRIPTOR;

typedef		struct __USB_CONFIGURATION_DESCRIPTOR
{
	U8 bLength;
	U8 bDescriptorType;
	U8 wTotalLengthL;
	U8 wTotalLengthH;
	U8 bNumInterfaces;
	U8 bConfigurationValue;
	U8 iConfiguration;
	U8 bmAttributes;
	U8 maxPower;
}USB_CONFIGURATION_DESCRIPTOR;

typedef		struct __USB_INTERFACE_DESCRIPTOR
{
	U8 bLength;
	U8 bDescriptorType;
	U8 bInterfaceNumber;
	U8 bAlternateSetting;
	U8 bNumEndpoints;
	U8 bInterfaceClass;
	U8 bInterfaceSubClass;
	U8 bInterfaceProtocol;
	U8 iInterface;
}USB_INTERFACE_DESCRIPTOR;

typedef		struct __USB_ENDPOINT_DESCRIPTOR
{
	U8 bLength;
	U8 bDescriptorType;
	U8 bEndpointAddress;
	U8 bmAttributes;
	U8 wMaxPacketSizeL;
	U8 wMaxPacketSizeH;
	U8 bInterval;
}USB_ENDPOINT_DESCRIPTOR;

typedef		struct __USB_CONFIGURATION_SET
{
	U8 ConfigurationValue;
}USB_CONFIGURATION_SET;

typedef		struct __USB_GET_STATUS
{
	U8 Device;
	U8 Interface;
	U8 Endpoint0;
	U8 Endpoint1;
	U8 Endpoint3;
}USB_GET_STATUS;

USB_GET_STATUS oStatusGet;

typedef		struct __USB_INTERFACE_GET
{
	U8 AlternateSetting;
}USB_INTERFACE_GET;

USB_INTERFACE_GET oInterfaceGet;

typedef		struct __USB_DESCRIPTORS
{
	USB_CONFIGURATION_DESCRIPTOR oDescConfig;
	USB_INTERFACE_DESCRIPTOR oDescInterface;
	USB_ENDPOINT_DESCRIPTOR oDescEndpt1;
	USB_ENDPOINT_DESCRIPTOR oDescEndpt3;
	USB_ENDPOINT_DESCRIPTOR oDescEndpt2;
	USB_ENDPOINT_DESCRIPTOR oDescEndpt4;
}USB_DESCRIPTORS;

typedef		struct __DEVICE_REQUEST
{
	U8 bmRequestType;  // Device Request offset 0
	U8 bRequest;       // Device Request offset 1
	U8 wValue_L;       // Device Request offset 2
	U8 wValue_H;       // Device Request offset 3
	U8 wIndex_L;       // Device Request offset 4
	U8 wIndex_H;       // Device Request offset 5
	U8 wLength_L;      // Device Request offset 6
	U8 wLength_H;      // Device Request offset 7
}DEVICE_REQUEST;


typedef struct
{
	U32 ScrCount;
	U32 ScrPointer;
	U32 CurrentScr; 
	U32 CurrentLength;
	U32 CurrentTotalLength;
	U32 SDBPointer;
	U32 AddrCount;
	U32 ScrAddr[255];
	U8   SystemInfoErr;
	U8   ScreenInfoErr;

}USB_SCR_RES;

// EP0 CSR register Bits
#define EP0_SENT_STALL              (0x01<<4)
#define EP0_DATA_END                (0x01<<3)
#define EP0_SETUP_END               (0x03<<2)
#define EP0_TX_SUCCESS              (0x01<<1)
#define EP0_RX_SUCCESS              (0x01<<0)

//  Defines for Endpoint CSR Register Bits
#define DMA_TOTAL_COUNT_ZERO        (0x1<<9)
#define SHORT_PKT_RECEIVED          (0x1<<8)
#define EP_FIFO_FLUSH               (0x1<<6)
#define EP_SENT_STALL               (0x1<<5)
#define EP_TX_SUCCESS               (0x1<<1)
#define EP_RX_SUCCESS               (0x1<<0)


#define EP0_STATE_INIT              (0)
#define EP0_STATE_GD_DEV_0_0        (11)
//#define EP0_STATE_GD_DEV_0_1        (14)
#define EP0_STATE_GD_DEV_1          (12)
#define EP0_STATE_GD_DEV_2          (13)
#define EP0_STATE_GD_CFG_0          (21)
#define EP0_STATE_GD_CFG_1          (22)
#define EP0_STATE_GD_CFG_2          (23)
#define EP0_STATE_GD_CFG_3          (24)
#define EP0_STATE_GD_CFG_4          (25)
#define EP0_STATE_GD_CFG_ONLY_0     (41)
#define EP0_STATE_GD_CFG_ONLY_1     (42)
#define EP0_STATE_GD_IF_ONLY_0      (44)
#define EP0_STATE_GD_IF_ONLY_1      (45)
#define EP0_STATE_GD_EP0_ONLY_0     (46)
#define EP0_STATE_GD_EP1_ONLY_0     (47)
#define EP0_STATE_GD_EP2_ONLY_0     (48)
#define EP0_STATE_GD_EP3_ONLY_0     (49)
#define EP0_STATE_GD_STR_I0         (30)
#define EP0_STATE_GD_STR_I1         (31)
#define EP0_STATE_GD_STR_I2         (32)
#define EP0_STATE_GD_DEV_QUALIFIER  (33)
#define EP0_STATE_GD_DEV_QUALIFIER_1 (55)
#define EP0_INTERFACE_GET           (34)


#define EP0_GET_STATUS0             (35)
#define EP0_GET_STATUS1             (36)
#define EP0_GET_STATUS2             (37)
#define EP0_GET_STATUS3             (38)
#define EP0_GET_STATUS4             (39)
#define EP0_STATE_GD_DEV_OTHER_SPEED (40)
#define EP0_STATE_GD_DEV_OTHER_SPEED_1 (56)

// SPEC1.1
// Standard bmRequestType (Type)
#define STANDARD_TYPE               0x00
#define CLASS_TYPE                  0x20
#define VENDOR_TYPE                 0x40
#define RESERVED_TYPE               0x60

// Standard bmRequestType (Recipient)
#define DEVICE_RECIPIENT            0
#define INTERFACE_RECIPIENT         1
#define ENDPOINT_RECIPIENT          2
#define OTHER_RECIPIENT             3

// Standard bRequest codes
#define STANDARD_GET_STATUS         0
#define STANDARD_CLEAR_FEATURE      1
#define STANDARD_RESERVED_1         2
#define STANDARD_SET_FEATURE        3
#define STANDARD_RESERVED_2         4
#define STANDARD_SET_ADDRESS        5
#define STANDARD_GET_DESCRIPTOR     6
#define STANDARD_SET_DESCRIPTOR     7
#define STANDARD_GET_CONFIGURATION  8
#define STANDARD_SET_CONFIGURATION  9
#define STANDARD_GET_INTERFACE      10
#define STANDARD_SET_INTERFACE      11
#define STANDARD_SYNCH_FRAME        12

// Descriptor types
#define DEVICE_DESCRIPTOR           1
#define CONFIGURATION_DESCRIPTOR    2
#define STRING_DESCRIPTOR           3
#define INTERFACE_DESCRIPTOR        4
#define ENDPOINT_DESCRIPTOR         5
#define DEVICE_QUALIFIER            6
#define OTHER_SPEED_CONFIGURATION   7

// string descriptor
#define LANGID_US_L                 (0x09)
#define LANGID_US_H                 (0x04)

// USB Endpoints states
#define EP0_STATE_IDLE              (0)
#define EP0_STATE_TRANSFER          (1)
#define EP0_STATE_RECEIVER          (2)

#define BULK_OUT_STATUS_NOSTALL     (0x0000)
#define BULK_OUT_STATUS_STALL       (0x0001)

#define DEVICE_STATUS_DEFAULT       (0x0000)
#define DEVICE_STATUS_SELFPOWERED   (0x0001)
#define DEVICE_STATUS_REMOTEWAKEUP  (0x0002)


#define DEVICE_DESC_SIZE            18
#define STRING_DESC0_SIZE           4
#define STRING_DESC1_SIZE           22
#define STRING_DESC2_SIZE           44
#define CONFIG_DESC_TOTAL_SIZE      32
#define CONFIG_DESC_SIZE            9
#define INTERFACE_DESC_SIZE         9
#define ENDPOINT_DESC_SIZE          7
#define DEVICE_QUALIFIER_SIZE       10
#define OTHER_SPEED_CONFIGURATION_SIZE 9

// INT_REG status value
#define INT_ERR                     (0xff80)
#define INT_REG_ERROR               (0xff1<<6)
#define INT_REG_VBUS                (0x1<<8)
#define INT_REG_VBUS_CLEAR          (0x1<<6)
#define INT_REG_HSP                 (0x1<<4)
#define INT_REG_SDE                 (0x1<<3)
#define INT_REG_RESET               (0x1)
#define INT_REG_RESUME              (0x1<<2)
#define INT_REG_SUSPEND             (0x1<<1)
#define INT_REG_EP4                 (0x1<<4)
#define INT_REG_EP3                 (0x1<<3)
#define INT_REG_EP2                 (0x1<<2)
#define INT_REG_EP1                 (0x1<<1)
#define INT_REG_EP0                 (0x1)
#define INT_DTB_MISMATCH            (0x1FF<<7)

// USB Dma Operation
#define DMA_AUTO_RX_DISABLE         (0x1<<5)
#define DMA_FLY_ENABLE              (0x1<<4)
#define DMA_FLY_DISABLE             (0x0<<4)
#define DMA_DEMEND_ENABLE           (0x1<<3)
#define DMA_DEMEND_DISABLE          (0x0<<3)
#define DMA_TX_START                (0x1<<2)
#define DMA_TX_STOP                 (0x0<<2)
#define DMA_RX_START                (0x1<<1)
#define DMA_RX_STOP                 (0x0<<1)
#define USB_DMA_MODE                (0x1<<0)
#define USB_INT_MODE                (0x0<<0)

#define MAX_BURST_INCR16            (0x3<<0)
#define MAX_BURST_INCR8             (0x2<<0)
#define MAX_BURST_INCR4             (0x1<<0)

#define DMA_ENABLE                  (0x1<<8)
#define DMA_DISABLE                 (0x0<<8)


// Feature Selectors
	#define EP_STALL          		0
	#define DEVICE_REMOTE_WAKEUP    1
	#define TEST_MODE				2

	/* Test Mode Selector*/
	#define TEST_J					1
	#define TEST_K					2
	#define TEST_SE0_NAK			3
	#define TEST_PACKET				4
	#define TEST_FORCE_ENABLE		5

	#define		TR_RSVD					0x4F20
	#define		TR_VBUS					0x8000
	#define		TR_EUERR				0x2000
	#define		TR_PERR					0x1000
	#define		TR_SPDSEL				0x00C0
	#define		TR_TMD					0x0010
	#define		TR_TPS					0x0008
	#define		TR_TKS					0x0004
	#define		TR_TJS					0x0002
	#define		TR_TSNS					0x0001


/*********************************************************************
						DEVICE QUALIFIER
**********************************************************************/
volatile const U8 aDeviceQualifierDescriptor[] =
{
	0x0a,                   //  0 desc size
	0x06,                   //  1 desc type (DEVICE)
	0x00,                   //  2 USB release
	0x02,                   //  3 => 2.00
	0x00,                   //  4 class
	0x00,                   //  5 subclass
	0x00,                   //  6 protocol
	64,          			//  7 max pack size
	0x01,                   //  8 number of other-speed configuration
	0x00,                   //  9 reserved
};

volatile const U8 aDeviceQualifierDescriptor_1[] =
{
	0x04,                   //  0 desc size
	0x06,                   //  1 desc type (DEVICE)
	0x00,                   //  2 USB release
	0x02,                   //  3 => 2.00
	0x00,                   //  4 class
	0x00,                   //  5 subclass
	0x00,                   //  6 protocol
	64,          			//  7 max pack size
	0x01,                   //  8 number of other-speed configuration
	0x00,                   //  9 reserved
};

volatile const U8 aOtherSpeedConfiguration[] =
{
	0x09,                   //  0 desc size
	0x07,                   //  1 desc type (other speed)
	0x20,                   //  2 Total length of data returned
	0x00,                   //  3 
	0x01,                   //  4 Number of interfaces supported by this speed configuration
	0x01,                   //  5 value to use to select configuration
	0x00,                   //  6 index of string desc
	CONF_ATTR_DEFAULT|CONF_ATTR_SELFPOWERED,//|CONF_ATTR_REMOTE_WAKEUP,   //  7 same as configuration desc
	0x25,                   //  8 same as configuration desc
	
};

volatile const U8 aOtherSpeedConfiguration_1[] =
{
  0x09, 0x07 ,0x20 ,0x00 ,0x01 ,0x01 ,0x00 ,0x80 ,0x19
  
 ,0x09 ,0x04 ,0x00 ,0x00 ,0x02 ,0xff ,0x00 ,0x00 ,0x00 
  
 ,0x07 ,0x05 ,0x82 ,0x02 ,0x40 ,0x00 ,0x00 
  
 ,0x07 ,0x05 ,0x04 ,0x02 ,0x40 ,0x00 ,0x00 
};

volatile const U8 aOtherSpeedConfiguration_2[] =
{
  0x09, 0x07 ,0x20 ,0x00 ,0x01 ,0x01 ,0x00 ,0x80 ,0x19
  
 ,0x09 ,0x04 ,0x00 ,0x00 ,0x02 ,0xff ,0x00 ,0x00 ,0x00 
  
 ,0x07 ,0x05 ,0x82 ,0x02 ,0x00 ,0x02 ,0x00 
  
 ,0x07 ,0x05 ,0x04 ,0x02 ,0x00 ,0x02 ,0x00 
};

 U8 aDescStr0[]=
{
	4, STRING_DESCRIPTOR, LANGID_US_L, LANGID_US_H
};

U8 aDescStr1[]= // Manufacturer
{
	//(0x14+2), STRING_TYPE,
	22, STRING_TYPE,
	'S', 0x0, 'y', 0x0, 's', 0x0, 't', 0x0, 'e', 0x0, 'm', 0x0, ' ', 0x0, 'M', 0x0,
	'C', 0x0, 'U', 0x0
};

U8 aDescStr2[]= // Product
{
	//(0x2a+2), STRING_TYPE,
	44, STRING_TYPE,
	'S', 0x0, 'E', 0x0, 'C', 0x0, ' ', 0x0, 'S', 0x0, '3', 0x0, 'C', 0x0, '2', 0x0,
	'4', 0x0, '5', 0x0, '0', 0x0, 'X', 0x0, ' ', 0x0, 'T', 0x0, 'e', 0x0, 's', 0x0,
	't', 0x0, ' ', 0x0, 'B', 0x0, '/', 0x0, 'D', 0x0
};

volatile USB_OP g_eOpMode;
volatile	U8*  g_pDownPt;
volatile	U32  g_uDownloadFileSize;
volatile	U32  g_uDownloadAddress;
U32  g_uFileTotalLen;
U32  g_uDownloadCount;
U32  g_cpucnt;
U16  usConfig;

USB_SCR_RES g_uSCR;
USB_DEVICE_DESCRIPTOR* g_poDescDevice;
USB_GET_STATUS* g_poStatusGet;
USB_INTERFACE_GET* g_poInterfaceGet;
USB_DESCRIPTORS* g_poDesc;
DEVICE_REQUEST* g_poDeviceRequest;

U32  g_uEp0State;
U32  g_uEp0SubState;	
USB_SPEED g_eSpeed;
//	U32  g_other_speed_length;
U32  g_uBulkInCount;
U32  g_uEp0MaxPktSize;
U32  g_uEp1MaxPktSize;
U32  g_uEp3MaxPktSize;	
U32  g_uEp2MaxPktSize;
U32  g_uEp4MaxPktSize;	
U32  g_uBulkInAddr;
U32  g_uEnumerationDone;

volatile U32 DeviceRequestLength;

#ifdef USBDMA
	static USB_OP eOpMode = USB_DMA;
#else
	static USB_OP eOpMode = USB_CPU;
#endif

U8	Ep3Buf[1024];

extern INT8U  ScreenDataBuffer[620000];
extern INT16U IndexBlock;
extern INT16U IndexBlockOffset;
extern INT8U IndexBlockBuffer[PAGE_SIZE];

extern void MMU_EnableICache(void); 


void Usb_Init(void)
{


	rMISCCR=rMISCCR&~(1<<12);  // USBD is 0 ,normal mode ,1 is suspend mode /

	rPWRCFG |= (0x1<<4); // phy power enable 
	rUSB_PHYCTRL = (0<<3)|(1<<2)|(0<<1)|(0<<0);

    	rUSB_PHYPWR = (0x0<<31)|(0x3<<4)|(0x0<<3)|(0x0<<2)|(0x0<<1)|(0x0<<0);
    //48Mhz clock on ,PHY2.0 analog block power on,XO block power on,XO block power in suspend mode,PHY 2.0 Pll power on ,suspend signal for save mode disable 
 	
	rUSB_CLKCON = (0x1<<1)|(0x1<<2)|(0x0<<1)|(0x0<<0); // vbus detect enable...
    //D+ pull up , USB2.0 Function clock Enable, USB1.1 HOST disable,USB2.0 PHY test enable

	pISR_USBD =(unsigned)Isr_Usb;	
    
	USBdInit();
	
	ClearPending(BIT_USBD);
	rINTMSK1 |= (BIT_USBD);   // usb interrupt disable 

	//eOpMode =USB_DMA;
	eOpMode =USB_CPU;


}

//static void __irq Isr_Usb(void)
void __irq Isr_Usb(void)
{
	//rINTMSK1 |=(BIT_USBD);
	//g_uDownloadFileSize = 0;
	HandleEvent();
	ClearPending(BIT_USBD);
	
	//rINTMSK1 &= ~(BIT_USBD);
}

void Usbdmain(void)
{
	SetOpMode(eOpMode);
	//WrPktEp0(aDescStr1, 22);
}

void WrPktEp0(U8 *buf, int num)
{
	int i;
	if (num&0x1) num++;
	
	for(i=0;i<num;i+=2)
	{
		Outp32(EP0_FIFO, (((*(buf+1))<<8)|*buf));
		buf +=2;
	}

}

void WrPktEp1(U8 *buf, int num)
{
	int i;
	U16 Wr_Data=0;

	for(i=0;i<num;i+=2)
	{
		Wr_Data=((*(buf+1))<<8)|*buf;
	   Outp32(EP1_FIFO, Wr_Data);
	   buf +=2;
	}
}

void PrintEp0Pkt(U8 *pt, U8 count)
{
	int i;
//	DbgUsb(("[DBG:"));
	for(i=0;i<count;i++)
	{

		Unitel_SendByte(DataToChar((pt[i]>>4) & 0x000F));

		Unitel_SendByte(DataToChar(pt[i] & 0x000F));
	}
//		DbgUsb(("%x,", pt[i]));
//	DbgUsb(("]"));
}

void RdPktEp3(U8 *buf, int num)
{
	int i;
	U16 Rdata;

	for(i=0;i<num;i+=2)
	{
		Inp32(EP3_FIFO, Rdata);
		buf[i] = (U8)Rdata;
		buf[i+1] = (U8)(Rdata>>8);
	}

	//g_pDownPt += num;
	g_uDownloadFileSize -= num;
}

BOOL IsEnumerationDone(void)
{
	if (g_uEnumerationDone == 0)
		return FALSE;
	else
		return TRUE;
}

void PrepareEp1Fifo(U32 BaseAddr)
{
	//int i;
	//U32 in_csr1;
	U8* BulkInBuf = (U8*)BaseAddr;

	if (g_uBulkInCount > g_uEp1MaxPktSize)
	{
		Outp32(INDEX_REG, EP1);
		Outp32(BYTE_WRITE_CNT_REG, g_uEp1MaxPktSize);
		WrPktEp1(BulkInBuf, g_uEp1MaxPktSize);

		g_uBulkInAddr = BaseAddr + g_uEp1MaxPktSize;
		g_uBulkInCount -= g_uEp1MaxPktSize;
	}
	else
	{
		Outp32(INDEX_REG, EP1);
		//printf("g_uBulkInCount= %d\n",g_uBulkInCount);
		Outp32(BYTE_WRITE_CNT_REG, g_uBulkInCount);
		WrPktEp1(BulkInBuf, g_uBulkInCount);

		g_uBulkInAddr = BaseAddr + g_uBulkInCount;
		g_uBulkInCount = 0;
	}


}

void FlushEp1Fifo(void)
{
	U32 OrgValue;
	Outp32(INDEX_REG, EP1);
	Inp32(EP_CON_REG, OrgValue);
	Outp32(EP_CON_REG, (1<<6));
	Outp32(EP_CON_REG, OrgValue);
}

void SetMaxPktSizes(USB_SPEED eSpeed)
{
	if (eSpeed == USB_HIGH)
	{
		g_eSpeed = USB_HIGH;
		g_uEp0MaxPktSize = 64;
		g_uEp1MaxPktSize = 512;
		g_uEp3MaxPktSize = 512;
		g_uEp2MaxPktSize = 64;
		g_uEp4MaxPktSize = 64;
		
	}
	else
	{
	
		g_eSpeed = USB_FULL;
		g_uEp0MaxPktSize = 64;
		g_uEp1MaxPktSize = 64;
		g_uEp3MaxPktSize = 64;
		g_uEp2MaxPktSize = 512;
		g_uEp4MaxPktSize = 512;
		
		
	}
	// EP0 Max Packet size settings
	Outp32(INDEX_REG, EP0);
	Outp32(MAX_PKT_REG, g_uEp0MaxPktSize); 	// max packet size

	// EP1 OUT Max Packet size settings
	Outp32(INDEX_REG, EP1);
	Outp32(MAX_PKT_REG, g_uEp1MaxPktSize); 	// max packet size

	// EP3 IN Max Packet size settings
	Outp32(INDEX_REG, EP3);
	Outp32(MAX_PKT_REG, g_uEp3MaxPktSize);	// max packet size
		
	// EP2 OUT Max Packet size settings
	Outp32(INDEX_REG, EP2);
	Outp32(MAX_PKT_REG, g_uEp2MaxPktSize); 	// max packet size

	// EP4 IN Max Packet size settings
	Outp32(INDEX_REG, EP4);
	Outp32(MAX_PKT_REG, g_uEp4MaxPktSize);	// max packet size
}

void SetOpMode(unsigned char mode)
{
	//U32 i;
	g_uDownloadFileSize = 0;
	g_eOpMode = mode;
}

BOOL VerifyChecksum(void)
{
	U8* CalcCSPt;
	U16 dnCS;
	U16 checkSum;
    U16 i=0;
    
	// checksum calculation
	CalcCSPt = (U8*)g_uDownloadAddress;
	checkSum = 0;
	while((U32)CalcCSPt < (g_uDownloadAddress+(g_uDownloadFileSize-8)))
	{
		checkSum += *CalcCSPt++;
		i++;
	}
	// checkSum was calculated including dnCS. So, dnCS should be subtracted.
	checkSum=checkSum - *((unsigned char *)(g_uDownloadAddress+g_uDownloadFileSize-8-2))
		- *( (unsigned char *)(g_uDownloadAddress+g_uDownloadFileSize-8-1) );

	dnCS=*((unsigned char *)(g_uDownloadAddress+g_uDownloadFileSize-8-2))+
		(*( (unsigned char *)(g_uDownloadAddress+g_uDownloadFileSize-8-1) )<<8);

	if (checkSum!=dnCS)
	{
		//printf("Checksum Error!!! MEM:%x DN:%x\n", checkSum, dnCS);
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

void GetDownFileInfo(U32* uDownAddr, U32* uDownFileSize)
{
	*uDownAddr = g_uDownloadAddress;
	*uDownFileSize = g_uDownloadFileSize;
}

void AllocateUSBDEV(void)
{
	g_poDescDevice = (USB_DEVICE_DESCRIPTOR*)malloc(sizeof (USB_DEVICE_DESCRIPTOR));
	g_poStatusGet = (USB_GET_STATUS*)malloc(sizeof (USB_GET_STATUS));
	g_poInterfaceGet = (USB_INTERFACE_GET*)malloc(sizeof (USB_INTERFACE_GET));
	g_poDesc = (USB_DESCRIPTORS*)malloc(sizeof (USB_DESCRIPTORS));
	g_poDeviceRequest = (DEVICE_REQUEST*)malloc(sizeof (DEVICE_REQUEST));
}

void FreeUSBDEV(void)
{
	if (g_poDescDevice)
		free(g_poDescDevice);
	if (g_poStatusGet)
		free(g_poStatusGet);
	if (g_poInterfaceGet)
		free(g_poInterfaceGet);
	if (g_poDesc)
		free(g_poDesc);
	if (g_poDeviceRequest)
		free(g_poDeviceRequest);
}


void USBdInit(void)
{

	g_uEnumerationDone = 0;
	//SetEndpoint();
	SetEndpoint();
	g_uEp0State = EP0_STATE_INIT;
	g_uEp0SubState = 0;
}

void SetDescriptorTable(void)
{     
	// Standard device descriptor
	g_poDescDevice->bLength=0x12;	// EP0_DEV_DESC_SIZE=0x12 bytes
	g_poDescDevice->bDescriptorType=DEVICE_TYPE;
	g_poDescDevice->bDeviceClass=0xFF; // 0x0  0xff 由供应商自定义
	g_poDescDevice->bDeviceSubClass=0x0;
	g_poDescDevice->bDeviceProtocol=0x0;
	g_poDescDevice->bMaxPacketSize0=g_uEp0MaxPktSize;

	g_poDescDevice->idVendorL=0x45;//0xe8;//0x51;//0x47;//0xe8;     //changed the value 06.10.16
	g_poDescDevice->idVendorH=0x53;//0x4;//0x09;//0x05;//0x04;     //changed the value 06.10.16

	g_poDescDevice->idProductL=0x34;//0x03;//0x20;//0x34;
	g_poDescDevice->idProductH=0x12;//0x16;//0x27;//0x12;

	g_poDescDevice->bcdDeviceL=0x00;
	g_poDescDevice->bcdDeviceH=0x01;
	g_poDescDevice->iManufacturer=0x1; // index of string descriptor
	g_poDescDevice->iProduct=0x2;	// index of string descriptor
	g_poDescDevice->iSerialNumber=0x0;
	g_poDescDevice->bNumConfigurations=0x1;

	g_poDescDevice->bcdUSBL=0x00;
	g_poDescDevice->bcdUSBH=0x02; 	// Ver 2.0

	// Standard configuration descriptor
	g_poDesc->oDescConfig.bLength=0x9;
	g_poDesc->oDescConfig.bDescriptorType=CONFIGURATION_TYPE;
	g_poDesc->oDescConfig.wTotalLengthL=0x20; // <cfg desc>+<if desc>+<endp0 desc>+<endp1 desc>
	g_poDesc->oDescConfig.wTotalLengthH=0;
	g_poDesc->oDescConfig.bNumInterfaces=1;
	// dbg    descConf.bConfigurationValue=2; // why 2? There's no reason.
	g_poDesc->oDescConfig.bConfigurationValue=1;
	g_poDesc->oDescConfig.iConfiguration=0;
	g_poDesc->oDescConfig.bmAttributes=CONF_ATTR_DEFAULT;//CONF_ATTR_DEFAULT|CONF_ATTR_SELFPOWERED;//;|CONF_ATTR_REMOTE_WAKEUP;//CONF_ATTR_DEFAULT; // bus powered only.
	g_poDesc->oDescConfig.maxPower=50; // draws 50mA current from the USB bus.
	//g_poDesc->oDescConfig.maxPower=50; // draws 100mA current from the USB bus.

	// Standard interface descriptor
	g_poDesc->oDescInterface.bLength=0x9;
	g_poDesc->oDescInterface.bDescriptorType=INTERFACE_TYPE;
	g_poDesc->oDescInterface.bInterfaceNumber=0x0;
	g_poDesc->oDescInterface.bAlternateSetting=0x0; // ?
	g_poDesc->oDescInterface.bNumEndpoints=2;	// # of endpoints except EP0
	g_poDesc->oDescInterface.bInterfaceClass=0xff;//0xff;//0x8;//0xff; // 0x0 ?
	g_poDesc->oDescInterface.bInterfaceSubClass=0x00;//0x00;//0x06;//0x0;
	g_poDesc->oDescInterface.bInterfaceProtocol=0x00;//0x00;//0x50;//0x0;
	g_poDesc->oDescInterface.iInterface=0x0;

	// Standard endpoint0 descriptor
	g_poDesc->oDescEndpt1.bLength=0x7;
	g_poDesc->oDescEndpt1.bDescriptorType=ENDPOINT_TYPE;
	g_poDesc->oDescEndpt1.bEndpointAddress=1|EP_ADDR_IN; // 2400Xendpoint 1 is IN endpoint.
	g_poDesc->oDescEndpt1.bmAttributes=EP_ATTR_BULK;
	g_poDesc->oDescEndpt1.wMaxPacketSizeL=(U8)g_uEp1MaxPktSize; // 64
	g_poDesc->oDescEndpt1.wMaxPacketSizeH=(U8)(g_uEp1MaxPktSize>>8);
	g_poDesc->oDescEndpt1.bInterval=0x0; // not used

	// Standard endpoint1 descriptor
	g_poDesc->oDescEndpt3.bLength=0x7;
	g_poDesc->oDescEndpt3.bDescriptorType=ENDPOINT_TYPE;
	g_poDesc->oDescEndpt3.bEndpointAddress=3|EP_ADDR_OUT; // 2400X endpoint 3 is OUT endpoint.
	g_poDesc->oDescEndpt3.bmAttributes=EP_ATTR_BULK;
	g_poDesc->oDescEndpt3.wMaxPacketSizeL=(U8)g_uEp3MaxPktSize; // 64
	g_poDesc->oDescEndpt3.wMaxPacketSizeH=(U8)(g_uEp3MaxPktSize>>8);
	g_poDesc->oDescEndpt3.bInterval=0x0; // not used
}

void Set_OtherSpeedConf_DescriptorTable(U32 length)
{
	// Standard device descriptor
	if (g_eSpeed == USB_HIGH) 
	{
		g_poDescDevice->bcdUSBL=0x00;
		g_poDescDevice->bcdUSBH=0x02; 	// Ver 2.0
		
		g_poDesc->oDescConfig.bDescriptorType=OTHER_SPEED_CONFIGURATION;
	
		// Standard endpoint2 descriptor
		g_poDesc->oDescEndpt2.bLength=0x7;
		g_poDesc->oDescEndpt2.bDescriptorType=ENDPOINT_TYPE;
		g_poDesc->oDescEndpt2.bEndpointAddress=2|EP_ADDR_IN; // 2400Xendpoint 1 is IN endpoint.
		g_poDesc->oDescEndpt2.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt2.wMaxPacketSizeL=(U8)g_uEp2MaxPktSize; // 
		g_poDesc->oDescEndpt2.wMaxPacketSizeH=(U8)(g_uEp2MaxPktSize>>8);
		g_poDesc->oDescEndpt2.bInterval=0x0; // not used

		// Standard endpoint4 descriptor
		g_poDesc->oDescEndpt4.bLength=0x7;
		g_poDesc->oDescEndpt4.bDescriptorType=ENDPOINT_TYPE;
		g_poDesc->oDescEndpt4.bEndpointAddress=4|EP_ADDR_OUT; // 2400X endpoint 3 is OUT endpoint.
		g_poDesc->oDescEndpt4.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt4.wMaxPacketSizeL=(U8)g_uEp4MaxPktSize; // 
		g_poDesc->oDescEndpt4.wMaxPacketSizeH=(U8)(g_uEp4MaxPktSize>>8);
		g_poDesc->oDescEndpt4.bInterval=0x0; // not used	
	
	    if (length ==9)
	    {
	    	Outp32(BYTE_WRITE_CNT_REG, 9);
			WrPktEp0((U8 *)&g_poDesc->oDescConfig+0, 9); // EP0_PKT_SIZE
		}
	    else if(length ==32)
		{
	    
	    	Outp32(BYTE_WRITE_CNT_REG, 32);
			WrPktEp0((U8 *)&aOtherSpeedConfiguration_1+0, 32); // EP0_PKT_SIZE
		 			
	    }
		g_uEp0State = EP0_STATE_INIT;
	
		g_poDescDevice->bcdUSBL=0x00;
		g_poDescDevice->bcdUSBH=0x02; 	// Ver 2.0
	
		g_poDesc->oDescConfig.bDescriptorType=CONFIGURATION_TYPE;
		// Standard endpoint0 descriptor
		g_poDesc->oDescEndpt1.bEndpointAddress=1|EP_ADDR_IN; // 2400Xendpoint 1 is IN endpoint.
		g_poDesc->oDescEndpt1.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt1.wMaxPacketSizeL=(U8)g_uEp1MaxPktSize; // 
		g_poDesc->oDescEndpt1.wMaxPacketSizeH=(U8)(g_uEp1MaxPktSize>>8);
		g_poDesc->oDescEndpt1.bInterval=0x0; // not used

		// Standard endpoint1 descriptor
		g_poDesc->oDescEndpt3.bEndpointAddress=3|EP_ADDR_OUT; // 2400X endpoint 3 is OUT endpoint.
		g_poDesc->oDescEndpt3.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt3.wMaxPacketSizeL=(U8)g_uEp3MaxPktSize; // 
		g_poDesc->oDescEndpt3.wMaxPacketSizeH=(U8)(g_uEp3MaxPktSize>>8);
		g_poDesc->oDescEndpt3.bInterval=0x0; // not used	
	
	}
	else {
		g_poDescDevice->bcdUSBL=0x00;
		g_poDescDevice->bcdUSBH=0x02; 	// Ver 2.0
	
		//g_poDesc->oDescConfig.bDescriptorType=CONFIGURATION_TYPE;
		g_poDesc->oDescConfig.bDescriptorType=OTHER_SPEED_CONFIGURATION;
		
		
		
		g_poDesc->oDescEndpt2.bLength=0x7;
		g_poDesc->oDescEndpt2.bDescriptorType=ENDPOINT_TYPE;
		g_poDesc->oDescEndpt2.bEndpointAddress=2|EP_ADDR_IN; // 2400Xendpoint 1 is IN endpoint.
		g_poDesc->oDescEndpt2.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt2.wMaxPacketSizeL=(U8)g_uEp2MaxPktSize; // 
		g_poDesc->oDescEndpt2.wMaxPacketSizeH=(U8)(g_uEp2MaxPktSize>>8);
		g_poDesc->oDescEndpt2.bInterval=0x0; // not used

		// Standard endpoint1 descriptor
		g_poDesc->oDescEndpt4.bLength=0x7;
		g_poDesc->oDescEndpt4.bDescriptorType=ENDPOINT_TYPE;
		g_poDesc->oDescEndpt4.bEndpointAddress=4|EP_ADDR_OUT; // 2400X endpoint 3 is OUT endpoint.
		g_poDesc->oDescEndpt4.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt4.wMaxPacketSizeL=(U8)g_uEp4MaxPktSize; // 
		g_poDesc->oDescEndpt4.wMaxPacketSizeH=(U8)(g_uEp4MaxPktSize>>8);
		g_poDesc->oDescEndpt4.bInterval=0x0; // not used	

	    if (length ==9)
	    {
	    	Outp32(BYTE_WRITE_CNT_REG, 9);
			WrPktEp0((U8 *)&g_poDesc->oDescConfig+0, 9); // EP0_PKT_SIZE
		}
	    else if(length ==32)
		{
	    
	    	Outp32(BYTE_WRITE_CNT_REG, 32);
			WrPktEp0((U8 *)&aOtherSpeedConfiguration_2+0, 32); // EP0_PKT_SIZE
		 			
	    }
	    
	    g_poDescDevice->bcdUSBL=0x00;
		g_poDescDevice->bcdUSBH=0x02; 	// Ver 2.0
		
		g_poDesc->oDescConfig.bDescriptorType=CONFIGURATION_TYPE;
		
		// Standard endpoint0 descriptor
		g_poDesc->oDescEndpt1.bEndpointAddress=1|EP_ADDR_IN; // 2400Xendpoint 1 is IN endpoint.
		g_poDesc->oDescEndpt1.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt1.wMaxPacketSizeL=(U8)g_uEp1MaxPktSize; // 
		g_poDesc->oDescEndpt1.wMaxPacketSizeH=(U8)(g_uEp1MaxPktSize>>8);
		g_poDesc->oDescEndpt1.bInterval=0x0; // not used

		// Standard endpoint1 descriptor
		g_poDesc->oDescEndpt3.bEndpointAddress=3|EP_ADDR_OUT; // 2400X endpoint 3 is OUT endpoint.
		g_poDesc->oDescEndpt3.bmAttributes=EP_ATTR_BULK;
		g_poDesc->oDescEndpt3.wMaxPacketSizeL=(U8)g_uEp3MaxPktSize; // 
		g_poDesc->oDescEndpt3.wMaxPacketSizeH=(U8)(g_uEp3MaxPktSize>>8);
		g_poDesc->oDescEndpt3.bInterval=0x0; // not used	
	
	
	}
}



void SetEndpoint(void)
{
	// *** End point information ***
	// EP0: control
	//U16 SysStatus;
	U16 Temp;

	Outp32(INDEX_REG, EP0);
	// For L18
	Outp32(EP_DIR_REG, 0x02); 		// EP1=> TX, EP2=>RX , 0b=report mode[1], 15b=report mode[2], 3b~8b=ep2 delay_con
	//Outp32(EP_INT_EN_REG, 0x07); 	// EP0, 1, 2 Interrupt enable, 15b=report mode[0], 3b~14b=ep0/1 delay_con
	Outp32(EP_INT_EN_REG, 0x4d0f);

	Inp32(EP_DIR_REG, Temp);
//	DbgUsb(("EP_DIR_REG : %x \n", Temp));
	Inp32(EP_INT_EN_REG, Temp);
//	DbgUsb(("EP_INT_EN_REG : %x \n", Temp));

	Outp32(SYS_CON_REG, 0x4123);		// error interrupt enable, 16bit bus, Little format, suspend&reset enable
	Outp32(EP0_CON_REG, 0x0000);
	Inp32(EP0_CON_REG,Temp);

	// EP1 OUT Max Packet size settings
	Outp32(INDEX_REG, EP1);
	Outp32(MAX_PKT_REG, g_uEp1MaxPktSize); 	// max packet size 512 bytes
	Outp32(EP_CON_REG, 0x0080); // dual enable

	// EP3 IN Max Packet size settings
	Outp32(INDEX_REG, EP3);
	Outp32(MAX_PKT_REG, g_uEp3MaxPktSize); 	// max packet size 512 bytes
	Outp32(EP_CON_REG, 0x0080);    		// dual enable

	
	Outp32(INDEX_REG, EP0);
	
}


void HandleEvent(void)
{
	U32 uStatus;
	U32 Temp;//,k;
	U16 ep_int_status, ep_int;

	Inp32(SYS_STATUS_REG, uStatus); // System status read
	//DbgUsb(("SYS_STATUS_REG : %x \n", uStatus));

	if (uStatus & INT_REG_VBUS)
	{
		Outp32(SYS_STATUS_REG, INT_REG_VBUS); // Interrupt Clear
		
		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  INT_REG_VBUS\n"));
		#endif
		
	}

	if (uStatus & 0xfc00) 	 // Error interrupt check
	{
		
		Inp32(SYS_STATUS_REG , Temp);
		Outp32(SYS_STATUS_REG, INT_ERR); // Interrupt Clear
		
	}

	if (uStatus & INT_REG_SUSPEND)
	{
		Outp32(SYS_STATUS_REG, INT_REG_SUSPEND); // Interrupt Clear

		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Suspend Mode\n"));
		#endif		
		//DbgUsb(("\n [USB_Diag_Log]  : Suspend Mode"));
		
	}

	if (uStatus & INT_REG_RESUME)
	{
		Outp32(SYS_STATUS_REG, INT_REG_RESUME); // Host software send ClearPortFeature. Interrupt Clear

		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Resume Mode\n"));
		#endif			
		//DbgUsb(("\n [USB_Diag_Log]  : Resume Mode \n"));
		
	}

	if (uStatus & INT_REG_RESET) // Reset interrupt
	{
		Outp32(SYS_STATUS_REG, INT_REG_RESET); // Interrupt Clear
		SetEndpoint();
		g_uEp0State = EP0_STATE_INIT;
		//DbgUsb(("\n [USB_Diag_Log]  : Reset Mode \n"));
		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Reset Mode\n"));
		#endif		
	
		
	}

	if (uStatus & INT_REG_SDE) // Device Speed Detection interrupt
	{
		Outp32(SYS_STATUS_REG, INT_REG_SDE); // Interrupt Clear
		//DbgUsb(("\n [USB_Diag_Log]  : Speed Detection interrupt \n"));
		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Speed Detection interrupt\n"));
		#endif		

		if (uStatus & INT_REG_HSP) // Set if Device is High speed or Full speed
		{
			Outp32(SYS_STATUS_REG, INT_REG_HSP); // High Speed Device Interrupt Clear?? may be not.
			//DbgUsb(("\n [USB_Diag_Log]  : High Speed Detection\n"));
			#ifdef USBUARTTEST
				Uart_SendString(("\n [USB_Diag_Log]  :  High Speed Detection\n"));
			#endif			
			SetMaxPktSizes(USB_HIGH);
			SetDescriptorTable();
		}
		else
		{
			SetMaxPktSizes(USB_FULL);
			SetDescriptorTable();
		}
	}
	else
	{
		if(uStatus & INT_REG_HSP)
		{
			//Outp32(SYS_STATUS_REG,INT_REG_HSP);
			Outp32(SYS_STATUS_REG,uStatus);
			Inp32(SYS_STATUS_REG,uStatus);
			//SetMaxPktSizes(USB_HIGH);
			//SetDescriptorTable();

			//rUSB_CLKCON &= ~(1<<31);
		}
	}

	
    
	Inp32(TEST_REG, Temp); // EP interrrupt status read		
	//DbgUsb(("TEST_REG : %x \n", Temp));
	  
	Inp32(EP_STATUS_REG, ep_int_status); // EP interrrupt status read		
	//DbgUsb(("EP_STATUS_REG : %x \n", ep_int_status));
	
	Inp32(EP_INT_REG, ep_int);	
	//DbgUsb(("EP_INT_REG : %x \n", ep_int));

	if (ep_int & INT_REG_EP0)
	{

		Outp32(EP_INT_REG, INT_REG_EP0); // Interrupt Clear
		HandleEvent_EP0();
		#ifdef USBUARTTEST
 			Uart_SendString(("\n [USB_Diag_Log]  :  Control Transfer Interrupt \n"));
		#endif		
	    
	}

	// Endpoint1 bulkIn
	else if (ep_int & INT_REG_EP1)
	{
		Outp32(EP_INT_REG, INT_REG_EP1); // Interrupt Clear
		HandleEvent_BulkIn();

		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Ep1 Interrupt  \n"));
		#endif
	}

	// Endpoint3 bulkOut
	else if (ep_int & INT_REG_EP3)
	{
		Outp32(EP_INT_REG, INT_REG_EP3); // Interrupt Clear

		//g_uDownloadFileSize =0;
		HandleEvent_BulkOut();

		#ifdef USBUARTTEST
			Uart_SendString(("\n [USB_Diag_Log]  :  Bulk Out Transfer Interrupt  \n"));
		#endif
	
	}

}

// ======================================================================
// endpointZeroFunction()
//
// This is function for Control Transfer.
// If it occurs endpoint0 interrupt, this function is called.
// This function check Device Request for Control Transfer type and
// call each other functions.
// ======================================================================

void HandleEvent_EP0(void)
{
	//U32 DeviceRequestLength,Temp,k;
	//U32 Temp;//,k;
	U16 ep0csr;
	U8 ep0csrtemp;
	U16 ReadCnt, i;
	U16 ReadBuf[64]={0x0000, };
	U8  setaddress;
	U32  uRemoteWakeUp;
	U32 tmp;

	
	#define TEST_PKT_SIZE 53
	#define TEST_ARR_SIZE 54
	
	
	
	U8 ahwTestPkt [TEST_ARR_SIZE] = {

		0x00,0x00, 0x00,0x00, 0x00,0x00,
	    0x00,0xAA, 0xAA,0xAA, 0xAA,0xAA, 0xAA,0xAA,
		0xAA,0xEE, 0xEE,0xEE, 0xEE,0xEE, 0xEE,0xEE,
		0xEE,0xFE, 0xFF,0xFF, 0xFF,0xFF, 0xFF,0xFF,
		0xFF,0xFF, 0xFF,0xFF, 0xFF,0x7F, 0xBF,0xDF,
		0xEF,0xF7, 0xFB,0xFD, 0xFC,0x7E, 0xBF,0xDF,
		0xEF,0xF7, 0xFB,0xFD, 0x7E,0x00, 0x00,0x00
	};

	Outp32(INDEX_REG, EP0);
	Inp32(EP0_STATUS_REG, ep0csr);


	//	DbgUsb((" Endpoint0 CSR Register = %x \n", ep0csr));
	// EP0 CSR register status check

	if (ep0csr & EP0_SENT_STALL) // SENT STALL : protocol stall.
	{
		
		#ifdef USBUARTTEST
			Uart_SendString(("Sent Stall\n"));
		#endif	
		
		Outp32(EP0_STATUS_REG, EP0_SENT_STALL);
		if (ep0csr & EP0_RX_SUCCESS)
			Outp32(EP0_STATUS_REG, EP0_RX_SUCCESS);
		
		g_uEp0State = EP0_STATE_INIT;
		return;
	}

	if (ep0csr & EP0_TX_SUCCESS)
	{
		
		#ifdef USBUARTTEST
			Uart_SendString(("EP0_TX_SUCCESS\n"));
		#endif
		
		Outp32(EP0_STATUS_REG, EP0_TX_SUCCESS);

		ep0csrtemp = ep0csr;
		//PrintEp0Pkt(&ep0csrtemp,1);
		
		return;
	}

	//	DbgUsb((" g_uEp0State = %x \n", g_uEp0State));
	if(ep0csr & EP0_RX_SUCCESS)
	{
		//ep0csrtemp = (g_uEp0State>>24) & 0xff;
		//PrintEp0Pkt(&ep0csrtemp,1);
		//ep0csrtemp = (g_uEp0State>>16) & 0xff;
		//PrintEp0Pkt(&ep0csrtemp,1);
		//ep0csrtemp = (g_uEp0State>>8) & 0xff;
		//PrintEp0Pkt(&ep0csrtemp,1);
		//ep0csrtemp = (g_uEp0State) & 0xff;
		//PrintEp0Pkt(&ep0csrtemp,1);		
		
		if (g_uEp0State == EP0_STATE_INIT)
		{

			#ifdef USBUARTTEST
				Uart_SendString(("EP0_RX_SUCCESS\n"));
			#endif
		
			Inp32(BYTE_READ_CNT_REG, ReadCnt);
			for(i=0;i<4;i++) Inp32(EP0_FIFO, ReadBuf[i]);

			Outp32(EP0_STATUS_REG, EP0_RX_SUCCESS);

			g_poDeviceRequest->bmRequestType=ReadBuf[0];
			g_poDeviceRequest->bRequest=ReadBuf[0]>>8;
			g_poDeviceRequest->wValue_L=ReadBuf[1];
			g_poDeviceRequest->wValue_H=ReadBuf[1]>>8;
			g_poDeviceRequest->wIndex_L=ReadBuf[2];
			g_poDeviceRequest->wIndex_H=ReadBuf[2]>>8;
			g_poDeviceRequest->wLength_L=ReadBuf[3];
			g_poDeviceRequest->wLength_H=ReadBuf[3]>>8;

			//PrintEp0Pkt((U8 *)g_poDeviceRequest, 8);

			switch (g_poDeviceRequest->bRequest)
			{
				case STANDARD_SET_ADDRESS:
					setaddress = (g_poDeviceRequest->wValue_L); // Set Address Update bit

					//0005 0300 0000 0000
					
					#ifdef USBUARTTEST
						Uart_SendString(("Func_ADDR_Setaddr : \n"));						
					#endif			
					
					Inp32(FUNC_ADDR_REG, tmp);
					g_uEp0State = EP0_STATE_INIT;
					break;

				case STANDARD_SET_DESCRIPTOR:
			//		DbgUsb(("\n MCU >> Set Descriptor \n"));			
					break;

				case STANDARD_SET_CONFIGURATION:
			//		DbgUsb(("\n MCU >> Set Configuration \n"));
					usConfig = g_poDeviceRequest->wValue_L; // Configuration value in configuration descriptor
					g_uEnumerationDone = 1;

					#ifdef USBUARTTEST
						Uart_SendString(("MCU >> Set Configuration \n"));						
					#endif					
				      /*
					if(g_uBulkInCount!=0)
					{
						g_uBulkInCount+=g_uEp1MaxPktSize;
						PrepareEp1Fifo(g_uBulkInAddr-g_uEp1MaxPktSize);
					}
					*/
					break;

				case STANDARD_GET_CONFIGURATION:
					// Uart_Printf("\n MCU >> Get Configruation \n");
					Outp32(BYTE_WRITE_CNT_REG, 1);
					Outp32(EP0_FIFO, usConfig);

					#ifdef USBUARTTEST
						Uart_SendString(("MCU >> Get Configuration \n"));						
					#endif	
					
					break;

				case STANDARD_GET_DESCRIPTOR:
					DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
								g_poDeviceRequest->wLength_L);
				
					switch (g_poDeviceRequest->wValue_H)
					{
					
						case DEVICE_DESCRIPTOR:
				//			DbgUsb(("\n MCU >> Get Device Descriptor \n"));
							
							DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
								g_poDeviceRequest->wLength_L);
								g_uEp0State = EP0_STATE_GD_DEV_0_0;
								
							#ifdef USBUARTTEST
								Uart_SendString(("MCU >> Get Device Descriptor \n"));						
							#endif	
								
							break;
						
						case CONFIGURATION_DESCRIPTOR:

							
							//8006000200000902

							DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
								g_poDeviceRequest->wLength_L);
							
							#ifdef USBUARTTEST
								Uart_SendString(("MCU >> Get Configuration Descriptor = \n"));						
							#endif
							
				//			DbgUsb(("\n MCU >> Get Configuration Descriptor = %x\n",DeviceRequestLength));
							if (DeviceRequestLength > CONFIG_DESC_SIZE){
							// === GET_DESCRIPTOR:CONFIGURATION+INTERFACE+ENDPOINT0+ENDPOINT1 ===
							// Windows98 gets these 4 descriptors all together by issuing only a request.
							// Windows2000 gets each descriptor seperately.
								g_uEp0State = EP0_STATE_GD_CFG_0;
							}
							else
								g_uEp0State = EP0_STATE_GD_CFG_ONLY_0; // for win2k
							break;

						case STRING_DESCRIPTOR :
						    DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
								g_poDeviceRequest->wLength_L);
								
							switch(g_poDeviceRequest->wValue_L)
							{
								case 0:
									g_uEp0State = EP0_STATE_GD_STR_I0;
									break;
								case 1:
									g_uEp0State = EP0_STATE_GD_STR_I1;
									break;
								case 2:
									g_uEp0State = EP0_STATE_GD_STR_I2;
									break;
								default:
										break;
							}
							break;
						case ENDPOINT_DESCRIPTOR:
							switch(g_poDeviceRequest->wValue_L&0xf)
							{
							case 0:
								g_uEp0State=EP0_STATE_GD_EP0_ONLY_0;
								break;
							case 1:
								g_uEp0State=EP0_STATE_GD_EP1_ONLY_0;
								break;
							default:
								break;
							}
							break;

						case DEVICE_QUALIFIER:
							DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
							g_poDeviceRequest->wLength_L);
						//	printf("devicequalify = %d\n",DeviceRequestLength);
							if (DeviceRequestLength ==4)
								g_uEp0State=EP0_STATE_GD_DEV_QUALIFIER;
							else if (DeviceRequestLength ==10) 
								g_uEp0State=EP0_STATE_GD_DEV_QUALIFIER_1;
													
							break;
							
					   	case OTHER_SPEED_CONFIGURATION :
							
							//DbgUsb(("\n MCU >> Get OTHER_SPEED_CONFIGURATION \n"));
							#ifdef USBUARTTEST
								Uart_SendString(("MCU >> Get OTHER_SPEED_CONFIGURATION \n"));								
							#endif								
							DeviceRequestLength = (U32)((g_poDeviceRequest->wLength_H << 8) |
							g_poDeviceRequest->wLength_L);
							g_uEp0State = EP0_STATE_GD_DEV_OTHER_SPEED;
							break;
						default:
							break;
				
					}
					break;

				case STANDARD_CLEAR_FEATURE:
					
					#ifdef USBUARTTEST
						Uart_SendString(("MCU >> Clear Feature \n"));								
					#endif					
			//		DbgUsb(("\n MCU >> Clear Feature \n"));
					switch (g_poDeviceRequest->bmRequestType)
					{
						case DEVICE_RECIPIENT:
							if (g_poDeviceRequest->wValue_L == 1)
								uRemoteWakeUp = FALSE;
							break;

						case ENDPOINT_RECIPIENT:
							if (g_poDeviceRequest->wValue_L == 0)
							{
								if ((g_poDeviceRequest->wIndex_L & 0x7f) == 0x00)
									g_poStatusGet->Endpoint0= 0;

								if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x81) // IN  Endpoint 1
									g_poStatusGet->Endpoint1= 0;

								if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x03) // OUT Endpoint 3
									g_poStatusGet->Endpoint3= 0;
							}
							break;

						default:
							break;
					}
					g_uEp0State = EP0_STATE_INIT;
					break;

				case STANDARD_SET_FEATURE:
				    
					#ifdef USBUARTTEST
						Uart_SendString(("MCU >> Set Feature \n"));								
					#endif				
			//		DbgUsb(("\n MCU >> Set Feature \n"));
					switch (g_poDeviceRequest->bmRequestType)
					{
						case DEVICE_RECIPIENT:
							if (g_poDeviceRequest->wValue_L == 1)
								uRemoteWakeUp = TRUE;
								break;

						case ENDPOINT_RECIPIENT:
							if (g_poDeviceRequest->wValue_L == 0)
							{
								if ((g_poDeviceRequest->wIndex_L & 0x7f) == 0x00)
									g_poStatusGet->Endpoint0= 1;

								if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x81)
									g_poStatusGet->Endpoint1= 1;

								if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x03)
									g_poStatusGet->Endpoint3= 1;
							}
							break;

						default:
							break;
					}
					//=======================================================
					
					switch (g_poDeviceRequest->wValue_L) {

						case EP_STALL:

							// TBD: addnl processing if reqd
							break;

						case TEST_MODE:						
							if ((0 != g_poDeviceRequest->wIndex_L ) ||(0 != g_poDeviceRequest->bmRequestType)) 
							break;
							
							// Set TEST MODE
								 	Inp32(TEST_REG,tmp);
									tmp|=TR_TMD;
							
							Outp32(TEST_REG,TR_TMD);
							
							switch( g_poDeviceRequest->wIndex_H)
							{
							
								case TEST_J:

									//Set Test J
									Inp32(TEST_REG,tmp);
									tmp|=TR_TJS;
									Outp32(TEST_REG,tmp);
									break;

								case TEST_K:

									//Set Test K
									Inp32(TEST_REG,tmp);
									tmp|=TR_TKS;
									Outp32(TEST_REG,tmp);
														
									break;

								case TEST_SE0_NAK:

									//Set Test SE0NAK
									Inp32(TEST_REG,tmp);
									tmp|=TR_TSNS;
									Outp32(TEST_REG,tmp);
									
									break;

								case TEST_PACKET:
									//printf ("Test_packet\n");
									Outp32(BYTE_WRITE_CNT_REG, TEST_PKT_SIZE);
									WrPktEp0(ahwTestPkt, TEST_PKT_SIZE);
								 	Inp32(TEST_REG,tmp);
									tmp|=TR_TPS;
									Outp32(TEST_REG,tmp);
									//printf ("tr=%0X\n", tmp);					 	
								 	

									break;
								}

							break;

						default:
							break;
					}
					//=======================================================
					
					g_uEp0State = EP0_STATE_INIT;
					break;

				case STANDARD_GET_STATUS:
					switch(g_poDeviceRequest->bmRequestType)
					{
						case  (0x80):
							g_poStatusGet->Device=((U8)uRemoteWakeUp<<1)|0x1;		// SelfPowered
							g_uEp0State = EP0_GET_STATUS0;
							break;

						case  (0x81):
							g_poStatusGet->Interface=0;
							g_uEp0State = EP0_GET_STATUS1;
							break;

						case  (0x82):
							if ((g_poDeviceRequest->wIndex_L & 0x7f) == 0x00)
								g_uEp0State = EP0_GET_STATUS2;

							if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x81)
								g_uEp0State = EP0_GET_STATUS3;

							if ((g_poDeviceRequest->wIndex_L & 0x8f) == 0x03)
								g_uEp0State = EP0_GET_STATUS4;
							break;

						default:
							break;
					}
					break;

				case STANDARD_GET_INTERFACE:
					g_uEp0State = EP0_INTERFACE_GET;
					break;

				case STANDARD_SET_INTERFACE:
					g_poInterfaceGet->AlternateSetting= g_poDeviceRequest->wValue_L;
					g_uEp0State = EP0_STATE_INIT;
					break;

				case STANDARD_SYNCH_FRAME:
					g_uEp0State = EP0_STATE_INIT;
					break;

				default:
					break;
			}
		}
	}
	TransferEp0();


}


// Not Supporting About Set Descriptor for USB Standard
void StandardSetDescriptor(void)
{
	// CLR_EP0_CSR_OUT_PACKET_READY;
}


// ======================================================================
// TransferEp0()
//
// This is function for Control Transfer Transmit.
// ======================================================================

void TransferEp0(void)
{
	//U32 i;
	//U32 dataLength;
	U16 usSysStatus;
	
	//U32 tmp;
	//U32 m;//,n=0;
	//U32 cha;
	

// 	CLR_EP0_CSR_OUT_PACKET_READY;
	switch (g_uEp0State)
	{
		case EP0_STATE_INIT:
			break;

		// === GET_DESCRIPTOR:DEVICE ===
		case EP0_STATE_GD_DEV_0_0:
			#ifdef STALL_CHECK
				Outp32(EP0_CON_REG, 0x2);
			#else 
					
					Outp32(BYTE_WRITE_CNT_REG, DeviceRequestLength&0x3ff);
					WrPktEp0((U8 *)g_poDescDevice+0, DeviceRequestLength&0x3ff); // EP0_PKT_SIZE
   				       g_uEp0State = EP0_STATE_INIT;
//12010002FF00004019043412000101020001
//000000000000000000000000000009022000
//01010080190904000002FF00000007058102
//00020007050302000200

//12010002FF00004019043412000101020001
					//PrintEp0Pkt((U8 *)g_poDescDevice+0, DeviceRequestLength&0x3ff);
			
			#endif
			break;

		case EP0_STATE_GD_CFG_0:
			Outp32(BYTE_WRITE_CNT_REG, 32);
		
			WrPktEp0((U8 *)&g_poDesc->oDescConfig+0, 32); // EP0_PKT_SIZE

			//PrintEp0Pkt((U8 *)&g_poDesc->oDescConfig+0, 32);
				
		    DeviceRequestLength=0;
			g_uEp0State = EP0_STATE_INIT;
			
		//	DbgUsb(("EndpointZeroTransfer(EP0_STATE_GD_CFG) =%x\n",DeviceRequestLength));
		
			
			break;
		case EP0_STATE_GD_CFG_ONLY_0:
			Outp32(BYTE_WRITE_CNT_REG, 9);
			WrPktEp0((U8 *)&g_poDesc->oDescConfig+0, 9); // EP0_PKT_SIZE


			//090220000101008019
			
			//PrintEp0Pkt((U8 *)&g_poDesc->oDescConfig+0, 9);
			
			g_uEp0State = EP0_STATE_INIT;
			break;
		
		// === GET_DESCRIPTOR:CONFIGURATION ONLY===
		
		case EP0_STATE_GD_DEV_QUALIFIER:
		//   DbgUsb(("\n MCU >> Get Device Qualifier Descriptor \n"));
		    Outp32(BYTE_WRITE_CNT_REG, 4);
			WrPktEp0((U8 *)aDeviceQualifierDescriptor+0, 4);
			g_uEp0State = EP0_STATE_INIT;
			break;
			
		case EP0_STATE_GD_DEV_QUALIFIER_1:
		//   DbgUsb(("\n MCU >> Get Device Qualifier Descriptor \n"));
		    
		    Outp32(BYTE_WRITE_CNT_REG, 10);
			WrPktEp0((U8 *)aDeviceQualifierDescriptor+0, 10);
			g_uEp0State = EP0_STATE_INIT;
			break;

        case EP0_STATE_GD_DEV_OTHER_SPEED:
			Set_OtherSpeedConf_DescriptorTable(DeviceRequestLength&0x3ff);
			g_uEp0State = EP0_STATE_INIT;
						
			break;

		// === GET_DESCRIPTOR:ENDPOINT 1 ONLY===
		case EP0_STATE_GD_EP0_ONLY_0:
			Outp32(BYTE_WRITE_CNT_REG, ENDPOINT_DESC_SIZE);
			Inp32(SYS_STATUS_REG, usSysStatus);
			WrPktEp0((U8 *)&g_poDesc->oDescEndpt1+0, ENDPOINT_DESC_SIZE);
			g_uEp0State = EP0_STATE_INIT;
			break;

		// === GET_DESCRIPTOR:ENDPOINT 2 ONLY===
		case EP0_STATE_GD_EP1_ONLY_0:
			Outp32(BYTE_WRITE_CNT_REG, ENDPOINT_DESC_SIZE);
			Inp32(SYS_STATUS_REG, usSysStatus);
			WrPktEp0((U8 *)&g_poDesc->oDescEndpt3+0, ENDPOINT_DESC_SIZE);
			g_uEp0State = EP0_STATE_INIT;
			break;


		case EP0_STATE_GD_STR_I0:
			DbgUsb(("[GDS0_%d]", g_uEp0SubState));
			Outp32(BYTE_WRITE_CNT_REG, 4);
			WrPktEp0(aDescStr0, 4);
			g_uEp0State = EP0_STATE_INIT;
	
			break;

			case EP0_STATE_GD_STR_I1:
			DbgUsb(("[GDS1_%d]", g_uEp0SubState));
			
				Outp32(BYTE_WRITE_CNT_REG, 22);
				WrPktEp0(aDescStr1, 22);
				
				g_uEp0State = EP0_STATE_INIT;
				
			
			break;
			
			case EP0_STATE_GD_STR_I2:
			DbgUsb(("[GDS2_%d]", g_uEp0SubState));
				Outp32(BYTE_WRITE_CNT_REG, 44);
				WrPktEp0(aDescStr2, 44);
				g_uEp0State = EP0_STATE_INIT;
			
		
			break;
		


		case EP0_INTERFACE_GET:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poInterfaceGet+0, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;


		case EP0_GET_STATUS0:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poStatusGet+0, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;

		case EP0_GET_STATUS1:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poStatusGet+1, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;

		case EP0_GET_STATUS2:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poStatusGet+2, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;

		case EP0_GET_STATUS3:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poStatusGet+3, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;

		case EP0_GET_STATUS4:
			Outp32(BYTE_WRITE_CNT_REG, 1);
			WrPktEp0((U8 *)g_poStatusGet+4, 1);
			g_uEp0State = EP0_STATE_INIT;
			break;

		default:
			break;


	}
}

// ======================================================================
// HandleBulkIn()
//
// This is function for Endpoint One ( Bulk In)function routine.
// ======================================================================
void HandleEvent_BulkIn_bk(void)
{
	U16 ep1csr;
	U32 temp;

	// EP1 CSR register status check
	Outp32(INDEX_REG, 0x01);
	Inp32(EP_STATUS_REG, ep1csr);
	Outp32(EP_STATUS_REG, ep1csr);
	Inp32(DMA_TOTAL_CNT1_REG, temp);
    
	if (ep1csr & EP_SENT_STALL) { // SENT STALL : protocol stall.
		DbgUsb(("Sent Stall \n"));
		Outp32(EP_STATUS_REG, EP_SENT_STALL);
	}

	if (ep1csr & EP_TX_SUCCESS)
	{
		Outp32(EP_STATUS_REG, EP_TX_SUCCESS); // Endpoint Status Register Clear
		if (g_eOpMode == USB_CPU)
		{
			if (g_uBulkInCount != 0)
				PrepareEp1Fifo(g_uBulkInAddr);
		}
	}

	if (DMA_TOTAL_COUNT_ZERO & ep1csr)
	{
		//printf("USB_DMA_MODE, DMA TX Done(DMA_TOTAL_COUNT_ZERO) !!\n");
		Outp32(DMA_CON_REG, DMA_TX_STOP|USB_INT_MODE);
		Outp32(FCON, DMA_DISABLE);
	}
}

// ======================================================================
// HandleBulkOut()
//
// This is function for Endpoint Three ( Bulk Out)function routine.
// ======================================================================
U32 checkTest = 0;

void HandleEvent_BulkOut_bk(void)
{
	//U32 ReadData;
	//U32 RecDataCnt;
	U16 ep3csr;//, ep2con, sys_status;
	//U32 CBWSignature=0, CBWTag=0;//, i;
	U8  TempBuf[16];
	U16 fifoCnt;
	U16 fifoCntByte;
	U32 DmaCurrAddr;
	U32 temp;
	U32 uUploadSize;
	U32 uUploadAddr;
	U32 uDownloadFileSize;
	
	//U32 j;
	
	Outp32(INDEX_REG, EP3);
	Inp32(EP_STATUS_REG, ep3csr);
	Outp32(EP_STATUS_REG, ep3csr);
 	DbgUsb(("Bulk Out Int, ep3csr : %x\n", ep3csr));
	Inp32(DMA_TOTAL_CNT1_REG, temp);
	DbgUsb(("EP3: DMA_TOTAL_CNT1_REG : %x\n", temp));

	if (ep3csr & EP_SENT_STALL) // SENT STALL : protocol stall.
	{
		Outp32(EP_STATUS_REG, EP_SENT_STALL);
		return;
	}

	if (ep3csr & EP_FIFO_FLUSH)
	{
		Outp32(EP_CON_REG, EP_FIFO_FLUSH);
		return;
	}

	if (DMA_TOTAL_COUNT_ZERO & ep3csr)
	{		
	    checkTest = 1;
		DbgUsb(("USB_DMA_MODE, DMA RX Done(DMA_TOTAL_COUNT_ZERO) !!\n"));
		
		Outp32(DMA_CON_REG, DMA_RX_STOP|USB_INT_MODE);
		
		g_eOpMode = USB_CPU;
		Inp32(DMA_MEg_CURRENT_ADDR, DmaCurrAddr);
		g_pDownPt = (U8 *)DmaCurrAddr;
		
		
		Outp32(FCON, DMA_DISABLE);
		
		DbgUsb(("DMA_TOTAL_COUNT_ZERO, ep3csr : %x\n", ep3csr));

		//dma_done =1;
	    
	    if (!ep3csr & EP_RX_SUCCESS)// if interrupt is not genarated then check the RX_success bit.
	  	return;
	  
	}

	if (ep3csr & EP_RX_SUCCESS)
	{
		//DbgUsb(("EP3_RX_PKT_SUCCESS\n"));
		#ifdef USBUARTTEST
			Uart_SendString(("\n EP3_RX_PKT_SUCCESS \n"));
		#endif		
		if (g_uDownloadFileSize==0)
		{
			Inp32(BYTE_READ_CNT_REG, fifoCnt);

			if (fifoCnt == 5)
			{
				RdPktEp3((U8 *)TempBuf, 10);
				temp = *((U8 *)(TempBuf+8))+
					(*((U8 *)(TempBuf+9))<<8);
				DbgUsb(("temp: %x\n", temp));
				if (temp==0x1)
				{
					uUploadAddr =
						*((U8 *)(TempBuf+0))+
						(*((U8 *)(TempBuf+1))<<8)+
						(*((U8 *)(TempBuf+2))<<16)+
						(*((U8 *)(TempBuf+3))<<24);

					uUploadSize =
						*((U8 *)(TempBuf+4))+
						(*((U8 *)(TempBuf+5))<<8)+
						(*((U8 *)(TempBuf+6))<<16)+
						(*((U8 *)(TempBuf+7))<<24);
				
					//printf("UploadAddress : %x, UploadSize: %x\n", uUploadAddr, uUploadSize);
					if (g_eOpMode == USB_CPU)
					{
						//DbgUsb(("CPU_MODE Bulk In Function\n"));
		#ifdef USBUARTTEST
			Uart_SendString(("CPU_MODE Bulk In Function\n"));
		#endif					
				
						g_uBulkInCount = uUploadSize;
						PrepareEp1Fifo(uUploadAddr);
					}
					else
					{
						//DbgUsb(("DMA_MODE Bulk In Function\n"));
		#ifdef USBUARTTEST
			Uart_SendString(("DMA_MODE Bulk In Function\n"));
		#endif						
						Outp32(FCON, DMA_ENABLE);				// USB Dma Enable in Core Outside
						Outp32(INDEX_REG, EP1);					// IN Direction  Device -> Host
						Outp32(DMA_IF_CON_REG, MAX_BURST_INCR16);
						Outp32(BYTE_WRITE_CNT_REG, g_uEp1MaxPktSize);
						Outp32(MAX_PKT_REG, g_uEp1MaxPktSize);
						Outp32(DMA_FIFO_CNT_REG, g_uEp1MaxPktSize);
						Outp32(DMA_CNT_REG, g_uEp1MaxPktSize);
						Outp32(DMA_MEg_BASE_ADDR, uUploadAddr);
						Outp32(DMA_TOTAL_CNT1_REG, (U16)uUploadSize);
						Outp32(DMA_TOTAL_CNT2_REG, (U16)(uUploadSize>>16));
						Outp32(DMA_CON_REG, DMA_FLY_ENABLE|DMA_TX_START|USB_DMA_MODE);
					}
				}
				g_uDownloadFileSize=0;
				return;
			}
			else
			{
			/*====
			    if(download_run==0)
   	    		{
					g_uDownloadAddress=tempDownloadAddress; 
	    		}
	    		else
	    		====*/
	    		{
	    			RdPktEp3((U8 *)TempBuf, 8);
					if (ep3csr&(0x1<<4))
						fifoCntByte = fifoCnt * 2 -1;
					else
						fifoCntByte = fifoCnt * 2;
					//DbgUsb(("downloadFileSize==0, 1'st BYTE_READ_CNT_REG : %x\n", fifoCntByte));
		#ifdef USBUARTTEST
			Uart_SendString(("downloadFileSize==0, 1'st BYTE_READ_CNT_REG :\n"));
		#endif
					g_uDownloadAddress=
						*((U8 *)(TempBuf+0))+
						(*((U8 *)(TempBuf+1))<<8)+
						(*((U8 *)(TempBuf+2))<<16)+
						(*((U8 *)(TempBuf+3))<<24);
	    		}

				/*====
				if(download_run==0)
   	    		{
   	    			RdPktEp3((U8 *)TempBuf, 8);
					if (ep3csr&(0x1<<4))
						fifoCntByte = fifoCnt * 2 -1;
					else
						fifoCntByte = fifoCnt * 2;
   	    		}
   	    			====*/
					DbgUsb(("downloadFileSize==0, 1'st BYTE_READ_CNT_REG : %x\n", fifoCntByte));
					g_uDownloadFileSize=
						*((U8 *)(TempBuf+4))+
						(*((U8 *)(TempBuf+5))<<8)+
						(*((U8 *)(TempBuf+6))<<16)+
						(*((U8 *)(TempBuf+7))<<24);

				g_pDownPt=(U8 *)g_uDownloadAddress;
				//DbgUsb(("downloadAddress : %x, downloadFileSize: %x\n", g_uDownloadAddress, g_uDownloadFileSize));
				
				RdPktEp3((U8 *)g_pDownPt, fifoCntByte-8); // The first 8-bytes are deleted.
			
				if (g_eOpMode == USB_CPU)
				{
					if (ep3csr & (0x2<<2))
					{
						Inp32(BYTE_READ_CNT_REG, fifoCnt);
						if (ep3csr&(0x1<<4))
							fifoCntByte = fifoCnt * 2 -1;
						else
							fifoCntByte = fifoCnt * 2;
						DbgUsb(("2'd BYTE_READ_CNT_REG : %x\n", fifoCntByte));
						RdPktEp3((U8 *)g_pDownPt, fifoCntByte);
					}
				}
			else
				{
						uDownloadFileSize = g_uDownloadFileSize-fifoCntByte;
					if (uDownloadFileSize > g_uEp3MaxPktSize)
					{
						Outp32(FCON, DMA_ENABLE);				// USB Dma Enable in Core Outside
						Outp32(INDEX_REG, EP3);					// OUT Direction  Host -> Device
						Outp32(DMA_IF_CON_REG, MAX_BURST_INCR16);
						Outp32(MAX_PKT_REG, g_uEp3MaxPktSize);
						Outp32(DMA_FIFO_CNT_REG, g_uEp3MaxPktSize);
						Outp32(DMA_CNT_REG, g_uEp3MaxPktSize);
						Outp32(DMA_MEg_BASE_ADDR, g_uDownloadAddress+fifoCntByte-8);
						uDownloadFileSize = (uDownloadFileSize/g_uEp3MaxPktSize)*g_uEp3MaxPktSize;
						
						Outp32(DMA_TOTAL_CNT1_REG, (U16)(uDownloadFileSize));
						Outp32(DMA_TOTAL_CNT2_REG, (U16)(uDownloadFileSize>>16));
						DbgUsb(("Out Direction DMA RX Start\n"));
						Delay(1);
						Outp32(DMA_CON_REG, DMA_FLY_ENABLE|DMA_RX_START|USB_DMA_MODE);
					}
					else
						g_eOpMode = USB_CPU;
				}
			}
		}

		else
		{	
			Inp32(EP_STATUS_REG, ep3csr);
			
			if (g_eOpMode == USB_CPU)
			{
				Inp32(BYTE_READ_CNT_REG, fifoCnt);
				if (ep3csr&(0x1<<4))
					fifoCntByte = fifoCnt * 2 -1;
				else
					fifoCntByte = fifoCnt * 2;
				DbgUsb(("downloadFileSize!=0, 1'st BYTE_READ_CNT_REG : %x\n", fifoCntByte));
				RdPktEp3((U8 *)g_pDownPt, fifoCntByte);
				if ((ep3csr & (0x2<<2)))
				{
					Inp32(BYTE_READ_CNT_REG, fifoCnt);
					if (ep3csr&(0x1<<4))
						fifoCntByte = fifoCnt * 2 -1;
					else
						fifoCntByte = fifoCnt * 2;
					DbgUsb(("2'd BYTE_READ_CNT_REG : %x\n", fifoCntByte));
					RdPktEp3((U8 *)g_pDownPt, fifoCntByte);
				
				}
			}
		}
		return;
	}
} 

void USB_UartTest(char *fmt)
{
	if(UART_PrintStr && 0x23==rUBRDIV1) 
	{
		Uart_Printf(fmt);
	}
}


void HandleEvent_BulkIn(void)
{
	U16 ep1csr;
	U32 temp;


	// EP1 CSR register status check
	Outp32(INDEX_REG, 0x01);
	Inp32(EP_STATUS_REG, ep1csr);
	Outp32(EP_STATUS_REG, ep1csr);
	Inp32(DMA_TOTAL_CNT1_REG, temp);
	

	if (ep1csr & EP_SENT_STALL)
	{ 
		
		Outp32(EP_STATUS_REG, EP_SENT_STALL);
	}

	if (ep1csr & EP_TX_SUCCESS)
	{

		Outp32(EP_STATUS_REG, EP_TX_SUCCESS); // Endpoint Status Register Clear		
	}
	

}

/*============================================================================================*/
void HandleEvent_BulkOut(void)
{
	U16 ep3csr;//, ep2con;//, sys_status;
	//U32 CBWSignature=0, CBWTag=0;//, i;
	U16 fifoCnt;
	U16 fifoCntByte;
	U16 ep3bufoffset;
	U32 temp;
	U32  uUploadAddr;
	INT16U PhysicalBlock;
	
	
	//DebugPrintf("BulkOut\n");
	rIR	=3;
	ep3csr	=rESR;
	rESR	=ep3csr;
		
	if (ep3csr & EP_SENT_STALL) // SENT STALL : protocol stall.
	{
		// Uart_Printf(" Sent Stall \n");
		rECR|=(1<<1);
		return;
	}

	if (ep3csr & EP_FIFO_FLUSH)
	{
		Outp32(EP_CON_REG, EP_FIFO_FLUSH);
		return;
	}

	if (ep3csr & EP_RX_SUCCESS)
	{		
		
		if (g_eOpMode == USB_CPU)
		{
			Inp32(BYTE_READ_CNT_REG, fifoCnt);

			if(g_uDownloadFileSize==0)
			{
				if(fifoCnt==5)
				{
					RdPktEp3(Ep3Buf, 10);

					temp = Ep3Buf[8]+(Ep3Buf[9]<<8);

					if (temp==0x1)
					{
						uUploadAddr = 
						Ep3Buf[0]+
						(Ep3Buf[1]<<8)+
						(Ep3Buf[2]<<16)+
						(Ep3Buf[3]<<24);
						
						g_uBulkInCount = 
						Ep3Buf[4]+
						(Ep3Buf[5]<<8)+
						(Ep3Buf[6]<<16)+
						(Ep3Buf[7]<<24);
							
						PrepareEp1Fifo(uUploadAddr);
					}
					g_uDownloadFileSize = 0;
					return;
				}
				else
				{
					
					RdPktEp3(Ep3Buf, 8);
					if (ep3csr&(0x1<<4))
						fifoCntByte = fifoCnt * 2 -1;
					else
						fifoCntByte = fifoCnt * 2;
					
					
					g_uDownloadAddress=
						Ep3Buf[0]+
						(Ep3Buf[1]<<8)+
						(Ep3Buf[2]<<16)+
						(Ep3Buf[3]<<24);

					g_uDownloadFileSize=
						Ep3Buf[4]+
						(Ep3Buf[5]<<8)+
						(Ep3Buf[6]<<16)+
						(Ep3Buf[7]<<24);

					g_uFileTotalLen = g_uDownloadFileSize-8;
					g_uDownloadCount = 0;

					if(fifoCntByte>512)
					{
						fifoCntByte = 512;
					}
					RdPktEp3(Ep3Buf, (fifoCntByte-8));
						
					g_uDownloadFileSize -= 8;

					ep3bufoffset = fifoCntByte-8;
					
					if (ep3csr & (0x2<<2))
					{
						Inp32(BYTE_READ_CNT_REG, fifoCnt);
						if (ep3csr&(0x1<<4))
						{
							fifoCntByte = fifoCnt * 2 -1;
						}
						else
						{
							fifoCntByte = fifoCnt * 2;
						}
						//EdbgOutputDebugString("2'd BYTE_READ_CNT_REG : %x\n", fifoCntByte);
						if(fifoCntByte>512)
						{
							fifoCntByte = 512;
						}				
						RdPktEp3(Ep3Buf, fifoCntByte);

						ep3bufoffset += fifoCntByte;
						
						
					}

					IndexBlock = Logical_To_Physical_Block(NAND_SCR_INDEX);
					IndexBlockOffset = 0;
					
					PhysicalBlock = Logical_To_Physical_Block(NAND_SCR_INFO);
					IndexBlockBuffer[2] = PhysicalBlock>>8;
					IndexBlockBuffer[3]	= PhysicalBlock;
					IndexBlockBuffer[4] = 0;					

					EraseScrFlash();

					USBDeviceReceiveData(Ep3Buf,ep3bufoffset);
					
				}
				
			}
			else
			{
				//RdPktEp3(Ep3Buf, 8);
				if (ep3csr&(0x1<<4))
					fifoCntByte = fifoCnt * 2 -1;
				else
					fifoCntByte = fifoCnt * 2;				


				if(fifoCntByte>512)
				{
					fifoCntByte = 512;
				}
				RdPktEp3(Ep3Buf, fifoCntByte);
				
				ep3bufoffset = fifoCntByte;	

				if (ep3csr & (0x2<<2))
				{
					Inp32(BYTE_READ_CNT_REG, fifoCnt);
					if (ep3csr&(0x1<<4))
					{
						fifoCntByte = fifoCnt * 2 -1;
					}
					else
					{
						fifoCntByte = fifoCnt * 2;
					}
					//EdbgOutputDebugString("2'd BYTE_READ_CNT_REG : %x\n", fifoCntByte);
					if(fifoCntByte>512)
					{
						fifoCntByte = 512;
					}				
					RdPktEp3((Ep3Buf+ep3bufoffset), fifoCntByte);
					
					ep3bufoffset += fifoCntByte;
					
				}

				USBDeviceReceiveData(Ep3Buf,ep3bufoffset);
				
			}

			

		}
		
	}
	
	

	if (DMA_TOTAL_COUNT_ZERO & ep3csr)
	{
		//EdbgOutputDebugString("USB_DMA_MODE, DMA RX Done(DMA_TOTAL_COUNT_ZERO) !!\n");
		Outp32(DMA_CON_REG, DMA_RX_STOP|USB_INT_MODE);
		Outp32(FCON, DMA_DISABLE);
	}
	
}





void HandleEvent_BulkOut_CPU(void)
{
	U16 ep3csr;//, ep2con;//, sys_status;
	//U32 CBWSignature=0, CBWTag=0;//, i;
	U16 fifoCnt;
	U16 fifoCntByte;
	//U32 temp;
	
	
	//DebugPrintf("BulkOut\n");
	rIR	=3;
	ep3csr	=rESR;
	rESR	=ep3csr;
		
	if (ep3csr & EP_SENT_STALL) // SENT STALL : protocol stall.
	{
		// Uart_Printf(" Sent Stall \n");
		rECR|=(1<<1);
		return;
	}

	if (ep3csr & EP_FIFO_FLUSH)
	{
		return;
	}

	if (ep3csr & EP_RX_SUCCESS)
	{		
		
		if (g_eOpMode == USB_CPU)
		{
			Inp32(BYTE_READ_CNT_REG, fifoCnt);
			if (ep3csr&(0x1<<4))
			{
				fifoCntByte = fifoCnt * 2 -1;
			}
			else
			{
				fifoCntByte = fifoCnt * 2;
			}

			//EdbgOutputDebugString("downloadFileSize!=0, 0x%x 1'st BYTE_READ_CNT_REG : %x\n", g_pDownPt, fifoCntByte);
			if(fifoCntByte>512)
			{
				fifoCntByte = 512;
			}
			RdPktEp3(Ep3Buf, fifoCntByte);
				

			if (ep3csr & (0x2<<2))
			{
				Inp32(BYTE_READ_CNT_REG, fifoCnt);
				if (ep3csr&(0x1<<4))
				{
					fifoCntByte = fifoCnt * 2 -1;
				}
				else
				{
					fifoCntByte = fifoCnt * 2;
				}
				//EdbgOutputDebugString("2'd BYTE_READ_CNT_REG : %x\n", fifoCntByte);
				if(fifoCntByte>512)
				{
					fifoCntByte = 512;
				}				
				RdPktEp3(Ep3Buf, fifoCntByte);
			}
				
			//USBDeviceReceiveData(Ep3Buf,fifoCntByte);  
		}
		
	}
	
	

	if (DMA_TOTAL_COUNT_ZERO & ep3csr)
	{
		//EdbgOutputDebugString("USB_DMA_MODE, DMA RX Done(DMA_TOTAL_COUNT_ZERO) !!\n");
		Outp32(DMA_CON_REG, DMA_RX_STOP|USB_INT_MODE);
		Outp32(FCON, DMA_DISABLE);
	}
}


void USBDeviceReceiveData(U8 *buf, int num)
{
	U32 i,j;
	int numtemp;

	switch(g_uDownloadCount)
	{
		case 0:
			g_uSCR.ScrPointer = *(buf+3);
			g_uSCR.ScrPointer += *(buf+6+g_uSCR.ScrPointer);
			g_uSCR.ScrPointer += *(buf+13+g_uSCR.ScrPointer);
			g_uSCR.ScrPointer += *(buf+16+g_uSCR.ScrPointer);
			g_uSCR.ScrPointer += *(buf+23+g_uSCR.ScrPointer);
			g_uSCR.ScrPointer += *(buf+26+g_uSCR.ScrPointer);
		       g_uSCR.ScrPointer += 29;
			g_uSCR.ScrCount  = *(buf+g_uSCR.ScrPointer);			
			g_uSCR.ScrPointer++;

			for(i=0;i<g_uSCR.ScrPointer;i++)
			{
				ScreenDataBuffer[i] = *(buf+i);				
			}
			
			g_uSCR.SystemInfoErr = USBSaveSystemInfo();

			numtemp = num -  g_uSCR.ScrPointer;			
			g_uSCR.CurrentScr = 0;
			g_uSCR.CurrentTotalLength = g_uSCR.ScrCount *4;
			g_uSCR.SDBPointer= 0;

			if(g_uSCR.CurrentTotalLength <= numtemp)
			{	
				g_uSCR.CurrentScr = 1;
				g_uSCR.AddrCount = g_uSCR.ScrCount;
			}
			else
			{
				g_uSCR.AddrCount = numtemp/4;				
			}

			j = g_uSCR.ScrPointer;
			for(i=0;i<=g_uSCR.AddrCount;i++)
			{			
				g_uSCR.ScrAddr[i] = 
						buf[j]+
						(buf[j+1]<<8)+
						(buf[j+2]<<16)+
						(buf[j+3]<<24);
				j += 4;
				
			}

			g_uSCR.ScrPointer += g_uSCR.AddrCount * 4;

			if(g_uSCR.CurrentScr==1)
			{	

				g_uSCR.CurrentLength = num -g_uSCR.ScrPointer;
					
				for(;g_uSCR.CurrentScr<=g_uSCR.ScrCount;g_uSCR.CurrentScr++)
				{
					if(g_uSCR.CurrentScr==g_uSCR.ScrCount)
					{
						g_uSCR.CurrentTotalLength = g_uFileTotalLen - 
											(g_uSCR.ScrAddr[g_uSCR.CurrentScr-1] & 0x00ffffff);
					}
					else
					{
						g_uSCR.CurrentTotalLength = (g_uSCR.ScrAddr[g_uSCR.CurrentScr] & 0x00ffffff)
											-(g_uSCR.ScrAddr[g_uSCR.CurrentScr-1] & 0x00ffffff);
					}

					if(g_uSCR.CurrentLength>=g_uSCR.CurrentTotalLength)
					{
						g_uSCR.ScreenInfoErr = USBSaveScreenInfo();
						g_uSCR.CurrentLength -= g_uSCR.CurrentTotalLength;
					}
					else
					{						
						//g_uSCR.SDBPointer = 0;
						for(j=0;j<g_uSCR.CurrentLength;j++)
						{
							ScreenDataBuffer[g_uSCR.SDBPointer++] = *(buf+g_uSCR.ScrPointer);
							g_uSCR.ScrPointer++;
						}
						break;
					}
				}
				
			}

			g_uSCR.ScrPointer = 0;
			
			break;
			
		default:

			if(g_uSCR.AddrCount==g_uSCR.ScrCount)
			{
				g_uSCR.CurrentLength = num -g_uSCR.ScrPointer;
					
				for(;g_uSCR.CurrentScr<=g_uSCR.ScrCount;g_uSCR.CurrentScr++)
				{
					if(g_uSCR.CurrentScr==g_uSCR.ScrCount)
					{
						g_uSCR.CurrentTotalLength = g_uFileTotalLen - 
											(g_uSCR.ScrAddr[g_uSCR.CurrentScr-1] & 0x00ffffff);
					}
					else
					{
						g_uSCR.CurrentTotalLength = (g_uSCR.ScrAddr[g_uSCR.CurrentScr] & 0x00ffffff)
											-(g_uSCR.ScrAddr[g_uSCR.CurrentScr-1] & 0x00ffffff);
					}

					if((g_uSCR.CurrentLength+g_uSCR.SDBPointer)>=g_uSCR.CurrentTotalLength)
					{
						//g_uSCR.CurrentLength = g_uSCR.CurrentTotalLength;
						g_uSCR.ScreenInfoErr = USBSaveScreenInfo();						
					}
					else
					{						
						//g_uSCR.SDBPointer = 0;
						for(j=0;j<g_uSCR.CurrentLength;j++)
						{
							ScreenDataBuffer[g_uSCR.SDBPointer++] = *(buf+g_uSCR.ScrPointer);
							g_uSCR.ScrPointer++;
						}
						break;
					}
				}		

			}
			else
			{
				
			}

			g_uSCR.ScrPointer = 0;
			
			break;
			
	}

	if(g_uSCR.CurrentScr == (g_uSCR.ScrCount+1))
	{
		WritePage(IndexBlock,0,IndexBlockBuffer);
	}

	g_uDownloadCount++;

}

INT8U USBSaveSystemInfo(void)
{
	INT16U temp;
	INT8U  Page;
	INT32U i;
	
	
	if (g_uSCR.ScrPointer>PAGE_SIZE)
	{
		for (Page=5,i=0;Page<5+g_uSCR.ScrPointer/PAGE_SIZE;Page++,i=i+PAGE_SIZE)
		{
			WritePage(IndexBlock,Page,&ScreenDataBuffer[i]);
		}
		
		if ((temp=g_uSCR.ScrPointer % PAGE_SIZE) != 0)	
		{
			for(;temp<PAGE_SIZE;temp++)
			{
				ScreenDataBuffer[i+temp] = 0;	
			}
			WritePage(IndexBlock,Page,&ScreenDataBuffer[i]);
		
		}
	}
	else
	{
		for (i=g_uSCR.ScrPointer;i<PAGE_SIZE;i++)
			ScreenDataBuffer[i] = 0;
		WritePage(IndexBlock,5,ScreenDataBuffer);
	}

	return TRUE;


}


INT8U USBSaveScreenInfo(void)
{
	INT32U ScrInfoCount,i,Block;
	INT8U  Page;
	//INT8U  CheckSum=0;
	INT16U  ScreenNum;
	INT16U LogicalBlock;
	int j;
	
	ScrInfoCount = g_uSCR.CurrentTotalLength - g_uSCR.SDBPointer;		
	
	for(i=0;i<ScrInfoCount;i++)
	{
		ScreenDataBuffer[g_uSCR.SDBPointer++] = Ep3Buf[g_uSCR.ScrPointer++];

	}

	g_uSCR.CurrentLength -= ScrInfoCount;
	g_uSCR.SDBPointer = 0;
	ScrInfoCount = g_uSCR.CurrentTotalLength;

	Block = IndexBlockBuffer[IndexBlockOffset+2];
	Block = (Block<<8) | IndexBlockBuffer[IndexBlockOffset+3];	
	Page = IndexBlockBuffer[IndexBlockOffset+4];		

	if (ScrInfoCount>PAGE_SIZE)
	{	
		i=0;

		while((ScrInfoCount/PAGE_SIZE) != 0)
		{
			WritePage(Block,Page,&ScreenDataBuffer[i]);
			Page= Page+1;
			if (Page > 63)
			{
				LogicalBlock = Physical_To_Logical_Block(Block);
				LogicalBlock++;
				Block = Logical_To_Physical_Block(LogicalBlock);
				Page = 0;
			}
			ScrInfoCount = ScrInfoCount-PAGE_SIZE;
			i=i+PAGE_SIZE;		
		}
		if (ScrInfoCount != 0)
		{
			for (j=ScrInfoCount;j<PAGE_SIZE;j++)
				ScreenDataBuffer[j] = 0;

			WritePage(Block,Page,&ScreenDataBuffer[i]);
			Page= Page+1;
			if (Page > 63)
			{
				LogicalBlock = Physical_To_Logical_Block(Block);
				LogicalBlock++;
				Block = Logical_To_Physical_Block(LogicalBlock);
				Page = 0;
			}

		}
	}
	else
	{
		for (i=ScrInfoCount;i<PAGE_SIZE;i++)
			ScreenDataBuffer[i] = 0;

		WritePage(Block,Page,ScreenDataBuffer);
		Page= Page+1;
		if (Page > 63)
		{
			LogicalBlock = Physical_To_Logical_Block(Block);
			LogicalBlock++;
			Block = Logical_To_Physical_Block(LogicalBlock);
			Page = 0;
		}

	}
	ScreenNum = ScreenDataBuffer[0];
	IndexBlockBuffer[IndexBlockOffset] = 0;
	IndexBlockBuffer[IndexBlockOffset+1] = ScreenNum;  		
	IndexBlockOffset = IndexBlockOffset+5;
	IndexBlockBuffer[IndexBlockOffset+2] = (Block>>8);
	IndexBlockBuffer[IndexBlockOffset+3] = Block;
	IndexBlockBuffer[IndexBlockOffset+4] = Page;

	return TRUE;

}

#endif
