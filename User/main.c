/* -------------------------------- system header -------------------------------- */
#include <stdio.h>
#include "freertos.h"
#include "task.h"
/* -------------------------------- user define header -------------------------------- */
#include "main.h"

volatile unsigned char mode = 1;    // 切换程序标志位

TaskHandle_t mytestHandler;

void test(void * arg)
{
	while (1) {
		if(1 == Key_GetNum()) {
			mode ++;
			if(mode > 3) {
				mode = 1;
			}
			Digital_Display(mode);	// LED显示当前模式
		} else {
			switch(mode) {
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
					mode = 1;
				break;
			}
		}
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
}

static void PrintfInfo(void)
{
    // get flash size(uint:KB)
    uint16_t flash_size = *(uint16_t*)0x1FFFF7E0;

    printf("====== Information ======\r\n");
	printf("CPU : STM32F103C8T6, Frequency : %dMHz\r\n", SystemCoreClock / 1000000);
	printf("Flash Size: %d KB\r\n", flash_size);
	printf("\r\n");
}

int main(void)
{
	/* hardware init */
	DeviceInit();
	/* print some hard info */
	PrintfInfo();
	
	/* 创建进程 */
	xTaskCreate(test, "freetest", 512,NULL,2,&mytestHandler);

	vTaskStartScheduler();
	
	while(1){} /* 正常情况代码不会跑到这里 */
}
