#ifndef _MQ2_H
#define _MQ2_H
 
//C�ļ�����Ҫ��������ͷ�ļ�
#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"
#include "math.h"
 
#define SMOG_READ_TIMES	10				//��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
//C�ļ��ж���ĺ���������
void MQ2_Init(void);
float Smog_Get_Vol(void);	//��ȡMQ7�������ĵ�ѹֵ
float MQ7_PPM_Calibration(void);
float Smog_GetPPM(void);
 
#endif

