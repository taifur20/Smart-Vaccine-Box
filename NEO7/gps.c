/*
 * gps.c
 *
 *  Created on: Aug 6, 2022
 *  Author: Md. Khairul Alam
 */

//USART1_Tx(PA9)、USART1_Rx(PA10)
//USART2_Tx(PA2)、USART2_Rx(PA3)
//USART3_Tx(PB10)、USART3_Rx(PB11)


#include "gps.h"
#include <rtthread.h>
#include <rthw.h>


#define RX_BUFFER_SIZE      1024
float lon_value = 90.14;
float lat_value = 23.18;
u8 GPS_BUF[RX_BUFFER_SIZE];
vu16 GPSMSG_FLAG = 0;
vu16 GPSMSG_INDEX = 0;
vu8 GPS_DEBUG = 0;


u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{
    u8 *p=buf;
    while(cx)
    {
        if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
        if(*buf==',')cx--;
        buf++;
    }
    return buf-p;
}

u32 NMEA_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}

int NMEA_Str2num(u8 *buf,u8*dx)
{
    u8 *p=buf;
    u32 ires=0,fres=0;
    u8 ilen=0,flen=0,i;
    u8 mask=0;
    int res;
    while(1)
    {
        if(*p=='-'){mask|=0X02;p++;}
        if(*p==','||(*p=='*'))break;
        if(*p=='.'){mask|=0X01;p++;}
        else if(*p>'9'||(*p<'0'))
        {
            ilen=0;
            flen=0;
            break;
        }
        if(mask&0X01)flen++;
        else ilen++;
        p++;
    }
    if(mask&0X02)buf++;
    for(i=0;i<ilen;i++)
    {
        ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen>5)flen=5;
    *dx=flen;
    for(i=0;i<flen;i++)
    {
        fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
    }
    res=ires*NMEA_Pow(10,flen)+fres;
    if(mask&0X02)res=-res;
    //rt_kprintf("%d", res);
    return res;
}

//GPRMC
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
    u8 *p1,dx;
    u8 posx;
    u32 temp;
    float rs;
    p1=(u8*)strstr((const char *)buf,"GPRMC");
    posx=NMEA_Comma_Pos(p1,1);
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);
        gpsx->utc.hour=temp/10000;
        gpsx->utc.min=(temp/100)%100;
        gpsx->utc.sec=temp%100;
        rt_kprintf("hour: %d,  min: %d, sec: %d \n\r", gpsx->utc.hour, gpsx->utc.min, gpsx->utc.sec);
    }
    posx=NMEA_Comma_Pos(p1,3);
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->latitude=temp/NMEA_Pow(10,dx+2);
        rs=temp%NMEA_Pow(10,dx+2);
        gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
        rt_kprintf("latitude: %d \n\r", gpsx->latitude);
        lat_value = gpsx->latitude/100000;
    }
    posx=NMEA_Comma_Pos(p1,4);
    if(posx!=0XFF)gpsx->nshemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,5);
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->longitude=temp/NMEA_Pow(10,dx+2);
        rs=temp%NMEA_Pow(10,dx+2);
        gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
        rt_kprintf("longitude: %d \n\r", gpsx->longitude);
        lon_value = gpsx->longitude/100000;
    }
    posx=NMEA_Comma_Pos(p1,6);
    if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);
    posx=NMEA_Comma_Pos(p1,9);
    if(posx!=0XFF)
    {
        temp=NMEA_Str2num(p1+posx,&dx);
        gpsx->utc.date=temp/10000;
        gpsx->utc.month=(temp/100)%100;
        gpsx->utc.year=2000+temp%100;
        rt_kprintf("day: %d,  month: %d, year: %d \n\r", gpsx->utc.date, gpsx->utc.month, gpsx->utc.year);
    }

}

//GPGSV
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
    u8 *p,*p1,dx;
    u8 len,i,j,slx=0;
    u8 posx;
    p=buf;
    p1=(u8*)strstr((const char *)p,"$GPGSV");
    len=p1[7]-'0';
    posx=NMEA_Comma_Pos(p1,3);
    if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<len;i++)
    {
        p1=(u8*)strstr((const char *)p,"$GPGSV");
        for(j=0;j<4;j++)
        {
            posx=NMEA_Comma_Pos(p1,4+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);
            else break;
            posx=NMEA_Comma_Pos(p1,5+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);
            else break;
            posx=NMEA_Comma_Pos(p1,6+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);
            else break;
            posx=NMEA_Comma_Pos(p1,7+j*4);
            if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);
            else break;
            slx++;
        }
        p=p1+1;
    }
}

//GPGGA
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GPGGA");
    posx=NMEA_Comma_Pos(p1,6);
    if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,7);
    if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,9);
    if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);
    //rt_kprintf("sta: %d,  num: %d, altitide: %d ", gpsx->gpssta, gpsx->posslnum, gpsx->altitude);
}

//GPGSA
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
    u8 *p1,dx;
    u8 posx;
    u8 i;
    p1=(u8*)strstr((const char *)buf,"$GPGSA");
    posx=NMEA_Comma_Pos(p1,2);
    if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);
    for(i=0;i<12;i++)
    {
        posx=NMEA_Comma_Pos(p1,3+i);
        if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
        else break;
    }
    posx=NMEA_Comma_Pos(p1,15);
    if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,16);
    if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
    posx=NMEA_Comma_Pos(p1,17);
    if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);
}

