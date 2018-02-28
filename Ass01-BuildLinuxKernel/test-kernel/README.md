## Build Linux Kernel Module
### How to use:
+ Create file hello-1.c as repo
+ Create file Makefile as repo (Note: Makefile, not makefile)
+ Run terminal:
	+ make
	+ sudo insmod ./hello-1.ko
+ Test Kernel Module:
	+ cat /proc/modules | grep hello
	+ dmesg
