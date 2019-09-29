#include "tm4c123gh6pm.h"	
#define ON_BOARD_LED						(*((volatile unsigned long *)0x40025038))
#include "PLL.h"
#include "SysTick.h"
#include "ADCSWTrigger.h"
// 2. Declarations Section
//   Global Variables
unsigned long In;  // input from PF4
unsigned long Out; // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void EnableInterrupts(void);
void WaitForInterrupt(void);  // low power mode
void EdgeCounter_Init(void);
void sawtoothWave(int delay);
void triangleWave(int delay); 
void squareWave(int delay); 
void sineWave(int delay); 
void Init_PortB(void); 
volatile unsigned long Counts = 0;
volatile unsigned long FallingEdges = 0;
float getDelay(unsigned long ADCvalue);

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

	// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
	unsigned long potentiometer, sensor1, sensor2, percent;
	Counts = 0;
  //PortF_Init();        						// Call initialization of port PF4 PF2    
	//SysTick_Init(16000000);        		//initialize SysTick timer
	ADC_Init298();
	EdgeCounter_Init();
	SysTick_Init();
	Init_PortB();
	GPIO_PORTF_DATA_R = 0x02;
	EnableInterrupts();          		 	//AFTER inits, should be global 	
	PLL_Init();
  while(1){
		// 50 MHz -> 20   nsec
		// 80 mhz -> 12.5 nsec
		// delay 65.1us / 20 nsec = 3255
		// 1 delay = units of 62.5 nsec for 80 MHz clock
		// 1 delay = units of 20   nsec for 50 MHz clock
		// 65.1us / 12.5nsec = 5208
		// 262 hz = 14.92us/12.5ns = 1194 delay units
		// 494 hz = 7.8uS/12.5ns = 625 delay units
		
		ADC_In298(&potentiometer, &sensor1, &sensor2); // sample AIN2(PE1), AIN9 (PE4), AIN8 (PE5)
		
		switch(FallingEdges){
			case 1:
				GPIO_PORTF_DATA_R = 0x02; // Red
				sawtoothWave(5150); 
				break;
			case 2:
				GPIO_PORTF_DATA_R = 0x04; // Blue
				squareWave(2550);
				break;
			case 3:
				GPIO_PORTF_DATA_R = 0x08; // Green
				triangleWave(2550); 
				break;
			
			case 4:
				GPIO_PORTF_DATA_R = 0x06; //pink
				 sineWave(5150); // 60hz
				break; 
			case 5:
				GPIO_PORTF_DATA_R = 0x0A; //yello
				sineWave(getDelay(potentiometer));
				break; 
		}
	}
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

void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
	FallingEdges += 1;
	if(FallingEdges > 5) {FallingEdges =1;}
}


float getDelay(unsigned long ADCvalue){
	int adcTable[]  = {4095, 3584, 3328, 3072, 2816, 2560, 2048, 1792, 1536, 1280, 1024,  768,  512,  256,    0};
	int distTable[] = { 565,  667,  694,  722,  747,  777,  810,  842,  890,  919,  965, 1015, 1067, 1129, 1194}; // delay table
	int hzTable[]   = { 494,  468,  449,  433,  418,  402,  386,  371,  355,  340,  324,  308,  293,  277,  262}; // hz table
	float distance_ADC = 0;  //  <---- THIS USE TO BE CALLed distance but is now changed to distance_ADC so be aware
	float calibration = 0;
	float a = 0;
	float b = 0;
	int ia = 0;
	int ib = 0;
	float m = 0;
	float l = 0;
	float lm;
	int i;
	int f;
	for(i = 0; i < 15; i = i + 1){
			if(ADCvalue > adcTable[i]){
				break;
			}
			else{
				a = adcTable[i+1];
				ia = i+1;
			}
		}
		
		for(f = 0; f < 15; f = f + 1){
			if(ADCvalue < adcTable[f]){
				b = adcTable[f];
				ib = f;
			}
			else {
				break;
			}
		}
		 m = b - a;
		 l = b - ADCvalue;
		lm = l / m ;
		
		//float distance = distTable[ib] + (lm * 5);
		//return distance;
		return distTable[ib] + (lm * 5);
}
