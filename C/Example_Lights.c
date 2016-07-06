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
#include "TFT_Printer.h"
#include "CloudTools.h"
#include "SensorsInterface.h"

// User defined variables for IFTTT.com/maker Channel API credentials and how long to wait for requests
char *IFTTT_KEY = "YourIFTTTMakerChannelKey";  // Your API key provided by IFTTT when connecting a Maker channel
char *IFTTT_EVENT = "HueBrightness";  // The name of the event you chose for the recipe to dim a Hue light
long IFTTT_TIMEOUT = 5;  // How long to wait in seconds for commands sent to IFTTT.com before timing out
float TOLERANCE = 2.0;  // How many times the lux levels per Hue brightness setting should be an acceptable range

// Global variables to store the calibrations
float lux_at_max = 0;  // The light level when the Hue is set to a max brightness of 100
float lux_at_min = 0;  // The light level when the Hue is set to a min brightness of 0
float lux_diff = 0;  // The difference in light level between the Hues brightness settings of 0 and 100
float lux_per_bright = 5;  // The difference in light level for each change in brightness level by 1
float desired_lux = 0;  // The desired light level to be matched by the Hue, set when the program is first run
int current_setting = 0;  // The last known value of brightness for the Hue, based on the last command sent

void setup()
{
    TFT_Setup(); // Set up the TFT LCD
    setupSensorian(); // Set up all the sensors on the Sensorian Shield
    desired_lux = getAmbientLight();  // Sets the desired light level to that of when the program ran
    printf("Desired Light: %f\n", desired_lux); //Print the desired light level
}

// Waits until the brightness changes to ensure the request worked given the current light level and desired direction
void wait_for_change(float pre_request_lux, char *direction)
{
    float tolerance = lux_per_bright * TOLERANCE;  // Sensitivity - How much the light should change to be different
    int slept = 0;  // Stores counter to time out the check for change in brightness in case the brightness is the same
    for(;;) {  // Loops until the brightness changes or it times out
        float current_lux = getAmbientLight();  // Get the current light value to check against in a second
        sleep(1);  // Waits for a second before checking brightness again to see if there was a change
        float post_request_lux = getAmbientLight();  // Get the current light value to check for changes
        if ((post_request_lux > pre_request_lux + tolerance || post_request_lux > current_lux + tolerance) &&
                (strcmp(direction,"BOTH") == 0 || strcmp(direction,"UP") == 0))
        {
            printf("Brightness went up\n");
            sleep(2);  // Waits 2 seconds before breaking the loop in case the bulb is still changing brightness
            break;  // Breaks the loop since a brightness change was detected
        }
        else if ((post_request_lux < pre_request_lux - tolerance || post_request_lux < current_lux - tolerance) &&
                (strcmp(direction,"BOTH") == 0 || strcmp(direction,"DOWN") == 0))
        {
            printf("Brightness went down\n");
            sleep(2);  // Waits 2 seconds before breaking the loop in case the bulb is still changing brightness
            break;  // Breaks the loop since a brightness change was detected
        }
        if (slept >= 20)  // Gives the check 20 seconds to catch rare edge cases where the IFTTT request takes a long time
        {
            printf("Timed out, brightness may be the same or similar\n");
            sleep(1);  // Waits a second before breaking the loop in case the bulb is still changing brightness
            break;  // Breaks the loop since no brightness change was detected after 30 seconds
        }
        slept += 1;  // Increases the counter for how long the brightness has been checked
    }
}

// Checks light levels when the Philips Hue light is set to maximum and minimum brightness
void calibrate() {
    printf("Calibrating...\n");
    float pre_request_lux = getAmbientLight();  // Gets the current light level before the request
    cloud_ifttt_trigger_values(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT, "100", "", "");  // Sets the Philips Hue brightness to 100
    wait_for_change(pre_request_lux, "UP");  // Waits until the brightness changes from the request or it times out
    lux_at_max = getAmbientLight();  // Stores the light value when the Philips Hue is set to max

    pre_request_lux = getAmbientLight();  // Gets the current light level before the request
    cloud_ifttt_trigger_values(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT, "0", "", "");  // Sets the Philips Hue brightness to 0
    wait_for_change(pre_request_lux, "DOWN");  // Waits until the brightness changes from the request or it times out
    lux_at_min = getAmbientLight();  // Stores the light value when the Philips Hue is set to min

    lux_diff = lux_at_max - lux_at_min;  // Calculates the difference in light between the max and min setting

    lux_per_bright = lux_diff / 100.0;  // Calculates how much the light level changes for each step of brightness

    printf("Min: %f Max: %f\n", lux_at_min, lux_at_max);
    printf("Difference: %f LuxPerBright: %f\n", lux_diff, lux_per_bright);
}

int main(void)
{
	setup();  // Set up the sensors for use
    calibrate();  // Calibrate the minimum and maximum light levels for the Philips Hue light
    printf("Running...\n");
    for(;;)
    {
        float current_lux = getAmbientLight();  // Get the current light level to see if it is in range
        char sensor_string[24] = {0};  // Create a string in which to store the light value and label
        sprintf(sensor_string, "Light: %f", current_lux);  // Add the light value to the string
        TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, sensor_string, 2);  // Print the current light value to LCD
        float current_difference = current_lux - desired_lux;  // Calculate how different the current and desired levels are
        if (current_difference < 0 && fabs(current_difference) > lux_per_bright)  // If negative difference, check low
        {
            int brightness_notches = floor(fabs(current_difference) / lux_per_bright);  // Calculate how much to change
            current_setting += brightness_notches;  // Increase the current brightness by the calculated amount
            printf("Light too low, increasing brightness by %d to %d\n", brightness_notches, current_setting);
            if (current_setting > 100)  // If the setting attempts to cross the maximum of 100, set it to 100
            {
                printf("Can't go brighter than 100, capping it\n");
                current_setting = 100;  // Cap the setting at the maximum of 100
            }
            char current_setting_str[4] = {0};  // Create a string in which to store the current brightness setting
            sprintf(current_setting_str, "%d", current_setting);  // Format the current brightness int into the string
            cloud_ifttt_trigger_values(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT, current_setting_str, "", "");  // Change the brightness
            wait_for_change(current_lux, "UP");  // Waits until the brightness changes/times out
        }
        else if (current_difference > 0 && current_difference > lux_per_bright)  // If positive difference, check high
        {
            int brightness_notches = floor(current_difference / lux_per_bright);  // Calculate how much to change
            current_setting -= brightness_notches;  // Decrease the current brightness by the calculated amount
            printf("Light too high, decreasing brightness by %d to %d\n", brightness_notches, current_setting);
            if (current_setting < 0)  // If the setting attempts to cross the minimum of 0, set it to 0
            {
                printf("Can't go darker than 0, capping it\n");
                current_setting = 0;  // Cap the setting at the minimum of 0
            }
            char current_setting_str[4] = {0};  // Create a string in which to store the current brightness setting
            sprintf(current_setting_str, "%d", current_setting);  // Format the current brightness int into the string
            cloud_ifttt_trigger_values(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT, current_setting_str, "", "");  // Change the brightness
            wait_for_change(current_lux, "DOWN");  // Waits until the brightness changes/times out
        }
        sleep(3);  // Wait for 3 seconds between each check for brightness
    }
}