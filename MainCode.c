#include <stdint.h>
#include <stdio.h>

//**GENERAL VIEW**//

/*
Using previous setup of 3 Bit DAC Lab, I'm going to make a simple piano with 3 buttons as the input set with certain frequency

Here are the plot for this little project:
1. Using PK1,PK2,PK3 as the output for the 3 Bit DAC like the previous lab
2. Using PC4,PC5, PC7 as the input for the 3 buttons ( will be set using edge interrupts)
3. SysTick will work like usual, if the button is not pressed, SysTick load value will be 0
4. In GPIO C Handler, create 3 possibilities ( if button PC4, or PC 5, or PC 6 or both of those three pressed, create a special Hz )

*/





//**ADDRESSES**//

//GPIO K 
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTK_DATA_R       (*((volatile uint32_t *)0x400613FC))
#define GPIO_PORTK_DIR_R        (*((volatile uint32_t *)0x40061400))
#define GPIO_PORTK_DEN_R        (*((volatile uint32_t *)0x4006151C))
	
//GPIO C
#define GPIO_PORTC_BASE_R       (*((volatile uint32_t *)0x40006000))
#define GPIO_PORTC_DATA_R       (*((volatile uint32_t *)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((volatile uint32_t *)0x40006400))
#define GPIO_PORTC_DEN_R        (*((volatile uint32_t *)0x4000651C))
#define GPIO_PORTC_PUR_R        (*((volatile uint32_t *)0x40006510)) //Offset 0x510
#define GPIO_PORTC_IS_R 				(*((volatile uint32_t *)0x40006404)) //Offset 0x404
#define GPIO_PORTC_IBE_R 				(*((volatile uint32_t *)0x40006408)) //Offset 0x408
#define GPIO_PORTC_IEV_R 				(*((volatile uint32_t *)0x4000640C)) //Offset 0x40C
#define GPIO_PORTC_ICR_R 				(*((volatile uint32_t *)0x4000641C)) //Offset 0x41C
#define GPIO_PORTC_IM_R 				(*((volatile uint32_t *)0x40006410)) //Offset 0x410
#define GPIO_PORTC_RIS_R 				(*((volatile uint32_t *)0x40006414)) //Offset 0x414
	



//SysTick
#define NVIC_STCTRL_R						(*((volatile uint32_t *)0xE000E010)) //Offset 0x010
#define NVIC_STRELOAD_R					(*((volatile uint32_t *)0xE000E014)) //Offset 0x014
#define NVIC_STCURRENT_R				(*((volatile uint32_t *)0xE000E018)) //Offset 0x018
#define NVIC_SYS_PRI3_R  				(*((volatile uint32_t *)0xE000E40C)) //PRI3 because SysTick interrupt ( OFFSET 0X40C)

//Edge Interrupts
#define NVIC_EN0_R  						(*((volatile uint32_t *)0xE000E100)) //Because based on the vector (2), still in the EN0 for the GPIO C, but in bit 2
#define NVIC_PRI0_R  						(*((volatile uint32_t *)0xE000E400)) //PRI0 because interrupt 2 ( Port C ) ( OFFSET 0X400)

//**GLOBAL VAR**//

const uint8_t SineWave[16] = {4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3}; //to match a sinewave form
uint8_t Index = 0; // for indexing the wave sample


//**FUNCTIONS**//
// Enable global interrupts
void EnableInterrupts(void) {
    __asm("CPSIE I");  // CPSIE I = Clear Interrupt Disable bit, enabling interrupts
}

// Wait for interrupt 
void WaitForInterrupts(void) {
    __asm("WFI");  // WFI = Wait For Interrupt instruction
}

void GPIO_K_Init(void)
{
	SYSCTL_RCGCGPIO_R = 0x0200; //0000 0010 0000 0000, Port K Activated
	GPIO_PORTK_DIR_R = 0x0E; //0000 1110, PK1 PK2 PK3 Output
	GPIO_PORTK_DEN_R = 0X0E; // Enable digital function for PK1,PK2,PK3
}

void GPIO_C_Init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x04; //turning on Clock for port C without disturbing other bits
	GPIO_PORTC_DEN_R = 0xF0; //0111 0000,PC4-6 digital enable
	GPIO_PORTC_DIR_R &= ~0xF0; //1000 1111, PC4-6 Input
	GPIO_PORTC_PUR_R |= 0xF0; // 0111 0000 , PC4-6 pull up enable
	GPIO_PORTC_IS_R &= ~0xF0; //1000 1111
	GPIO_PORTC_IBE_R &= ~0xF0;
	GPIO_PORTC_IEV_R &= ~0xF0;
	GPIO_PORTC_ICR_R = 0xF0;
	GPIO_PORTC_IM_R |= 0xF0;
	
	NVIC_PRI0_R = (NVIC_PRI0_R & 0xFF1FFFFF) | 0x00200000; //Priority 1
	NVIC_EN0_R = 0x04; //bit 2 cause interrupt 2
}

void Systick_Init(void)
{	
	NVIC_STCTRL_R = 0;
	//NVIC_STRELOAD_R = 10000-1; //1600 Hz, with 16Mhz base frequency
	NVIC_STCURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x60000000; //Priority 3
	NVIC_STCTRL_R = 0x07; // enable bit 0-2
}

void DAC_Funct(uint8_t value)
{
	GPIO_PORTK_DATA_R = (value << 1); //shift bit 1 to left because we are using PK1,PK2,PK3
	
}

void SysTick_Handler(void)
{

	if((GPIO_PORTC_DATA_R & 0xF0) != 0xF0) // Because of pull up ( 1 when not pressed, 0 when pressed) we can utilize this as a trigger so the wave on is on
	{
	Index = (Index + 1 )& 0x0F;
	DAC_Funct(SineWave[Index]);
	}
	else
	{
		DAC_Funct(0);
	}
}

void GPIOC_Handler (void)
{
	if(GPIO_PORTC_RIS_R & 0x10) //If PC4 is true / active / pressed
	{
		
		NVIC_STRELOAD_R = 3822- 1; // Putting 261.63 Hz to play or DO (16 * 261.63  = 4186.08 Hz. 16 Mhz / 4186.08 Hz = 3822.19
	}
	
	else if(GPIO_PORTC_RIS_R & 0x20) //If PC5 is true / active / pressed
	{
		
		NVIC_STRELOAD_R =  3405 - 1; // Putting 293.66 Hz to play or RE ( 16 * 293.66 = 4698.56 Hz . 16 Mhz / 4698.56 Hz  =  3,405.29 
	}
	
	else if(GPIO_PORTC_RIS_R & 0x80) //If PC6 is true / active / pressed
	{
		
		NVIC_STRELOAD_R = 3034 - 1; // Putting 329.63 Hz to play or MI (16 * 329.64  = 5274.08 Hz. 16 Mhz / 5274.08 Hz = 3,033.70
	}
	
	GPIO_PORTC_ICR_R = 0x70; // clear interrupt
}

int main (void)
{
	GPIO_K_Init();
	GPIO_C_Init();
	Systick_Init();
	EnableInterrupts();
	while (1)
	{
		WaitForInterrupts();
	}
}