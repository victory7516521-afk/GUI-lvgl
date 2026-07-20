#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
#include "myimage.h"
#include "tim.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_widgets.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"


/* 任务句柄 */
TaskHandle_t app_task_init_handle = NULL;
TaskHandle_t app_task_lvgl_handle = NULL;

/* 任务函数*/
static void app_task_init(void *pvParameters);
static void app_task_lvgl(void *pvParameters);


// 声明一个自定义的 TTF 字体
LV_FONT_DECLARE(my_font);

// 定义一个标签演示函数
void label_demo2(void)
{
    // 创建一个新的标签对象，并将其添加到当前活动的屏幕上
    lv_obj_t *label1 = lv_label_create(lv_scr_act());

    // 设置标签的字体为之前声明的自定义 TTF 字体
    lv_obj_set_style_text_font(label1, &my_font, 0);

    // 设置标签的文本内容
    lv_label_set_text(label1, " 好世界 ");



    // 将标签对齐到屏幕中心，并向上偏移 40 像素
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);


}


int main(void)
{

	
     /* 创建app_task_init任务 */
    xTaskCreate((TaskFunction_t)app_task_init,          /* 任务入口函数 */
                (const char *)"app_task_init",          /* 任务名字 */
                (uint16_t)256,                          /* 任务栈大小 */
                (void *)NULL,                           /* 任务入口函数参数 */
                (UBaseType_t)4,                         /* 任务的优先级 */
                (TaskHandle_t *)&app_task_init_handle); /* 任务控制块指针 */
    /* 开启任务调度 */
    vTaskStartScheduler();
}


void app_task_init(void *pvParameters)
{
	u16 id;
	//中断优先级分组，且一个工程只能设置一次
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	Usart1_init(115200);
	
	Led_Init();
	//1ms中断一次，用于lvgl心跳
	Tim3_init(83, 999);
	
 // 初始化LVGL图形库
    // 这是使用LVGL前的必要步骤，用于初始化内部数据结构和变量
    lv_init();
    
    // 初始化LVGL显示设备接口
    // 该函数会配置与显示硬件相关的参数，如屏幕分辨率、颜色深度等
    // 并注册显示驱动回调函数
    lv_port_disp_init();

    // 初始化LVGL输入设备接口
    // 配置触摸屏或按键等输入设备的驱动
    // 注册输入设备回调函数
    lv_port_indev_init();

    // 运行LVGL的小部件演示程序
    // 这会创建一个包含各种LVGL控件(按钮、滑块等)的演示界面
   // lv_demo_widgets();
    
    // 运行LVGL性能基准测试
    // 这会创建一个测试界面，用于评估图形渲染性能
    // 注意：通常不会同时运行demo和benchmark
    // lv_demo_benchmark();    
    
    // 初始化TIM3定时器，设置定时周期为1毫秒
    // 这个定时器通常用于提供LVGL的系统心跳(tick)
    // LVGL需要定期(1-10ms)调用lv_tick_inc()来维持内部计时	


    
    /* 创建app_task_lvgl任务 */
    xTaskCreate((TaskFunction_t)app_task_lvgl,          /* 任务入口函数 */
                (const char *)"app_task_lvgl",          /* 任务名字 */
                (uint16_t)512,                          /* 任务栈大小 */
                (void *)NULL,                           /* 任务入口函数参数 */
                (UBaseType_t)7,                         /* 任务的优先级 */
                (TaskHandle_t *)&app_task_lvgl_handle); /* 任务控制块指针 */

   // vTaskDelete(NULL);
				
	for(;;)
	{
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		vTaskDelay(1000);
	}

}
static void app_task_lvgl(void *pvParameters)
{
    
    
    //LVGL案例
    lv_demo_widgets();
    
    
    for (;;)
    {
        //定期检查与执行lvgl所有已注册的周期性任务(诸如界面元素动画更新、定时数据刷新、系统状态监测等操作)
        lv_task_handler();

        vTaskDelay(5);
    }
}





//编写中断服务函数。
//1ms进入中断一次
void   TIM3_IRQHandler(void)
{
	//判断标志位是否置1
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
        /* 系统时间向前推进一个单位的时间间隔 */
        lv_tick_inc(1);  //lv_tick_inc参数范围：1~10
		
		//清空中断标志位
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}

}

