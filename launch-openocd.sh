#!/bin/sh
sudo openocd -f /usr/share/openocd/scripts/interface/cmsis-dap.cfg -f /usr/share/openocd/scripts/target/kx.cfg $*
