#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "mq9.h"

//ALIENTEK ̽����STM32F407������ ʵ��13
//LCD��ʾʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
u8 Smog_yu = 30;//CO����ֵ
int main(void)
{ 
	float Smog_ppm = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	MQ2_Init();
	LED_Init();					  //��ʼ��LED
 	LCD_Init();           //��ʼ��LCD FSMC�ӿ�				 	
  	while(1) 
	{		 
		
//		int a;
//		a = MQ7_PPM_Calibration();
//		LCD_ShowNum(30+64,170,a,2,16);
		
    Smog_ppm = Smog_GetPPM();
//		printf("����Ũ�ȣ�%.3f  ������ֵ��%d\n",Smog_ppm, Smog_yu);		
		LCD_ShowString(30,150,200,12,16,"Smog_ppm:   .   ");
		LCD_ShowString(30,170,200,12,16,"Smog_yu:  ");
		LCD_ShowNum(30+72,150,Smog_ppm,3,16);		//��ʾ����Ũ��	  
    LCD_ShowNum(30+104,150,(int)(Smog_ppm*1000)%1000,3,16);		//��ʾ����Ũ��		
		LCD_ShowNum(30+64,170,Smog_yu,2,16);		//��ʾ������ֵ	 
			
		LED0=!LED0;	 
		delay_ms(100);	
	} 
}

