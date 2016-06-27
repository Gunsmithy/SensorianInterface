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

#endif //C_CLOUDTOOLS_H
