/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "queue.h"
//#include "bsp_led_driver.h"
//#include "bsp_led_handler.h"

#include "system_adaption.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

////开LED
//led_status_t led_on_myown  (void)
//{
//    printf("led is on\r\n");
//    return LED_OK;
//}

////关LED
//led_status_t led_off_myown  (void)
//{
//    printf("led is off\r\n");
//    return LED_OK;
//}

////将接口与真正实现函数连接起来（给结构体成员赋值）
//led_operations_t led_operations_myown = {
//    .pf_led_on  = led_on_myown,
//    .pf_led_off = led_off_myown
//};

////获取当前时间戳
//led_status_t pf_get_time_ms_mywon( uint32_t * const time_stamp)
//{
//     printf("get time now timezero\r\n");
//     *time_stamp = 0;
//     return LED_OK;
//}

////将接口与真正实现函数连接起来（给结构体成员赋值）
//time_base_ms_t time_base_ms_myown = {
//    .pf_get_time_ms = pf_get_time_ms_mywon
//};


////driver层中RTOS延时
//led_status_t pf_os_delay_ms_myown  ( const uint32_t delay_time)
//{
//	
////    printf("pf_os_delay_ms now delay 1ms\r\n");
////    //test
////    for(int i = delay_time; i > 0 ; i --)
////    {
////    
////    }
////    printf("finished\r\n");
////    return LED_OK;
//	
//	vTaskDelay(delay_time);
//    printf("delay [%d]ms finished\r\n", delay_time);
//    return LED_OK;
//	
//}

////将接口与真正实现函数连接起来（给结构体成员赋值）
//os_delay_t os_delay_myown = {
//    .pf_os_delay_ms = pf_os_delay_ms_myown
//};




////driver层测试函数
//void Test_1()
//{
//  led_status_t ret = LED_OK;
//  bsp_led_driver_t led1;
//  bsp_led_driver_t led2;
//  ret = led_driver_inst(&led1,
//                        &led_operations_myown,
//                        &os_delay_myown,
//                        &time_base_ms_myown); 
//  ret = led_driver_inst(&led2,
//                        &led_operations_myown,
//                        &os_delay_myown,
//                        &time_base_ms_myown); 
//  ret = led1.pf_led_countroler(&led1,5, 30, PROPORTIONN_1_1);
//  ret = led1.pf_led_countroler(&led2,2, 10, PROPORTIONN_1_1);
//    
//}



////handler层中RTOS延时
//led_handler_status_t os_delay_ms_hanler_1  ( const uint32_t delay_time)
//{
//    vTaskDelay(delay_time);
//#ifdef HANLDER_1_DEBUG
//      printf("os_delay_ms_hanler_1 \r\n");
//#endif
//    return LED_OK;
//}


////将接口与真正实现函数连接起来（给结构体成员赋值）
//os_delay_t handler_1_os_delay = {
//   .pf_os_delay_ms = os_delay_ms_hanler_1
//};



////handler层中RTOS队列创建
//led_handler_status_t os_queue_create_handler_1 (uint32_t const item_num,
//                                                uint32_t const item_size,
//                                                void ** const queue_handler)
//{
//#ifdef HANLDER_1_DEBUG
//      printf("os_queue_create_handler_1 \r\n");
//#endif
//	
//  
//  QueueHandle_t temp_queue_handle = NULL;
//  temp_queue_handle = xQueueCreate(item_num, item_size);
//  if( NULL == temp_queue_handle)
//  {
//    return HANDLER_ERRORRESOURCE;
//  }
//  else
//  {
//    *queue_handler = temp_queue_handle;
//    return HANDLER_OK;
//  }
//}


////handler层中RTOS队列发送
//led_handler_status_t os_queue_put_handler_1 (void * const queue_handler,
//                                             void * const item,
//                                             uint32_t timeout)
//{
//	
//#ifdef HANLDER_1_DEBUG
//      printf("os_queue_put_handler_1 \r\n");
//#endif
//	
//  led_handler_status_t ret = HANDLER_OK;
//  if( NULL == queue_handler || NULL == item || timeout > portMAX_DELAY )
//  {
//    return HANDLER_ERRORRESOURCE;
//  }
//  else
//  {
//    ret = xQueueSend(queue_handler, item , timeout);
//    if (ret == pdFALSE)
//    {
//      ret = HANDLER_ERROR;
//    }
//    return HANDLER_OK;
//  }
//}



