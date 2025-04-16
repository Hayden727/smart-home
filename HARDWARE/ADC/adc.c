#include "adc.h"
#include "delay.h"

void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef  ADC_CommonInitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  
	//PC0作为模拟通道输入引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	ADC_DeInit();//复位所有ADC
	
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//是否开启DMA模式
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent; //ADC的独立工作模式
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div8; //时钟分频因子72/8,时钟不能超过36M
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//采样之间的延迟
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;//ADC分辨率   
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//是否使用扫描模式   //不使用
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//单次转换OR连续转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//外部触发使能方式//禁止触发方式，使用软件触发
	//ADC_InitStruct.ADC_ExternalTrigConv=   ; //触发模式,这里不需要设置
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion=1; //规则通道序列长度     
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE); //ADC使能
	
  //ADC_SoftwareStartConv(ADC1);
}

//注意通道的值要选
u16 Get1_ADC1(u8 ch)
{
  ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_480Cycles);//通道的配置，这里指通道10
	ADC_SoftwareStartConv(ADC1);

	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
		
	return ADC_GetConversionValue(ADC1);	
}

u16 Get1_ADC1_Average(u8 ch,u8 times)
{
	u32 temp = 0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp += Get1_ADC1(ch);
		//delay_ms(0);
	}
	return temp/times;
}

