#!/bin/sh
#fixer using external programmer
openocd -f interface/ftdi/olimex-arm-usb-tiny-h.cfg -f interface/ftdi/olimex-arm-jtag-swd.cfg -f target/kx.cfg -c "init;kinetis mdm mass_erase; kinetis mdm halt;reset halt;program debug/amm-tiny.elf;reset;exit"
