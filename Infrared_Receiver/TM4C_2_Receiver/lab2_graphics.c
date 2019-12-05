// Lab1.c
// Runs on TM4C123
// Uses ST7735.c LCD.
// Jonathan Valvano
// August 5, 2015
// Possible main program to test the lab
// Feel free to edit this to match your specifications


#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "ST7735.h"
#include "PLL.h"
#include "walking.h"
#include "tm4c123gh6pm.h"
void DelayWait10ms(uint32_t n);
// const will place these structures in ROM


int main(void){
	uint32_t x = 40, ht = 0;	
	int32_t l = 15, h = 15;
	
  PLL_Init(12);
  ST7735_InitR(INITR_REDTAB);
	
	// DRAW THE GROUND
	ST7735_FillScreen(ST7735_CYAN);
	ST7735_FillRect(0, 150,128, 30, ST7735_GREEN);
	  
	// DRAW THE SUN
	ST7735_FillCircle(100, 30, 15,ST7735_YELLOW);
	
  while(1){
		
  // DRAW THE MAN
	ST7735_XYPlotMan( x, l, h, ST7735_RED);
	
  DelayWait10ms(10);
		
	// REMOVE THE MAN
	ST7735_XYPlotMan( x, l, h, ST7735_CYAN);
 
	// MOVE THE COORDINATES OF THE MAN
	x++;
 
	// CODE for MOVING the LEG
	l--;
	if (l == -15)
	    l = 15;
 
	// CODE for MOVING the HAND    
	if (ht == 1)
	    h++;
	else
	    h--;
 
	if (h == 15)
	   ht = 0;
	else if (h == -15)   
		ht = 1;
 
	//Reset parameters
	  if(x>170){
			x = 40, ht = 0,l = 15, h = 15;
    }
	}

  
} 


// Subroutine to wait 10 msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}
