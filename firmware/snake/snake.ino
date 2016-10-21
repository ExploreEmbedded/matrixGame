/* 
 *  Snake game for the LED display Matrix, written at ExploreEmbedded. 
 *  for more: 
 *  
 *  https://github.com/ExploreEmbedded/matrixGame 
 *  
 *  ALGORITHM:
 *  DRAW:create a snake of 3 pixels
 *  MOVE:user moves the snake 
 *  FEED:feed the snake randomly
 *  GROW:grow the snake, make it move faster everytime it eats
 *  DETECT: Moniter and see if the snake eats itself.
 *  WINNER: If snake keeps growing till a certain time, you win!
 *  
 *  TODO: restructure the code to look like algo above
 *  1. A single array can be used to hold the snake, instead of 2.
 *  2. Add music to into and end! 
 *  
*/  

#include "LedControl.h" 
#include "font.h"

//pins for display
const int chipSelect = 10;
const int sck = 13;
const int dout = 11;
const int maxDisplays = 2;


//pins for joyStick

const int joyX = A0;
const int joyY = A1;
const int joySelect = 2;

//speaker
const int speakerPin = 7;

static boolean gameOver = 0;

LedControl matrixDisplay = LedControl(dout, sck, chipSelect, maxDisplays);


void setup() 
{
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
  matrixDisplay.shutdown(0, false);
  matrixDisplay.setIntensity(0, 4);
  matrixDisplay.clearDisplay(0);

  //intialize second display

  matrixDisplay.shutdown(1, false);
  matrixDisplay.setIntensity(1, 8);
  matrixDisplay.clearDisplay(1);
  //drawBounds();

   
}

void loop() 
{                 
        if(!gameOver)
        { 
          t_drawSnake(snakeDirection());
        }  
        else
        {  
            scrollMsg("Game Over!", 5000);
            gameOver = 0;           
        }       
}




void scrollMsg(char *msg, uint16_t scrollTime)
{
  //function used to scroll messages on the matrix display
  //scrolls input @message for a given @time in millisecs

  //converts message to dot matrix display pattern and saves it to buffer
  //the buffer is used to scroll message for a given time
  
      
	char msgBuff[120]; 
	int segCnt = 0, buffLen, dotCnt, gCnt;
	long scrollStartTime;
	uint8_t scrollSpeed = 25; //time in milisecs, decrease it to scroll faster, increase to scroll slower
	
	
	scrollStartTime = millis();
	
	
	//create dot matrix patter in local buffer 
	while(msg[gCnt]!= '\0')
	{
	   for(dotCnt =0; dotCnt<7; dotCnt++)
	   {
		  msgBuff[(gCnt*7)+dotCnt] = pgm_read_byte(&Font[msg[gCnt]-32][dotCnt]);
	   }  
		gCnt++;
	}
	
	buffLen = gCnt * 7;  
	
	//scroll the msg for the given time.
	
	do
	{	
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

			delay(scrollSpeed);
      
			matrixDisplay.clearDisplay(0);
		    matrixDisplay.clearDisplay(1);      
		}  
		 
	}while((millis()-scrollStartTime) > scrollTime);
}

