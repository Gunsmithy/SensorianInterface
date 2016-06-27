/**
 * @file PiTools.h
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Extra tools related to the Raspberry Pi for Sensorian projects
 */

#ifndef __PiTools_H__
#define __PiTools_H__

void pi_get_cpu_serial(char *buf);
float pi_get_cpu_temperature();
void pi_get_interface_ip(char *buf);
void pi_shutdown_blocking(int delay);
void pi_shutdown(int delay);
void pi_reboot_blocking(int delay);
void pi_reboot(int delay);

#endif //C_PITOOLS_H
