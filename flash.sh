#!/bin/bash
DEVICE=/dev/sdg #default for AMMDK
MNTPOINT=/mnt/AMMDK #default for AMMDK


if [ $# -eq 0 ] #Check to see if path argument was supplied
        then
            echo "Please supply a path to the file you want to flash. Usage : ./flash.sh ./path-to-file.bin"
        else
            FIRMWARE="$1" #Argument - path to .bin file (e.g ./amm-tiny.bin)
            sudo mkdir -p $MNTPOINT #create a mount point, dont complain if it exists
            if [ ! -e $DEVICE ]; then echo "Can't find device"; exit 1; fi # check if device exists
            sudo umount  $DEVICE #unmount the device [Linux automounts it :(]
            sudo modprobe msdos #wake msdos module
            sudo mount -t msdos $DEVICE $MNTPOINT # mount device with msdos type
            sudo cp "$FIRMWARE" "$MNTPOINT" # copy the firmware
            sync # write any data buffered in memory out to disk
            sleep 1 # we have to wait (experiment :))
            sudo umount $DEVICE # i need not explain :)
        fi
#Modified from : http://karibe.co.ke/2014/04/changing-the-firmware-on-freescale-freedom-boards-in-linux/