void t_drawSnake(int v_dir)
{
    //reads the joystick analog sensors and returns the direction
    // 0 - Stationary  1 - Up  2 - Right 3 - Down 4-Right

    //A 2D array hold the snake position with pixels
    static uint8_t snake[][20] = {
                                  {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},
                                  {11,1},{12,1},{13,1},{14,1},{15,1}
                                 }; 
    static uint8_t n_snake[][20] = {
                                    {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},
                                    {11,1},{12,1},{13,1},{14,1},{15,1}
                                   };
                                   
    static uint8_t v_len = 2, xPos,yPos, v_oDir; 
    static uint8_t o_cnt, i_cnt, v_matrix = 0; 

    static uint16_t snakeSpeed = 100; // in milli seconds

    //food
    static uint8_t *foodPos; //pointer to hold snake food position   
    static boolean foodDraw = 1, foodEat = 0;
    

    //make new snake in the the given direction
    
    for(o_cnt =0; o_cnt<v_len-1; o_cnt++)
    {
         for(i_cnt=0; i_cnt<2; i_cnt++)
         n_snake[o_cnt][i_cnt] = snake[o_cnt+1][i_cnt];          
    }
    

    switch(v_dir)
    {
      case 0: break;
      case 1: 
            n_snake[v_len-1][0] = (snake[v_len-1][0]%16);
            n_snake[v_len-1][1] = (snake[v_len-1][1]+1)%8; break;
      case 2: 
            n_snake[v_len-1][0] = (snake[v_len-1][0]+1)%16; 
            n_snake[v_len-1][1] = snake[v_len-1][1]%8; break;
      case 3: 
            n_snake[v_len-1][0] = snake[v_len-1][0]%16;
            
            if((snake[v_len-1][1]-1) < 0)
            {
               n_snake[v_len-1][1] = 7;
            }
            else
            {
              n_snake[v_len-1][1] = (snake[v_len-1][1]-1)%8; 
            }
            break;
      case 4: 
            if((snake[v_len-1][0]-1)<0)
            {
              n_snake[v_len-1][0] = 15;
            }
            else
            {
              n_snake[v_len-1][0] = (snake[v_len-1][0]-1)%16;
            }
            
            n_snake[v_len-1][1] = snake[v_len-1][1]%8; 
            
            break;
    }

  
    //clear old Snake
    
    for(o_cnt =0; o_cnt < v_len; o_cnt++)
    {
          if(snake[o_cnt][0]<8)
          v_matrix = 0;        
          if(snake[o_cnt][0] >= 8)
          v_matrix = 1;
          matrixDisplay.setLed(v_matrix,(snake[o_cnt][0])%8, (snake[o_cnt][1]),LOW); //draw new snake
    }

    


    //Display the new snake
    for(o_cnt =0; o_cnt < v_len; o_cnt++)
    {
          if(n_snake[o_cnt][0]<8)
          v_matrix = 0;        
          if(n_snake[o_cnt][0] >= 8)
          v_matrix = 1;
          matrixDisplay.setLed(v_matrix,(n_snake[o_cnt][0])%8, (n_snake[o_cnt][1]),HIGH); //draw new snake
    }


    //copy the snake for next time

    for(o_cnt =0; o_cnt<v_len; o_cnt++)
    {
         for(i_cnt=0; i_cnt<2; i_cnt++)
         snake[o_cnt][i_cnt] = n_snake[o_cnt][i_cnt];            
    }


    //if food is not drawn draw it and make the flag zero
     
    if(foodDraw)
    {
      foodPos = drawFood();
      foodDraw = 0;
    } 
  
    if(foodPos[0]>7)
    {
      matrixDisplay.setLed(1, foodPos[0]%8, foodPos[1], HIGH );
    }
    else
    {
      matrixDisplay.setLed(0, foodPos[0], foodPos[1], HIGH );
    }


    
    //check if snakes eats the food and grow the snake
    
    if((n_snake[v_len-1][0] == foodPos[0])&&(n_snake[v_len-1][1] == foodPos[1]))
    {
       Serial.println("snake eats food"); 
       beep(speakerPin, 400, snakeSpeed); 
       foodDraw = 1; //new food needs to be drawn 

      Serial.println("old food cleared");
      
      if(foodPos[0]>7)
      {
        matrixDisplay.setLed(1, foodPos[0]%8, foodPos[1], LOW );
      }
      else
      {
        matrixDisplay.setLed(0, foodPos[0], foodPos[1], LOW );
      }
        
      
       
        
        v_len++;  

        switch(v_dir)
        {
          case 1: 
                n_snake[v_len-1][0] = foodPos[0];
                n_snake[v_len-1][1] = foodPos[1]+1;
                snake[v_len-1][0] = foodPos[0];
                snake[v_len-1][1] = foodPos[1]+1;
                
                break;
          case 2:
                n_snake[v_len-1][0] = foodPos[0]+1;
                n_snake[v_len-1][1] = foodPos[1];
                snake[v_len-1][0] = foodPos[0]+1;
                snake[v_len-1][1] = foodPos[1];
                break;
                
          case 3:
                n_snake[v_len-1][0] = foodPos[0];
                n_snake[v_len-1][1] = foodPos[1]-1;
                snake[v_len-1][0] = foodPos[0];
                snake[v_len-1][1] = foodPos[1]-1;
                
                break;
                
          case 4:
                n_snake[v_len-1][0] = foodPos[0]-1;
                n_snake[v_len-1][1] = foodPos[1];
                snake[v_len-1][0] = foodPos[0]-1;
                snake[v_len-1][1] = foodPos[1];
                break;         
          
        }          
       snakeSpeed -= 7; //increase the speed by decreasing the delay
    }

   //check for snake collision  
   //if head hits any part on snake body the game is over

  
    for(o_cnt = 1; o_cnt < v_len-1; o_cnt++)
    {
        if((n_snake[o_cnt][0] == n_snake[0][0])&&(n_snake[o_cnt][1] == n_snake[0][1]))
        {
            //Serial.println("game over!");
            gameOver = 1; 
            v_len = 2;  //reduce the snake length
            snakeSpeed = 100; //reset the snake speed
             march(); //some music
        }

    }  

   if(v_len >14)
   {
        scrollMsg("You Win!!!", 2000);
        march(); //some music
        v_len = 2;
        snakeSpeed = 100;
   }  

   

    
    delay(snakeSpeed);
      
}


