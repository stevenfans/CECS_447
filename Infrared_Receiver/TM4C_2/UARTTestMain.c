#include "PLL.h"
#include "UART.h"
#include "Tone.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "IR_Demod.h"
#include <stdio.h>
#include <stdlib.h>

#define SIGNAL       				(*((volatile unsigned long *)0x40004030))
#define LIGHT 							(*((volatile unsigned long *)0x40025038))
#define RED 		0x02; 
#define BLUE 		0x04; 
#define GREEN   0x08; 
#define WHITE   0x0E; 

int running = 0; 
int startPulseDetected =0; 
int  packet[5]; // array to hold 1 or 0
unsigned long packet_element = 0; 
unsigned long loop_0,loop_1=0; 
int testingFlag = 0; 
int firstTime = 0; 
int packetArray[1050]; 
int arrayFull = 0; 
int packet_array_element; 
int done=0; 

void PORTF_Init(void);
void Timer0_Init(void); 

int checkStartPulse(void); 
//void check1(); 
//void check0(); 
//void masterReset();
//void readPacket(); 
void decodePacket(void); 

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}


int main(void){
	unsigned long frequency = 262;
	unsigned long last_frequency = 262;
	unsigned long freq_value;  
	//char string [10]; 
	unsigned int k ; 
	
	PLL_Init();
  UART_Init();
	UART2_Init();
	
	Init_PortA(); 
	Init_PortB();
	PORTF_Init();
	SysTick_Init(800); 
	Timer0_Init();
 
	UART_OutChar('a'); 
	OutCRLF(); 
  while(1){
		//UART_OutUDec(done); 
		if(done){
			for(k=0; k<5;k++){
				UART_OutUDec(packet[k]);
			}
			OutCRLF(); 
			done=0; 
			}

	}
}

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


// interrupt checks to see the start of the decoding
void GPIOPortA_Handler(void){
	if (GPIO_PORTA_RIS_R & 0x04){ // sw1 is pressed
		GPIO_PORTA_ICR_R = 0x04; // clear flag
		GPIO_PORTF_DATA_R = 0x08; 
		if (running==0&&firstTime==0){
			running = 1; 
			firstTime=1; 
			packet_element=0; 
		}
	}
}

int checkStartPulse(){
	//determine if its there is an accurate start pulse
	if(packetArray[100] ==0){ // check to see if the signal was low 
		UART_OutString("Start Pulse Detected"); OutCRLF(); 
		return 1; 							// start signal should be low at this point 
	}
	else {
		UART_OutString("Start Pulse NOT Detected");
		return 0; 
	}
 }

 void decodePacket(){
	 int count=267; 
	 unsigned int k; 
	 //skip the start pulse 
	 for(k=0;k<5;k++){
		 if(packetArray[count]==0){//logic 1
			 packet[k]=1;
			 count += 135;
			 //UART_OutString("*********   1    ********");
		 }
		 else{//logic 0
			 packet[k]=0; 
			 count+=88;
			 //UART_OutString("***********  0    **********");

		 }
	 }
	 startPulseDetected=0; 
	 done = 1;
	 firstTime=0; 
	 UART_OutString("DONE FLAG: "); UART_OutUDec(done); OutCRLF(); 
 }



void SysTick_Handler(){
}

void Timer0_Init(){//10 us
	SYSCTL_RCGCTIMER_R |= 0x01; //Enable Timer0
	TIMER0_CTL_R   = 0; //disable timer0a for setup
	TIMER0_CFG_R   = 0x00000000; //32 bit mode
	TIMER0_TAMR_R  = 0x00000002; //periodic mode, down-count
	TIMER0_TAILR_R = 800;//10 us period
	TIMER0_TAPR_R  = 0;//bus clock resolution?
	TIMER0_ICR_R   = 0x00000001;//clear TIMER0A flag
	TIMER0_IMR_R   = 0x00000001;//arm timer interrupt
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x20000000; //priority 1
	NVIC_EN0_R = 1<<19;
	
	TIMER0_CTL_R = 0x00000001;    //enable TIMER0A
}

void Timer0A_Handler(){//called every 100 us
	 TIMER0_ICR_R = 0x00000001; //acknowledge timer0A flag

	if(running){
		if (SIGNAL==0) packetArray[packet_array_element]=0;  
		else packetArray[packet_array_element]=1;  	
		packet_array_element++;
	}
	if(packet_array_element==1050){
		running = 0;
		packet_array_element=0; 
		testingFlag = 1; 
		arrayFull=1; 
		startPulseDetected = checkStartPulse(); 
	}
	
	if(startPulseDetected){
		decodePacket(); 
	}
	
	  
}

