#ifndef VfdClock_h
#define VfdClock_h
#include <DS3231.h>
#include <Wire.h>

class VfdClock
{
  private:
    DS3231 _clock;
    //grid pins (in ordered array from L to R)
    int _gridPins[9];

	char _displayBuffer[9];
	
    //decimal point grid pin
    int _decPointPin; 

    //digit segment grid pins
    int _segAPin;
    int _segBPin;
    int _segCPin;
    int _segDPin;
    int _segEPin;
    int _segFPin;
    int _segGPin;
	
	boolean _is12Hr;
	boolean _isPM;

  public:
    VfdClock();
	void writeDigits(byte time, int offset);
	void setDigit(byte digit);
	void setDash();
	void setDecPoint();
	void displayTime();
};
#endif
