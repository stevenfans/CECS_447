<<<<<<< HEAD
#include "tm4c123gh6pm.h"	
#define ON_BOARD_LED						(*((volatile unsigned long *)0x40025038))
#include "PLL.h"
#include "SysTick.h"
=======
// This is your first program to run on the LaunchPad
// You will run this program without modification as your Lab 2
// If the left switch SW1 is 
//      not pressed the LED toggles blue-red
//      pressed the LED toggles blue-green

// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// Lab2_HelloLaunchPad, Input from PF4, output to PF3,PF2,PF1 (LED)
// Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi
// Date: January 15, 2016

// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses

///////////////////////////////////////
//  Register definitions
///////////////////////////////////////
//Port f
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//Interrupt Port F
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI7_R             (*((volatile unsigned long *)0xE000E41C))  // IRQ 28 to 31 Priority Register
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_IS_R         (*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R        (*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R        (*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_IM_R         (*((volatile unsigned long *)0x40025410))
#define GPIO_PORTF_RIS_R        (*((volatile unsigned long *)0x40025414))
#define GPIO_PORTF_ICR_R        (*((volatile unsigned long *)0x4002541C))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_PDR_R        (*((volatile unsigned long *)0x40025514))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
//Systick
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

>>>>>>> f5d9ce207b37f8e9af71f3927fa695fcc59e2f16
// 2. Declarations Section
//   Global Variables
unsigned long In;  // input from PF4
unsigned long Out; // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void EnableInterrupts(void);
void WaitForInterrupt(void);  // low power mode
void EdgeCounter_Init(void);
<<<<<<< HEAD
void sawtoothWave(int delay);
void triangleWave(int delay); 
void squareWave(int delay); 
void Init_PortB(void); 
=======
>>>>>>> f5d9ce207b37f8e9af71f3927fa695fcc59e2f16
volatile unsigned long Counts = 0;
volatile unsigned long FallingEdges = 0;


//void SysTick_Init(unsigned long period){
//  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
//  NVIC_ST_RELOAD_R = period-1;// reload value
//  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
//  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
//                              // enable SysTick with core clock and interrupts
//  NVIC_ST_CTRL_R = 0x07;
//}


//void Systick_1_Second(int seconds){
//	unsigned long i;
//	for (i = 0; i <= seconds;  i++){
//		WaitForInterrupt();
//	}
//}

	// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
	
	//unsigned int i;
	Counts = 0;
<<<<<<< HEAD
  //PortF_Init();        						// Call initialization of port PF4 PF2    
	//SysTick_Init(16000000);        		//initialize SysTick timer
	EdgeCounter_Init();
	SysTick_Init();
	Init_PortB();
	GPIO_PORTF_DATA_R = 0x02;
	EnableInterrupts();          		 	//AFTER inits, should be global 	
	PLL_Init();
  while(1){
    //Systick_1_Second(FallingEdges);	//delay using systick
		//ON_BOARD_LED ^= 0x04; 					// toggle blue led on tm4c
		//GPIO_PORTF_DATA_R = 0x02; 
		//sawtoothWave(5150);
		squareWave(660000);
		//squareWave(10000); 
		//triangleWave(0); 
//		switch(FallingEdges){
//		case 1:
//			GPIO_PORTF_DATA_R = 0x02; // Red
//			sawtoothWave(0);
//			SysTick_Wait(3255); 
//			break;
//		case 2:
//			GPIO_PORTF_DATA_R = 0x04; // Blue
//			triangleWave(0);
//			break;
//		case 3:
//			GPIO_PORTF_DATA_R = 0x08; // Green
//			FallingEdges = 0;
//			squareWave(0);
//			break;
//	}
  }
}

void sawtoothWave(int delay){
	unsigned int i; 
	for (i = 0; i < 256; i++){
		GPIO_PORTB_DATA_R = i ; 
		SysTick_Wait(delay); 
	}
}

void triangleWave(int delay){
	unsigned int i; 
	for (i = 0; i < 256; i++){
		GPIO_PORTB_DATA_R = i; 
		//TODO: INCLUDE DELAY
		SysTick_Wait(2550);
	}
	 
	for (i = 255; i>0; i--){
		GPIO_PORTB_DATA_R = i; 
		//TODO: INCLUDE DELAY
		SysTick_Wait(2550); 
	}
}

void squareWave (int delay){

	GPIO_PORTB_DATA_R = 0xFF; 
	
	SysTick_Wait(delay);
	
	
	GPIO_PORTB_DATA_R = 0x00; 
	
	SysTick_Wait(delay); 
	
}
=======
	FallingEdges = 1;
  //PortF_Init();        // Call initialization of port PF4 PF2    
	SysTick_Init(16000000);        // initialize SysTick timer
	EdgeCounter_Init();
	EnableInterrupts();           // * AFTER inits, should be global
  while(1){
    Systick_1_Second(FallingEdges);
    GPIO_PORTF_DATA_R = 0x04;    // LED is blue
    Systick_1_Second(FallingEdges);
		GPIO_PORTF_DATA_R = 0x00;    // LED is off
  }
}

>>>>>>> f5d9ce207b37f8e9af71f3927fa695fcc59e2f16

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C	
// white    RGB    0x0E
// pink     R-B    0x06

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...


void SysTick_Handler(void){
	//Nothing important - no handling
  //GPIO_PORTF_DATA_R ^= 0x04;       // toggle PF2
  //Counts = Counts + 1;
}

// global variable visible in Watch window of debugger
// increments at least once per button press
void EdgeCounter_Init(void){      
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
  FallingEdges = 0;             // (b) initialize counter
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

<<<<<<< HEAD
void Init_PortB(void){ 
	//unsigned int delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
 // delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_CR_R |= 0xFF;           // allow changes to PB7-0      
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0xFF  ;          // 5) PB7-PB0 output 
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_DEN_R |= 0xFF;          // 7) enable digital pins P7-PB0   
}

=======
>>>>>>> f5d9ce207b37f8e9af71f3927fa695fcc59e2f16
void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
	FallingEdges += 1;
	GPIO_PORTB_DATA_R += 1; 
	GPIO_PORTF_DATA_R = 0x08; 
	if(FallingEdges > 3) {FallingEdges =0 ; }
}
