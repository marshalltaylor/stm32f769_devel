/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "main_cubemx.h"
#include "tim.h"
#include "stm32f4xx_it.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* References ----------------------------------------------------------------*/
extern volatile uint32_t SYSTICK_VALUE; //from main_cubemx... TODO: check
extern uint32_t fastRunTimeTicks; //from tim.c

/* Functions -----------------------------------------------------------------*/
void bspInit(void)
{
	// Without this garbage sysTick_Handler won't be linked?
	volatile void * trash = SysTick_Handler;
	UNUSED(trash);

	main_cubemx();
}

uint32_t millis(void)
{
	return SYSTICK_VALUE;
};

uint32_t ulGetFastRunTimeTicks(void)
{
	return fastRunTimeTicks;
};
