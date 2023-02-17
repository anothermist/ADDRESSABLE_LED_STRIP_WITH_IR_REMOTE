#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "math.h"

//#define DS3231_I2C			hi2c1
#define DS3231_ADDRESS		0xD0

unsigned int decToBcd(unsigned int val);
unsigned int bcdToDec(unsigned int val);
void DS3231_Update(void);
unsigned int DS3231_getSec(void);
unsigned int DS3231_getMin(void);
unsigned int DS3231_getHrs(void);
unsigned int DS3231_getWeekDay(void);
unsigned int DS3231_getDate(void);
unsigned int DS3231_getMonth(void);
unsigned int DS3231_getYear(void);
unsigned int DS3231_getAlarm1Sec(void);
unsigned int DS3231_getAlarm1Min(void);
unsigned int DS3231_getAlarm1Hour(void);
unsigned int DS3231_getAlarm1WeekDay(void);
unsigned int DS3231_getAlarm1Date(void);
unsigned int DS3231_getAlarm2Min(void);
unsigned int DS3231_getAlarm2Hour(void);
unsigned int DS3231_getAlarm2WeekDay(void);
unsigned int DS3231_getAlarm2Date(void);
double DS3231_getTemp(void);
void DS3231_setSec(unsigned int value);
void DS3231_setMin(unsigned int value);
void DS3231_setHrs(unsigned int value);
void DS3231_setWeekDay(unsigned int value);
void DS3231_setDate(unsigned int value);
void DS3231_setMonth(unsigned int value);
void DS3231_setYear(unsigned int value);
void DS3231_setAlarm1Sec(unsigned int value);
void DS3231_setAlarm1Min(unsigned int value);
void DS3231_setAlarm1Hour(unsigned int value);
void DS3231_setAlarm1Day(unsigned int value);
void DS3231_setAlarm1Date(unsigned int value);
void DS3231_setAlarm2Min(unsigned int value);
void DS3231_setAlarm2Hour(unsigned int value);
void DS3231_setAlarm2Day(unsigned int value);
void DS3231_setAlarm2Date(unsigned int value);
//uint64_t DS3231_getUnix(uint64_t zoneCorrection);
//float DS3231_getMoonDay(void);

#endif /* INC_DS3231_H_ */