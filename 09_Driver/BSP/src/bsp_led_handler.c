#include "bsp_led_handler.h"


//控制LED的请求事件（由调用者发出，被扔进队列）
typedef struct {
	
	//控制LED所需的参数
    uint32_t Cycle_time;
    uint32_t blink_times;
    proportion_t proportion_on_off;
	
	//已注册的LED对象编号
    led_index_t index;
	
} led_event_t;




//控制LED进行闪烁的函数（复制driver层中的led_control）
led_handler_status_t led_blink_handler( bsp_led_driver_t * self )
{
    led_handler_status_t ret = HANDLER_OK;
	
    //校验对象的存在
    if( NULL == self || NOT_INITED == self->is_inited ) 
    {
        
#ifdef DEBUG
    DEBUG_OUT("HANDLER_ERRORPARAMETER in led_blink_handler\r\n");
#endif
        ret = HANDLER_ERRORPARAMETER;
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
    DEBUG_OUT("HANDLER_ERRORPARAMETER in led blink\r\n");
#endif
            ret = HANDLER_ERRORPARAMETER;
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



//接收请求并调用控制LED的函数（控制行为与请求的中间函数）
//主要就是通过请求找到控制LED所需的参数，并调用控制LED的函数
led_handler_status_t __event_process(bsp_led_handler_t * self, led_event_t msg)
{
	
    led_handler_status_t ret = HANDLER_OK;

    
	
    
    if(!( ( MAX_INSTANCE_NUMBER >  msg.index ) || ( LED_NOT_INITIALIZED == msg.index ) ) )
    {
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE __event_process index checking\r\n");
#endif
        return HANDLER_ERRORRESOURCE;
    }

    if( LED_NOT_INITIALIZED == msg.index )
    {
        
#ifdef DEBUG
        DEBUG_OUT("__event_process LED_NOT_INITIALIZED\r\n");
#endif
        return HANDLER_ERRORRESOURCE;
    }

    if( INIT_PATTERN == self -> instances.led_instance_group[msg.index] )
    {
       
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE at __event_process\r\n");
#endif
        return HANDLER_ERRORRESOURCE;
    }
    
#ifdef DEBUG
        DEBUG_OUT("Start Processing at __event_process\r\n");
#endif
    
    printf("Cycle_time = [%d]", msg.Cycle_time);
    printf("blink_times = [%d]", msg.blink_times);
    printf("proportion_on_off = [%d]", msg.proportion_on_off);
    printf("index = [%d]", msg.index);

	
	//通过请求结构体拿到控制LED所需要的参数
    (self->instances.led_instance_group[msg.index])->cycle_time_ms = msg.Cycle_time;
    (self->instances.led_instance_group[msg.index])->blink_times = msg.blink_times;
    (self->instances.led_instance_group[msg.index])->proportion_on_off = msg.proportion_on_off;
    
	
	//调用控制LED的函数
    ret = led_blink_handler(self->instances.led_instance_group[msg.index]);
    
    if( HANDLER_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("event processed failed at __event_process\r\n");
#endif
        return HANDLER_ERROR;
    }
    
#ifdef DEBUG
        DEBUG_OUT("event processed at __event_process\r\n");
#endif

}





//初始化存放对象的数组
static led_handler_status_t __array_init(bsp_led_driver_t * array[], uint32_t array_size)
{
	
    for(int i = 0; i < array_size; i++)
    {
        array[i] = (bsp_led_driver_t *)INIT_PATTERN;
    }
    
	
    return HANDLER_OK;
	
}


//统一控制LED的任务（核心）
//将阻塞点转移到这里。接收请求，并进行对应LED的控制。
led_handler_status_t handler_thread( void *argument)
{    
     osDelay(1000);
#ifdef DEBUG
        DEBUG_OUT("Start_handler_thread \r\n");
#endif

    led_handler_status_t ret = HANDLER_OK;
    bsp_led_handler_t * p_led_handler;
    led_event_t msg;
    
	
    printf("parameters in thread = [%p]\r\n",argument);
    
    osDelay(10);
    if ( NULL != argument )
    {
        p_led_handler = argument;
    }
    
	


    for(;;)
    {
        DEBUG_OUT("Start_handler_thread 2\r\n");
		
		//接收请求（控制部分待完善）
        ret = p_led_handler -> p_os_queue_interface -> pf_os_queue_get(p_led_handler -> queue_handler, &msg, 0);
        if ( HANDLER_OK  == ret )
        {
            DEBUG_OUT("the message received \r\n");
			
			__event_process(p_led_handler, msg);
        }
        osDelay(1000);
    }

}



//提供给外层的函数（提出请求，即发送队列信息给到任务）
led_handler_status_t handler_led_control(bsp_led_handler_t * const self,
                                         uint32_t Cycle_time,      
                                         uint32_t blink_times,
                                         proportion_t proportion_on_off,
                                         led_index_t const index)
{    
	
#ifdef DEBUG
        DEBUG_OUT("Start_handler_led_control \r\n");
#endif
    led_handler_status_t ret = HANDLER_OK;

	
#ifdef DEBUG
        DEBUG_OUT("Checing the target statues \r\n");
#endif
    
    //确认handler对象已经初始化过
    if( NOT_INITED == self->is_inited ) 
    {
        
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE \r\n");
#endif
		
        ret = HANDLER_ERRORRESOURCE;
        return ret;
    }

#ifdef DEBUG
        DEBUG_OUT("Checing the input parameters \r\n");
#endif
    
    
    if( !( (Cycle_time  < 10000 ) && (blink_times < 1000  ) &&
           ((PROPORTIONN_1_3 <= proportion_on_off) &&  
           (PROPORTIONN_1_1 >= proportion_on_off)) &&(index <= MAX_INSTANCE_NUMBER) ) )
    {
		
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORRESOURCE \r\n");
#endif
		
        ret = LED_ERRORPARAMETER;
        return ret;
		
    }
   

#ifdef DEBUG
        DEBUG_OUT("Sending event to LED queue \r\n");
#endif
	
	//将需要的参数放入到请求结构体
	led_event_t let_event = {
        .Cycle_time = Cycle_time,
        .blink_times = blink_times,
        .proportion_on_off = proportion_on_off};

	//向控制任务发送请求
    ret = self->p_os_queue_interface->pf_os_queue_put( self->queue_handler, &let_event, 0);
    
    if( HANDLER_OK != ret )
    {
		
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORNOMEMORY\r\n");
#endif

        return ret;
    }

#ifdef DEBUG
        DEBUG_OUT("Sending event to LED queue successfully!!\r\n");
#endif
    
    return ret;

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
//并创建统一管理LED的任务和因该任务存在的请求队列
led_handler_status_t led_handler_inst (
                                  bsp_led_handler_t * const self, 
#ifdef OS_SUPPORTING 
                                  os_delay_t * const os_delay,
                                  handler_os_queue_t * const os_queue,
                                  handler_os_critical_t * const os_critical,
								  handler_os_thread_t * const os_thread,
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
        NULL == os_delay || NULL == os_queue || NULL == os_critical  || NULL == os_thread ||
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
	self->p_os_thread = os_thread;
	
 
    //连接到handler层中实现的LED对象注册函数
	//交出该层向外提供的接口
	self->pf_led_countroler = handler_led_control;

    self->pf_led_register = led_register;
    
	
	
	
	//创建handler层中统一管理LED的任务
    printf("parameters = [%p]\r\n",self);
    ret = self->p_os_thread->pf_os_thread_create ( handler_thread, "handler_thread", 256, self, 0, &self->thread_handler);
    
	
	if( HANDLER_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORNOMEMORY at pf_os_thread_create\r\n");
#endif
        return ret;
    }

	
    //创建请求队列（提出的请求放入队列，统一管理LED的任务接收到队列就进行相应的控制）
    self->p_os_queue_interface->pf_os_queue_create( 10, sizeof(led_event_t), &(self->queue_handler));
	
	
    if( HANDLER_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("HANDLER_ERRORNOMEMORY at pf_os_queue_create\r\n");
#endif
		
		//任务删除
        self->p_os_thread->pf_os_thread_delete(self->thread_handler);
		
        return ret;
    }
	
	

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