uint8_t snakeDirection()
{

   //reads the joystick analog sensors and returns the direction
   // 0 - Stationary  1 - Up  2 - Right 3 - Down 4-Right
   
   boolean uFlag = 0, dFlag = 0, rFlag = 0, lFlag = 0; 
   uint8_t v_xPos, v_yPos;
   static uint8_t direction; //retain direction during subsequent calls

    //decrease the range
   v_xPos = map(analogRead(joyX), 0, 1024, 0, 16); 
   v_yPos = map(analogRead(joyY), 0, 1024, 0, 16);
  
   if((v_xPos>=0)&&(v_xPos < 4))
   {  
      lFlag = 1;
      direction = 3;
   }

   if((v_xPos >= 12)&&(v_xPos < 16))
   {
      rFlag = 1;
      direction = 1;
   }

   if((v_yPos >=0 )&&(v_yPos<4))
   {
      dFlag =1;
      direction = 4;
   }

   if((v_yPos >= 12)&&(v_yPos<16))
   {
      uFlag = 1;
      direction =2;
   }
   return direction;
  
}


uint8_t* drawFood()
{   
    uint8_t v_xPos, v_yPos,  v_matrix=0; //position of food
    static uint8_t v_Pos[2]; // to retain value
    
    
    v_xPos = random(15);
    v_yPos = random(7);
    v_matrix = random(2);

   // matrixDisplay.setLed(v_matrix,(v_xPos%8),v_yPos,HIGH);

    v_Pos[0] = v_xPos%8;
    v_Pos[1] = v_yPos;

    Serial.print(v_Pos[0]);
    Serial.print("  ---   ");
    Serial.println(v_Pos[1]);

    
    return v_Pos;
    
}
void drawBounds()
{
  int bRow, bCol, drawSpeed =10; 
  matrixDisplay.setRow(0,0,0xff);
  delay(drawSpeed);
  for(bRow=1; bRow<16; bRow++)
  {
    if(bRow<8)
    {
      matrixDisplay.setRow(0,bRow,0x81);  
    }
    else
    {
      matrixDisplay.setRow(1,(bRow%8),0x81);
    }
    delay(drawSpeed);
  }
  delay(drawSpeed);
  matrixDisplay.setRow(1,7,0xff);
 
}

void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{ 
   
    int x;   
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
 
    for (x=0;x<loopTime;x++)   
    {    
        digitalWrite(speakerPin,HIGH);
        delayMicroseconds(delayAmount);
        digitalWrite(speakerPin,LOW);
        delayMicroseconds(delayAmount);
    }    

    delay(20);
    //a little delay to make all notes sound separate
}    

//music for win or loose

void march()
{    
    beep(speakerPin, N_a, 50); 
    beep(speakerPin, N_a, 50);     
    beep(speakerPin, N_a, 50); 
    beep(speakerPin, N_f, 35); 
    beep(speakerPin, N_cH, 15);
    
    beep(speakerPin, N_a, 50);
    beep(speakerPin, N_f, 35);
    beep(speakerPin, N_cH, 15);
    beep(speakerPin, N_a, 100);
    //first bit
    
    beep(speakerPin, N_eH, 50);
    beep(speakerPin, N_eH, 50);
    beep(speakerPin, N_eH, 50);    
    beep(speakerPin, N_fH, 35); 
    beep(speakerPin, N_cH, 15);
    
    beep(speakerPin, N_gS, 50);
    beep(speakerPin, N_f, 35);
    beep(speakerPin, N_cH, 15);
    beep(speakerPin, N_a, 100);
    //second bit...    
}



