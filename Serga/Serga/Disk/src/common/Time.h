//  [12/26/2009 Sasha]
#pragma once

class Time {
	static const long long usInHour = (long long)60 * 60 * 1000 * 1000;
	static const long long usInMinute = 60L * 1000 * 1000;
	static const long long usInSecond = 1000L * 1000;
	static const long long usInMsec = 1000L;
public:
	Time();
	Time(int hours, int minutes = 0, int seconds = 0, int msecs = 0, int usecs = 0);
	Time(long long timeUs);
	void FromUs(long long timeUs);
	char* ToStdString();
	void Reset();
	// Вычесть из времени время time. Если time больше внутреннего времени, возвращается false
	bool SubTime(Time &time);
	long long ToUSec();
    long long ToMSec();

	int hours;
	int minutes;
	int seconds;
	int msecs;
	int usecs;
};