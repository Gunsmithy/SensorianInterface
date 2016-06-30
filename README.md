# SensorianInterface
Python and C APIs for the Sensorian Shield on Raspberry Pi with additional helper functions  
Compatible with the latest official Raspbian image (2016-05-27) and Python 2.7  
Previous versions may be compatible but are currently untested  

Code partially written and maintained myself with the use of the Sensorian Shield firmware  
Some files were created by members of the original Sensorian project and have been adopted by me  
Please see Sensorian on [GitHub](https://github.com/sensorian) or their [main website](http://sensorian.io/) for details  
Currently written in Python with the use of Python and C drivers/DLLs  

To get this working in a split, just follow these simple instructions!  
To summarize for experts, simply clone/download the repo wherever and run the install script!  

1. Install the latest Raspbian image to your Pi following [this official guide](https://www.raspberrypi.org/documentation/installation/installing-images/)  
2. Boot up the Pi, and connect to it however you like. A display cable is not necessary.  
3. Run `sudo raspi-config` to your liking, remember that you can simply boot to console, no display required.  
4. Be sure to enable the SPI and I2C interfaces in Advanced Options, and optionally SSH.   
5. Download/clone the project to a directory by navigating to it and typing the following line  
  * `git clone https://github.com/Gunsmithy/SensorianInterface.git`  
6. Change into the new directory using `cd SensorianInterface` then `cd C` or `cd P*`  
7. Type `chmod +x install.sh` and then run `./install.sh` and wait for it to finish  
8. When it is finally done installing dependencies, reboot the Pi with `sudo reboot`  
9. Finally, `sudo python test.py` or `sudo make && sudo ./TEST` to run the included tests!  

In summary, the code bases are broken into 4 main libraries:  
1. `SensorsInterface.c/py` - These are the main helper APIs for easily getting all sensor values  
2. `TFT_Printer.c/py` - These are helper APIs for printing a message to the screen that is wrapped to fit  
3. `PiTools.c/py` - Adds additional functionality to your Sensorian project related to the Raspberry Pi  
4. `CloudTools.c/py` - Adds additional functionality to your Sensorian project related to cloud integration  
Check out the example code, `test.c/py`, to see how these APIs can be used to quickly build applications  

For your benefit, some example programs have also been added to demonstrate some practical applications  
1. `Example_Lights.c/py` - Adjusts the brightness of a Philips Hue lightbulb to maintain a certain light level  
  * This could be used to turn up your lights as the sun goes down in a windowed room  
2. `Example_Door.c/py` - Triggers an IFTTT recipe when a door is opened using a magnet on the door frame  
  * Mount the Pi to the door or frame and the magnet on the other and it will sense when the magnet moves away  

Hope you find it useful, feel free to contribute or tell me things you'd like to see!