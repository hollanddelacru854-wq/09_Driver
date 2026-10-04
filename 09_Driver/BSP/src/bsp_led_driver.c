
//驱动层driver中的功能实现（对外接口的具体实现）

#include "bsp_led_driver.h"



//LED对象的初始化
led_status_t led_driver_init( bsp_led_driver_t * const self)
{
    led_status_t ret = LED_OK;
    
    DEBUG_OUT("led init start \r\n");
	
	//判断对象存在
    if( NULL == self )
    {
        #ifdef DEBUG
        DEBUG_OUT("LED_ERRORPARAMETER\r\n");
        return LED_ERRORPARAMETER;
        #endif // DEBUG
    }
    
	
	//将对象中的指针和变量进行初始化
	//（变量赋初值，使用指针通过接口调用真正实现函数，即读出来使用）
    self->p_led_opes_inst->pf_led_off();
    self->p_os_time_delay->pf_os_delay_ms(600);
    uint32_t time_stamp = 0;
    self->p_time_base_ms->pf_get_time_ms(&time_stamp);

    return ret;
    
}




led_status_t led_driver_inst (bsp_led_driver_t * const self, 
                              led_operations_t * const led_ops,
#ifdef OS_SUPPORTING
                              os_delay_t * const os_delay,
#endif
                              time_base_ms_t * const time_base)
{
    led_status_t ret = LED_OK;
    DEBUG_OUT("led inst Kick-off \r\n");

	//判断对象确实存在
    if( NULL == self||NULL == led_ops||NULL == os_delay||NULL == time_base)
    {
        #ifdef DEBUG
        DEBUG_OUT("LED_ERRORPARAMETER\r\n");
        #endif
		return LED_ERRORPARAMETER;
    }

	//判断是否已经初始化过（防止再次初始化）
    if( INITED == self->is_inited )
    {
#ifdef DEBUG
        DEBUG_OUT("LED_ERRORRESOURCE\r\n");
        return LED_ERRORRESOURCE;
#endif // DEBUG
    }

#ifdef DEBUG
    DEBUG_OUT("led inst start\r\n");
#endif  // DEBUG


	//指向具体真正的结构体
    self->p_led_opes_inst =   led_ops;
    self->p_os_time_delay =  os_delay;
    self->p_time_base_ms  = time_base;
    

	//给一些变量赋值
    self->blink_times   =                   0;
    self->cycle_time_ms =                   0;
    self->proportion_on_off = PROPORTIONN_x_x;
    
	//执行初始化操作
    ret = led_driver_init(self);
    if( LED_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("LED init failed\r\n");
#endif  // DEBUG
        self->p_led_opes_inst =  NULL;
        self->p_os_time_delay =  NULL;
        self->p_time_base_ms  =  NULL;
        return ret;
    }
    
    self->is_inited = INITED ;
    DEBUG_OUT("LED init finished\r\n");
    
    return ret;

    

}
                        
