/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : Md. Khairul Alam
* Version            : V1.0.0
* Date               : 2022/08/30
* Description        : Main program body.
* License-Identifier : Apache-2.0
*******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "../LCD/LiquidCrystal.h"
#include "../HSM20G/hsm20g.h"
#include "../GPRS/usart2.h"
#include "../GPRS/gprs.h"
#include "../NEO7/gps.h"
#include "../NEO7/usart.h"


/* Global typedef */


/* Global Variable */
extern u8 GPS_BUF[RX_BUFFER_SIZE];
extern vu16 GPSMSG_FLAG;
extern vu16 GPSMSG_INDEX;
extern vu8 GPS_DEBUG;

extern float lon_value;
extern float lat_value;

u8 GPS_SEND_BUF[RX_BUFFER_SIZE];
nmea_msg gpsx;

s32 temperature;
s32 humidity;

u8 api_key[] = "api_key=B3FPE7GTVY1ISGQS&field1=";

/* Thread control block declaration */
rt_thread_t read_sensor_thread  = RT_NULL;
rt_thread_t display_thread  = RT_NULL;
rt_thread_t read_location_thread = RT_NULL;
rt_thread_t data_to_cloud_thread  = RT_NULL;
rt_thread_t notification_thread  = RT_NULL;


void System_INIT(void){
    usart2_configuration();           //SIM800 GPRS Port configuration
    usart_configuration();            //GPS module port configuration
    timer_configuration(2123, 5632);  //timer configuration for GPS receive
    rt_lcd_init();                    //LCD initialization
    ADC_Function_Init();              //ADC initialization for temperature & humidity sensor
}



/*********************************************************************/


void read_sensor(void* parameter)
{

    while(1)
    {

        temperature = get_temp_value();
        humidity = get_humid_value();
        //rt_kprintf( "Temperature  =%d\r\n", temperature);
        //rt_kprintf( "Humidity  =%d\r\n", humidity);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
    }
}


void display(void* parameter)
{

    char temp[BUFSIZ];
    char humid[BUFSIZ];
    char lon[BUFSIZ];
    char lat[BUFSIZ];

    while(1)
    {
        //convert int to char array
        snprintf(temp, sizeof(temp), "%d", temperature);
        snprintf(humid, sizeof(humid), "%d", humidity);
        /*
        //convert float to char array
        int lon_int  = (int) lon_value;
        float lon_float = (abs(lon_value) - abs(lon_int)) * 100000;
        int lon_fraction = (int)lon_float;
        snprintf(lon, sizeof(lon), "%d.%d", lon_int, lon_fraction);

        int lat_int  = (int) lat_value;
        float lat_float = (abs(lat_value) - abs(lat_int)) * 100000;
        int lat_fraction = (int)lat_float;
        snprintf(lat, sizeof(lat), "%d.%d", lat_int, lat_fraction);
        */
        rt_lcd_clear();
        rt_lcd_setCursor(0, 0);
        rt_lcd_print("T: ");
        rt_lcd_print(temp);
        rt_lcd_print("C");
        rt_lcd_print(" H: ");
        rt_lcd_print(humid);
        rt_lcd_print("%");
        rt_lcd_setCursor(0, 1);
        //rt_lcd_print("Lo:90.41La:23.18");
        rt_lcd_print("Lo:");
        rt_lcd_print(lon);
        rt_lcd_print("La:");
        rt_lcd_print(lat);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
    }
}

void read_location(void* parameter)
{
    while(1)
    {

        if(GPSMSG_FLAG)
        {
            int len=GPSMSG_INDEX;
            int i = 0;
            for(i=0;i<len;i++)
               {
                 GPS_SEND_BUF[i]=GPS_BUF[i];
               }
            GPSMSG_INDEX = 0;
            GPSMSG_FLAG=0;
            GPS_SEND_BUF[i]=0;
            GPSMSG_Analysis(&gpsx,(u8*)GPS_SEND_BUF);
            if (0 == GPS_DEBUG)
                rt_kprintf("%s",GPS_SEND_BUF);
            if (1 == GPS_DEBUG)
                Send_NMEA_MSG(&gpsx);
        }
    }
}


void data_to_cloud(void* parameter)
{

    while(1)
    {

        send_data(api_key, temperature, humidity);
        //send data to cloud every 3 seconds
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
    }
}

void send_notification(void* parameter)
{

    while(1)
    {

        if(temperature>4)
            temperature_high_alart();
        if(humidity>50)
            humidity_high_alart();
        rt_thread_mdelay(500);
        rt_thread_mdelay(500);
    }
}

void Run(void)
{

    /**********Creating the threads************/
    read_sensor_thread =                            //thread control block pointer
        rt_thread_create( "ReadSensor",             //thread name
                          read_sensor,              //thread entry function
                          RT_NULL,                  //thread entry function parameters
                          512,                      //thread stack size
                          1,                        //thread priority
                          20);                      //thread time slice
        if (read_sensor_thread != RT_NULL)
            rt_thread_startup(read_sensor_thread);  //make the thread enter the ready state

    display_thread = rt_thread_create("Display", display, RT_NULL, 512, 2, 20);
    if (display_thread != RT_NULL)
        rt_thread_startup(display_thread);

    read_location_thread = rt_thread_create("Location", read_location, RT_NULL, 512, 2, 20);
    if (read_location_thread != RT_NULL)
         rt_thread_startup(read_location_thread);

    data_to_cloud_thread = rt_thread_create( "DataToCloud", data_to_cloud, RT_NULL, 512, 2, 20);
    if (data_to_cloud_thread != RT_NULL)
        rt_thread_startup(data_to_cloud_thread);

    notification_thread = rt_thread_create( "SendNotification", send_notification, RT_NULL, 512, 2, 20);
    if (notification_thread != RT_NULL)
        rt_thread_startup(notification_thread);

}


/*************************************************************************/
int main(void)
{
    rt_kprintf("\r\n MCU: CH32V307\r\n");
    rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");

    System_INIT();
    Run();

}
