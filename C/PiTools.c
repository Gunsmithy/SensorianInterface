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

/**
 * @brief Store the Raspberry Pi's CPU serial number in a passed char array buffer
 * @param buf A string array of chars buffer in which to store the CPU serial, should be size 17
 * @return none
 */
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

/**
 * @brief Get the Raspberry Pi's CPU temperature from the system file and return as a float
 * @return float of the CPU temperature
 */
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

/**
 * @brief Get the local IP address of an active networking interface on the Raspberry Pi
 * @param buf A string array of chars buffer in which to store the local IP, should be size 16
 * @return none
 */
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

/**
 * @brief Sends a system command to shut down the Raspberry Pi after a given delay, blocks execution
 * @param delay Integer seconds of how long to wait before sending the shut down command
 * @return none
 */
void pi_shutdown_blocking(int delay)
{
    sleep(delay);
    system("sudo shutdown -h now");
}

/**
 * @brief Forks the program to shut down the Raspberry Pi after a given delay, doesn't block execution
 * @param delay Integer seconds of how long to wait before sending the shut down command
 * @return none
 */
void pi_shutdown(int delay)
{
    int pid = fork();
    if (pid == 0)
    {
        pi_shutdown_blocking(delay);
    }
}

/**
 * @brief Sends a system command to reboot the Raspberry Pi after a given delay, blocks execution
 * @param delay Integer seconds of how long to wait before sending the reboot command
 * @return none
 */
void pi_reboot_blocking(int delay)
{
    sleep(delay);
    system("sudo shutdown -r now");
}

/**
 * @brief Forks the program to reboot the Raspberry Pi after a given delay, doesn't block execution
 * @param delay Integer seconds of how long to wait before sending the reboot command
 * @return none
 */
void pi_reboot(int delay)
{
    int pid = fork();
    if (pid == 0)
    {
        pi_reboot_blocking(delay);
    }
}