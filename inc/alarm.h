#ifndef	_ALARM_H
#define	_ALARM_H

#define	ALM_APP_NAME_LEN	256

#define	ALM_INVALID		0	/* 取消状态  */ 
#define	ALM_ONCE		1	/* 一次性    */ 
#define	ALM_DAYS		2	/* 每天      */ 
#define	ALM_WEEKLY		3	/* 每周      */ 
#define	ALM_MONTHLY		4	/* 每月      */ 
#define	ALM_QUARTERLY	5	/* 每季度    */
#define	ALM_YEARLY		6	/* 每年      */ 
#define	ALM_FIXEDTICKS	7	/* 固定间隔  */

typedef struct _alarm_app_{ 
	char	AppName[ALM_APP_NAME_LEN];	/* 应用程序的标识 */
	void	(*Rebuild)(void);			/* 复位时重组ALARM项的回调函数 */ 
}ALM_APP_T; 

typedef struct _cur_alarm_info{ 
	char 	AppName[ALM_APP_NAME_LEN];	/* 应用程序的标识 */
	DWORD	UserData;					/* 用户数据 */ 
} ALM_CINFO_T;

typedef struct _alarm_item_{ 
	char 	AppName[ALM_APP_NAME_LEN];	/* 应用程序的标识 */
	BYTE 	ItemType;					/* ALARM 类型（一次\每天\每月……）*/ 
	DWORD 	Interval;					/* 固定间隔类型的时间间隔（单位：分）*/ 
	SYSTEMTIME DateTime;				/* ALARM日期时间 */ 
	DWORD 	UserData;					/* 用户数据 */ 
}ALM_ITEM_T;

char ALM_Init(void);
char ALM_RegisterApp(ALM_APP_T almApp); 
char ALM_DeRegisterApp(const char *appName);
BYTE ALM_GetAlarmInfo(ALM_CINFO_T *pAlmInfo, WORD *pAlmCount, WORD maxRequest);
char ALM_TestResponse(SYSTEMTIME *pInDT,  SYSTEMTIME *pOutDT, char itemType, DWORD interval); 
DWORD ALM_AddAlarm(ALM_ITEM_T *pAlmItem);
DWORD ALM_DelAlarm(DWORD itemNo);
DWORD ALM_ModifyAlarm(ALM_ITEM_T *pAlmItem, DWORD itemNo);
BYTE ALM_RemarkAlmItem(ALM_ITEM_T *pAlmItem, DWORD itemNo);

void OpenAlarm(void (*cbf)(void));
void CloseAlarm(void);
U8 AlarmSetDate(DATETIME_T *date);
U8 AlarmGetDate(DATETIME_T *date);
void AlarmProc(void);

#endif	/* _ALARM_H */