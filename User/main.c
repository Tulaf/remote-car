/* -------------------------------- system header -------------------------------- */
#include <stdio.h>
#include "freertos.h"
#include "task.h"
#include "semphr.h"
/* -------------------------------- user define header -------------------------------- */
#include "main.h"

volatile unsigned char opt_mode = 1;

TaskHandle_t key_control_handler;
TaskHandle_t mode_switch_handler;


void key_control(void *arg)
{
	Digital_Display(opt_mode);	// LED display current opt_mode
	while(1) {
		int key_state = Key_GetNum();
		if(key_state == 1) {
			opt_mode ++;
			if(opt_mode > 3) {
				opt_mode = 1;
			}
			Digital_Display(opt_mode);	// LED display current opt_mode
		} else {
			//do nothing
		}

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

void mode_switch(void * arg)
{
	while (1) {
		switch(opt_mode) {
			case 1: 
				Robot_Traction();
				EXTI_DeInit();
			break;

			case 2:
				Robot_Avoidance();
			break;

			case 3:
				Irscan();
			break;

			default:
				opt_mode = 1;
			break;
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

static void DeviceInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Key_Init();					//按键初始化						
	IRremote_Init();           // 红外遥控器初始化
	Irtracking_Init();         // 红外循迹初始化
	Irobstacle_Init();         // 红外避障初始化
	LEDSEG_Init();			   // LED初始化
	Serial_Init();             // 串口初始化
	robot_Init();              // 机器人初始化
	log_message(INFO, "Device init completely\r\n");
}

static void PrintfInfo(void)
{
    // get flash size(uint:KB)
    uint16_t flash_size = *(uint16_t*)0x1FFFF7E0;

    log_message(INFO, "====== Information ======\r\n");
	log_message(INFO, "CPU : STM32F103C8T6, Frequency : %dMHz\r\n", SystemCoreClock / 1000000);
	log_message(INFO, "Flash Size: %d KB\r\n", flash_size);
	log_message(INFO, "====== Information ======\r\n");
}

int main(void)
{
	/* hardware init */
	DeviceInit();
	/* print some hard info */
	PrintfInfo();

	/* create key control task */
	xTaskCreate(key_control, "key_control", 512,NULL,1,&key_control_handler);
	/* create mode switch task */
	xTaskCreate(mode_switch, "mode_switch", 512,NULL,2,&mode_switch_handler);

	vTaskStartScheduler();
	
	while(1){} /* 正常情况代码不会跑到这里 */
}



