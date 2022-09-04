/*
 *  hsm20g.c
 *
 *  Created on: Aug 5, 2022
 *  Author: Md. Khairul Alam
 */
#include "debug.h"
#include "hsm20g.h"
#include <rtthread.h>
#include <rthw.h>

s16 Calibrattion_Val1 = 0;
s16 Calibrattion_Val2 = 0;

void  ADC_Function_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  , ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2  , ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_DeInit(ADC2);

    ADC_InitStructure.ADC_Mode = ADC_Mode_InjecSimult;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_OutputBuffer = ADC_OutputBuffer_Disable;
    ADC_InitStructure.ADC_Pga = ADC_Pga_1;

    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_InjectedSequencerLengthConfig(ADC1, 1);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );

    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE);   //disable buffer
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val1 = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);   //enable buffer

    ADC_Init(ADC2, &ADC_InitStructure);
    ADC_InjectedSequencerLengthConfig(ADC2, 1);
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );

    ADC_SoftwareStartInjectedConvCmd(ADC2, ENABLE);
    ADC_Cmd(ADC2, ENABLE);

    ADC_BufferCmd(ADC2, DISABLE);   //disable buffer
    ADC_ResetCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC2));
    ADC_StartCalibration(ADC2);
    while(ADC_GetCalibrationStatus(ADC2));
    Calibrattion_Val2 = Get_CalibrationValue(ADC2);

    ADC_BufferCmd(ADC2, ENABLE);   //enable buffer
}

//void get_temp_value(void)
s32 get_temp_value(void)
{
    ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
    rt_thread_mdelay(100);
    ADC_SoftwareStartInjectedConvCmd(ADC1, DISABLE);
    rt_thread_mdelay(100);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC ));
        u16 adc_value = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);

    int i = 0;
    s32 converted_value = 0;
    s32 value_mv = 0;
    for(i=0; i<15; i++)
    {
        converted_value = Get_TEMP_ConversionVal(adc_value);
        value_mv += (converted_value * 3300 / 4096);
    }
    value_mv = value_mv/15;
    s32 temperature = ((-0.00015 * value_mv * value_mv) + (value_mv * 0.2268) - 11.4945);
    //s32 temperature=(5.26*value_mv*value_mv*value_mv)-(27.34*value_mv*value_mv)+(68.87*value_mv)-17.81;

    rt_kprintf( "Temp  =%d\r\n", temperature);

    return temperature;
}

//void get_humid_value(void)
s32 get_humid_value(void)
{
    ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
    rt_thread_mdelay(100);
    ADC_SoftwareStartInjectedConvCmd(ADC1, DISABLE);
    rt_thread_mdelay(100);

    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_JEOC ));
        u16 adc_value = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);

    s32 converted_value = Get_HUMID_ConversionVal(adc_value);
    s32 value_mv = (converted_value * 3300 / 4096);

    s32 relative_humidity = ((0.0319 * value_mv) - 11.4378);
    //s32 relative_humidity = ((0.1507 * value_mv) - 11.4378);

    rt_kprintf( "Humid  =%d\r\n", relative_humidity);

    return relative_humidity;
}


u16 Get_TEMP_ConversionVal(s16 val)
{
    if((val+Calibrattion_Val1)<0) return 0;
    if((Calibrattion_Val1+val)>4095||val==4095) return 4095;
    return (val+Calibrattion_Val1);
}

u16 Get_HUMID_ConversionVal(s16 val)
{
    if((val+Calibrattion_Val2)<0) return 0;
    if((Calibrattion_Val2+val)>4095||val==4095) return 4095;
    return (val+Calibrattion_Val2);
}
