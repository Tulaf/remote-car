#include "main.h"

void Robot_Traction(void)                     //机器人循迹子程序
{
	if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 0) {
		makerobo_run(70,0);   // 前进
	} else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 0) {//左侧压线
		makerobo_Left(70,0);    //左侧修正
	} else if(Left_Irtracking_Get() == 0 && Right_Irtracking_Get() == 1) {// 右侧压线
		makerobo_Right(70,0); // 右侧修正
	} else if(Left_Irtracking_Get() == 1 && Right_Irtracking_Get() == 1) {// 全部压线，停止
		makerobo_brake(0); // 停止
	}
}

void Robot_Avoidance(void)     // 红外避障子程序
{
	if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 1) {
		makerobo_run(70,0);   // 前进
	} else if(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) { //右侧检测到红外信号
		makerobo_Left(70,400); 
	}
	else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) { // 左侧检测到红外信号
		makerobo_Right(70,400);
	}			
	else if(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) {// 两侧传感器同时检测到红外信号
		makerobo_brake(400);          // 调用电机后退函数
		makerobo_Spin_Right(70,900);  // 调用电机右转函数
	}
}

void ControlCar_Ircontrol(void) // 定义红外遥控器控制
{
	uint8_t buf[2];
	uint8_t data_code=0; 
	
	if(IR_Receiveflag == 1) {//如果红外接收到
		IR_Receiveflag = 0; //清零
		log_message(DEBUG,"红外接收码 %0.8X\r\n",IR_Receivecode);	//打印
		data_code=IR_Receivecode>>8;
		IR_Receivecode = 0; //接收码清零

		buf[0] = data_code/16;
		buf[1] = data_code%16;

		log_message(DEBUG, "buf[0]:%d\r\n",buf[0]);
		log_message(DEBUG, "buf[1]:%d\r\n",buf[1]);
	} if(buf[0] == 11 && buf[1] == 1) {
			makerobo_run(70,2000);  // 前进2s
	} else if(buf[0] == 13 && buf[1] == 4) {
		makerobo_back(70,2000); // 后退2s
	}else if(buf[0] == 9 && buf[1] == 1) {
		makerobo_Spin_Left(70,2000); //左转
	} else if(buf[0] == 14 && buf[1] == 1) {
		makerobo_Spin_Right(70,2000); // 右转
	} else if(buf[0] == 8 && buf[1] == 1) {
		makerobo_brake(0); // 停止
	} else {
		makerobo_brake(0); // 停止
	}
}

void Irscan(void)
{
	uint8_t Tim=0,Ok=0,Data,Num=0;
	while(1) {
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1) {
			 Tim=IRremote_Counttime();//获得此次高电平时间

			 if(Tim>=250) break;//不是有用的信号

			 if(Tim>=200 && Tim<250) {
			 	Ok=1;//收到起始信号
			 } else if(Tim>=60 && Tim<90) {
			 	Data=1;//收到数据 1
			 } else if(Tim>=10 && Tim<50) {
			 	Data=0;//收到数据 0
			 }

			 if(Ok==1) {
				IR_Receivecode<<=1;
				IR_Receivecode+=Data;
				if(Num>=32) {
					IR_Receiveflag=1;
					break;
				}
			 }
			 Num++;
		}
		ControlCar_Ircontrol();
	}
	EXTI_ClearITPendingBit(EXTI_Line8);	
}


