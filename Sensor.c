#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <curl/curl.h>
#include <time.h>
#include "ifttt.h"

#define BUFSIZE 256

char *devaddr = "/sys/bus/w1/devices/28-02131345a7aa/w1_slave";

int main(void)
{
    char buf[BUFSIZE];
    char tempBuf[5];
    char line[256];
    char lowTemp[5];
    char highTemp[5];
    char curr[100];
    char min[100];
    char max[100];
    int fd;
    int ret;
    int i, j;
    int tRead;
    tRead = 0;
    float initialTemp, minTemp, maxTemp;
    minTemp = 200;
    float temp; /* temperature in degrees celsius */

    while (1)
    {
        fd = open(devaddr, O_RDONLY);
        if (fd == -1)
        {
            perror("Failed to open /sys/bus/w1/devices/28-02131345a7aa/w1_slave!");
            exit(1);
        }
        tRead++;
        while (1)
        {
            ret = read(fd, buf, BUFSIZE);
            if (ret == 0)
            {
                break;
            }
            if (ret == -1)
            {
                perror("read error");
                close(fd);
                exit(1);
            }
        }

        /*
        while ((numberRead = ret) > 0)
        {
            strncpy(tempBuf, strstr(buf, "t=") + 2, 5);
            temp = strtof(tempBuf, NULL);
            printf("Temperature: %.3f C \n", temp / 1000);
        }
        close(fd);
        */

        for (i = 0; i < sizeof(buf); i++)
        {
            if(buf[i] == 't')
            {
                for (j = 0; j < sizeof(tempBuf); j++)
                {
                    tempBuf[j] = buf[i + 2 + j];
                }
            }
        }
        
        /*printf("%d\n", tRead);*/
        temp = (float) atoi (tempBuf) / 1000;
        sprintf(curr, "Current Temp: %.3f", tempBuf);
        sprintf(min, "Min Temp: %.3f", minTemp);
        sprintf(max, "Max Temp: %.3f", maxTemp);

        if (tRead == 1)
        {   
            initialTemp = temp;
            /*ifttt("https://maker.ifttt.com/trigger/temp_updated/with/key/bl_a9Xsn0n6sba11As-ihy", curr, min, max);*/
        }
        if (temp < minTemp)
        {
            minTemp = temp;
        }
        if(temp > maxTemp)
        {
            maxTemp = temp;
        }
        if (fabs(maxTemp - initialTemp) || fabs(minTemp - initialTemp) >= 1)
        {
            ifttt("https://maker.ifttt.com/trigger/temp_updated/with/key/bl_a9Xsn0n6sba11As-ihy", curr, min, max); /* send to email */
            ifttt("https://maker.ifttt.com/trigger/temp_updated2/with/key/bl_a9Xsn0n6sba11As-ihy", curr, min, max); /* send to mobile */
            /*printf("changed");*/
        }

        printf("%.3f C (MAX)\n", maxTemp);
        printf("%.3f C (MIN)\n", minTemp);
        printf("%.3f C (CURRENT)\n", temp);
        close(fd);
    }
    return 0;
}   