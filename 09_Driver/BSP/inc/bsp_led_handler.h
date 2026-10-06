#ifndef __BSP_LED_HANDLER_H__
#define __BSP_LED_HANDLER_H__


#include "bsp_led_driver.h"
#include <stdio.h>
#include <stdint.h>



#define OS_SUPPORTING            //有RTOS，有OS层
#define DEBUG                    //调试模式
#define DEBUG_OUT(X)   printf(X) //调试打印
#define INIT_PATTERN  (0xA6A6A6A6) //初始化数组

typedef struct bsp_led_driver bsp_led_driver_t;


//handler层是否已经被初始化
typedef enum
{
	HANDLER_NOT_INITED     = 1,      /* LED Run-time error without cas       */ 
    HANDLER_INITED         = 0,      /* LED Operation completed succes       */
} led_handler_init_t;


//灯的索引（编号）
typedef enum
{
	LED_1 = 0,      
    LED_2,                           
    LED_3,                           
    LED_4,                           
    LED_5,                           
    LED_6,                           
    LED_7,                           
    LED_8,                           
    LED_9,                           
    LED_10,                          
    MAX_INSTANCE_NUMBER,			//数组最大容量
    LED_NOT_INITIALIZED = 0xFFFFFFFF	//无效索引
} led_index_t;


//操作handler层的状态码（操作handler层的动作是否成功）
typedef enum
{
	HANDLER_OK             = 0,      /* LED Operation completed succes       */
	HANDLER_ERROR          = 1,      /* LED Run-time error without cas       */
	HANDLER_ERRORTIMEOUT   = 2,      /* LED Operation failed with time       */
	HANDLER_ERRORRESOURCE  = 3,      /* LED Resource not available.          */
	HANDLER_ERRORPARAMETER = 4,      /* LED Parameter error.                 */
	HANDLER_ERRORNOMEMORY  = 5,      /* LED Out of memory.                   */
	HANDLER_ERRORISR       = 6,      /* LED Not allowed in ISR context       */
	HANDLER_RESERVED       = 0xFF,   /* LED Reserved                         */
} led_handler_status_t;


//获取当前时间戳的接口
typedef struct
{
    led_handler_status_t (*pf_get_time_ms)  ( uint32_t * const );
} handler_time_base_ms_t;


//有OS层的情况
#ifdef OS_SUPPORTING

//RTOS的延时接口
typedef struct
{
    led_handler_status_t (*pf_os_delay_ms)  ( const uint32_t );
} handler_os_delay_t;


//RTOS中队列的接口
typedef struct
{
    //创建队列
    led_handler_status_t (*pf_os_queue_create)  (uint32_t const item_num,
                                                 uint32_t const item_size,
                                                 void ** const queue_handler);
    
    //队列发送
    led_handler_status_t (*pf_os_queue_put)  (void * const queue_handler,
                                              void * const item,
                                              uint32_t timeout );
    
    //队列接收                                        
    led_handler_status_t (*pf_os_queue_get)  (void * const queue_handler,
											  void * const msg,
											  uint32_t timeout );
    
    //队列删除                                      
    led_handler_status_t (*pf_os_queue_delete )  (void * const queue_handler);

} handler_os_queue_t;


//RTOS中临界区的接口
typedef struct
{
	//进入临界区
    led_handler_status_t (*pf_os_critical_enter) (void );
	
	//退出临界区
    led_handler_status_t (*pf_os_critical_exit ) (void );

} handler_os_critical_t;

#endif



typedef struct bsp_led_handler bsp_led_handler_t;


//实现某个灯对象注册到handler的函数指针（注册进入handler进行统一管理）
typedef led_handler_status_t (*pf_handler_led_register_t)(bsp_led_handler_t * const self, bsp_led_driver_t * const led_driver, led_index_t * const index);


//注册手册（记录注册的driver数）
typedef struct
{
    uint32_t led_instance_num;			//当前的注册数（即driver数）
    bsp_led_driver_t * led_instance_group[MAX_INSTANCE_NUMBER];		//灯指针数组

} instance_registered_t;


//一个handler对象所拥有的参数（性质）
typedef struct bsp_led_handler
{
    //是否已初始化
    uint8_t is_inited;
    
    //注册手册
    instance_registered_t instances;   
    
	//时基接口
    handler_time_base_ms_t *p_time_base_ms;
    
	//RTOS接口
#ifdef OS_SUPPORTING
	//延时
    os_delay_t *p_os_time_delay;
	
	//队列
    handler_os_queue_t *p_os_queue_interface;
	
	//临界区
    handler_os_critical_t *p_os_critical;
#endif

    //灯控制接口
    pf_led_control_t pf_led_countroler;
    
	//灯注册接口
    pf_handler_led_register_t pf_led_register;

}bsp_led_handler_t;


//函数对外声明

//构造一个对象（led_handler的构造函数）
led_handler_status_t led_handler_inst (
                                  bsp_led_handler_t * const self, 
#ifdef OS_SUPPORTING 
                                  os_delay_t * const os_delay,
                                  handler_os_queue_t * const os_queue,
                                  handler_os_critical_t * const os_critical,
#endif
                                  handler_time_base_ms_t * const  time_base  );


#endif

