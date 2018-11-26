This is the AMMDK k66f code, currently both the remote api k66f end and several application-specific endpoints.

# Usage

You need the [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) which you can install from the [debian](https://packages.debian.org/buster/gcc-arm-none-eabi) repos. You also need embedded `gdb` and `openocd` > 0.10.0. To install build requirements and other development tools:

    sudo apt-get install gcc-arm-none-eabi cmake gdb-multiarch openocd

If you are using an AMMDK it should have come with these preinstalled. On OSX:

    brew cask install gcc-arm-embedded

This will also install `gdb`.

# Compile

    ./gencmake.sh && make

Running `make` when `CMakelists.txt` has changed will crash due to no `ARMGCC_DIR` environment variable. You can fix this by setting ARMGCC_DIR in your environment to the value it takes in `gencmake.sh`.

# Debug

In one terminal on the AMMDK, run `./launch_openocd.sh`. In another terminal, run `arm-none-eabi-gdb debug/$YOUR_FILE_HERE.elf`. At the `(gdb) ` prompt run:

    (gdb) target remote 127.0.0.1:3333
    (gdb) load
    (gdb) monitor reset halt
    (gdb) break main
    (gdb) continue

You should be dropped at a prompt at the first line of `main()`.
