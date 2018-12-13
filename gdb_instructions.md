### To launch openOCD
```
$ ./launch-openocd.sh
```
You can also use `launch-openocd-remote.sh` if you wish to run `gdb` on a different machine.

### To connect gdb to an openOCD session
```
(gdb) target remote localhost:3333 # (or whatever the reported ip was)
```
If you are running gdb on a remote machine either:
- start openocd with -c "bindto 0.0.0.0" to allow nonlocal connections or
- use ssh port forwarding as shown below
If the connection is refused try ssh port fowarding with
`ssh amm@som-ip -L 3344:localhost:3333`
then you can do `target remote localhost:3344` in gdb

### To flash
```
(gdb) monitor reset halt
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

### Build
```
$ ./clean.sh #to clear cmake caches
$ ./gencmake.sh
$ make
```

### To create a binary file for direct flashing
Note that gdb can flash .elfs directly.
You need this only if you are using openOCD to flash.

```
arm-none-eabi-objcopy -O binary amm-tiny.elf amm-tiny.bin
```
