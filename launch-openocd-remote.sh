#!/bin/sh
#by default openocd binds to localhost, only use this if the network is trusted
sudo openocd -c "bindto 0.0.0.0" -f /usr/share/openocd/scripts/interface/cmsis-dap.cfg -f /usr/share/openocd/scripts/target/kx.cfg $*
