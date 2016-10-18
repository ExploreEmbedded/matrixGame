#include "LedControl.h" // library for the led matrix
#include "matrixgame.h"
#include "pingpong.h"
#include <avr/pgmspace.h>

#define maxItemSize 15 //Maximum characters for a game Name
//Spearker/buzzer

//games to support
//pingpong
//snake
//tetris
//brick breaker...


const int speakerPin = 7;

//pins for display
const int chipSelect = 10;
const int sck = 13;
const int dout = 11;
const int maxDisplays = 2;

//pins for joyStick

const int joyX = A0;
const int joyY = A1;
const int joySelect = 2;


//joyStick Pins

//fonts



uint8_t gCnt = 0, charCnt = 0, itemSize = 0;
//Game Menu

uint8_t itemSelected =0; 


char menu[][maxItemSize] = {"Tetris", "Ping pong", "B Rally", "Race", "Armagadon"};
char submenu[][maxItemSize] = {"Enter to Play", "Highest Score"};

LedControl matrixDisplay = LedControl(dout, sck, chipSelect, maxDisplays);

void setup()
{

  pinMode(joySelect,INPUT_PULLUP); 
  matrixDisplay.shutdown(0, false);
  matrixDisplay.setIntensity(0, 4);
  matrixDisplay.clearDisplay(0);

  //intialize second display

  matrixDisplay.shutdown(1, false);
  matrixDisplay.setIntensity(1, 8);
  matrixDisplay.clearDisplay(1);

  Serial.begin(9600);

  itemSelected = dispMenu(menu,sizeof(menu)/maxItemSize);

}




void loop() {
  // put your main code here, to run repeatedly:

    
    //Serial.println(map(analogRead(joyX),0,1024,-1,2));

    Serial.print("Item Selected");
    Serial.println(itemSelected);

    pingPongInit();
    pingPongPlay();
    //delay(1000);
    


}






int dispMenu(char msg[][maxItemSize], int menuLength) 
{

  char msgBuff[120]; 
  int msgCnt, segCnt = 0, buffLen, dotCnt, msgNum=1, msgLen;
  int dispLen = 0; 
  buffLen = 0;
  
  
  do{ 
        msgNum += map(analogRead(joyX),0,1024,-1,2);
        if((msgNum >= menuLength)||(msgNum<0))
        msgNum = 0; 
        Serial.print("Message Number");
        Serial.println(msgNum);
        
        while(msg[msgNum][gCnt]!= '\0')
        {
             for(dotCnt =0; dotCnt<7; dotCnt++)
             {
                msgBuff[(gCnt*7)+dotCnt] = pgm_read_byte(&Font[msg[msgNum][gCnt]-32][dotCnt]);
             }  
              gCnt++;
        }
        Serial.print(gCnt);
        buffLen = gCnt * 7;  
      
        for(gCnt =0; gCnt< buffLen; gCnt++)
        {
             for(segCnt = 16; segCnt > 0; segCnt--)
             {    
               if(segCnt<8)
               {
      
                  if((gCnt+segCnt)<buffLen)
                  {
                         matrixDisplay.setRow(0,segCnt,msgBuff[(gCnt*1)+segCnt]);
                  }
                  else
                  {
                        matrixDisplay.setRow(0,segCnt,0);
                  }
                  
              }
              else
              {
                 if((gCnt+segCnt)<buffLen)
                 {
                  matrixDisplay.setRow(1,segCnt%8,msgBuff[(gCnt*1)+segCnt]);
                 }
                 else
                 {
                  matrixDisplay.setRow(1,segCnt%8,0);
                 }
              }            
            }   
      
            delay(25);
            matrixDisplay.clearDisplay(0);
            matrixDisplay.clearDisplay(1);      
         }    
         gCnt = 0; 
  }while(digitalRead(joySelect));    
  while(!digitalRead(joySelect));
  return msgNum;
}

        

