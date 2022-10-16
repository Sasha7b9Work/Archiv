//  [12/26/2009 Sasha]
#include "Time.h"
#include <stdio.h>

//  [12/26/2009 Sasha]
Time::Time():hours(0), minutes(0), seconds(0), msecs(0), usecs(0) {

}

//  [12/26/2009 Sasha]
Time::Time(int _hours, int _minutes, int _seconds, int _msecs, int _usecs):
	hours(_hours),
	minutes(_minutes),
	seconds(_seconds),
	msecs(_msecs),
	usecs(_usecs) {
}

//  [12/26/2009 Sasha]
Time::Time(long long _timeUs) {
	FromUs(_timeUs);
}

//  [12/26/2009 Sasha]
void Time::FromUs(long long _timeUs) {
	hours = (int)(_timeUs / usInHour);
	_timeUs -= hours * usInHour;
	minutes = (int)(_timeUs / usInMinute);
	_timeUs -= minutes * usInMinute;
	seconds = (int)(_timeUs / usInSecond);
	_timeUs -= seconds * usInSecond;
	msecs = (int)(_timeUs / usInMsec);
	usecs = (int)_timeUs - msecs * (int)usInMsec;
}

//  [12/26/2009 Sasha]
char * Time::ToStdString() {
	static char buffer[1000] = {0};
#pragma warning(disable:4996)
	sprintf(buffer, "%d:%d:%d:%d:%d", hours, minutes, seconds, msecs, usecs);
#pragma warning(default:4996)
	return buffer;
}

//  [12/26/2009 Sasha]
void Time::Reset() {
	hours = minutes = seconds = msecs = usecs = 0;
}

//  [12/26/2009 Sasha]
bool Time::SubTime(Time &_time) {
	long long deltaUs = _time.ToUSec() - ToUSec();
	FromUs(deltaUs);
	return deltaUs > 0;
}

//  [12/26/2009 Sasha]
long long Time::ToUSec() {
	return (hours * usInHour + minutes * usInMinute + seconds * usInSecond + msecs * usInMsec + usecs);
}

//  [12/28/2009 Sasha]
long long Time::ToMSec() {
    return (hours * usInHour + minutes * usInMinute + seconds * usInSecond + msecs * usInMsec) / 1000;
}