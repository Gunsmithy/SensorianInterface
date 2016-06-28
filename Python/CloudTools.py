#!/usr/bin/env python

"""CloudTools.py: Adds basic networking functionality such as IP detection and request sending"""

import subprocess
import requests

__author__ = "Dylan Kauling"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"


# Get the current public IP from the icanhazip site
# Don't update too frequently, that's not cool
def get_public_ip():
    # Initiate a subprocess to run a curl request for the public IP
    proc = subprocess.Popen(["curl", "-s", "-4", "icanhazip.com"], stdout=subprocess.PIPE)
    (out, err) = proc.communicate()
    # Return the response of the request when safe
    return out.rstrip()


# Sends a request to an IFTTT Maker Channel with the given key and event name
def ifttt_trigger(key="xxxxxxxxxxxxxxxxxxxxxx", event="SensorianEvent", timeout=5, value1="", value2="", value3=""):
    payload = {'value1': str(value1), 'value2': str(value2), 'value3': str(value3)}
    url = "https://maker.ifttt.com/trigger/" + event + "/with/key/" + key
    # Make a GET request to the IFTTT maker channel URL using the event name and key
    try:
        r = requests.post(url, data=payload, timeout=timeout)
        print(r.text)  # For debugging GET requests
    except requests.exceptions.ConnectionError:
        print("IFTTT ERROR - requests.exceptions.ConnectionError - Check connection and server")
    except requests.exceptions.InvalidSchema:
        print("IFTTT ERROR - requests.exceptions.InvalidSchema - Check the URL")
    except requests.exceptions.Timeout:
        print("IFTTT TIMEOUT - requests.exceptions.Timeout - Please try again or check connection")
