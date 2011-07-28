export top_dir		:= .
export include_dir  := $(top_dir)/include
export src_dir      := $(top_dir)
export build        := $(top_dir)/build

cc  :=  gcc
as  :=  gcc
ld  :=  ld

cc_flags    := -ffreestanding -nostdinc -nostdlib -Wall -Wextra -Winline -O2 -MD 
as_flags    := -Wall -MD $(addprefix -I, $(include_dir))
ld_flags    := -static -nostdlib -Ttext=0x100000  #-s

cc_includes := -include include/globl.h $(addprefix -I, $(include_dir)) 

# for 64bit host
cc_flags 	+= -m32
as_flags	+= -m32
ld_flags	+= --oformat=elf32-i386 -melf_i386

objs		:= $(src_dir)/boot.S
objs		+= $(wildcard $(src_dir)/[^b]*.S)
objs		+= $(wildcard $(src_dir)/*.c)
objs		+= $(wildcard $(src_dir)/*/*.c)

objs		:= $(patsubst $(src_dir)/%.S, $(build)/%.o, $(objs))
objs		:= $(patsubst $(src_dir)/%.c, $(build)/%.o, $(objs))

kernel      := kernel

mnt_dir     := mount_point
image       := cosec.img

### Use native mount/umount for a GRUB installation, fuseext2 fails at this
# do_mount	:= sudo mount -o loop 
# do_umount	:= sudo umount 
# do_install	:= sudo cp 
do_mount	:= fuseext2 -o rw+,uid=`id -u`,gid=`id -g` 
do_umount	:= fusermount -u 
do_install	:= cp

log_name	:= fail.log
objdump     := $(kernel).objd
pipe_file	:= com

vbox_name   := COSEC
qemu_flags	:= -fda $(image) -boot a -m 32 -net nic,model=rtl8139  -ctrl-grab

.PHONY: run install mount umount clean
.PHONY: qemu vbox bochs

#VBoxManage startvm $(vbox_name) 2>&1 | tee $(log_name);	
run:	install
	
	@echo "\n#### Running..." && \
	if [ $$DISPLAY ] ; then	\
		make vbox || make qemu || make bochs || \
			echo "@@@@ Error: VirtualBox, qemu or Bochs must be installed";	\
	else qemu $(qemu_flags) -curses;	fi

qemu:	install 
	@if [ -S $(pipe_file) ]; 							\
	then qemu $(qemu_flags) -serial unix:$(pipe_file);	\
	else qemu $(qemu_flags);							\
	fi 

vbox:	install
	VBoxManage startvm $(vbox_name)

bochs:	install
	bochs


install:	mount  $(kernel) 
	@$(do_install) $(build)/$(kernel) $(mnt_dir) \
	&& echo "\n## Kernel installed" && make umount

$(mnt_dir):	
	@mkdir $(mnt_dir) 

mount:	$(image) $(mnt_dir)
	@$(do_mount) $(image) $(mnt_dir) \
	&& echo "## Image mounted"

umount:	
	@$(do_umount) $(mnt_dir) &&	echo "## Image unmounted" || true; \
	rmdir $(mnt_dir)

$(image):	 
	@echo "\n#### Checking image"
	@if [ ! -e $(image) ]; then	\
        echo -en "\n## Generating image...\t\t"; \
		cp res/fd.img.bz2 .;	\
		bunzip2 fd.img.bz2;	\
		mv fd.img $(image);	\
		make mount \
			&& mkdir -p $(mnt_dir)/boot/grub/ \
			&& $(do_install) res/menu.lst $(mnt_dir)/boot/grub \
			&& make umount;	\
		echo -e "## ...generated\n";	\
	fi

$(kernel): $(build) $(objs) 
	@echo "\n#### Linking..."
	@echo -n "LD: "
	$(ld) -o $(build)/$(kernel)	$(objs) $(ld_flags) && echo "## ...linked"
	@if [ `which objdump 2>/dev/null` ]; then objdump -d $(build)/$(kernel) > $(objdump); fi
	@if [ `which ctags 2>/dev/null ` ]; then ctags -R *; fi
	
$(build):
	@echo "\n#### Compiling"
	@mkdir -p $(build)
	@for d in * ; do		\
		[ -d $$d ] && mkdir $(build)/$$d || true;	\
	done
	
$(build)/%.o : %.c
	@echo -n "CC: "
	$(cc) -c $< -o $@ $(cc_includes) $(cc_flags) -MT $(subst .d,.c,$@) 

$(build)/%.o : $(src_dir)/%.S
	@echo -n "AS: "
	$(as) -c $< -o $@ $(as_flags) -MT $(subst .d,.c,$@)

$(pipe_file):
	mkfifo $(pipe_file)

clean:
	rm -rf $(build)

help:
	@echo "USAGE:"; \
	echo "	[run] - (default action) compile sources, link, generate image if needed, install kernel, search a virtual machine, run"; \
	echo "	qemu | vbox | bochs - make all the things needed for run and run in the specified emulator"; \
	echo "	install - check kernel and image and install former to latter";	\
	echo "	kernel - compile and link kernel"; \
	echo "	mount/umount - mount/umount image (root privileges are required unless using FUSE)"

include $(wildcard $(addprefix /*.d, $(build)))

