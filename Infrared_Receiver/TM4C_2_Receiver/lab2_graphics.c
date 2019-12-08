#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "ST7735.h"
#include "PLL.h"
#include "walking.h"
#include "tm4c123gh6pm.h"
void DelayWait10ms(uint32_t n);
// const will place these structures in ROM

void Timer0_Init();
void PortC_Init();

int total = 0;
int counter[1000];
int counter_position = 0;

int main(void){
	uint32_t x = 40, ht = 0;	
	int32_t l = 15, h = 15;
	
  PLL_Init(12);
	Timer0_Init();
	PortC_Init();
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




void Timer0_Init(){//10 us   10 us/12.5ns = 800
	SYSCTL_RCGCTIMER_R |= 0x01; //Enable Timer0
	TIMER0_CTL_R   = 0; //disable timer0a for setup
	TIMER0_CFG_R   = 0x00000000; //32 bit mode
	TIMER0_TAMR_R  = 0x00000002; //periodic mode, down-count
	TIMER0_TAILR_R = 800 - 1;  //10 us period
	TIMER0_TAPR_R  = 0;//bus clock resolution?
	TIMER0_ICR_R   = 0x00000001;//clear TIMER0A flag
	TIMER0_IMR_R   = 0x00000001;//arm timer interrupt
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x20000000; //priority 1
	NVIC_EN0_R = 1<<19;
	
	TIMER0_CTL_R = 0x00000001;    //enable TIMER0A
}

void Timer0A_Handler(){//called every 10 us
	 TIMER0_ICR_R = 0x00000001; //acknowledge timer0A flag
		// read PD0
		counter[counter_position] = GPIO_PORTC_DATA_R&0x10; // read PD4 into In
		// counter_position += 1;
		total += GPIO_PORTC_DATA_R&0x10;
}



void PortC_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000004;     // 1) C clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTC_LOCK_R = 0x4C4F434B;   // 2) unlock PortD PF0  
  GPIO_PORTC_CR_R = 0x1F;           // allow changes to PD4-0       
  GPIO_PORTC_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTC_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTC_DIR_R = 0x0E;          // 5) PD4,PD0 input, PD3,PD2,PD1 output   
  GPIO_PORTC_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTC_PUR_R = 0x11;          // enable pullup resistors on PD4,PD0       
  GPIO_PORTC_DEN_R = 0x1F;          // 7) enable digital pins PD4-PD0        
}
