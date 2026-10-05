#ifndef __BSP_LED_DRIVER_H__
#define __BSP_LED_DRIVER_H__



#include <stdio.h>
#include <stdint.h>



#define INITED           1       /* LED is inited                            */
#define NOT_INITED       0       /* LED is not inited                        */

#define OS_SUPPORTING            /* OS_SUPPORTING depending on OS avaliable  */
#define DEBUG                    /* Enable DEBUG                             */
#define DEBUG_OUT(X)   printf(X) /* DEBUG output infoto indicate statues     */

typedef struct bsp_led_driver bsp_led_driver_t;


//操作灯的状态码（操作灯的动作是否成功）
typedef enum
{
	LED_OK             = 0,      /* LED Operation completed successfully.    */
	LED_ERROR          = 1,      /* LED Run-time error without case matched  */
	LED_ERRORTIMEOUT   = 2,      /* LED Operation failed with timeout        */
	LED_ERRORRESOURCE  = 3,      /* LED Resource not available.              */
	LED_ERRORPARAMETER = 4,      /* LED Parameter error.                     */
	LED_ERRORNOMEMORY  = 5,      /* LED Out of memory.                       */
	LED_ERRORISR       = 6,      /* LED Not allowed in ISR context           */
	LED_RESERVED       = 0xFF,   /* LED Reserved                             */
} led_status_t;

//点灯周期
typedef enum
{
    PROPORTIONN_1_3    = 0,      /* LED Operation completed successfully.    */
    PROPORTIONN_1_2    = 1,      /* LED Operation completed successfully.    */
    PROPORTIONN_1_1    = 2,      /* LED Operation completed successfully.    */
    PROPORTIONN_x_x    = 0xFF,   /* LED Operation completed successfully.    */
} proportion_t;


//灯开关的接口
typedef struct
{
    led_status_t (*pf_led_on)  (void);/* LED Operation completed successfu   */
    led_status_t (*pf_led_off) (void);/* LED Operation completed successfu   */
} led_operations_t;


//获取当前时间戳的接口（结构体里存函数指针）
typedef struct
{
    led_status_t (*pf_get_time_ms)  ( uint32_t * const );/* LED Op           */
} time_base_ms_t;

//OS层的延时接口
#ifdef OS_SUPPORTING
typedef struct
{
    led_status_t (*pf_os_delay_ms)  ( const uint32_t );/* LED Op           */
} os_delay_t;
#endif //OS_SUPPORTING


//实现灯控制的函数指针（驱动内部实现，不依赖外部，直接使用裸露的函数指针即可）
//参数解释：/*Cycle_time[ms]*/		/* blink_times[times]*/		/*proportion_on_off*/
typedef led_status_t (*pf_led_control_t)(bsp_led_driver_t * const self,uint32_t ,uint32_t ,proportion_t);


//一个灯对象创建时需要的参数（即一个对象所拥有的性质）
typedef struct bsp_led_driver
{
	
	
    //是否已经初始化过
    uint8_t is_inited;

    //一些变量（有关灯这个对象）
    
	//闪烁周期
    uint32_t cycle_time_ms;
	
	
    //闪烁次数
    uint32_t blink_times;
	
    //亮灭比
    proportion_t proportion_on_off;

	
    //一些指向结构体的指针（依赖外部接口，利用结构体指针实现解耦）
    
	//开关灯的接口
    led_operations_t *p_led_opes_inst;
	
	//时基接口
    time_base_ms_t *p_time_base_ms;
	
    //RTOS延时接口
#ifdef OS_SUPPORTING
    os_delay_t *p_os_time_delay;
#endif //OS_SUPPORTING
	
	
    
	//灯控制的函数指针（不依赖外部接口，驱动内部实现的函数，面向外部层）
    pf_led_control_t pf_led_countroler;
	
}bsp_led_driver_t;



//函数对外声明

//构造一个对象（led_driver的构造函数）
led_status_t led_driver_inst (
                                      bsp_led_driver_t * const self, 
                                      led_operations_t * const led_ops,
#ifdef OS_SUPPORTING
                                      os_delay_t * const os_delay,
#endif
                                      time_base_ms_t * const time_base );
                



#endif // End of __BSP_LED_DRIVER_H__

