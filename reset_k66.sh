#!/bin/bash
#Remember to chmod +x this file to make it executable:) 
  echo "Resetting the chip"
  sudo openocd -f kinetis_daplink.cfg -c "init;reset;exit"
  echo "Done!"