//GPVTG
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
    u8 *p1,dx;
    u8 posx;
    p1=(u8*)strstr((const char *)buf,"$GPVTG");
    posx=NMEA_Comma_Pos(p1,7);
    if(posx!=0XFF)
    {
        gpsx->speed=NMEA_Str2num(p1+posx,&dx);
        if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);
    }
}

void Send_NMEA_GPRMC(nmea_msg *gpsx)
{
    rt_kprintf("year:%d ", gpsx->utc.year);
    rt_kprintf("month:%d ", gpsx->utc.month);
    rt_kprintf("date:%d ", gpsx->utc.date);
    rt_kprintf("hour:%d ", (gpsx->utc.hour+8)%24);
    rt_kprintf("min:%d ", gpsx->utc.min);
    rt_kprintf("sec:%d ", gpsx->utc.sec);

    rt_kprintf("%c latit:", gpsx->nshemi);
    rt_kprintf("%f ", (float)gpsx->latitude/100000);

    rt_kprintf("%c longi:", gpsx->ewhemi);
    rt_kprintf("%f\r\n", (float)gpsx->longitude/100000);
}

void Send_NMEA_GPGSV(nmea_msg *gpsx)
{
    int i, j = gpsx->svnum;
    rt_kprintf("satellites in view£º%d \r\n", gpsx->svnum);
    rt_kprintf("satellite number  elevation in degrees    azimuth in degrees to true   SNR in dB\r\n");
    for (i = 0; i < j; i++)
    {
        rt_kprintf("%d\t\t\t",gpsx->slmsg[i].num);
        rt_kprintf("%d\t\t\t",gpsx->slmsg[i].eledeg);
        rt_kprintf("%d\t\t\t",gpsx->slmsg[i].azideg);
        rt_kprintf("%d",gpsx->slmsg[i].sn);
        rt_kprintf("\r\n");
    }
}

void Send_NMEA_GPGGA(nmea_msg *gpsx)
{
    rt_kprintf("GPS quality indicator: %d    Number of satellites in view: %d    high:%.3f\r\n", \
        gpsx->gpssta, gpsx->posslnum, (float)gpsx->altitude/10);
}

void Send_NMEA_GPGSA(nmea_msg *gpsx)
{
    int i;
    rt_kprintf("mode: %d\tPDOP: %.3f\tHDOP:%.3f\tVDOP:%.3f", \
        gpsx->fixmode, (float)gpsx->pdop/100, (float)gpsx->hdop/100, (float)gpsx->vdop/100);
    rt_kprintf("\r\nNO:");
    for (i = 0 ; i < gpsx->posslnum; i++)
    {
        rt_kprintf("%d ",gpsx->possl[i]);
    }
    rt_kprintf("\r\n");
}

void Send_NMEA_GPVTG(nmea_msg *gpsx)
{
    rt_kprintf("speed: %f\r\n\r\n", (float)gpsx->speed/1000);
}

void GPSMSG_Analysis(nmea_msg *gpsx, u8 *buf)
{
        NMEA_GPRMC_Analysis(gpsx,buf);
        NMEA_GPGSV_Analysis(gpsx,buf);
        NMEA_GPGGA_Analysis(gpsx,buf);
        NMEA_GPGSA_Analysis(gpsx,buf);
        NMEA_GPVTG_Analysis(gpsx,buf);
}

void Send_NMEA_MSG(nmea_msg *gpsx)
{
        Send_NMEA_GPRMC(gpsx);
        Send_NMEA_GPGSV(gpsx);
        Send_NMEA_GPGGA(gpsx);
        Send_NMEA_GPGSA(gpsx);
        Send_NMEA_GPVTG(gpsx);
}



/*      //sample gps message
 *      u8 buffer[] = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40";
        NMEA_GPGGA_Analysis(&gpsx, (u8*)buffer);
        u8 buffer1[] = "$GPRMC,235316.000,A,4003.9040,N,10512.5792,W,0.09,144.75,141112,,*19";
        NMEA_GPRMC_Analysis(&gpsx, (u8*)buffer1);
        u8 buffer2[] = "$GPGSA,A,3,22,18,21,06,03,09,24,15,,,,,2.5,1.6,1.9*3E";
        NMEA_GPGSA_Analysis(&gpsx, (u8*)buffer2);
        u8 buffer3[] = "$GPGSV,2,1,08,02,74,042,45,04,18,190,36,07,67,279,42,12,29,323,36*77";
        NMEA_GPGSV_Analysis(&gpsx, (u8*)buffer3);
        u8 buffer4[] = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K";
        NMEA_GPVTG_Analysis(&gpsx, (u8*)buffer4);
        Send_NMEA_MSG(&gpsx);
*/


/*
        usart_configuration();
        timer_configuration(3999, 7199);

        if(GPSMSG_FLAG)
                {
                    len=GPSMSG_INDEX;
                    for(i=0;i<len;i++)
                        {
                        GPS_SEND_BUF[i]=GPS_BUF[i];
                        }
                    GPSMSG_INDEX = 0;
                    GPSMSG_FLAG=0;
                    GPS_SEND_BUF[i]=0;
                    GPSMSG_Analysis(&gpsx,(u8*)GPS_SEND_BUF);
                    if (0 == GPS_DEBUG);
                        printf("%s",GPS_SEND_BUF);
                    if (1 == GPS_DEBUG)
                        Send_NMEA_MSG(&gpsx);
                }

*/
