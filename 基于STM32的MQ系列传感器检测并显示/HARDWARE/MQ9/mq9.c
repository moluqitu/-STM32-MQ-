#include "mq9.h"
 
#define CAL_PPM  10  // У׼������PPMֵ
#define RL	     10  // RL��ֵ
#define R0	     97  // R0��ֵ
 
 
void MQ2_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	/* ���ź�ADC��ʱ��ʹ�ܡ�*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 
	/*����������Ϊģ�⹦��ģʽ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ�⹦��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*��ADC�ĳ������á�*/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;		//84�ͣȣ�/2������42�ͣȣ�
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//��ֹMDA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//ADCͨ�����ü��
	ADC_CommonInit(&ADC_CommonInitStructure);
 
	/* ADC1 ��ʼ�� ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;		//�ֱ���
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//��ֹɨ��			
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//����Ҫ�ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;					//һ��ת��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC1�Ĳ���ͨ��4�������ͨ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);
 
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}
 
uint32_t MQ2_ADC_Read(void)
{
	/* �������������� */ 
	ADC_SoftwareStartConv(ADC1);
	
	//�ȴ�ת������
	while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	return ADC_GetConversionValue(ADC1);
}
 
//����ƽ��ֵ
u16 ADC1_Average_Data(u8 ADC_Channel)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)	//#define SMOG_READ_TIMES	10	��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
	{
		temp_val+=MQ2_ADC_Read();	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//�õ�ƽ��ֵ
    return (u16)temp_val;//���������ADCƽ��ֵ
}
 
//��ȡMQ7�������ĵ�ѹֵ
float Smog_Get_Vol(void)
{
	u16 adc_value = 0;//���Ǵ�MQ-7������ģ���ѹ�����ADCת���л�õ�ԭʼ����ֵ����ֵ�ķ�ΧΪ0��4095����ģ���ѹ��ʾΪ����ֵ
	float voltage = 0;//MQ-7������ģ��ĵ�ѹ�������һ����̼��Ũ�ȳ�����
	
	adc_value = ADC1_Average_Data(ADC_Channel_4);//#define SMOG_ADC_CHX	ADC_Channel_4	���������������ڵ�ADCͨ�����
	delay_ms(5);
	
    voltage  = (3.3/4096.0)*(adc_value);
	
	return voltage;
}
///*********************
// ������У׼���������ݵ�ǰ����PPMֵ���õ�RS��ѹֵ�����Ƴ�R0ֵ��
// �ڿ��������й�����R0Ϊ26
//float MQ7_PPM_Calibration()
//{
//	float RS = 0;
//	float R0 = 0;
//	RS = (3.3f - Smog_Get_Vol()) / Smog_Get_Vol() * RL;//RL	10  // RL��ֵ
//	R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);//CAL_PPM  10  // У׼������PPMֵ
//	return R0;
//}
//**********************/
 
// ����Smog_ppm
float Smog_GetPPM()
{
	float RS = (3.3f - Smog_Get_Vol()) / Smog_Get_Vol() * RL;
	float ppm = 98.322f * pow(RS/R0, -1.458f);
	return  ppm;
}

