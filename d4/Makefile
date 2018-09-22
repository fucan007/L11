

ROOTFS_DIR = /opt/mymod

APP_NAME = test_char

CROSS_COMPILE = /home/fucan/gcc-4.6.4/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc

ifeq ($(KERNELRELEASE), )

KERNEL_DIR = /home/fucan/linux/linux-3.14-fs4412
CUR_DIR = $(shell pwd)

all :
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) modules

clean :
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) clean
	
install:
	cp -raf *.ko $(APP_NAME)   $(ROOTFS_DIR)/drv_module


else

obj-m += test_char_drv.o


endif
