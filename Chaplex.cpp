/*
||
|| @author Stefan Götze 
||         using Alexander Brevig - Charlieplex library
|| @version 0.1
||
|| Changes by Christoph Tack:
    - reducing memory footprint
    - storing states in a byte instead of byte array.  That allows for 8x7 LEDs.
        If you want more, change the type to uint16 or uint32.
*/

#include "Chaplex.h"

Chaplex::Chaplex(byte* userPins, byte nrOfPins): numberOfPins(nrOfPins)
{
    pins = malloc(sizeof(byte) * numberOfPins);
    memcpy(pins, userPins, nrOfPins);
    ledCtrl = (byte *)malloc(sizeof(byte) * numberOfPins);
    allClear();
    showLedState();
}

Chaplex::~Chaplex()
{
   if (pins)
   {
      delete[] pins;
   }
   if(ledCtrl)
   {
       delete[] ledCtrl;
   }
}

// set control-array for one LED ON/OFF
bool Chaplex::setLedState(charlieLed led, LEDSTATE state)
{
    if ((led.a<numberOfPins) && (led.c<numberOfPins) && (led.a!=led.c))
    {
        bitWrite(ledCtrl[led.a],led.c, state);
        return true;
    }
    return false;
}

//set control-array for all LEDs OFF
void Chaplex::allClear()
{
    memset(ledCtrl, OFF, numberOfPins);
}

// Turn a single LED ON at full brightness or OFF.  All other LEDs will be turned off as well.
void Chaplex::setSingleLed(charlieLed led, LEDSTATE state)
{
    if(!setLedState(led, state))
    {
        return;
    }
    for (byte i=0; i<numberOfPins; i++)
    {
        pinMode(pins[i], i==led.a || i==led.c ? OUTPUT:INPUT);
    }
    digitalWrite(pins[led.a], HIGH);
    digitalWrite(pins[led.c], LOW);
}


// Write led states to pins.  Needs to be called regularly for persistance of vision.
void Chaplex::showLedState()
{
    pinMode(pins[ledRow==0 ? numberOfPins-1 : ledRow-1],INPUT);
    for (byte i=0; i<numberOfPins; i++)
    {
        if(i!=ledRow)
        {
            pinMode(pins[i],bitRead(ledCtrl[ledRow],i) ? OUTPUT : INPUT);
            digitalWrite(pins[i],LOW);
        }
    }
    digitalWrite(pins[ledRow],HIGH);
    pinMode(pins[ledRow],OUTPUT);
    if (++ledRow >= numberOfPins)
    {
        ledRow = 0;
    }
}
