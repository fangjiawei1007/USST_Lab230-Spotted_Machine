/*------------------------------------------------------------------------------
 *
 * Filename: usbd_h.h
 *
 * Contents: 'class USBD20' 
 * 
 * Notes:
 *  1. 
 *
 * Authors : Hyunsun.ahn
 *
 * Copyright (c) 2005 SAMSUNG Electronics.
 *
 *------------------------------------------------------------------------------*/
 
#ifndef __USBD20_H__
#define __USBD20_H__
//#include "def.h"
/*
enum USB_OP
{
	USB_CPU, USB_DMA
};

enum USB_SPEED 
{
	USB_FULL, USB_HIGH
};

*/
#define		USB_OP		unsigned char 
#define		USB_CPU 		0
#define		USB_DMA		1

#define		USB_SPEED		unsigned char
#define		USB_FULL		0
#define		USB_HIGH		1


struct USB_DEVICE_DESCRIPTOR;
struct USB_GET_STATUS;
struct USB_INTERFACE_GET;
struct USB_DESCRIPTORS;
struct DEVICE_REQUEST;


extern void AllocateUSBDEV(void);
extern void FreeUSBDEV(void);
void Usbdmain(void);
void USBdInit(void);
void HandleEvent(void);
void SetOpMode(unsigned char eMode);
BOOL IsEnumerationDone(void);
extern void Usb_Init(void);
void GetDownFileInfo(U32* uDownAddr, U32* uDownFileSize);
BOOL  VerifyChecksum(void);

//static void __irq Isr_Usb(void);
void __irq Isr_Usb(void);

void PrepareEp1Fifo(U32 uBaseAddr);
void SetDescriptorTable(void);
void SetEndpoint(void);
void StandardSetDescriptor(void);
void TransferEp0(void);
void HandleEvent_EP0(void);
void HandleEvent_BulkOut(void);
void HandleEvent_BulkIn(void);
void USBDeviceReceiveData(U8 *buf, int num);
void FlushEp1Fifo(void);
#ifdef		__TEST
void RdPktEp3(U16 *buf, int num);
#else
void RdPktEp3(U8 *buf,int num);
#endif
void SetMaxPktSizes(USB_SPEED eSpeed);	

void WrPktEp0(U8 *buf, int num);

void UsbdMain(void);

void USB_UartTest(char *fmt);

INT8U USBSaveSystemInfo(void);
INT8U USBSaveScreenInfo(void);	

   extern volatile U32  g_uDownloadFileSize;
extern volatile U32  g_uDownloadAddress;
extern volatile U8*  g_pDownPt;	


#endif 
