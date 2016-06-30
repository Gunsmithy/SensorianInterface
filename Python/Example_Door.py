#!/usr/bin/env python

"""Example_Door.py: Example code to act as a door sensor and send a notification through IFTTT when it opens"""

import time
import tft_printer
import SensorsInterface
import CloudTools

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"

# User defined variables for IFTTT.com/maker Channel API credentials and how long to wait for requests
IFTTT_KEY = "YourIFTTTMakerChannelKey"  # Your API key provided by IFTTT when connecting a Maker channel
IFTTT_EVENT = "DoorOpened"  # The name of the event you chose for the recipe to trigger when the door is opened
IFTTT_TIMEOUT = 5  # How long to wait in seconds for commands sent to IFTTT.com before timing out
TOLERANCE = 1000  # How much the magnetometer values need to change to signal
SECONDS = 5  # How long to give the user to prepare for the calibration steps

# Global variables to store the calibrations
xClosed = 0
yClosed = 0
zClosed = 0


# Sets up the Sensorian sensors for use and prints the current magnetic forces to the screen to test
def setup():
    print("Setting up...")
    SensorsInterface.setupSensorian()  # Prepare the sensors on the Sensorian Shield
    time.sleep(2)  # Wait 2 seconds or some sensors won't be ready
    (x, y, z) = SensorsInterface.getMagnetometer()  # Get the current force values of the magnetometer
    sensor_string = "X: " + str(x) + " Y: " + str(y) + " Z: " + str(z)  # Add the magnet values to the string
    tft_printer.screen_print_rotated(sensor_string, 1)  # Print the completed string to the landscape display


# Calibrates the door for its closed state
def calibrate():
    global xClosed, yClosed, zClosed
    seconds = SECONDS  # The number of seconds to give the user to prepare the device in the door closed position
    while seconds > 0:  # While the user still has time to prepare
        # print("Calibrating door closed state in " + str(seconds))
        calibrating = "Calibrating door closed state in " + str(seconds)
        print(calibrating)
        tft_printer.screen_print_rotated(calibrating, 1)
        time.sleep(1)  # Wait for a second
        seconds -= 1  # Reduce the time remaining for preparation by 1 second
    (xClosed, yClosed, zClosed) = SensorsInterface.getMagnetometer()  # Get the current force values of the magnetometer
    calibrated = "Calibrated door closed state to " + str(TOLERANCE) + " within " + str((xClosed, yClosed, zClosed))
    print(calibrated)
    tft_printer.screen_print_rotated(calibrated, 1)
    time.sleep(3)  # Waits 3 seconds so the user can read the screen


# Contains the main looping execution of the program
def main():
    print("Running...")
    door_was_open = False  # A boolean to store whether or not the door was already open if it was detected open again
    while True:
        (x, y, z) = SensorsInterface.getMagnetometer()  # Get the current force values of the magnetometer
        sensor_string = "X: " + str(x) + " Y: " + str(y) + " Z: " + str(z)  # Add the magnet values to the string
        # If any of the sensor values differ from the calibrated closed amount by more than the tolerance, it is open
        if xClosed - TOLERANCE > x or x > xClosed + TOLERANCE or yClosed - TOLERANCE > y or y > yClosed + TOLERANCE or \
                zClosed - TOLERANCE > z or z > zClosed + TOLERANCE:
            if not door_was_open:
                sensor_string += " Door opened!"
                print(sensor_string)
                CloudTools.ifttt_trigger(IFTTT_KEY, IFTTT_EVENT, IFTTT_TIMEOUT)
                door_was_open = True
        else:
            door_was_open = False
        tft_printer.screen_print_rotated(sensor_string, 1)  # Print the completed string to the landscape display
        time.sleep(1)  # Wait for 1 seconds between each check for magnetic forces


# Assuming this program is run itself, execute normally
if __name__ == "__main__":
    setup()  # Set up the sensors for use
    calibrate()  # Set the approximate values for the door being closed
    try:  # Try running the main method
        main()
    except KeyboardInterrupt:  # Halt the program when a keyboard interrupt is received from the console
        print("...Quitting ...")
