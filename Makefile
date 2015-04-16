all: kernel
	echo "Building all"	

.PHONY: kernel
kernel:
	make -C kernel
