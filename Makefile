obj-m += chardev.o


.PHONY: check clean test_read

all:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) modules
	~/sign_module.sh chardev.ko

check:
	lsmod | grep chardev

test_read:
	gcc test_read.c -o test_read

clean:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) clean