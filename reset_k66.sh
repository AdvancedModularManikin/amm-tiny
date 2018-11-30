#!/bin/bash
#Remember to chmod +x this file to make it executable:) 
  echo "Resetting the chip"
  sudo openocd -f /usr/share/openocd/scripts/interface/cmsis-dap.cfg -f /usr/share/openocd/scripts/target/kx.cfg -c "init;reset;exit"
  echo "Done!"
