#!/usr/bin/env python

"""Example_Door.py: Example code to act as a door sensor and send a notification through IFTTT when it opens"""

import time
import tft_printer
import SensorsInterface

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"

# User defined variables for IFTTT.com/maker Channel API credentials and how long to wait for requests
IFTTT_KEY = "YourIFTTTMakerChannelKey"
IFTTT_EVENT = "DoorOpened"
IFTTT_TIMEOUT = 5

# Global variables to store the calibrations


# Sets up the Sensorian sensors for use and prints the current light level to the screen to test
def setup():
    SensorsInterface.setupSensorian()  # Prepare the sensors on the Sensorian Shield
    time.sleep(2)  # Wait 2 seconds or some sensors won't be ready
    (x, y, z) = SensorsInterface.getMagnetometer()  # Get the current force values of the magnetometer
    sensor_string = "X: " + str(x) + " Y: " + str(y) + " Z: " + str(z)  # Add the magnet values to the string
    tft_printer.screen_print_rotated(sensor_string, 1)  # Print the completed string to the landscape display


# Contains the main looping execution of the program
def main():
    while True:
        (x, y, z) = SensorsInterface.getMagnetometer()  # Get the current force values of the magnetometer
        sensor_string = "X: " + str(x) + " Y: " + str(y) + " Z: " + str(z)  # Add the magnet values to the string
        tft_printer.screen_print_rotated(sensor_string, 1)  # Print the completed string to the landscape display
        time.sleep(1)  # Wait for 1 seconds between each check for magnetic forces


# Assuming this program is run itself, execute normally
if __name__ == "__main__":
    setup()  # Set up the sensors for use

    try:  # Try running the main method
        main()
    except KeyboardInterrupt:  # Halt the program when a keyboard interrupt is received from the console
        print("...Quitting ...")
