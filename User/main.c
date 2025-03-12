#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"
#include "Serial.h"
#include "string.h"
#include "Motor.h"

uint8_t KeyNum;			//定义用于接收键码的变量
uint8_t RxData;			//定义用于接收串口数据的变量
float Angle;			//定义角度变量

int main(void)
{
	/*模块初始化*/
	Motor_Init();
	OLED_Init();		//OLED初始化
	Servo_Init();		//舵机初始化
	Key_Init();			//按键初始化
	/*串口初始化*/
	Serial_Init();		//串口初始化
	
	/*显示静态字符串*/
	//OLED_ShowString(1, 1, "Angle:");	//1行1列显示字符串Angle:
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	while (1)
	{	
		//Motor_SetSpeed(0);
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);				//OLED清除指定位置，并显示接收到的数据包
			
			/*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)			//如果收到LED_ON指令
			{										//点亮LED
				Servo_SetAngle(150);		
				Serial_SendString("LED_ON_OK\r\n");				//串口回传一个字符串LED_ON_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_ON_OK");				//OLED清除指定位置，并显示LED_ON_OK
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)	//如果收到LED_OFF指令
			{										//熄灭LED
				Servo_SetAngle(50);
				Serial_SendString("LED_OFF_OK\r\n");			//串口回传一个字符串LED_OFF_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFF_OK");			//OLED清除指定位置，并显示LED_OFF_OK
			}else if (strcmp(Serial_RxPacket, "LED_OFFDJ") == 0)	//如果收到LED_OFF指令
			{
				Motor_SetSpeed(0);									//熄灭LED
				Serial_SendString("LED_OFFDJ_OK\r\n");			//串口回传一个字符串LED_OFF_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFFDJ_OK");			//OLED清除指定位置，并显示LED_OFF_OK
			}else if (strcmp(Serial_RxPacket, "LED_ONDJ") == 0)	//如果收到LED_OFF指令
			{
				Motor_SetSpeed(100);									//熄灭LED
				Serial_SendString("LED_ONDJ_OK\r\n");			//串口回传一个字符串LED_OFF_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_ONDJ_OK");			//OLED清除指定位置，并显示LED_OFF_OK
			}
			else						//上述所有条件均不满足，即收到了未知指令
			{
				Serial_SendString("ERROR_COMMAND\r\n");			//串口回传一个字符串ERROR_COMMAND
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");			//OLED清除指定位置，并显示ERROR_COMMAND
			}
			
			Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
		KeyNum = Key_GetNum();			//获取按键键码
		if (KeyNum == 1)				//按键1按下
		{
			Angle += 30;				//角度变量自增30
			if (Angle > 180)			//角度变量超过180后
			{
				Angle = 0;				//角度变量归零
			}
		}
		
		Angle = 120;
//		Servo_SetAngle(120);			//设置舵机的角度为角度变量
		//OLED_ShowNum(1, 7, Angle, 3);	//OLED显示角度变量
	}
}
