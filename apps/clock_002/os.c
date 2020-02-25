/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
//#include "main_cubemx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "sketch.h"
#include "bsp.h"
#include "osTasks.h"

#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
//osThreadId defaultTaskHandle;
//osThreadId configShellTaskHandle;
//osThreadId midiTaskHandle;
//osThreadId hardwareTaskHandle;
//osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void taskHardwareStart(void const * argument);
   
/* USER CODE END FunctionPrototypes */
extern void xPortSysTickHandler(void);

void StartDefaultTask(void const * argument);
void configShellTaskStart(void * argument);
void midiTaskStart(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  //TODO: Should we check for scheduler running first?
  bspRegisterSysTickCallback(xPortSysTickHandler);
//
//  /* USER CODE END Init */
//
//  /* USER CODE BEGIN RTOS_MUTEX */
//  /* add mutexes, ... */
//  /* USER CODE END RTOS_MUTEX */
//
//  /* Create the semaphores(s) */
//  /* definition and creation of myBinarySem01 */
//  osSemaphoreDef(myBinarySem01);
//  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);
//
//  /* USER CODE BEGIN RTOS_SEMAPHORES */
//  /* add semaphores, ... */
//  /* USER CODE END RTOS_SEMAPHORES */
//
//  /* USER CODE BEGIN RTOS_TIMERS */
//  /* start timers, add new ones, ... */
//  /* USER CODE END RTOS_TIMERS */
//
//  /* Create the thread(s) */
//  /* definition and creation of defaultTask */
//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
//
//  /* definition and creation of configShellTask */
//  osThreadDef(configShellTask, configShellTaskStart, osPriorityNormal, 0, 2048);
//  configShellTaskHandle = osThreadCreate(osThread(configShellTask), NULL);

// Example:
//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );
  xTaskCreate( configShellTaskStart, "config", 2048, (void*) 1, tskIDLE_PRIORITY, NULL);
//
//  /* definition and creation of midiTask */
//  osThreadDef(midiTask, midiTaskStart, osPriorityIdle, 0, 128);
//  midiTaskHandle = osThreadCreate(osThread(midiTask), NULL);
//
//  /* USER CODE BEGIN RTOS_THREADS */
//  /* definition and creation of hardwareTask */
//  osThreadDef(hardwareTask, taskHardwareStart, osPriorityNormal, 0, 128);
//  hardwareTaskHandle = osThreadCreate(osThread(hardwareTask), NULL);
//
//  
//    if (defaultTaskHandle == NULL)
//	{
//		while(1);
//	}
//    if (configShellTaskHandle == NULL)
//	{
//		while(1);
//	}
//    if (midiTaskHandle == NULL)
//	{
//		while(1);
//	}
//	if (hardwareTaskHandle == NULL)
//	{
//		while(1);
//	}
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	for(;;)
	{
		taskPanel();
		//osDelay(1);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_configShellTaskStart */
/**
* @brief Function implementing the configShellTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_configShellTaskStart */
void configShellTaskStart(void * argument)
{
  /* USER CODE BEGIN configShellTaskStart */
  /* Infinite loop */
	for(;;)
	{
		taskConsole();
		//osDelay(1);
	}
  /* USER CODE END configShellTaskStart */
}

/* USER CODE BEGIN Header_midiTaskStart */
/**
* @brief Function implementing the midiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_midiTaskStart */
void midiTaskStart(void const * argument)
{
  /* USER CODE BEGIN midiTaskStart */
  /* Infinite loop */
	for(;;)
	{
		taskMidi();
	}
  /* USER CODE END midiTaskStart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void osInit(void)
{
	// Run arduino like setup lop
	setup();
	
	MX_FREERTOS_Init();
	
	// Init other os objects
	
	
	// Start os and don't come back
	vTaskStartScheduler();
	
	//Old entry point:	AppEntry();
}

void taskHardwareStart(void const * argument)
{
	while(1)
	{
		taskHardware();
	}
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
