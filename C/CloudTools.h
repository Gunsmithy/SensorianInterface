/**
 * @file CloudTools.h
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Extra tools related to Cloud integration for Sensorian projects
 */

#ifndef C_CLOUDTOOLS_H
#define C_CLOUDTOOLS_H

void cloud_get_public_ip(char *buf);
void cloud_ifttt_trigger(char *key, char *event, long timeout);
void cloud_ifttt_trigger_values(char *key, char *event, long timeout, char *value1, char *value2, char *value3);

#endif //C_CLOUDTOOLS_H
