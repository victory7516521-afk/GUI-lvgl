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

volatile u8 g_rx_flag = 0;
volatile u8 g_count = 0;

volatile u8 g_buffer[32] = {0};  
volatile u8 g_rxuffer[32] = {0};


void USART1_IRQHandler(void)
{

	//判断标志位是否置1
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//清空中断标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		//串口1接收数据
		g_buffer[g_count++] = USART_ReceiveData(USART1);
		
		if(g_buffer[g_count - 1] == ':') //判断结束符是否为:
		{
			//过滤结束符   HCL11:   HCL10:
			for(int i=0; i<g_count - 1; i++)
			{
				g_rxuffer[i] = g_buffer[i];
			}
			
			g_rx_flag = 1; //接收数据标志位
			
			g_count = 0;//新的数据帧从g_buffer[0]开始接受
			
			
			memset(g_buffer, 0, sizeof(g_buffer));
		
		
		}
		       
	
	}

}



/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static const lv_font_t *font;

/**
 * @brief  例1
 * @param  无
 * @return 无
 */
static void lv_example_label_1(void)
{
    if (scr_act_width() <= 320)
        font = &lv_font_montserrat_10;
    else if (scr_act_width() <= 480)
        font = &lv_font_montserrat_14;
    else
        font = &lv_font_montserrat_20;

    lv_obj_t* label = lv_label_create(lv_scr_act());                                /* 定义并创建标签 */
    lv_label_set_text(label, "#0000ff Re-color# #ff00ff words# #ff0000 of a# "      /* 设置标签文本 */
                             "label, align the lines to the center"
                             "and  wrap long text automatically.");
    lv_label_set_recolor(label, true);                                              /* 启用标签文本重新着色 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置标签文本字体 */
    lv_obj_set_width(label, scr_act_width() / 3);                                   /* 设置标签宽度 */
    lv_obj_align(label, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);                  /* 设置标签位置 */
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);         /* 设置标签文本对齐方式 */


}

/**
 * @brief  例2
 * @param  无
 * @return 无
 */
static void lv_example_label_2(void)
{
    lv_obj_t* label = lv_label_create(lv_scr_act());                                /* 定义并创建标签 */
    lv_label_set_text(label, "It is a circularly scrolling text. ");                /* 设置标签文本 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置标签文本字体 */
    lv_obj_set_width(label, scr_act_width() / 3);                                   /* 设置标签宽度 */
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);                   /* 设置标签长模式：循环滚动 */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);                                     /* 设置标签位置 */
}

/**
 * @brief  例3
 * @param  无
 * @return 无
 */
static void lv_example_label_3(void)
{
    lv_obj_t* label = lv_label_create(lv_scr_act());                                /* 定义并创建标签 */
    lv_label_set_text_fmt(label, "Label can set text like %s", "printf");           /* 设置标签文本 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置标签文本字体 */
    lv_obj_set_width(label, scr_act_width() / 3);                                   /* 设置标签宽度 */
    lv_obj_align(label, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                   /* 设置标签位置 */
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);         /* 设置标签文本对齐方式 */

    lv_obj_t* label_shadow = lv_label_create(lv_scr_act());                         /* 定义并创建阴影标签 */
    lv_label_set_text(label_shadow, lv_label_get_text(label));                      /* 设置标签文本 */
    lv_obj_set_style_text_font(label_shadow, font, LV_PART_MAIN);                   /* 设置标签文本字体 */
    lv_obj_set_width(label_shadow, scr_act_width() / 3);                            /* 设置标签宽度 */
    lv_obj_set_style_text_opa(label_shadow, LV_OPA_30, LV_PART_MAIN);               /* 设置标签文本透明度 */
    lv_obj_set_style_text_color(label_shadow, lv_color_black(), LV_PART_MAIN);      /* 设置标签文本颜色 */
    lv_obj_set_style_text_align(label_shadow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);  /* 设置标签文本对齐方式 */
    lv_obj_align_to(label_shadow, label, LV_ALIGN_TOP_LEFT, 3, 3);                  /* 设置标签位置 */
}


//初始UI界面
void UI_Start(void)
{
    lv_example_label_1();
    lv_example_label_2();
    lv_example_label_3();
}


int main(void)
{

	
	u16 id;
	//中断优先级分组，且一个工程只能设置一次
	//抢占优先级范围:0~3  响应优先级:0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_init();

	Usart1_init(115200);
	
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
    lv_demo_widgets();
    
    // 运行LVGL性能基准测试
    // 这会创建一个测试界面，用于评估图形渲染性能
    // 注意：通常不会同时运行demo和benchmark
    // lv_demo_benchmark();    
    
    // 初始化TIM3定时器，设置定时周期为1毫秒
    // 这个定时器通常用于提供LVGL的系统心跳(tick)
    // LVGL需要定期(1-10ms)调用lv_tick_inc()来维持内部计时	
	
	
	while(1) 
	{
		lv_task_handler();
		delay_ms(5);
	}
	
	return 0;
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
