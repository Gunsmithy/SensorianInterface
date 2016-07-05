#!/usr/bin/env python

"""test.py: Tests the functionality of tft_printer, SensorsInterface and NetworkingTools"""

import time
import tft_printer
import SensorsInterface
import PiTools
import CloudTools

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"

SensorsInterface.setupSensorian()  # Prepare the sensors on the Sensorian Shield

SensorsInterface.ledOn()  # Turn on the Sensorian Orange LED

# Print White Hello World to the display in Portrait mode
tft_printer.screen_print_rotated("Hello world!", 0)

time.sleep(2)  # Wait for 2 seconds before continuing.

# Print a longer black string on a white background to the display in Landscape mode
tft_printer.screen_print_rotated("This is a longer string to demonstrate the wrapping and text background", 1,
                                 colour=(0, 0, 0, 0), background=(255, 255, 255))

time.sleep(2)  # Wait for 2 seconds before continuing.

sensor_string = ""  # Prepare an empty string to be printed to the screen 

sensor_string += " Light: " + str(SensorsInterface.getAmbientLight())  # Add the light value to the string

sensor_string += " Temp: " + str(SensorsInterface.getTemperature())  # Add the temperature to the string

sensor_string += " Alt: " + str(SensorsInterface.getAltitude())  # Add the altitude to the string

sensor_string += " Press: " + str(SensorsInterface.getBarometricPressure())  # Add the pressure to the string

sensor_string += " Button: " + str(SensorsInterface.getTouchpad())  # Add the last button press to the string

sensor_string += " Accel: " + str(SensorsInterface.getAccelerometer())  # Add the accelerometer value to the string

sensor_string += " Magnet: " + str(SensorsInterface.getMagnetometer())  # Add the magnetometer value to the string

sensor_string += " Time: " + str(SensorsInterface.getRTCCtime())  # Add the date and time to the string

tft_printer.screen_print_rotated(sensor_string, 1)  # Print the completed string to the landscape display

time.sleep(2)  # Wait for 2 seconds before proceeding

tools_string = ""  # Prepare an empty string to be printed to the screen

tools_string += " Serial: " + PiTools.get_cpu_serial()  # Add the RPi's CPU Serial number to the string

tools_string += " CPU Temp: " + str(PiTools.get_cpu_temperature())  # Add the RPi's CPU Serial number to the string

tools_string += " Local: " + PiTools.get_interface_ip("eth0")  # Add the local IP of eth0 to the string

tools_string += " Public: " + CloudTools.get_public_ip()  # Add the public IP of the RPi to the string

tft_printer.screen_print_rotated(tools_string, 1)  # Print the completed string to the landscape display

# Send a trigger to a recipe created using the Maker Channel on IFTTT.com
CloudTools.ifttt_trigger("YourIFTTTMakerChannelKey", "YourRecipeEventName", 5)

# Send a trigger to a recipe created using the Maker Channel on IFTTT.com along with variables in a JSON
CloudTools.ifttt_trigger("YourIFTTTMakerChannelKey", "YourRecipeEventName", 5, "1", 2, 3.0)

# print("Telling Pi to reboot in 10")
# PI_TOOLS.reboot_pi(10)
# print("Rebooting in 10, Ctrl+C to cancel")

SensorsInterface.ledOff()  # Turn off the Sensorian Orange LED
