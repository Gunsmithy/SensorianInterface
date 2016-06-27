#!/usr/bin/env python

"""PiTools.py: Adds basic Raspberry Pi functionality such as serial reading and """

import socket
import fcntl
import struct
import os
from multiprocessing import Process

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"


# Returns the global CPU serial variable
def get_cpu_serial():
    # Extract serial from cpuinfo file
    temp_serial = "0000000000000000"
    # Get cpu serial from the designated CPU info file
    try:
        # Create a read-only file object for the /proc/cpuinfo file
        f = open('/proc/cpuinfo', 'r')
        try:
            # Search all the lines in the CPU info
            for line in f:
                # Find the line that contains the serial
                if line[0:6] == 'Serial':
                    # Read the specific characters from the line that has the serial
                    temp_serial = line[10:26]
        # Close the file when done reading it, sucessfully or otherwise
        finally:
            f.close()
    # If there's a problem reading the serial, return an error serial
    except (IOError, OSError):
        temp_serial = "ERROR000000000"
    # Return the CPU serial: Initial, Correct or Error
    finally:
        return temp_serial


# Get the current temperature of the Raspberry Pi's CPU
def get_cpu_temperature():
    temp_path = '/sys/class/thermal/thermal_zone0/temp'
    temp_file = open(temp_path)
    cpu = temp_file.read()
    temp_file.close()
    return float(cpu) / 1000


# Get the IP of the passed interface
def get_interface_ip(interface):
    # Create a socket to use to query the interface
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Try to get the IP of the passed interface
    try:
        ipaddr = socket.inet_ntoa(fcntl.ioctl(
            s.fileno(),
            0x8915,  # SIOCGIFADDR
            struct.pack('256s', interface[:15])
        )[20:24])
    # If it fails, return an empty IP address
    except IOError:
        ipaddr = "0.0.0.0"
    # Close the socket whether an IP was found or not
    finally:
        s.close()
    # Return the IP Address: Correct or Empty
    return ipaddr


# Calls the shutdown.py helper script to shutdown the pi after a given delay in seconds, defaults to 5
def shutdown_pi_blocking(delay=5):
    os.system("sudo python shutdown.py -h --time=" + str(delay))


# Calls the normally blocking shutdown_pi_blocking function in a separate process to be non blocking	
def shutdown_pi(delay=5):
    shutdown_helper = Process(target=shutdown_pi_blocking, args=(delay,))
    shutdown_helper.start()


# Calls the shutdown.py helper script to reboot the pi after a given delay in seconds, defaults to 5
def reboot_pi_blocking(delay=5):
    os.system("sudo python shutdown.py -r --time=" + str(delay))


# Calls the normally blocking reboot_pi_blocking function in a separate process to be non blocking
def reboot_pi(delay=5):
    reboot_helper = Process(target=reboot_pi_blocking, args=(delay,))
    reboot_helper.start()
