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
#include "CloudTools.h"
#include "SensorsInterface.h"

// User defined variables for IFTTT.com/maker Channel API credentials and how long to wait for requests
char *IFTTT_KEY = "YourIFTTTMakerChannelKey";  // Your API key provided by IFTTT when connecting a Maker channel
char *IFTTT_EVENT = "DoorOpened";  // The name of the event you chose for the recipe to dim a Hue light
long IFTTT_TIMEOUT = 5;  // How long to wait in seconds for commands sent to IFTTT.com before timing out
int TOLERANCE = 1000;  // How much the magnetometer values need to change to signal
int SECONDS = 5;  // How long to give the user to prepare for the calibration steps

// Global variables to store the calibrations
int xClosed = 0;
int yClosed = 0;
int zClosed = 0;


// Sets up the Sensorian sensors for use and prints the current magnetic forces to the screen to test
void setup()
{
    printf("Setting up...\n");
    SPI_Initialize(); // Prepare the SPI bus for use by the LCD
    TFT_Initialize(); // Prepare the TFT LCD for use
    TFT_Background(BLACK); // Clear the screen to Black

    setupSensorian(); // Set up all the sensors on the Sensorian Shield
    sleep(2); // Wait 2 seconds or some sensors won't be ready
    pollFXOS(); //Poll the sensor for the current accelerometer and magnetometer values
    char magnet_string[64];
    sprintf(magnet_string, "Magnetometer X: %d, Y: %d, Z: %d\n", getMagX(), getMagY(), getMagZ());
    printf(magnet_string);
    TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, magnet_string, 2);
}


// Calibrates the door for its closed state
void calibrate() {
    int seconds = SECONDS;  // The number of seconds to give the user to prepare the device in the door closed position
    while (seconds > 0)  // While the user still has time to prepare
    {
        char calibrating[48];
        sprintf(calibrating, "Calibrating door closed state in %d\n", seconds);
        printf(calibrating);
        TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, calibrating, 2);
        sleep(1);  // Wait for a second
        seconds -= 1;  // Reduce the time remaining for preparation by 1 second
    }
    pollFXOS();  // Poll the sensor for the current accelerometer and magnetometer values
    xClosed = getMagX();  // Read the magnetic forces in the x direction and store it to the global variables
    yClosed = getMagY();  // Read the magnetic forces in the y direction and store it to the global variables
    zClosed = getMagZ();  // Read the magnetic forces in the z direction and store it to the global variables
    char calibrated[96];
    sprintf(calibrated, "Calibrated door closed state to %d within %d, %d, %d\n", TOLERANCE, xClosed, yClosed, zClosed);
    printf(calibrated);
    TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, calibrated, 2);
    sleep(3);  // Waits 3 seconds so the user can read the screen
}


// Contains the main looping execution of the program
int main(void)
{
	setup();  // Set up the sensors for use
    calibrate();  // Set the approximate values for the door being closed
    printf("Running...\n");
    int door_was_open = 0;  // A boolean to store whether or not the door was already open if it was detected open again
    for(;;)
    {
        pollFXOS(); // Poll the sensor for the current accelerometer and magnetometer values
        int x = getMagX();  // Read the magnetic forces in the x direction and store it locally
        int y = getMagY();  // Read the magnetic forces in the y direction and store it locally
        int z = getMagZ();  // Read the magnetic forces in the z direction and store it locally
        char magnet_string[64];
        sprintf(magnet_string, "Magnetometer X: %d, Y: %d, Z: %d", x, y, z);
        // If any of the sensor values differ from the calibrated closed amount by more than the tolerance, it is open
        if ((xClosed - TOLERANCE > x || x > xClosed + TOLERANCE || yClosed - TOLERANCE > y || y > yClosed + TOLERANCE ||
                zClosed - TOLERANCE > z || z > zClosed + TOLERANCE) && !(x == 0 && y == 0 && z == 0))
        {
            if (door_was_open == 0)  // If the door was not already open
            {
                strcat(magnet_string, " Door opened!\n");
                printf(magnet_string);
                cloud_ifttt_trigger(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT);
                door_was_open = 1;  // Store that the door is currently open
            }
        }
        else
        {
            door_was_open = 0;  // Reset the door's last known state to closed
        }
        TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, magnet_string, 2);
        sleep(1);  // Wait for 1 second between each check for magnetic forces
    }
}