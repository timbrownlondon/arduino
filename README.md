# arduino
Code to run on Arduino boards.

Using Ubuntu 22 with Dell laptop the Arduino IDE has difficulty detect arduino boards.

    lsusb | grep CH34
    Bus 002 Device 006: ID 1a86:7523 QinHeng Electronics CH340 serial converter

I found this useful https://github.com/juliagoda/CH341SER
Download that, extract and cd into the dir with Makefile

    make
    sudo make load
