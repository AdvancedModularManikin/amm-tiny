This repository contains the AMMDK k66f code. It currently includes both the endpoint for the remote api and several application-specific endpoints.

# Usage

You need the [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) which you can install from the [debian](https://packages.debian.org/buster/gcc-arm-none-eabi) repos. You also need embedded `gdb` and `openocd` > 0.10.0. To install build requirements and other development tools:

    sudo apt-get install gcc-arm-none-eabi cmake gdb-multiarch openocd

If you are using an AMMDK it should have come with these preinstalled. On OSX:

    brew cask install gcc-arm-embedded

This will also install `gdb`.

# Compiling

    $ ./gencmake.sh && make

Running `make` when `CMakelists.txt` has changed will crash due to no `ARMGCC_DIR` environment variable.
You can fix this by setting ARMGCC_DIR in your environment to the value it takes in [`gencmake.sh`](gencmake.sh).
If the cmake cache goes out of sync, [`clean.sh`](clean.sh) will remove all build artifacts.

By default, the code is built into a `debug/` directory.

# Flashing
Once the code is compiled, it needs to be flashed onto the k66f.

## Via CLI

For convenience, the provided [`flash.sh`](flash.sh) script handles file type conversion and verification.

    $ ./flash.sh debug/amm-tiny.elf

The [`flash.sh`](flash.sh) script accepts either `.elf` or `.bin` files.

## Using `gdb`
Note that `gdb` can flash `.elf`s directly.
You need this only if you are using openOCD to flash.

```
arm-none-eabi-objcopy -O binary amm-tiny.elf amm-tiny.bin
```

Start a `gdb` session (described below), and use the following commands:
```
(gdb) interrupt 
(gdb) load
(gdb) monitor reset halt
```
To change which file `gdb` will load:
```
(gdb) interrupt 
(gdb) file debug/new-file.elf
(gdb) load
(gdb) monitor reset halt
```

Then, to restart the k66f, run one of:

```
(gdb) c # for continue, gdb understands unique prefixes
```
```
(gdb) monitor go
```

# Debug

## In brief

In one terminal on the AMMDK, run [`./launch_openocd.sh`](launch_openocd.sh).
In another terminal, run `gdb-multiarch debug/$YOUR_FILE_HERE.elf`.
At the `(gdb) ` prompt run:

    (gdb) target remote 127.0.0.1:3333
    (gdb) load
    (gdb) monitor reset halt
    (gdb) break main
    (gdb) continue

You should be dropped at a prompt at the first line of `main()`.

### To launch openOCD
```
$ ./launch-openocd.sh
```
You can also use `launch-openocd-remote.sh` if you wish to run `gdb` on a different machine.

### To connect gdb to an openOCD session
```
(gdb) target remote 127.0.0.1:3333 # (or whatever the reported ip was)
```
If you are running gdb on a remote machine either:

- start openocd with `-c "bindto 0.0.0.0"` to allow nonlocal connections or
- use ssh port forwarding as shown below

If the connection is refused try ssh port fowarding with
`ssh amm@som-ip -L 3344:localhost:3333`
then you can do `target remote localhost:3344` in `gdb`.

# Documentation

See [documentation.md](documentation.md) and the [spi documentation](https://github.com/AdvancedModularManikin/spi_proto).

## Examples

[`heartrateLED.cpp`](source/heartrateLED.cpp) and its [twin](https://github.com/AdvancedModularManikin/DDS/blob/master/AMM_Modules/src/HeartRateLED.cpp) in the DDS repo offer an example of the datagram API.

[`click-test.cpp`](https://github.com/AdvancedModularManikin/spi_proto/blob/master/test/click-test.cpp) offers an example of the use of the remote API.
