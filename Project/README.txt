Project Title: Linux Device Driver for USB to LED Subsystem

Summary: LED subsystem is employed to communicate from PC to LEDs connected to AVR ATMEGA 8 through USB. Here we have used VUSB in the firmware side . usb_control_msg() was to communicate for echo and trigger commands. Additionally a trigger named 'triggrable' is also made which can be activated/deactivated using echo command.


Firmware build process: Build using Makefile
Commands--make clean --> make all--> avrdude -c usbasp -p m8 -U flash:w:main.hex
 

Kernel Space Driver build process: Build using Makefile
Commands for driver module: make--> sudo insmod usbtoled.ko 

        
Trigger build process: This module will add a trigger called 'triggerable' in the trigger file of all leds in the LED Subsystem
Commands for trigger module: make--> sudo insmod trig3.ko
 
 How to run the system:
         Go to sys/class/leds/<led file name>
         
Commands:
1) sudo chmod 777 brightness   //This will add superuser privilage to brightness file which is necessary to edit this file
2) echo 1 > brightness         //This will turn the leds on
3) echo 0 > brightness         //This will turn the leds off
4) cat trigger      //This will open trigger file and you can see the newly added trigger 'triggerable' here
5) sudo chmod 777 trigger      //This will add super user privilege to trigger file which is necessary to edit this file                             
6) echo triggerable > trigger  //This will activate the newly added trigger for the led
7) echo none > trigger         //This will deactivate the previously added trigger for the led
