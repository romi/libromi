master  
![Build Status](https://github.com/romi/libromi/workflows/CI/badge.svg?branch=master)
[![codecov](https://codecov.io/gh/libromi/libr/branch/master/graph/badge.svg)](https://codecov.io/gh/romi/libromi)  
ci_dev  
![Build Status](https://github.com/romi/libromi/workflows/CI/badge.svg?branch=ci_dev)
[![codecov](https://codecov.io/gh/romi/libromi/branch/ci_dev/graph/badge.svg)](https://codecov.io/gh/romi/libromi)

# libromi

Libromi provides common abstractions and functions for ROMI applications.



# Using the serial camera on Raspberry Pi

The serial camera is intended to be used with a Raspberry Pi 4 (the
host) and a Raspberri Pi Zero W (the camera).

In order to work, the serial ports of both Raspberry Pi's have to be
set up correctly.

* Start raspi-config ($ sudo raspi-config)
* Select "Interface option", then "P6 Serial port"
* The following question will be asked: "Would you like a shell login to be accessible over serial?". Answer <No>
* Then this question will be asked: "Would you like the serial port hardware to be enabled?". Answer <Yes>
* Reboot.
* After rebooting, edit /boot/config.txt
* Replace "dtoverlay=pi3-miniuart-bt" with "dtoverlay=pi3-disable-bt"
  If you can't find "dtoverlay=pi3-miniuart-bt", then insert "dtoverlay=pi3-disable-bt" at the end of the file. 
* Reboot again.

If also is well, the following symbolic links should be set up (TBC):

lrwxrwxrwx 1 root root 7 Apr  4 16:46 /dev/serial0 -> ttyAMA0
lrwxrwxrwx 1 root root 5 Apr  4 16:46 /dev/serial1 -> ttyS0


# Using the GPIO pins on Raspberry Pi 4

Make sure the kernel version is more recent than 4.8:

$ uname -a
Linux hostname 5.10.17-v7l+ #1403 SMP Mon Feb 22 11:33:35 GMT 2021 armv7l GNU/Linux

Check that the GPIO devices are there. The RPi4 has two gpio chips,
the Pi Zero W only one:

$ ls -al /dev/gpiochip*
crw-rw---- 1 root gpio 254, 0 Apr  2 17:59 /dev/gpiochip0
crw-rw---- 1 root gpio 254, 1 Apr  2 17:59 /dev/gpiochip1

Install the gpiod package:

$ sudo apt install gpiod libgpiod-dev

Check whether the gpio tools work:

$ sudo gpiodetect
$ sudo gpioinfo gpiochip0




