/**
 * @file Test.c
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Test code to demonstrate functionality of SensorsInterface, TFT_Printer, PiTools and CloudTools
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "SPI.h"
#include "TFT.h"
#include "TFT_Printer.h"
#include "PiTools.h"
#include "CloudTools.h"
#include "SensorsInterface.h"

int main(void)
{
	SPI_Initialize(); //Prepare the SPI bus for use by the LCD
	TFT_Initialize(); //Prepare the TFT LCD for use
	TFT_Background(BLACK); //Clear the screen to Black

    setupSensorian(); //Set up all the sensors on the Sensorian Shield
    sleep(2); //Wait 2 seconds or some sensors won't be ready

	orange_led_on();  //Turn on the Sensorian Orange LED
	
    printf("Light: %f\n", getAmbientLight()); //Print the current light level
    pollMPL(); //Poll the sensor for the current temperature, altitude and pressure
    printf("Temperature: %d\n", getTemperature()); //Print the last polled temperature
    printf("Altitude: %d\n", getAltitude()); //Print the last polled altitude
    printf("Pressure: %d\n", getBarometricPressure()); //Print the last polled pressure
    pollFXOS(); //Poll the sensor for the current accelerometer and magnetometer values
    printf("Magnetometer X: %d, Y: %d, Z: %d\n", getMagX(), getMagY(), getMagZ()); //Print last polled magnet values
    printf("Accelerometer X: %d, Y: %d, Z: %d\n", getAccelX(), getAccelY(), getAccelZ()); //Print last accel values
    poll_rtcc(); //Poll the real time clock to get the current date and time
    printf("Date (DD/MM/YY): %d, %d, %d\n", get_rtcc_date(), get_rtcc_month(), get_rtcc_year()); //Print the date
    printf("Time: %d:%d:%d\n", get_rtcc_hour(), get_rtcc_minute(), get_rtcc_second()); //Print the last polled time

	//Create a sample string/char array to print to the screen
	char s[] = "This is a long string that will wrap with the display to fit if possible.";
	
	printf("Print Test\n");
	TFT_Printer_Print(s); //Print the string to the screen with the default settings
	sleep(1);
	
	printf("Print Color Test\n");
	TFT_Printer_PrintColor(RED, GREEN, s); //Print the string to the screen, changing the color
	sleep(1);
	
	printf("Print Size Test\n");
	TFT_Printer_PrintSize(s, 2); //Print the string to the screen, changing the size
	sleep(1);
	
	printf("Print Both Test\n");
	TFT_Printer_PrintBoth(YELLOW, BLUE, s, 3); //Print the string to the screen changing the color and size
	sleep(1);

	printf("Print All Test\n");
	TFT_Printer_PrintAll(PORTRAIT, WHITE, BLACK, s, 2); //Print the string to the screen, changing color,
	sleep(1);											//size and the orientation
	
	printf("Print Test 2\n");
	TFT_Printer_Print(s); //Print the string to the screen again, using the last settings applied
	sleep(1);

    char ip_address[16] = {0}; //Creates a buffer in which to store the interface IP
    pi_get_interface_ip(ip_address); //Passes the buffer to the function which gets the interface IP
    char public_ip[16] = {0}; //Creates a buffer in which to store the public IP
    cloud_get_public_ip(public_ip); //Passes the buffer to the function which gets the public IP
    char ips_printed[57]; //Create a char array in which to store the string to be printed to the LCD
    strcpy(ips_printed, "Interface IP: "); //Start the new string off with the label Interface IP
    strcat(ips_printed, ip_address); //Add the string containing the temperature to the end of the string
    strcat(ips_printed, " Public IP: "); //Add the label Public IP to the end of the string
    strcat(ips_printed, public_ip); //Add the string containing the public ip to the end of the string
    TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, ips_printed, 2); //Prints both IP addresses to the LCD
    sleep(1);

    char *key = "YourIFTTTMakerChannelKey"; //Create a string for your IFTTT Maker Channel Key
    char *event = "YourRecipeEventName"; //Create a string for your IFTTT Maker Channel Event
    long timeout = 5; //Create a long for the number of seconds to wait before timing out the request
    cloud_ifttt_trigger(key, event, timeout); //Send a HTTP request to trigger an IFTTT Maker Channel Recipe
    //Send a HTTP request to trigger an IFTTT Maker Channel Recipe along with 3 values in a JSON
    cloud_ifttt_trigger_values(key, event, timeout, "Value1", "2", "3.0");

    float cpu_temp = pi_get_cpu_temperature(); //Store the float of the CPU temperature returned by the function
    char cpu_temp_str[10]; //Create a char array in which to store the string equivalent of the temperature
    sprintf(cpu_temp_str, "%f", cpu_temp); //Convert the CPU temperature float to a char array and store it
    char cpu_temp_printed[20]; //Create a char array in which to store the string to be printed to the LCD
    strcpy(cpu_temp_printed, "CPU TEMP: "); //Start the new string off with the label CPU TEMP
    strcat(cpu_temp_printed, cpu_temp_str); //Add the string containing the temperature to the end of the string
    TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, cpu_temp_printed, 2); //Prints CPU Temperature to the LCD
    sleep(1);

    char cpu_serial[17] = {0}; //Creates a buffer in which to store the CPU serial
    pi_get_cpu_serial(cpu_serial); //Passes the buffer to the function which gets the CPU Serial
    char cpu_serial_printed[29]; //Create a char array in which to store the string to be printed to the LCD
    strcpy(cpu_serial_printed, "CPU SERIAL: "); //Start the new string off with the label CPU SERIAL
    strcat(cpu_serial_printed, cpu_serial); //Add the string containing the serial to the end of the string
    TFT_Printer_PrintAll(LANDSCAPE_INV, WHITE, BLACK, cpu_serial_printed, 1); //Prints CPU serial to the LCD
    sleep(1);

	orange_led_off();  //Turn off the Sensorian Orange LED

    //printf("Rebooting\n");
    //pi_reboot(5); //Reboots the Raspberry Pi in the given number of seconds without blocking program execution

    return 0; //End the program
}