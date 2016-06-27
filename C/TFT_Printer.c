/**
 * @file TFT_Printer.c
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Prints a given message to the TFT LCD wrapped to fit if possible
 */
 
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "SPI.h"
#include "TFT.h"
#include "TFT_Printer.h"

orientation_t lastMode = LANDSCAPE_INV; //The last orientation given, defaults to Landscape Inverted
int lastColor = WHITE; //The last text color given, defaults to White
int lastBackground = BLACK; //The last background color given, defaults to Black
int lastSize = 1; //The last font size given, defaults to 1

/**
 * @brief Wraps a colored and size string and prints it to the screen as ASCII characters
 * @param maxChars How many characters to fit on each line, no word length should exceed this
 * @param maxLines How many lines to fit on the display, the message will be cut off if it can't fit
 * @param color Color of text above background.
 * @param background Color of text background.
 * @param message Pointer to string array of chars
 * @param size Size of font
 * @return none
 */
void TFT_Printer_PrintWrap(const int maxChars, const int maxLines, int color, int background, char * message, int size)
{
    int i; //The position in the whole message, iterates through as the message is parsed
	int j; //The position in a substring of which character to print
	int k; //The position counting backwards through a maximum length line until a blank is found
	int newLoc; //Counting positions in the message until a line reaches the maximum length
	int lastLoc; //The last position in the total message where a blank was found and was wrapped
	int x, y; //Co-ordinates on the LCD to print the next character
	x = 0; //X coordinate on the screen to print the next character
	y = 0; //Y coordinate on the screen to print the next character
    lastLoc = 0; //Start looking for max length lines starting from the 0 position of the message
    newLoc = 0; //No substrings have been found yet in the message so the last position was 0
	int lineCount = 0; //Counts how many lines have been printed to the display

    for (i = 0; message[i] != '\0'; ++i, ++newLoc) //Iterate through the message until the end
	{
		if (lineCount < maxLines) //Check that there is still room on the display for another line
		{
			if (newLoc >= maxChars) //If the current spot is the maximum size of a line
			{
				for (k = i; k > 0; --k) //Iterate backwords through the message from this point
				{
					if (k - lastLoc <= maxChars && message[k] == ' ') //Until a blank space is found
					{
						char subbuff[k - lastLoc + 1]; //Create a substring for the size until the blank
						memcpy( subbuff, &message[lastLoc], k - lastLoc); //Copy the message until the blank
						x = 0; //Reset printing to the left-most side of the screen
						j = 0; //Reset the iterator to 0
						while (j < k - lastLoc) //Iterate through the new substring
						{
							TFT_ASCII(x, y, color, background, subbuff[j], size); //Print a character
							x += FONT_LENGTH * size; //Space out the next letter by the size of a letter
							j++; //Increase the iterator
						}
						y += FONT_HEIGHT * size; //Space out the next line by the height of a letter
						lastLoc = k + 1; //Save the point in the message where the last substring ended
						lineCount++; //Increase the line counter since a new line was printed
						break; //Break the backwards for loop to start finding the next substring
					}
				}
				newLoc = i - lastLoc; //Reset the line length counter to the end of the last substring
			}
		}
    }
	if (lineCount < maxLines) //Handles the last line if there is room on the screen
	{
		int remaining = strlen(message) - lastLoc; //How many characters are left in the string
		char subbuff2[remaining + 1]; //Create a substring for the remaining characters
		memcpy( subbuff2, &message[lastLoc], remaining); //Copy the remaining character to the new substring
		x = 0; //Reset printing to the left-most side of the screen
		j = 0; //Reset the iterator to 0
		while (j < remaining) //Iterate through the remaining characters
		{
			TFT_ASCII(x, y, color, background, subbuff2[j], size); //Print a character to the screen
			x += FONT_LENGTH * size; //Space out the next letter by the size of a letter
			j++; //Increase the iterator
		}
	}
}

/**
 * @brief Print a sized and colored string wrapped on the screen at the specified orientation
 * @param mode The orientation at which to print the string
 * @param color Color of text above background.
 * @param background Color of text background.
 * @param message Pointer to string array of chars
 * @param size Size of font
 * @return none
 */
void TFT_Printer_PrintAll(orientation_t mode, int color, int background, char * message, int size)
{
	int maxChars; //Integer to store how many characters to fit in a line on the display
	int maxLines; //Integer to store how many lines to fit on the display
	lastMode = mode; //Set the last value of orientation mode to the new value
	lastColor = color; //Set the last value of color to the new value
	lastBackground = background; //Set the last value of background to the new value
	lastSize = size; //Set the last value of size to the new value
	if (mode == 0 || mode == 2 || mode == 3 || mode == 5) //If new orientation is a Portrait type
	{
		maxChars = floor(LCD_HEIGHT/(FONT_LENGTH*size)); //Calculate # of characters that fit on a line
		maxLines = floor(LCD_LENGTH/(FONT_HEIGHT*size));
		TFT_SetRotation(mode); //Set the LCD rotation to the given portrait-type orientation
		TFT_Background(background); //Clears the screen by setting the background to the given color
	}
	else if (mode == 6 || mode == 4 || mode == 1 || mode == 7) //If new orientation is a Landscape type
	{
		maxChars = floor(LCD_LENGTH/(FONT_LENGTH*size)); //Calculate # of characters that fit on a line
		maxLines = floor(LCD_HEIGHT/(FONT_HEIGHT*size));
		TFT_SetRotation(PORTRAIT); //Set to portrait first since set background doesn't work otherwise
		TFT_Background(background); //Clears the screen by setting the background to the given color
		TFT_SetRotation(mode); //Set back to the given landscape-type orientation
	}
	TFT_Printer_PrintWrap(maxChars, maxLines, color, background, message, size); //Wrap message on LCD
}

/**
 * @brief Print a string wrapped on the screen using default or previous other parameters
 * @param message Pointer to string array of chars
 * @return none
 */
void TFT_Printer_Print(char * message)
{
	TFT_Printer_PrintAll(lastMode, lastColor, lastBackground, message, lastSize); //Print message to LCD
}

/**
 * @brief Print a colored string wrapped on the screen using default or previous other parameters
 * @param color Color of text above background.
 * @param background Color of text background.
 * @param message Pointer to string array of chars
 * @return none
 */
void TFT_Printer_PrintColor(int color, int background, char * message)
{
	TFT_Printer_PrintAll(lastMode, color, background, message, lastSize); //Print message to LCD
	lastColor = color; //Set the last value of color to the new value
	lastBackground = background; //Set the last value of background to the new value
}

/**
 * @brief Print a sized string wrapped on the screen using default or previous other parameters
 * @param message Pointer to string array of chars
 * @param size Size of font
 * @return none
 */
void TFT_Printer_PrintSize(char * message, int size)
{
	TFT_Printer_PrintAll(lastMode, lastColor, lastBackground, message, size); //Print message to LCD
	lastSize = size; //Set the last value of size to the new value
}

/**
 * @brief Print a sized and colored string wrapped on the screen using default or previous orientation
 * @param color Color of text above background.
 * @param background Color of text background.
 * @param message Pointer to string array of chars
 * @param size Size of font
 * @return none
 */
void TFT_Printer_PrintBoth(int color, int background, char * message, int size)
{
	TFT_Printer_PrintAll(lastMode, color, background, message, size); //Print message to LCD
	lastColor = color; //Set the last value of color to the new value
	lastBackground = background; //Set the last value of background to the new value
	lastSize = size; //Set the last value of size to the new value
}