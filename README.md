| Master                                                                                                             | CI_Dev                                                                                                             |
|--------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------|
| ![Build Status](https://github.com/romi/libromi/workflows/CI/badge.svg?branch=master)                              | ![Build Status](https://github.com/romi/libromi/workflows/CI/badge.svg?branch=ci_dev)                              | 
| [![codecov](https://codecov.io/gh/libromi/libr/branch/master/graph/badge.svg)](https://codecov.io/gh/romi/libromi) | [![codecov](https://codecov.io/gh/romi/libromi/branch/ci_dev/graph/badge.svg)](https://codecov.io/gh/romi/libromi) |

# libromi

Libromi provides common abstractions and functions for ROMI applications.

## Using the Serial Camera on Raspberry Pi

The serial camera is designed to be used with a Raspberry Pi 4 (the host) and a Raspberry Pi Zero W (the camera).

To configure the system correctly, follow these steps to set up the serial ports on both Raspberry Pi devices:

1. Start `raspi-config`:
    ```shell
    sudo raspi-config
    ```
2. Select "Interface Options," then "P6 Serial Port."
3. Answer the following configuration questions:
  - **"Would you like a shell login to be accessible over serial?"**  
    Respond with `<No>`.
  - **"Would you like the serial port hardware to be enabled?"**  
    Respond with `<Yes>`.
4. Reboot the system.
5. After rebooting, edit `/boot/config.txt`.  
   Replace the line:
   ```text
   dtoverlay=pi3-miniuart-bt
   ```
   with:
   ```text
   dtoverlay=pi3-disable-bt
   ```
  - If the `dtoverlay=pi3-miniuart-bt` line is not present, simply add `dtoverlay=pi3-disable-bt` at the end of the file.
6. Reboot the system again.

If the configuration is correct, the following symbolic links should be created (to be confirmed):

```shell
lrwxrwxrwx 1 root root 7 Apr  4 16:46 /dev/serial0 -> ttyAMA0
lrwxrwxrwx 1 root root 5 Apr  4 16:46 /dev/serial1 -> ttyS0
```

## Using the GPIO Pins on Raspberry Pi 4

Ensure that the kernel version of your Raspberry Pi 4 is newer than 4.8.

1. Verify the kernel version by running the following command:
    ```shell
    uname -a
    ```
   The output should resemble something like this:
    ```text
    Linux hostname 5.10.17-v7l+ #1403 SMP Mon Feb 22 11:33:35 GMT 2021 armv7l GNU/Linux
    ```

2. Check for the presence of GPIO devices. The Raspberry Pi 4 has two GPIO chips, while the Raspberry Pi Zero W has only one. Run:
    ```shell
    ls -al /dev/gpiochip*
    ```
   The output should look something like this:
    ```text
    crw-rw---- 1 root gpio 254, 0 Apr  2 17:59 /dev/gpiochip0
    crw-rw---- 1 root gpio 254, 1 Apr  2 17:59 /dev/gpiochip1
    ```

3. Install the `gpiod` package:
    ```shell
    sudo apt install gpiod libgpiod-dev
    ```

4. Confirm that the GPIO tools are working:
    ```shell
    sudo gpiodetect
    sudo gpioinfo gpiochip0
    ```