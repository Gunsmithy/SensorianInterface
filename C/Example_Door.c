/**
 * @file Example_Lights.c
 * @author Dylan Kauling
 * @date 29 June 2016
 * @brief Example code to adjust the brightness of a Philips Hue light to maintain a particular light level
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "SPI.h"
#include "TFT.h"
#include "TFT_Printer.h"
#include "PiTools.h"
#include "CloudTools.h"
#include "SensorsInterface.h"

// User defined variables for IFTTT.com/maker Channel API credentials and how long to wait for requests
char *IFTTT_KEY = "YourIFTTTMakerChannelKey";  // Your API key provided by IFTTT when connecting a Maker channel
char *IFTTT_EVENT = "HueBrightness";  // The name of the event you chose for the recipe to dim a Hue light
long IFTTT_TIMEOUT = 5;  // How long to wait in seconds for commands sent to IFTTT.com before timing out

// Global variables to store the calibrations


void setup()
{
    printf("Setting up...\n");
    SPI_Initialize(); // Prepare the SPI bus for use by the LCD
    TFT_Initialize(); // Prepare the TFT LCD for use
    TFT_Background(BLACK); // Clear the screen to Black

    setupSensorian(); // Set up all the sensors on the Sensorian Shield
    sleep(2); // Wait 2 seconds or some sensors won't be ready
    pollFXOS(); //Poll the sensor for the current accelerometer and magnetometer values
    printf("Magnetometer X: %d, Y: %d, Z: %d\n", getMagX(), getMagY(), getMagZ()); //Print last polled magnet values
}


int main(void)
{
	setup();  // Set up the sensors for use
    printf("Main program commencing...\n");
    for(;;) {
        pollFXOS(); //Poll the sensor for the current accelerometer and magnetometer values
        printf("Magnetometer X: %d, Y: %d, Z: %d\n", getMagX(), getMagY(), getMagZ()); //Print last polled magnet values
        sleep(1);  // Wait for 1 second between each check for magnetic forces
    }
}