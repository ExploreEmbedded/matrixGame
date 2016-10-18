#ifndef _PINGPONG_H
#define _PINGPONG_H

#include "arduino.h"


/***************************************************************************************************
                             Function prototypes
***************************************************************************************************/
void pingPongInit();
void pingPongPlay();
void setLED(int,int,bool);
void setROW(int, char);
void showBars(bool);
void showLetter(int, uint8_t *);
void copyLetter(uint8_t *, int);
void winner();
void beep();
void march();
void loose();

/**************************************************************************************************/

#endif
