Project Title: Linux Device Driver for USB to LED Subsystem
Summary: LED subsystem is employed to communicate from PC to LEDs connected to AVR ATMEGA 8 through USB. Here we have used VUSB in the firmware side . usb_control_msg() was to communicate for echo and trigger commands. Additionally a trigger named 'triggrable' is also made which can be activated/deactivated using echo command.

Firmware build process: Build using Makefile
        Commands-- make clean --> make all--> avrdude -c usbasp -p m8 -U flash:w:main.hex

Kernel Space Driver build process: uild using Makefile
          Commands-- make--> sudo insmod usbtolrd.ko
