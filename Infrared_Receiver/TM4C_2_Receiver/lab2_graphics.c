#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "ST7735.h"
#include "SysTick.h"
#include "PLL.h"
#include "waveforms.h"
#include "walking.h"
#include "tm4c123gh6pm.h"

int frequency = 262;
int command[5] = {0,0,0,1,0};

int main(void){
	PLL_Init(4);
	SysTick_Init(80000000/256/frequency); // Clk / sine wave steps / desired frequency
	Init_PortB();
  ST7735_InitR(INITR_REDTAB);
	
	
  while(1){
		if (command[0]==0 && command[1]==0 && command[2]== 0 && command[3]==0 && command[4] == 0){
			walking_animation();
		}
		else if (command[0]==0 && command[1]==0 && command[2]== 0 && command[3]==0 && command[4] == 1){
			waving_animation();		}
		else if (command[0]==0 && command[1]==0 && command[2]== 0 && command[3]==1 && command[4] == 0){
			car_animation();
		}
		else if (command[0]==0 && command[1]==0 && command[2]== 0 && command[3]==1 && command[4] == 1){
			face_animation();
		}
		else{
			clear_animation();
		}
	}
  
}

