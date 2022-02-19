obj-m:=dd1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
