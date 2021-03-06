#include <stdlib.h>

#include <cosec/log.h>

#include <arch/multiboot.h>
#include <arch/mboot.h>

struct vbe_info {
    void *control_info;
    void *mode_info;
    void *mode;
    void *interface_seg;
    void *interface_off;
    void *interface_len;
};

struct ext_info {
    uint flags;
    uint mem_lower;
    uint mem_upper;
    uint boot_device;
    const char *cmdline;
    size_t mods_count;
    module_t* mods_addr;
    elf_section_header_table_t* syms;
    uint mmap_length;
    void * mmap_addr;
    uint drives_length;
    void * drives_addr;
    uint config_table;
    const char *bootloader_name;
    void *apm_table;
    struct vbe_info *vbe;
} mboot;

#define is_set(bf, bit) \
    (((bf) >> (bit)) & 1)

void mboot_info_parse(struct multiboot_info *mbi) {
    k_printf("multiboot_info at *%x\n", (uint)mbi);
    mboot.flags = mbi->flags;
    if (is_set(mboot.flags, 0)) {
        mboot.mem_lower = mbi->mem_lower;
        mboot.mem_upper = mbi->mem_upper;
        k_printf("mem_lower = %d Kb, mem_upper = %d Kb\n", mboot.mem_lower, mboot.mem_upper);
    } else {
        mboot.mem_lower = 0;
        mboot.mem_upper = 0;
    }
    mboot.boot_device = is_set(mboot.flags, 1) ? mbi->boot_device : 0;
    mboot.cmdline = (is_set(mboot.flags, 2)) ? (const char *)mbi->cmdline : null;
    if (is_set(mboot.flags, 3)) {
        mboot.mods_count = mbi->mods_count;
        mboot.mods_addr = (module_t *)mbi->mods_addr;
    } else {
        mboot.mods_count = 0;
        mboot.mods_addr = 0;
    }
    mboot.syms = (elf_section_header_table_t *)(is_set(mboot.flags, 5)? &(mbi->u) : null);
    if (is_set(mboot.flags, 6)) {
        mboot.mmap_length = mbi->mmap_length;
        mboot.mmap_addr = (void *)(mbi->mmap_addr);
    } else {
        mboot.mmap_length = 0;
        mboot.mmap_addr = null;
    }

    uint *mbia = (uint *)mbi;

    if (is_set(mboot.flags, 7)) {
        mboot.drives_length = mbia[13];
        mboot.drives_addr = (void *)mbia[14];
        k_printf("drives: %d (info at *0x%x)\n", mboot.drives_length, mboot.drives_addr);
    } else {
        mboot.drives_length = 0;
        mboot.drives_addr = null;
    }
    mboot.config_table = (is_set(mboot.flags, 8) ? (uint)(mbia[15]) : 0);
    mboot.bootloader_name = (is_set(mboot.flags, 9) ? (const char *)(mbia[16]) : null );
    mboot.apm_table = (is_set(mboot.flags, 10) ? (void *)(mbia[17]) : null);
    mboot.vbe = (is_set(mboot.flags, 11)? (void *)(mbia + 18) : null);
}

const char* mboot_cmdline(void) {   return mboot.cmdline;       }
uint32_t    mboot_mmap_length(void) {   return mboot.mmap_length;   }
void *      mboot_mmap_addr(void)   { return mboot.mmap_addr;   }
uint32_t    moot_drives_length(void)    { return mboot.drives_length;   }

elf_section_header_table_t* mboot_kernel_shdr(void) {
    return mboot.syms;
}

void mboot_modules_info(count_t *count, module_t **modules) {
    *count = is_set(mboot.flags, 3) ? mboot.mods_count : 0;
    *modules = mboot.mods_addr;
}

uint mboot_uppermem_kb(void) {
    return mboot.mem_upper;
}

void print_mboot_info(void) {
    k_printf("Booted with: %s\n", mboot.bootloader_name);
    k_printf("Kernel args: %s\n", mboot.cmdline);
    k_printf("Memory: lower=%d Kb, higher=%d Kb\n", mboot.mem_lower, mboot.mem_upper);
    k_printf("Modules count: %d\n", mboot.mods_count);
    k_printf("Drives count: %d\n", mboot.drives_length);
    k_printf("Boot device = 0x%x\n", mboot.boot_device);
}
