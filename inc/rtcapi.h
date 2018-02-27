#ifndef _RTC_API_H
#define	_RTC_API_H

typedef struct tagDATETIME
{
    U16 year;
    U16 month;
    U16 week;
    U16 day;
    U16 hour;
    U16 minute;
    U16 second;
    U16 millisecond;
}DATETIME_T;

typedef struct _SYSTEMTIME {
	U16 wYear;
	U16 wMonth;
	U16 wDayOfWeek;
	U16 wDay;
	U16 wHour;
	U16 wMinute;
	U16 wSecond;
	U16 wMilliseconds;
}SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct tagDATE
{
	U16 year;
	U16 month;
	U16 week;//for padding
	U16 day;
}DATE_T;

typedef struct tagTIME
{
	U16 hour;
	U16 minute;
	U16 second;
}TIME_T;

int RtcGetDate(DATETIME_T *date);
int RtcSetDate(DATETIME_T *date);

U8 RtcIsLeapYear(U16 year);
U8 RtcCheckDateValid(U16 year, U16 month, U16 day);
U8 RtcGetWeekNo(U16 year, U16 month, U16 day);
U8 RtcGetDaysOfMonth(U16 year, U16 month);
char RtcCmpDateTime(DATETIME_T *date1, DATETIME_T *date2);
U32 RtcGetDateInterval(DATE_T *date1, DATE_T *date2);
U32 RtcGetTimeInterval(DATETIME_T* pTime1, DATETIME_T *pTime2);
U16 RtcGetPastDays(U16 year, U16 month, U16 day);
U8 RtcDateAddDays(DATE_T *date, U16 days);
U8 RtcDateSubDays(DATE_T *date, U16 days);
U8 RtcGetNextMonth(DATE_T *date);
U8 RtcGetPreMonth(DATE_T *date);
U8 RtcGetNextYear(DATE_T *date);
U8 RtcGetPreYear(DATE_T *date);

U8 RtcGetAnimalYear(DATE_T *date, U8 solar);
U8 RtcSolarConvertLunar(DATE_T *src_date);
U8 RtcLunarConvertSolar(DATE_T *src_date, DATE_T *dec_date);
U8 RtcLunarDayOfMonth(U16 year, U16 month);

#endif	/* _RTC_API_H */