////handler层中RTOS队列接收
//led_handler_status_t os_queue_get_handler_1 (void * const queue_handler,
//                                             void * const msg,
//                                             uint32_t timeout)
//{
//	
//#ifdef HANLDER_1_DEBUG
//      printf("os_queue_get_handler_1 \r\n");
//#endif
//	
//  led_handler_status_t ret = HANDLER_OK;
//  if( NULL == queue_handler || NULL == msg || timeout > portMAX_DELAY )
//  {
//    return HANDLER_ERRORRESOURCE;
//  }
//  else
//  {
//    ret = xQueueReceive(queue_handler, msg , timeout);
//    if (ret == pdFALSE)
//    {
//      ret = HANDLER_OK;
//    }
//    return HANDLER_ERROR;
//  }
//  
//}


////handler层中RTOS队列删除
//led_handler_status_t os_queue_delete_handler1 ( void * const queue_handler)
//{
//	
//#ifdef HANLDER_1_DEBUG
//      printf("os_queue_delete_handler1 \r\n");
//#endif
//	
//    led_handler_status_t ret = HANDLER_OK;
//    if( NULL == queue_handler )
//    {
//      return HANDLER_ERRORRESOURCE;
//    }
//	
//    vQueueDelete(queue_handler);
//	
//    return HANDLER_OK;
//}



////将接口与真正实现函数连接起来（给结构体成员赋值）
//handler_os_queue_t handler1_os_queue = {
//  .pf_os_queue_create = os_queue_create_handler_1,
//  .pf_os_queue_put = os_queue_put_handler_1,
//  .pf_os_queue_get = os_queue_get_handler_1,
//  .pf_os_queue_delete = os_queue_delete_handler1};

//  
//  
////handler层中RTOS进入临界区
//led_handler_status_t os_critical_enter_handler_1 (void )
//{
//#ifdef HANLDER_1_DEBUG
//      printf("os_critical_enter_handler_1 \r\n");
//#endif
//	
//  vPortEnterCritical(); 
//  return    HANDLER_OK;
//}


////handler层中RTOS退出临界区
//led_handler_status_t os_critical_exit_handler_1 (void )
//{
//#ifdef HANLDER_1_DEBUG
//      printf("os_critical_exit_handler_1 \r\n");
//#endif
//  
//  vPortExitCritical(); 
//  return HANDLER_ERROR;
//}


////将接口与真正实现函数连接起来（给结构体成员赋值）
//handler_os_critical_t handler1_os_critical = {
//    .pf_os_critical_enter = os_critical_enter_handler_1,
//    .pf_os_critical_exit  = os_critical_exit_handler_1};



////获取当前时间戳
//led_handler_status_t get_time_ms_handler1 ( uint32_t * const p_os_tick )
//{
//    if( NULL == p_os_tick )
//    {
//      return HANDLER_ERRORRESOURCE;
//    }
//    *p_os_tick = HAL_GetTick();
//}


////将接口与真正实现函数连接起来（给结构体成员赋值）
//handler_time_base_ms_t handler1_time_base = {
//    .pf_get_time_ms = get_time_ms_handler1
//};



////handler层中RTOS创建任务
//led_handler_status_t thread_create_handler1 (
//				void * const task_code,
//                const char * const task_name,
//                const uint32_t stack_depth,
//                void * const parameters,
//                uint32_t priority,
//                void ** const task_handler)
//{
//    BaseType_t ret = pdPASS;
//#ifdef HANLDER_1_DEBUG
//	
//	printf("parameters = [%p]\r\n",parameters);
//	printf("thread_create_handler1 \r\n");
//	
//#endif
//	
//    ret =  xTaskCreate( task_code, "led_handler_1", 128 * 4, parameters,
//                  (osPriority_t) osPriorityNormal, (TaskHandle_t * const)task_handler );
//	
//#ifdef HANLDER_1_DEBUG
//      printf("thread_create_handler1 ret = [%d]\r\n", ret);
//#endif
//	
//    if ( pdPASS != ret )
//    {
//        return HANDLER_ERRORRESOURCE;
//    }
//    else
//    {
//        return HANDLER_OK;
//    }

//}

////handler层中RTOS删除任务
//led_handler_status_t thread_delete_handler1 ( void * const queue_handler)
//{
//    led_handler_status_t ret = HANDLER_OK;

//    if ( NULL == queue_handler )
//    {
//       return HANDLER_ERRORRESOURCE;
//    }

