/**
 * @file TFT_Printer.h
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Prints a given message to the TFT LCD wrapped to fit if possible
 */

#ifndef __TFT_Printer_H__
#define __TFT_Printer_H__

#include "TFT.h"

#define LCD_LENGTH 160
#define LCD_HEIGHT 128
#define FONT_LENGTH 6
#define FONT_HEIGHT 8

void	TFT_Printer_Print(char * message);
void	TFT_Printer_PrintColor(int color, int background, char * message);
void	TFT_Printer_PrintSize(char * message, int size);
void	TFT_Printer_PrintBoth(int color, int background, char * message, int size);
void 	TFT_Printer_PrintAll(orientation_t mode, int color, int background, char * message, int size);

#endif