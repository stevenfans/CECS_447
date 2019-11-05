// UARTTestMain.c
// Runs on LM4F120/TM4C123
// Used to test the UART.c driver
// Daniel Valvano
// September 12, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART.h"
#include "SysTick.h"


void PORTF_Init(void);
void sawtoothWave(int delay);
void triangleWave(int delay); 
void squareWave(int delay); 
void sineWave(int delay); 
void Init_PortB(void);
void EnableInterrupts(void);

const int sineTable[256]={128,131,134,137,140,143,146,149,
152,156,159,162,165,168,171,174,
176,179,182,185,188,191,193,196,
199,201,204,206,209,211,213,216,
218,220,222,224,226,228,230,232,
234,235,237,239,240,242,243,244,
246,247,248,249,250,251,251,252,
253,253,254,254,254,255,255,255,
255,255,255,255,254,254,253,253,
252,252,251,250,249,248,247,246,
245,244,242,241,239,238,236,235,
233,231,229,227,225,223,221,219,
217,215,212,210,207,205,202,200,
197,195,192,189,186,184,181,178,
175,172,169,166,163,160,157,154,
151,148,145,142,138,135,132,129,
126,123,120,117,113,110,107,104,
101,98,95,92,89,86,83,80,
77,74,71,69,66,63,60,58,
55,53,50,48,45,43,40,38,
36,34,32,30,28,26,24,22,
20,19,17,16,14,13,11,10,
9,8,7,6,5,4,3,3,
2,2,1,1,0,0,0,0,
0,0,0,1,1,1,2,2,
3,4,4,5,6,7,8,9,
11,12,13,15,16,18,20,21,
23,25,27,29,31,33,35,37,
39,42,44,46,49,51,54,56,
59,62,64,67,70,73,76,79,
81,84,87,90,93,96,99,103,
106,109,112,115,118,121,124,128}; 


int main(void){
	unsigned char x;
  unsigned char i;
  char string[20];  // global to assist in debugging
  unsigned long c;
	unsigned long previous = 'r'; //default r: sawtooth
	
	SysTick_Init();
		Init_PortB();
  
  UART_Init();              // initialize UART
	
	
  OutCRLF();
	
	PORTF_Init();
		EnableInterrupts();          		 	//AFTER inits, 
	
	PLL_Init();
	
  GPIO_PORTF_DATA_R = 0x02; // Red
	
	UART_OutString("UART Initialized"); OutCRLF();
	UART_OutString("Sine     (S)"); OutCRLF();
	UART_OutString("Triangle (T)"); OutCRLF();
	UART_OutString("Square   (Q)"); OutCRLF();
	UART_OutString("Sawtooth (R)"); OutCRLF();
	
	
	
  while(1){
		// Baud rate: 115,200
		// Data: 8 bit
		// Parity: none
		// Stop: 1 bit
		c = UART_InCharNonBlocking();
		
		
		if (c=='s') {
			UART_OutString("sine wave displayed");
			OutCRLF();
			previous = 's';
			
		}
		else if (c=='t'){
			UART_OutString("triangle wave displayed");
			OutCRLF();
			previous = 't';
		}
		else if (c == 'q'){
			UART_OutString("square wave displayed");
			OutCRLF();
			previous = 'q';
		}
		else if (c == 'r'){
			UART_OutString("Sawtooth wave displayed");
			OutCRLF();
			previous = 'r';
		}
		else if (c=='0'){
			// No message
			previous = previous;
		}
			
    if (previous == 'r'){
			sawtoothWave(5150);
		}
		else if (previous == 't'){
			triangleWave(2550); 
		}
		else if (previous == 's'){
			sineWave(5150);
		}
		else if (previous == 'q'){
			squareWave(2550);
			
		}
		//else { sawtooth default }
  }
}


// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C	
// white    RGB    0x0E
// pink     R-B    0x06

// global variable visible in Watch window of debugger
// increments at least once per button press
void PORTF_Init(void){      
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        	
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x10;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x10;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}


void sawtoothWave(int delay){
	unsigned int i; 
	for (i = 0; i < 256; i++){
		GPIO_PORTB_DATA_R = i ; 	
		SysTick_Wait(5100); 
	}
}

void triangleWave(int delay){
	unsigned int i; 
	for (i = 0; i < 256; i++){
		GPIO_PORTB_DATA_R = i; 
		SysTick_Wait(delay);
	}
	 
	for (i = 255; i>0; i--){
		GPIO_PORTB_DATA_R = i; 
		SysTick_Wait(delay); 
	}
}

void squareWave (int delay){
	unsigned int i; 
	for (i = 0; i < 256; i++){
		GPIO_PORTB_DATA_R = 0xFF; 
		SysTick_Wait(delay);
	}
	 
	for (i = 255; i>0; i--){
		GPIO_PORTB_DATA_R = 0x00; 
		SysTick_Wait(delay); 
	}
}
void sineWave(int delay){	
	unsigned int i ; 
	for(i=0; i<256; i++){
		GPIO_PORTB_DATA_R = sineTable[i]; 
		SysTick_Wait(delay); 
	}
}


void Init_PortB(void){ 
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  GPIO_PORTB_CR_R |= 0xFF;           // allow changes to PB7-0      
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0xFF  ;          // 5) PB7-PB0 output 
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_DEN_R |= 0xFF;          // 7) enable digital pins P7-PB0   
}
