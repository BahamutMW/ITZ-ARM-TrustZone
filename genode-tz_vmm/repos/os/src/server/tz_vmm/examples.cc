#include <itz.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <examples.h>

#define PAGE_SIZE 1 << 12

void map_addr(Vm *_vm, unsigned char * data, unsigned long length){

    unsigned char *memory = malloc(PAGE_SIZE);


    /* this is the address to map */
    char *addr_str = data;
    addr_t addr = (addr_t) strtoul(addr_str, NULL, 16);

    /* get the symbol's memory page*/ 
    if (VMI_FAILURE == vmi_read_va(_vm, addr, 0, memory, 16)) {
        printf("failed to map memory.\n");
        goto error_exit;
    }
    vmi_print_hex(data, PAGE_SIZE);

}

