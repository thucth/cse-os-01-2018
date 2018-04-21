make
sudo insmod ./hello-1.ko

check : cat /proc/modules | grep hello

rmmod hello-1

dmesg

