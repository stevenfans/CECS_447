// PWM LED RED  PF.1
#include "PWM.h"
#include "../tm4c123gh6pm.h"

void PWM_Init(uint16_t period, uint16_t duty){
	volatile unsigned long delay; 
	SYSCTL_RCGCPWM_R  |= 0x02;			// 1) activate PWM1
	SYSCTL_RCGCGPIO_R |= 0x20;		  // 2) activate portF
	delay = SYSCTL_RCGCGPIO_R;   
	GPIO_PORTF_AFSEL_R |= 0x02;			// 3) enable Alt Functions for PF1
	
	// configure alternate functions
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & ~0x000000F0) | 0x00000050;
	GPIO_PORTF_AMSEL_R &= ~0x02;		// 4) disable analog funcitonality
	GPIO_PORTF_DEN_R |= 0x02;				// 5) enable PBF.1
	
	SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;
	SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;
	SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;
	
	PWM1_2_CTL_R = 0; 							// counter load value
	PWM1_2_GENA_R = 0xC8;			
	PWM1_2_GENB_R = 0xC08;
	PWM1_2_LOAD_R = period - 1;			// cycles needed to count down to 0 
	PWM1_2_CMPA_R = duty - 1;
	PWM1_2_CMPB_R = duty - 1; 
	
	PWM1_2_CTL_R |= 0x00000001;			// start pwm1
	PWM1_ENABLE_R |= 0x20;					// enable 
}

void PWM_PF1_Duty(uint16_t duty){
	PWM1_2_CMPB_R = duty - 1; 
}