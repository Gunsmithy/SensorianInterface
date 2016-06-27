/**
 * @file PiTools.c
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Extra tools related to the Raspberry Pi for Sensorian projects
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


void pi_get_cpu_serial(char *buf)
{
    FILE *cpuSerialFile;
    char serial[17];
    cpuSerialFile = fopen("/proc/cpuinfo", "r");
    char buffer[64];
    while (fgets(buffer, 64, (FILE*)cpuSerialFile) != NULL)
    {
        // process buffer
        if(strstr(buffer, "Serial") != NULL) {
            memcpy( serial, &buffer[10], 16 );
            serial[16] = '\0';
        }
    }
    fclose(cpuSerialFile);
    strncpy(buf, serial, 17);
}

float pi_get_cpu_temperature()
{
    FILE *cpuTempFile;
    char tempBuff[6];
    cpuTempFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf(cpuTempFile, "%s", tempBuff);
    fclose(cpuTempFile);
    int cpuTempInt = atoi(tempBuff);
    float cpuTempFloat = cpuTempInt/1000.0;
    return cpuTempFloat;
}

void pi_get_interface_ip(char *buf)
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;
    char intstr[6] = "";
    char ipstr[16];

    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family==AF_INET)
        {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            sprintf(intstr,"%s", ifa->ifa_name);
            sprintf(ipstr,"%s", addr);
        }
    }
    strncpy(buf, ipstr, 16);
}

void pi_shutdown_blocking(int delay)
{
    sleep(delay);
    system("sudo shutdown -h now");
}

void pi_shutdown(int delay)
{
    int pid = fork();
    if (pid == 0)
    {
        pi_shutdown_blocking(delay);
    }
}

void pi_reboot_blocking(int delay)
{
    sleep(delay);
    system("sudo shutdown -r now");
}

void pi_reboot(int delay)
{
    int pid = fork();
    if (pid == 0)
    {
        pi_reboot_blocking(delay);
    }
}