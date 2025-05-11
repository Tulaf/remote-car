/* -------------------------------- system header -------------------------------- */
#include <stdio.h>
#include "freertos.h"
#include "task.h"
/* -------------------------------- user define header -------------------------------- */
#include "main.h"

volatile unsigned char mode = 1;    // �л������־λ

TaskHandle_t mytestHandler;

void test(void * arg)
{
	while (1) {
		if(1 == Key_GetNum()) {
			mode ++;
			if(mode > 3) {
				mode = 1;
			}
			Digital_Display(mode);	// LED��ʾ��ǰģʽ
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
	Key_Init();					//������ʼ��						
	IRremote_Init();           // ����ң������ʼ��
	Irtracking_Init();         // ����ѭ����ʼ��
	Irobstacle_Init();         // ������ϳ�ʼ��
	LEDSEG_Init();			   // LED��ʼ��
	Serial_Init();             // ���ڳ�ʼ��
	robot_Init();              // �����˳�ʼ��
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
	
	/* �������� */
	xTaskCreate(test, "freetest", 512,NULL,2,&mytestHandler);

	vTaskStartScheduler();
	
	while(1){} /* ����������벻���ܵ����� */
}
