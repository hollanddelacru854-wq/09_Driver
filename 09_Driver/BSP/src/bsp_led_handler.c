#include "bsp_led_handler.h"



//初始化存放对象的数组
static led_handler_status_t __array_init(bsp_led_driver_t * array[], uint32_t array_size)
{
	
    for(int i = 0; i < array_size; i++)
    {
        array[i] = (bsp_led_driver_t *)INIT_PATTERN;
    }
    
	
    return HANDLER_OK;
	
}


//LED对象注册函数（注册到handler层进行统一管理）
led_handler_status_t led_register ( bsp_led_handler_t * const self,
                                    bsp_led_driver_t * const led_driver, 
                                    led_index_t * const index)
{

#ifdef DEBUG
        DEBUG_OUT("Start_led_register \r\n");
#endif
    led_handler_status_t ret = HANDLER_OK;
    
	//校验参数
    if( NULL == led_driver || NULL == index || NOT_INITED == self->is_inited ) 
    {
        
#ifdef DEBUG
        DEBUG_OUT("LED_ERRORPARAMETER \r\n");
#endif
        ret = HANDLER_ERRORPARAMETER;
        return ret;
		
    }

    //校验要注册的driver已经初始化
    if ( INITED != led_driver->is_inited )
    {
        
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE \r\n");
#endif
        ret = HANDLER_ERRORRESOURCE;
        return ret;
    }


    //检查数组有无空位（防止越界）
    if( ( MAX_INSTANCE_NUMBER - self->instances.led_instance_num ) == 0 )
    {
        ret = HANDLER_ERRORRESOURCE;
        return ret;
    }
    



#ifdef OS_SUPPORTING
	//进入临界区
    self->p_os_critical->pf_os_critical_enter();
#endif

    if( (MAX_INSTANCE_NUMBER - self->instances.led_instance_num) > 0  )
    {
		//写入数组
        self->instances.led_instance_group[self->instances.led_instance_num] = led_driver;
		
		//更新编号数值（加一）
        *index = self->instances.led_instance_num;
        self->instances.led_instance_num++;
		
    } 

#ifdef OS_SUPPORTING
	//退出数组
    self->p_os_critical->pf_os_critical_exit();
#endif

    

#ifdef DEBUG
        DEBUG_OUT("led_register Succees!\r\n");
#endif
    return ret;
}





//led_handler的构造函数（主要作用就是接入依赖于外层的接口，交出该层向外提供的接口）
led_handler_status_t led_handler_inst (
                                  bsp_led_handler_t * const self, 
#ifdef OS_SUPPORTING 
                                  os_delay_t * const os_delay,
                                  handler_os_queue_t * const os_queue,
                                  handler_os_critical_t * const os_critical,
#endif
                                  handler_time_base_ms_t * const time_base )
{
    led_handler_status_t ret = HANDLER_OK;
#ifdef DEBUG
    DEBUG_OUT("Handler inst Kick-off \r\n");
#endif

	//校验参数
    if( NULL == self ||
#ifdef OS_SUPPORTING
        NULL == os_delay || NULL == os_queue ||
#endif
        NULL == time_base)
    {
		
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORPARAMETER\r\n");
#endif
        ret = HANDLER_ERRORPARAMETER;
        return ret;
    }


	//判断是否已经初始化过（防止再次初始化）
    if( INITED == self->is_inited )
    {
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE\r\n");
#endif
        ret = HANDLER_ERRORPARAMETER;
        return ret;
    }

#ifdef DEBUG
    DEBUG_OUT("Handler inst start\r\n");
#endif




	//函数指针指向具体实现函数（包括HAL层、OS层、驱动层、APP层）
	//接入依赖于外层的接口
    self->p_time_base_ms = time_base;
    self->p_os_time_delay = os_delay;
    self->p_os_queue_interface = os_queue;
    self->p_os_critical = os_critical;
 
    //连接到handler层中实现的LED对象注册函数
	//交出该层向外提供的接口
    self->pf_led_register = led_register;
    

	//给一些变量赋值
    self->instances.led_instance_num  = 0;
	
	//执行初始化操作（数组初始化）
    ret = __array_init(self->instances.led_instance_group, MAX_INSTANCE_NUMBER);
	
	
if( HANDLER_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORNOMEMORY\r\n");
#endif
        return ret;
    }

    //初始化成功后说明已经初始化
    self->is_inited = INITED;

	
#ifdef DEBUG
    DEBUG_OUT("LED handler init finished\r\n");
#endif
    
    return ret;
}                     

