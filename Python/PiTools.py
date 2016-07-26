#!/usr/bin/env python

"""PiTools.py: Extra tools related to the Raspberry Pi for Sensorian projects like Shutdown, Reboot, CPU info, etc. """

import socket
import fcntl
import struct
import os
from multiprocessing import Process

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"


def get_cpu_serial():
    """Returns the Raspberry Pi's unique CPU serial string.

    Can be used to uniquely identify the Pi for use in multi-Pi systems.
    """
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


def get_cpu_temperature():
    """Returns a float of the Raspberry Pi's current CPU temperature."""
    temp_path = '/sys/class/thermal/thermal_zone0/temp'
    temp_file = open(temp_path)
    cpu = temp_file.read()
    temp_file.close()
    return float(cpu) / 1000


def get_interface_ip(interface):
    """Returns the IP address of the passed interface, defaults to 0.0.0.0.

    Expects a string like lo, eth0, wlan0, etc.
    """
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


def shutdown_pi_blocking(delay=5):
    """Shuts down the Pi after a given delay in seconds. Blocks program execution.

    Calls the shutdown.py helper script. Defaults to 5 seconds.
    """
    os.system("sudo python shutdown.py -h --time=" + str(delay))


def shutdown_pi(delay=5):
    """Shuts down the Pi after a given delay in seconds. Does not block program execution.

    Calls shutdown_pi_blocking() in a separate process. Defaults to 5 seconds.
    """
    shutdown_helper = Process(target=shutdown_pi_blocking, args=(delay,))
    shutdown_helper.start()


def reboot_pi_blocking(delay=5):
    """Reboots the Pi after a given delay in seconds. Blocks program execution.

    Calls the shutdown.py helper script.  Defaults to 5 seconds.
    """
    os.system("sudo python shutdown.py -r --time=" + str(delay))


def reboot_pi(delay=5):
    """Reboots the Pi after a given delay in seconds. Does not block program execution.

    Calls reboot_pi_blocking() in a separate process.  Defaults to 5 seconds.
    """
    reboot_helper = Process(target=reboot_pi_blocking, args=(delay,))
    reboot_helper.start()
