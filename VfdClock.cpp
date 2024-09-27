#include "VfdClock.h"
#define DASH_BYTE 255
#define NULL_BYTE 254

VfdClock::VfdClock()
{
  Wire.begin();
  _is12Hr = false;
  _isPM = false;
  
  //define pin numbers
  _gridPins[0] = 47;
  _gridPins[1] = 44;
  _gridPins[2] = 45;
  _gridPins[3] = 42;
  _gridPins[4] = 43;
  _gridPins[5] = 40;
  _gridPins[6] = 41;
  _gridPins[7] = 38;
  _gridPins[8] = 39;
  _decPointPin = 36;
  _segAPin = 52;
  _segBPin = 53;
  _segCPin = 50;
  _segDPin = 51;
  _segEPin = 48;
  _segFPin = 49;
  _segGPin = 46;
  
  //set driver pins as outputs
  pinMode(_decPointPin, OUTPUT);
  pinMode(_segAPin, OUTPUT);
  pinMode(_segBPin, OUTPUT);
  pinMode(_segCPin, OUTPUT);
  pinMode(_segDPin, OUTPUT);
  pinMode(_segEPin, OUTPUT);
  pinMode(_segFPin, OUTPUT);
  pinMode(_segGPin, OUTPUT);
  for (int i = 0; i < 9; i++)
  {
	pinMode(_gridPins[i], OUTPUT);
  }
  
  //init the display buffer
  for (int i = 0; i < 9; i++)
  {
	_displayBuffer[i] = NULL_BYTE;
  }
}

//write the given byte to the display buffer digit-by-digit starting from the end of the buffer + offset.
void VfdClock::writeDigits(byte time, int offset)
{
  //handle zero time case (write '00' to buffer)
  if (time == 0)
  {
	_displayBuffer[8 - offset] = 0;
	_displayBuffer[8 - offset - 1] = 0;
  }
  else
  {
	int i = 0;
    while (time > 0)
    {
	  //write starting at the end of the buffer (split byte into digits with mod 10)
	  _displayBuffer[8 - offset - i] = time % 10;
	
	  //go to the next largest digit
	  time /= 10;
	  i++;
    }
  
    //add a leading zero to the buffer if necessary
    if (i < 2)
    {
	  _displayBuffer[8 - offset - i - 1] = 0;
    }
  }
}

//reads the current time and prints it to the tube in the format 'HH-MM-SS'.
void VfdClock::displayTime()
{
	//get time as bytes
  byte hour = _clock.getHour(_is12Hr, _isPM);
  byte min  = _clock.getMinute();
  byte sec  = _clock.getSecond();
  
  //write display buffer from right to left (seconds, then mins, then hour digits)
  writeDigits(sec, 0);
  _displayBuffer[6] = DASH_BYTE;
  writeDigits(min, 3);
  _displayBuffer[3] = DASH_BYTE;
  writeDigits(hour, 6);
  
  for (int i = 1; i < 9; i++)
  {
	//illuminate the current digit grid and turn off the previous grid
	digitalWrite(_gridPins[i], LOW);
	digitalWrite(_gridPins[i-1], HIGH);
	
	//write the digit to the current grid and delay long enough to let the VFD catch up
	setDigit(_displayBuffer[i]);
	delay(2);
  }
  
  //don't leave the last digit on
  digitalWrite(_gridPins[8], HIGH);
}

//set the segment pins to display the given byte (0-9, DASH_BYTE for '-', or NULL_BYTE for nothing)
void VfdClock::setDigit(byte digit)
{
  switch (digit)
  {
	//NOTE: HIGH sets segment off, LOW sets it on (ULN2803 has inverted inputs)
	case 0:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, LOW);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, HIGH);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 1:
	  digitalWrite(_segAPin, HIGH);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, HIGH);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, HIGH);
	  digitalWrite(_segGPin, HIGH);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 2:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, HIGH);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, LOW);
	  digitalWrite(_segFPin, HIGH);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 3:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, HIGH);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 4:
	  digitalWrite(_segAPin, HIGH);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, HIGH);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 5:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, HIGH);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 6:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, HIGH);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, LOW);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 7:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, HIGH);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, HIGH);
	  digitalWrite(_segGPin, HIGH);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 8:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, LOW);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case 9:
	  digitalWrite(_segAPin, LOW);
	  digitalWrite(_segBPin, LOW);
	  digitalWrite(_segCPin, LOW);
	  digitalWrite(_segDPin, LOW);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, LOW);
	  digitalWrite(_segGPin, LOW);
	  digitalWrite(_decPointPin, HIGH);
	  break;
	case DASH_BYTE:
	  digitalWrite(_segAPin, HIGH);
      digitalWrite(_segBPin, HIGH);
      digitalWrite(_segCPin, HIGH);
      digitalWrite(_segDPin, HIGH);
      digitalWrite(_segEPin, HIGH);
      digitalWrite(_segFPin, HIGH);
      digitalWrite(_segGPin, LOW);
      digitalWrite(_decPointPin, HIGH);
      break;
	case NULL_BYTE:
	default:
	  digitalWrite(_segAPin, HIGH);
	  digitalWrite(_segBPin, HIGH);
	  digitalWrite(_segCPin, HIGH);
	  digitalWrite(_segDPin, HIGH);
	  digitalWrite(_segEPin, HIGH);
	  digitalWrite(_segFPin, HIGH);
	  digitalWrite(_segGPin, HIGH);
	  digitalWrite(_decPointPin, HIGH);
	break;
  }
}
  
void VfdClock::setDash()
{      
  digitalWrite(_segAPin, HIGH);
  digitalWrite(_segBPin, HIGH);
  digitalWrite(_segCPin, HIGH);
  digitalWrite(_segDPin, HIGH);
  digitalWrite(_segEPin, HIGH);
  digitalWrite(_segFPin, HIGH);
  digitalWrite(_segGPin, LOW);
  digitalWrite(_decPointPin, HIGH);
}  

void VfdClock::setDecPoint()
{
  digitalWrite(_segAPin, HIGH);
  digitalWrite(_segBPin, HIGH);
  digitalWrite(_segCPin, HIGH);
  digitalWrite(_segDPin, HIGH);
  digitalWrite(_segEPin, HIGH);
  digitalWrite(_segFPin, HIGH);
  digitalWrite(_segGPin, HIGH);
  digitalWrite(_decPointPin, LOW);
}