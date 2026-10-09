
#ifndef __SYSTEM_ADAPTION_H__
#define __SYSTEM_ADAPTION_H__




#define OS_SUPPORTING
#define HANLDER_1_DEBUG



#include <stdio.h>
#include <stdint.h>



#include "cmsis_os.h"



#include "main.h"
#include "usart.h"
#include "gpio.h"



#include "queue.h"
#include "task.h"
#include "main.h"



#include "bsp_led_driver.h"
#include "bsp_led_handler.h"



#define INIT_PATTERN_SYSTEM (uint8_t)0xEC


//操作adaption层的状态码（操作adaption层的动作是否成功）
typedef enum
{
	SYSTEM_OK             = 0,      /* LED Operation completed successfully  */
	SYSTEM_ERROR          = 1,      /* LED Run-time error without case matc  */
	SYSTEM_ERRORTIMEOUT   = 2,      /* LED Operation failed with timeout     */
	SYSTEM_ERRORRESOURCE  = 3,      /* LED Resource not available.           */
	SYSTEM_ERRORPARAMETER = 4,      /* LED Parameter error.                  */
	SYSTEM_ERRORNOMEMORY  = 5,      /* LED Out of memory.                    */
	SYSTEM_ERRORISR       = 6,      /* LED Not allowed in ISR context        */
	SYSTEM_RESERVED       = 0xFF,   /* LED Reserved                          */
} system_status_t;






extern bsp_led_handler_t handler_1;
extern bsp_led_driver_t led1;


//LED开关具体实现函数声明
led_status_t led_on_myown (void);
led_status_t led_off_myown (void);
extern led_operations_t led_operations_myown;


//获取当前时间戳的具体实现函数声明（driver层）
led_status_t pf_get_time_ms_mywon( uint32_t * const time_stamp);
extern time_base_ms_t time_base_ms_myown;


//driver层中RTOS延时的具体实现函数声明
led_status_t pf_os_delay_ms_myown  ( const uint32_t delay_time);
extern os_delay_t os_delay_myown;


//driver层测试函数
void Test_1();


//handler层中RTOS延时的具体实现函数声明
led_handler_status_t os_delay_ms_hanler_1  ( const uint32_t delay_time);
extern os_delay_t handler_1_os_delay;


//handler层中RTOS队列创建的具体实现函数声明
led_handler_status_t os_queue_create_handler_1 (uint32_t const item_num,
                                                uint32_t const item_size,
                                                void ** const queue_handler);
												
//handler层中RTOS队列发送的具体实现函数声明
led_handler_status_t os_queue_put_handler_1    (void * const queue_handler,
                                                void * const item,
                                                uint32_t timeout);
												
//handler层中RTOS队列接收的具体实现函数声明
led_handler_status_t os_queue_get_handler_1    (void * const queue_handler,
                                                void * const msg,
                                                uint32_t timeout);
												
//handler层中RTOS队列删除的具体实现函数声明
led_handler_status_t os_queue_delete_handler1  ( void * const queue_handler);
extern handler_os_queue_t handler1_os_queue;


//handler层中RTOS进入临界区的具体实现函数声明
led_handler_status_t os_critical_enter_handler_1(void );
												
//handler层中RTOS退出临界区的具体实现函数声明
led_handler_status_t os_critical_exit_handler_1 (void );
extern handler_os_critical_t handler1_os_critical;

												  
//获取当前时间戳的具体实现函数声明（driver层）
led_handler_status_t get_time_ms_handler1  ( uint32_t * const p_os_tick );
extern handler_time_base_ms_t handler1_time_base;


//handler层中RTOS创建任务的具体实现函数声明
led_handler_status_t thread_create_handler1 ( void * const task_code,
											  const char * const task_name,
											  const uint32_t stack_depth,
											  void * const parameters,
											  uint32_t priority,
											  void ** const task_handler);

											  
//handler层中RTOS删除任务的具体实现函数声明
led_handler_status_t thread_delete_handler1 ( void * const queue_handler);
extern handler_os_thread_t handler1_os_thread ;            


											  
											  
											  
//handler层测试函数（加上OS层）
void Test_3();
                

				
				
				
				
				
				

//实现资源分配的函数（即对象的初始化和构造）
led_status_t system_init_resources ( void );
                

				

#endif

