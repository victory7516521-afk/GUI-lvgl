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


/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static const lv_font_t* font;       /* 定义字体 */

static lv_obj_t *label_speed;       /* 速度提示文本 */
static lv_obj_t *btn_speed_up;      /* 加速按钮 */
static lv_obj_t *btn_speed_down;    /* 减速按钮 */
static lv_obj_t *btn_stop;          /* 急停按钮 */

static int32_t speed_val = 0;       /* 速度值 */

/**
 * @brief  按钮回调
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
 
static void btn_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* 获取触发源 */

    if(target == btn_speed_up)                      /* 加速按钮 */
    {
        speed_val += 30;
    }
    else if(target == btn_speed_down)               /* 减速按钮 */
    {
        speed_val -= 30;
    }
    else if(target == btn_stop)                     /* 急停按钮 */
    {
        speed_val = 0;
    }
    lv_label_set_text_fmt(label_speed, "Speed : %d RPM", speed_val);/*类似于printf 更新速度值 */
}


/**
 * @brief  速度值提示标签
 * @param  无
 * @return 无
 */
static void lv_example_label(void)
{
   
	/* 根据活动屏幕宽度选择字体 */
    if (scr_act_width() <= 320)
    {
        font = &lv_font_montserrat_12;
    }
    else if (scr_act_width() <= 480)
    {
        font = &lv_font_montserrat_14;
    }
    else
    {
        font = &lv_font_montserrat_20;
    }

    label_speed = lv_label_create(lv_scr_act());                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(label_speed, "Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);           /* 设置标签位置 */
}
/**
 * @brief  加速按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_up(void)
{
    btn_speed_up = lv_btn_create(lv_scr_act());                                     /* 创建加速按钮 */
    lv_obj_set_size(btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);       /* 设置按钮大小 */
    lv_obj_align(btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);           /* 设置按钮位置 */
    lv_obj_add_event_cb(btn_speed_up, btn_event_cb, LV_EVENT_CLICKED, NULL);        /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_speed_up);                                /* 创建加速按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Speed +");                                            /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

/**
 * @brief  减速按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_down(void)
{
    btn_speed_down = lv_btn_create(lv_scr_act());                                   /* 创建加速按钮 */
    lv_obj_set_size(btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);     /* 设置按钮大小 */
    lv_obj_align(btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            /* 设置按钮位置 */
    lv_obj_add_event_cb(btn_speed_down, btn_event_cb, LV_EVENT_CLICKED, NULL);      /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_speed_down);                              /* 创建减速按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Speed -");                                            /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

/**
 * @brief  急停按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_stop(void)
{
    btn_stop = lv_btn_create(lv_scr_act());                                         /* 创建急停按钮 */
    lv_obj_set_size(btn_stop, scr_act_width() / 4, scr_act_height() / 6);           /* 设置按钮大小 */
    lv_obj_align(btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                /* 设置按钮位置 */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(btn_stop, btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_stop);                                    /* 创建急停按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Stop");                                               /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

//初始UI界面
void UI_Start(void)
{
    lv_example_label();             /* 速度提示标签 */
    lv_example_btn_up();            /* 加速按钮 */
    lv_example_btn_down();          /* 减速按钮 */
    lv_example_btn_stop();          /* 急停按钮 */
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
    //lv_demo_widgets();
    
    // 运行LVGL性能基准测试
    // 这会创建一个测试界面，用于评估图形渲染性能
    // 注意：通常不会同时运行demo和benchmark
    // lv_demo_benchmark();    
    
    // 初始化TIM3定时器，设置定时周期为1毫秒
    // 这个定时器通常用于提供LVGL的系统心跳(tick)
    // LVGL需要定期(1-10ms)调用lv_tick_inc()来维持内部计时	
	UI_Start();
	
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

