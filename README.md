This is the AMMDK k66f code, currently both the remote api k66f end and several application-specific endpoints.

# Usage

You need the [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) which you can install from the [debian](https://packages.debian.org/buster/gcc-arm-none-eabi) repos. You also need embedded `gdb` and `openocd` > 0.10.0. To install build requirements and other development tools:

    sudo apt-get install gcc-arm-none-eabi cmake gdb-multiarch openocd

If you are using an AMMDK it should have come with these preinstalled. On OSX:

    brew cask install gcc-arm-embedded

This will also install `gdb`.

# Compile

    $ ./gencmake.sh && make

Running `make` when `CMakelists.txt` has changed will crash due to no `ARMGCC_DIR` environment variable.
You can fix this by setting ARMGCC_DIR in your environment to the value it takes in `gencmake.sh`.
If the cmake cache goes out of sync, `clean.sh` will remove all build artifacts.

## To create a binary file for direct flashing
Note that gdb can flash .elfs directly.
You need this only if you are using openOCD to flash.

```
arm-none-eabi-objcopy -O binary amm-tiny.elf amm-tiny.bin
```

# To flash

If you are in a `gdb` session (described below) you can use the following commands:
```
(gdb) interrupt 
(gdb) load
(gdb) monitor reset halt
```

### To start
One of:

```
(gdb) c # for continue, gdb understands unique prefixes
```
```
(gdb) monitor go
```

You can also use the `flash.sh` script provided.
It accepts either `.elf` or `.bin` files.

    $ ./flash.sh debug/amm-tiny.elf

# Debug

## In brief

In one terminal on the AMMDK, run `./launch_openocd.sh`.
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
