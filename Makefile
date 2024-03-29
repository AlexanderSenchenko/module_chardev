obj-m += chardev.o


.PHONY: check clean test_read test_ioctl reload create_dev

all:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) modules
	~/sign_module.sh chardev.ko

check:
	lsmod | grep chardev

test_read:
	gcc test_read.c -o test_read

test_ioctl:
	gcc test_ioctl.c -o test_ioctl

reload:
	rmmod chardev
	insmod chardev.ko

create_dev:
	mknod -m 666 /dev/chardev c 236 0

clean:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) clean