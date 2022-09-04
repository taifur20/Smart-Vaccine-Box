/*
 * gprs.c
 *
 *  Created on: Aug 6, 2022
 *  Author: Md. Khairul Alam
 */
#include "debug.h"
#include "usart2.h"
#include "gprs.h"
#include <string.h>
#include <rtthread.h>
#include <rthw.h>

void send_data(char *msg, int first_val, int second_val){
    rt_thread_mdelay(1000);
    send_command("AT\r\n");                                             // Check Communication
    rt_thread_mdelay(500);
    send_command("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");              // Connection type GPRS
    rt_thread_mdelay(500);
    send_command("AT+SAPBR=3,1,\"APN\",\"gpinternet\"\r\n");            // APN of the provider
    rt_thread_mdelay(500);
    send_command("AT+SAPBR=1,1\r\n");                                   // Open GPRS context
    rt_thread_mdelay(4000);
    send_command("AT+SAPBR=2,1\r\n");                                   // Query the GPRS context
    rt_thread_mdelay(3000);
    send_command("AT+HTTPINIT\r\n");                                    // Initialize HTTP service
    rt_thread_mdelay(3000);
    send_command("AT+HTTPPARA=\"CID\",1\r\n");                          // Set parameters for HTTP session
    rt_thread_mdelay(3000);
    send_command("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update\"\r\n"); // Set parameters for HTTP session
    rt_thread_mdelay(5000);
    send_command("AT+HTTPDATA=33,10000\r\n");                           // POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data
    rt_thread_mdelay(2000);

    send_apikey(msg, first_val, second_val);
    rt_thread_mdelay(5000);

    send_command("AT+HTTPACTION=1\r\n");                                 // Start POST session
    rt_thread_mdelay(3000);
    send_command("AT+HTTPTERM\r\n");                                     // Terminate HTTP service
    rt_thread_mdelay(3000);
    send_command("AT+SAPBR=0,1\r\n");                                    // Close GPRS context
    rt_thread_mdelay(2000);
}

void send_apikey(char *msg, int temp, int humid){
    u8 count = 0;
    u8 *api_key;
    char temp_str[10];
    char humid_str[10];
    char tmp1[60];
    char tmp2[60];
    char tmp3[60];
    char key_final[60];

    api_key = msg;
    sprintf(temp_str, "%d", temp);
    sprintf(humid_str, "%d", humid);

    strcpy(tmp1, api_key);
    strcat(tmp1, temp_str);
    strcpy(tmp2, tmp1);
    strcat(tmp2, "&field2=");
    strcpy(tmp3, tmp2);
    strcat(tmp3, humid_str);
    strcpy(key_final, tmp3);
    strcat(key_final, "\r\n");

    while(count < strlen(key_final))
        {
            USART_SendData(USART2, key_final[count++]);
            while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
            {
            }
        }

}

void send_command(u8 *buf){
    u8 *command, count = 0;
    command = buf;
    while(count < strlen(command))
        {
            USART_SendData(USART2, command[count++]);
            while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
            {
            }
        }
}


void send_test_sms(void)
{
   send_command("AT\r\n");                         /* Check Communication */
   rt_thread_mdelay(500);
   send_command("AT+CMGF=1\r\n");                  // Configuring TEXT mode
   rt_thread_mdelay(200);
   send_command("AT+CMGS=\"+8801719xxxxxx\"\r\n"); // Configuring TEXT mode, set you number
   rt_thread_mdelay(200);
   send_command("RISC-V WCH Test Message.\r\n");   // Configuring TEXT mode
   rt_thread_mdelay(200);
   USART_SendData(USART2, 26);
   rt_kprintf("SMS Sent");
   rt_thread_mdelay(500);
}

void make_test_call(void)
{
   send_command("AT\r\n");               /* Check Communication */
   rt_thread_mdelay(2500);
   send_command("ATD+01719xxxxxx;\r\n"); // Configuring TEXT mode
   rt_thread_mdelay(10000);
   rt_thread_mdelay(10000);
   send_command("ATH\r\n");
   rt_kprintf("\r\nCall Sent");
   rt_thread_mdelay(500);
}


void temperature_high_alart(void)
{
   send_command("AT\r\n");                         /* Check Communication */
   rt_thread_mdelay(500);
   send_command("AT+CMGF=1\r\n");                  // Configuring TEXT mode
   rt_thread_mdelay(200);
   send_command("AT+CMGS=\"+8801719xxxxxx\"\r\n"); // Configuring TEXT mode, set your number
   rt_thread_mdelay(200);
   send_command("Temperature is higher than normal.\r\n");   // Configuring TEXT mode
   rt_thread_mdelay(200);
   USART_SendData(USART2, 26);
   rt_kprintf("SMS Sent");
   rt_thread_mdelay(500);
}

void humidity_high_alart(void)
{
   send_command("AT\r\n");                         /* Check Communication */
   rt_thread_mdelay(500);
   send_command("AT+CMGF=1\r\n");                  // Configuring TEXT mode
   rt_thread_mdelay(200);
   send_command("AT+CMGS=\"+8801719xxxxxx\"\r\n"); // Configuring TEXT mode
   rt_thread_mdelay(200);
   send_command("Humidity is higher than normal.\r\n");   // Configuring TEXT mode
   rt_thread_mdelay(200);
   USART_SendData(USART2, 26);
   rt_kprintf("SMS Sent");
   rt_thread_mdelay(500);
}
