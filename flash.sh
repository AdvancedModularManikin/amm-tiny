#!/bin/bash
#Remember to chmod +x this file to make it executable:) 
DEVICE=/dev/sdg #default for AMMDK
MNTPOINT=/mnt/AMMDK #default for AMMDK
#Check to see if path was supplied
if [ $# -eq 0 ]; then
        echo "Please supply a path to the file you want to flash. Usage : ./flash.sh ./path-to-file.bin"
        else
            sudo mkdir -p $MNTPOINT #create a mount point, dont complain if it exists
            if [ ! -e $DEVICE ]; then echo "Can't find device"; exit 1; fi # check if device exists
            sudo umount  $DEVICE #unmount the device [Linux automounts it :(]
            sudo modprobe msdos #wake msdos module
            sudo mount -t msdos $DEVICE $MNTPOINT # mount device with msdos type

            INPUT="$1" #Argument - path to file (e.g ./amm-tiny.bin)
            if [[ "$INPUT" == *".elf"* ]]
                then
                    echo "Found an .elf converting to .bin"
                    sudo objcopy -O binary "$INPUT" temp-converted-file.bin #convert elf to bin and and copy
                    echo "Copying bin to device"
                    sudo cp temp-converted-file.bin "$MNTPOINT"
                    echo "removing temporary bin file"
                    sudo rm temp-converted-file.bin
            elif [[ "$INPUT" == *".bin" ]]
                then
                    echo "Found a .bin copying to device"
                    sudo cp "$INPUT" "$MNTPOINT" #copy the bin
            else
                echo "Unknown Input File Type"
                exit 1
            fi
            sync # write any data buffered in memory out to disk
            sleep 1 # we have to wait (experiment :))
            sudo umount $DEVICE # i need not explain :)
            echo "Done!"
         fi