//    vTaskDelete(queue_handler);

//    return ret;
//}

////将接口与真正实现函数连接起来（给结构体成员赋值）
//handler_os_thread_t handler1_os_thread = {
//  .pf_os_thread_create = thread_create_handler1,
//  .pf_os_thread_delete = thread_delete_handler1
//};





////handler层测试函数
//void Test_2()
//{
//	
//	//构造 led_handler 对象
//    led_handler_status_t ret = HANDLER_OK;
//    bsp_led_handler_t handler_1;		
//    ret = led_handler_inst ( &handler_1, &handler_1_os_delay, &handler1_os_queue,
//							 &handler1_os_critical, &handler1_os_thread, &handler1_time_base);
//    

//	//构造LED对象
//    led_status_t ret1 = LED_OK;
//    bsp_led_driver_t led1;
//    bsp_led_driver_t led2;
//    ret1 = led_driver_inst(&led1,&led_operations_myown,&os_delay_myown,&time_base_ms_myown); 
//    ret1 = led_driver_inst(&led2,&led_operations_myown,&os_delay_myown,&time_base_ms_myown); 
//    
//	
//	ret1 = led1.pf_led_countroler(&led1,5, 30, PROPORTIONN_1_1);
//    //ret1 = led2.pf_led_countroler(&led2,2, 10, PROPORTIONN_1_1);


//	
//    led_index_t handler_1_led_index_1 = LED_NOT_INITIALIZED;
//	
//	//注册LED1
//    ret = handler_1.pf_led_register( &handler_1,&led1,&handler_1_led_index_1);
//    
//	
//    printf("The return of handler_1.pf_led_register is [%d]\r\n", ret);  
//	
//    printf("The registered &led1 index is LED_[%d]\r\n", (handler_1_led_index_1+1));
//    
//	
//	//注册LED2
//    ret = handler_1.pf_led_register( &handler_1,&led2,&handler_1_led_index_1);
//            
//	
//    printf("The return of handler_1.pf_led_register is [%d]\r\n", ret);    
//	
//    printf("The registered &led2 index is LED_[%d]\r\n", (handler_1_led_index_1+1));
//	
//	
//	
//	//表面是控制LED闪烁（实际上是向控制任务发送请求）
//	handler_1.pf_led_countroler(&handler_1, 100U, 1U, PROPORTIONN_1_2, handler_1_led_index_1);
//    
//}


////handler层测试函数（加上OS层）
//void Test_3()
//{
//	//****************************** 有关handler层 ************************//
//	
//	//完成 led_handler 对象的构造
//    printf("System Starting.....\r\n");
//    led_handler_status_t ret = HANDLER_OK;   
//    bsp_led_handler_t handler_1;         
//    ret = led_handler_inst ( &handler_1,&handler_1_os_delay,&handler1_os_queue,
//                            &handler1_os_critical,&handler1_os_thread,&handler1_time_base);
//	
//	
//	//完成LED对象的构造
//    led_status_t ret1 = LED_OK;
//    bsp_led_driver_t led1;
//    ret1 = led_driver_inst( &led1,&led_operations_myown,&os_delay_myown,&time_base_ms_myown); 
//    
//	
//	if (HANDLER_OK == ret )
//    {
//        printf("handler_1 has been instantiated \r\n");
//    }
//    
//	//****************************** 有关handler层 ************************//
//	

//    //****************************** 有关APP层 ************************//
//	
//	//完成 led_handler1 对象的注册
//    led_index_t handler_1_led_index_1 = LED_NOT_INITIALIZED;
//    ret = handler_1.pf_led_register( &handler_1, &led1, &handler_1_led_index_1);
//    
//    if (HANDLER_OK == ret )
//    {
//        printf("leds have been registered \r\n");
//    }

//    //调用LED控制函数（表面是控制LED闪烁，实际上是向控制任务发送请求）
//    ret = handler_1.pf_led_countroler(&handler_1, 100U, 1U, PROPORTIONN_1_2, handler_1_led_index_1);
//    
//	if (HANDLER_OK == ret )
//    {
//        printf("led1 has been controlled\r\n");
//    }

//	//****************************** 有关APP层 ************************//
//    
//    while(1);
//}






/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  

	

  printf("hello world\r\n");
  system_init_resources();
  Test_3();
	
	
  printf("hello world2\r\n");
  for(;;)
  {
//	printf("hello world\r\n");
	osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

