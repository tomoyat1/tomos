# tomos
The feature-poor spaghetti code operating system...

Feature incomplete (by far). TODOs are as follow
* Complete memory management, including designing it.
* Scheduler
* Shell
* userland

For compilation, a i686-elf cross compiler is needed. Compilation has succeeded using GCC 4.9.2 and GNU Binutils 2.25  
[GCC](https://gcc.gnu.org "GCC")  
[GNU Binutils](http://www.gnu.org/software/binutils/ "GNU Binutils")  

Configure and build Binutils with the following commands. 

```
./configure --target=$TARGET --prefix="$PREFIX"\
--with-sysroot --disable-nls --disable-werror
make
make install
```

Where 

```
$TARGET="i686-elf"
$PREFIX="/path/to/installation/dir"
```

The path.sh script assumes

```
$PREFIX="$HOME/opt/i686"
```

If you have changed this to something else, modify path.sh accordingly.

Configure and build GCC with the following commands

```
./configure --target=$TARGET --prefix="$PREFIX" --disable-nls\
--enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```

Additionally, GMP, MPFR, MPC, ISL, and cloog might be needed. Whether they are currently needed or not have not been tested, and might be used in the future. 

GNU Make and QEMU are needed to build the OS.

To build the OS, go to the top directory and run

```
source path.sh
make
```

To run it, issue the command

```
make run
```

This will boot the kernel using qemu-system-i386 with the -kernel option. Any multiboot(1) compliant bootloader (such as grub) may be used if desired.

