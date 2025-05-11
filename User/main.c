/* -------------------------------- system header -------------------------------- */
#include <stdio.h>
#include "freertos.h"
#include "task.h"
#include "semphr.h"
/* -------------------------------- user define header -------------------------------- */
#include "main.h"

volatile unsigned char mode = 1;    // �л������־λ

TaskHandle_t key_control_handler;
TaskHandle_t mode_switch_handler;

// ���廥�������
SemaphoreHandle_t mode_mutex;

void key_control(void *arg)
{
	while(1) {
		int key_state = Key_GetNum();
		if(key_state == 1) {
			xSemaphoreTake(mode_mutex, portMAX_DELAY);
			mode ++;
			if(mode > 3) {
				mode = 1;
			}
			xSemaphoreGive(mode_mutex);

			vTaskDelay(pdMS_TO_TICKS(200)); // ���Ӱ���ȥ�����ӳ�
			printf("key_control - Mode changed to %d\r\n", mode);
		} else {
			printf("key_control - No key press detected\r\n");
		}

		Digital_Display(mode);	// LED display current mode
		vTaskDelay(pdMS_TO_TICKS(50));
		printf("key_control - Delayed\r\n");
	}
}

void mode_switch(void * arg)
{
	while (1) {
		xSemaphoreTake(mode_mutex, portMAX_DELAY);
		switch(mode) {
			case 1: 
				Robot_Traction();
				EXTI_DeInit();
				printf("mode_switch - Case 1\r\n");
			break;

			case 2:
				Robot_Avoidance();
				printf("mode_switch - Case 2\r\n");
			break;

			case 3:
				Irscan();
				printf("mode_switch - Case 3\r\n");
			break;

			default:
				mode = 1;
				printf("mode_switch - Default case\r\n");
			break;
		}
		xSemaphoreGive(mode_mutex);

		vTaskDelay(pdMS_TO_TICKS(20));
		printf("mode_switch - Delayed\r\n");
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

	printf("Device initialization completed.\r\n");
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

	// ����������
	mode_mutex = xSemaphoreCreateMutex();

	if (mode_mutex == NULL) {
		printf("Failed to create mutex.\r\n");
		while(1);
	}

	/* �������� */
	xTaskCreate(key_control, "key_control", 512,NULL,1,&key_control_handler);
	xTaskCreate(mode_switch, "mode_switch", 512,NULL,2,&mode_switch_handler);

	vTaskStartScheduler();
	
	while(1){} /* ����������벻���ܵ����� */
}



