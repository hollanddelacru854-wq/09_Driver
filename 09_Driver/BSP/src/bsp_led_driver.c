
//驱动层driver中的功能实现（对外接口的具体实现）

#include "bsp_led_driver.h"



//灯闪烁函数（控制灯的具体动作函数）
//从控制的抽象到闪烁的具体
led_status_t led_blink( bsp_led_driver_t * self )
{
    led_status_t ret = LED_OK;
	
    //校验对象的存在
    if( NULL == self || NOT_INITED == self->is_inited) 
    {
#ifdef DEBUG
    DEBUG_OUT("LED_ERRORPARAMETER in led blink\r\n");
#endif
        ret = LED_ERRORPARAMETER;
        return ret;
    }
    
	//对象存在执行的逻辑
    {
        //创建局部变量承接传入的参数
        uint32_t cycle_time_local;
        uint32_t blink_times_local;
        proportion_t proportion_local;
        uint32_t led_toggle_time;

		//接收所需参数
        cycle_time_local = self->cycle_time_ms;		//闪烁周期
        blink_times_local = self->blink_times;		//闪烁次数
        proportion_local = self->proportion_on_off;		//亮灭比

		
        //判断亮灭比
        if( PROPORTIONN_1_1 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 2;
        }
        else if(PROPORTIONN_1_2 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 3;
        }
        else if(PROPORTIONN_1_3 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 4;
        }
        else 
        {
#ifdef DEBUG
    DEBUG_OUT("LED_ERRORPARAMETER in led blink\r\n");
#endif
            ret = LED_ERRORPARAMETER;
            return ret;
        }

        //结合闪烁次数、闪烁周期、亮灭比的点灯控制
        for(uint32_t i = 0; i < blink_times_local; i++)
        {

            for(uint32_t j = 0; j < cycle_time_local; j++)
            {
				
                self->p_os_time_delay->pf_os_delay_ms(1);
                if( j < led_toggle_time )
                {
                    self->p_led_opes_inst->pf_led_on();
                }
                else
                {
                    self->p_led_opes_inst->pf_led_off();
                }
            }
        }

    }

    return ret;
}




//驱动中控制灯的函数（驱动内部实现，外部通过指针间接调用）
//参数：对象自己、一个闪烁周期、闪烁次数、亮灭比（占空比）
static led_status_t led_control   (bsp_led_driver_t * const self, uint32_t cycle_time,
									uint32_t blink_times, proportion_t proportion)
{
    
    led_status_t ret = LED_OK;
	
    //确保对象存在并且已被初始化过
    if( NULL == self || NOT_INITED == self->is_inited) 
	{
        ret = LED_ERRORPARAMETER;
        return ret;
    }

	//保证参数在合理范围内
    if( !( (cycle_time  < 10000 )&&(blink_times < 1000  )&&( (PROPORTIONN_1_3 <= proportion)
		&& (PROPORTIONN_1_1 >= proportion) ) ) )
    {
        ret = LED_ERRORPARAMETER;
        return ret;
    }

   //给一些变量赋值（传入的“需求”，即完成灯控制所需要的参数）
    self->cycle_time_ms        =  cycle_time;
    self->blink_times          = blink_times;
    self->proportion_on_off    =  proportion;

    //执行具体的灯闪烁逻辑
    ret = led_blink(self);

    return ret;
}





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





//LED对象的构造函数
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
                        
