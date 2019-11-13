// PWM LED RED  PF.1
#include "PWM.h"
#include "../tm4c123gh6pm.h"

void PWM_Init(uint16_t period, uint16_t duty){
	volatile unsigned long delay; 
	SYSCTL_RCGCPWM_R  |= 0x01;			// 1) activate PWM0
	SYSCTL_RCGCGPIO_R |= 0x20;		  // 2) activate portF
	delay = SYSCTL_RCGCGPIO_R;   
	GPIO_PORTF_AFSEL_R |= 0x02;			// 3) enable Alt Functions for PF1
	
	// configure alternate functions
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & ~0x000000F0) | 0x00000050;
	GPIO_PORTF_AMSEL_R &= ~0x04;		// 4) disable analog funcitonality
	GPIO_PORTF_DEN_R |= 0x04;				// 5) enable PBF.1
	
	SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;
	SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;
	SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;
	
	PWM0_2_CTL_R = 0; 							// counter load value
	PWM0_2_GENA_R = 0xC8;						
	PWM0_2_LOAD_R = period - 1;			// cycles needed to count down to 0 
	PWM0_2_CMPA_R = duty - 1;
	
	PWM0_2_CTL_R |= 0x00000001;			// start pwm0 	
	PWM0_ENABLE_R |= 0x40;					// enable 
}

void PWM_PF1_Duty(uint16_t duty){
	PWM0_2_CMPA_R = duty - 1; 
}