watchdog_interrupt
============
http://elegantcircuits.com/2014/10/14/introduction-to-the-avr-watchdog-timer/

CLK = 8MHZ
avrdude -p atmega328p -P /dev/ttyUSB0 -c avrisp -b 19200 -B 1 -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m 

Enable CKDIV8 fuse
avrdude -p atmega328p -P /dev/ttyUSB0 -c avrisp -b 19200 -B 1 -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m 
