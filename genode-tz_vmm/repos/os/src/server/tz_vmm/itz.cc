
/* Genode includes */
#include <base/env.h>
#include <base/sleep.h>
#include <base/thread.h>
#include <drivers/board_base.h>
#include <drivers/trustzone.h>
#include <vm_state.h>

/* local includes */
#include <vm.h>
#include <m4if.h>
#include <serial.h>
#include <block.h>
#include <vm_base.h>

/* external includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdint.h>

#include <inttypes.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <itz.h>

#define BUF 128 /* can change the buffer size as well */
#define TOT 100 /* change to accomodate other sizes, change ONCE here */

using namespace Vmm;
using namespace Genode;


///ITZ MODIFIED

/**
 * Stub Functions (They do absolutely nothing!) 
 * Descriptions are according to ITZ API
 */

//Initializes access to a specific VM or file given a name. 
status_t vmi_init (Vm *_vm, uint32_t flags, const char *name){
  return VMI_SUCCESS;
}

//Initializes access to a specific VM with a custom configuration source.
status_t vmi_init_custom (Vm *_vm, uint32_t flags, const char config){
  return VMI_SUCCESS;
}

//Completes initialization. Call this after calling vmi_init with VMI_INIT_PARTIAL.
status_t vmi_init_complete (Vm *_vm, const char *config){
  return VMI_SUCCESS;
}

//Completes initialization. Call this after calling vmi_init or vmi_init_custom with VMI_INIT_PARTIAL. 
status_t vmi_init_complete_custom (Vm *_vm, uint32_t flags, const char config){
  return VMI_SUCCESS;

}

//Initialize or reinitialize the paging specific functionality of ITZ.
page_mode_t vmi_init_paging (Vm *_vm, uint8_t force_reinit){
  return VMI_PM_AARCH32;
}

//Destroys an instance by freeing memory and closing any open handles.
status_t vmi_destroy (Vm *_vm){
  return VMI_SUCCESS;
}

//Reads count bytes from memory located at the kernel symbol sym and stores the output in buf.
size_t vmi_read_ksym(Vm * _vm, char * sym, void * buf, size_t count){
  uint64_t vaddr;
  vaddr = vmi_translate_ksym2v(_vm, sym);
  vmi_read_va(_vm, vaddr, 0, buf, count); 
  return 0;
};


size_t vmi_read_va (Vm *_vm, uint64_t vaddr, int32_t pid, void *buf, size_t count){

  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = count;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);
  buf = _buf;
  free(_buf);
  //Genode::printf("Content of the virtual address: %s\n", (char*)_buf);
  return count;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_8_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint8_t *value){
  uint8_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = 8;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint8_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of 8 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_16_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint16_t *value){
  uint16_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = 16;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint16_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //::printf("Content of 16 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_32_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value){
  uint32_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = 32;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint32_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //value = (uint32_t*)_buf;
  //Genode::printf("Content of 32 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

//Read 64 bits from a given virtual address
status_t vmi_read_64_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint64_t *value){
  uint64_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = 64;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint64_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of 64 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
      //TODO
}

//Read string from memory starting at given virtual address
void* vmi_read_str_va (Vm *_vm, uint64_t vaddr, int32_t pid){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);
  Genode::addr_t buf_base = addr;
  _buf_size = 32;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);
  return _buf;
}

//Reads count bytes from memory located at the physical address paddr and stores the output in buf.
size_t vmi_read_pa (Vm *_vm, uint64_t paddr, void *buf, size_t count){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = count;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);
  buf = _buf;
  free(_buf);
  //Genode::printf("Content of the physical address: %s\n", (char*)_buf);
  return 0;

}


//Reads 8 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_8_pa (Vm *_vm, uint64_t paddr, uint8_t * value){
  uint8_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = 8;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint8_t*)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of 8 bits of the physical address: %x\n", _buf[0]);
  return VMI_SUCCESS;

}


//Reads 16 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_16_pa (Vm *_vm, uint64_t paddr, uint16_t * value){
  uint16_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = 16;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint16_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of 16 bits of the physical address: %x\n %x\n", _buf[0], _buf[1]);
  return VMI_SUCCESS;

}


//Reads 32 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_32_pa (Vm *_vm, uint64_t paddr, uint32_t * value){
  uint32_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = 32;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint32_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of of the physical address: %x\n %x\n %x\n %x\n", _buf[0],_buf[1],_buf[2],_buf[3]);
  return VMI_SUCCESS;

}


//Reads 64 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_64_pa (Vm *_vm, uint64_t paddr, uint64_t * value){
  uint64_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = 64;

  Genode::addr_t  buf_top  = buf_base + _buf_size;
  ram = _vm->ram();
  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }

  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint64_t *)(ram->local() + buf_off);
  value = _buf;
  free(_buf);
  //Genode::printf("Content of of the physical address: %x\n %x\n %x\n %x\n %x\n %x\n %x\n %x\n", _buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6],_buf[7]);
  return VMI_SUCCESS;

}

//Performs the translation from a kernel virtual address to a physical address.
 uint64_t vmi_translate_kv2p (Vm *_vm, uint64_t vaddr){
    Genode::addr_t phys_addr_SW = _vm->va_to_pa(vaddr);
    Genode::printf("Corresponding Physical Address: %lu\n", phys_addr_SW);
    return phys_addr_SW;  return 0;
 }

//Translate kernel symbol to virtual address
uint64_t vmi_translate_ksym2v (Vm *_vm, const char *symbol){
    char line[TOT][BUF];
    FILE *plist;
    plist = NULL;     
    int i;
    i = 0;
    int total;
    total = 0;


    plist = fopen("System.map", "r");
    while(fgets(line[i], BUF, plist)) {
        /* get rid of ending \n from fgets */
        line[i][Genode::strlen(line[i]) - 1] = '\0';
        i++;
    }

    total = i;

    for(i = 0; i < total; ++i){
      if (strstr(line[i], symbol)) {
        char** pair = strsplit(line[i]," ");
        Genode::printf("%s\n", pair[0]);
        return (uint64_t)pair[0];
      } else {

      }
    }

return 0;     
}

//Translate physical address to virtual address
uint64_t vmi_translate_p2uv (Vm *_vm, uint64_t phy_addr, int32_t pid){
      //Genode::Io_mem_connection _ram_iomem(phy_addr, 4);
      //Ram _ram(phy_addr, 4,
      //(Genode::addr_t)Genode::env()->rm_session()->attach(_ram_iomem.dataspace()));

  Ram *  ram;
  ram = _vm->ram();
  Genode::addr_t virt_addr_SW  =  ram->va(phy_addr);
  Genode::printf("Corresponding Virtual Address: %lu\n", virt_addr_SW);
  return virt_addr_SW;
      //Genode::printf("Corresponding Virtual Address: %s\n", _ram.local());
}

uint64_t vmi_translate_uv2p(Vm *_vm, uint64_t vaddr, int32_t pid){
  Genode::addr_t phys_addr_SW = _vm->va_to_pa(vaddr);
  Genode::printf("Corresponding Physical Address: %lu\n", phys_addr_SW);
  return phys_addr_SW;
};

    //Translate normal world virtual address to secure world physical address
    //Genode::addr_t phys_addr_SW = _vm->va_to_pa(virt_addr_NW);

    //Translate secure world physical address to secure world virtual address
    //Genode::addr_t virt_addr_SW  =  ram->va(phys_addr_SW);


//Writes count bytes to memory located at the virtual address vaddr from buf.
size_t vmi_write_va (Vm *_vm, uint64_t vaddr, int32_t pid, void *buf, size_t count){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);

  Genode::addr_t buf_base = addr;

  _buf_size = count;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, buf, count);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return 0;


}

    //Writes 8 bits to memory, given a virtual address.
status_t vmi_write_8_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint8_t *value){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);

  Genode::addr_t buf_base = addr;

  _buf_size = 8;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 1);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a virtual address.
status_t vmi_write_16_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint16_t *value){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);

  Genode::addr_t buf_base = addr;

  _buf_size = 16;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 2);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

} 


//Writes 8 bits to memory, given a virtual address.
status_t vmi_write_32_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint32_t * value) {
   void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);

  Genode::addr_t buf_base = addr;

  _buf_size = 32;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 4);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
    return VMI_SUCCESS;

  }

  //Writes 8 bits to memory, given a virtual address.
status_t vmi_write_64_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint64_t * value) {
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  uint64_t addr;
  addr = vmi_translate_uv2p(_vm, vaddr, 0);

  Genode::addr_t buf_base = addr;

  _buf_size = 64;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 1);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

  }

    //Writes count bytes to memory located at the physical address paddr from buf.
size_t vmi_write_pa (Vm *_vm, uint64_t paddr, void *buf, size_t count){
  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;

  _buf_size = count;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, buf, count);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return 0;
}

  //Writes 8 bits to memory, given a physical address.
status_t vmi_write_8_pa(Vm * _vm, uint64_t paddr, uint8_t * value) {
  uint8_t *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;
  _buf_size = sizeof(value);

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (uint8_t *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, sizeof(value));

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_16_pa(Vm * _vm, uint64_t paddr, uint16_t * value) {
   void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;

  _buf_size = 16;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 2);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_32_pa(Vm * _vm, uint64_t paddr, uint32_t * value) {
   void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;

  _buf_size = 32;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 4);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_64_pa(Vm * _vm, uint64_t paddr, uint64_t * value) {
   void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;

  _buf_size = 64;

  Genode::addr_t buf_top  = buf_base + _buf_size;
  ram = _vm->ram();

  Genode::addr_t ram_top  = ram->base() + ram->size();

  bool buf_err;
  buf_err  = buf_top  <= buf_base;
  buf_err |= buf_base <  ram->base();
  buf_err |= buf_top  >= ram_top;
  if (buf_err) {
    PERR("Illegal block buffer constraints");
  }
  Genode::addr_t buf_off = buf_base - ram->base();
  _buf = (void *)(ram->local() + buf_off);

  Genode::memcpy(_buf, value, 1);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Prints out the hex and ascii version of a chunk of bytes from virtual address
void vmi_print_hex_va(Vm * _vm, uint64_t vaddr, int32_t pid, size_t length) {
  Genode::addr_t phys_addr_SW = _vm -> va_to_pa(vaddr);
  vmi_print_hex_pa(_vm, phys_addr_SW, length);

}

//Prints out the hex and ascii version of a chunk of bytes from physical address
void vmi_print_hex_pa(Vm * _vm, uint64_t paddr, size_t length) {
  unsigned int i;
  unsigned char buff[17];
  unsigned char * pc = (unsigned char * ) paddr;

  // Process every byte in the data.
  for (i = 0; i < length; i++) {
    // Multiple of 16 means new line (with line offset).

    if ((i % 16) == 0) {
      // Just don't print ASCII for the zeroth line.
      if (i != 0)
        Genode::printf("  %s\n", buff);

      // Output the offset.
      Genode::printf("  %04x ", i);
    }

    // Now the hex code for the specific character.
    Genode::printf(" %02x", pc[i]);

    // And store a printable ASCII character for later.
    if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
      buff[i % 16] = '.';
    } else {
      buff[i % 16] = pc[i];
    }

    buff[(i % 16) + 1] = '\0';
  }

  // Pad out last line if not exactly 16 characters.
  while ((i % 16) != 0) {
    Genode::printf("   ");
    i++;
  }
}


//Prints out the hex and ascii version of a chunk of bytes.
void vmi_print_hex_ksym(Vm * _vm, char * sym, size_t length){
  uint64_t paddr;
  paddr = vmi_translate_ksym2v(_vm,sym);
  vmi_print_hex_pa(_vm, paddr, length);
}

//Prints out the hex and ascii version of a chunk of bytes.
void vmi_print_hex(unsigned char * data, unsigned long length){
  unsigned char buff[17];
  unsigned int i;

// Process every byte in the data.
  for (i = 0; i < length; i++) {
    // Multiple of 16 means new line (with line offset).

    if ((i % 16) == 0) {
      // Just don't print ASCII for the zeroth line.
      if (i != 0)
        Genode::printf("  %s\n", buff);

      // Output the offset.
      Genode::printf("  %04x ", i);
    }

    // Now the hex code for the specific character.
    Genode::printf(" %02x", data[i]);

    // And store a printable ASCII character for later.
    if ((data[i] < 0x20) || (data[i] > 0x7e)) {
      buff[i % 16] = '.';
    } else {
      buff[i % 16] = data[i];
    }

    buff[(i % 16) + 1] = '\0';
  }

  // Pad out last line if not exactly 16 characters.
  while ((i % 16) != 0) {
    Genode::printf("   ");
    i++;
  }
}

/**
 * Get and Set Functions
 */

//Gets the name of the VM that ITZ is accessing.
char * vmi_get_name(Vm * _vm){
  char * name; 
  name = (char*)"Genode";
  return name;

}

//Gets the id of the VM that ITZ is accessing.
unsigned long vmi_get_vmid(Vm * _vm){
  return 0;

}

// Gets the current access mode for ITZ, which tells what resource is being using to access the memory (e.g., VMI_XEN, VMI_KVM, or VMI_FILE).
uint32_t vmi_get_access_mode(Vm * _vm){
  return 0;
}

// Gets the current page mode for ITZ, which tells what type of address translation is in use (e.g., VMI_PM_LEGACY, VMI_PM_PAE, or VMI_PM_IA32E).
page_mode_t vmi_get_page_mode(Vm * _vm){
    return VMI_PM_AARCH32;
}

// Gets the current address width for the given vmi_instance_t
uint8_t vmi_get_address_width(Vm * _vm){
  return Trustzone::NONSECURE_RAM_SIZE + Trustzone::SECURE_RAM_SIZE;
}

// Get the OS type that ITZ is currently accessing. This is simple windows or linux (no version information).
os_t vmi_get_ostype(Vm * _vm){
  return VMI_OS_LINUX;
}

// Get the version of Windows that ITZ is currently accessing. This is the simple Windows version - no service pack or patch level is provided.
win_ver_t vmi_get_winver(Vm * _vm){
  return VMI_OS_WINDOWS_NONE;
}

// Get string represenatation of the version of Windows that ITZ is currently accessing.
const char * vmi_get_winver_str(Vm * _vm){
  return 0;
}

// Get the version of Windows based on the provided KDVB address. This is the simple Windows version - no service pack or patch level is provided.
win_ver_t vmi_get_winver_manual(Vm * _vm, uint64_t kdvb_pa){
  return VMI_OS_WINDOWS_NONE;
}

//Get the memory offset associated with the given offset_name. Valid names include everything in the /etc/ITZ.conf file.
uint64_t vmi_get_offset(Vm * _vm, char * offset_name){
  uint64_t offset = 0;

  if (Genode::strcmp("pid",offset_name)==0){
    offset = (uint64_t) 0xe8;
  }else if (Genode::strcmp("name",offset_name)==0){
    offset = (uint64_t) 0x8;
  }else if (Genode::strcmp("tasks",offset_name)==0){
    offset = (uint64_t) 0xac;
  }
  
  return offset;
}

// Gets the memory size of the guest or file that is currently accessing. This is effectively the max physical address that you can access in the system.
uint64_t vmi_get_memsize(Vm * _vm){
  return Trustzone::NONSECURE_RAM_SIZE;
}

//    Gets the memory size of the guest that  is accessing. This information is required for any interaction with of VCPU registers.
unsigned int vmi_get_num_vcpus(Vm * _vm){
  return 0;
}

//    Gets the current value of a VCPU register. This currently only supports control registers. When is accessing a raw memory file, this function will fail.
status_t vmi_get_vcpureg(Vm * _vm, reg_t * value, registers_t reg, unsigned long vcpu){
  return VMI_SUCCESS;
}

//    Sets the current value of a VCPU register. This currently only supports control registers. When ITZ is accessing a raw memory file, this function will fail. Operating upon an unpaused VM with this function is likely to have unexpected results.
status_t vmi_set_vcpureg(Vm * _vm, reg_t value, registers_t reg, unsigned long vcpu){
  return VMI_SUCCESS;
}

//Pauses the VM. Use vmi_resume_vm to resume the VM after pausing it. If accessing a memory file, this has no effect.
status_t vmi_pause_vm(Vm * _vm){
  return VMI_SUCCESS;
}

//Resumes the VM.
status_t vmi_resume_vm(Vm * _vm){
  return VMI_SUCCESS;
}

/**
 * Cache Functions
 */

// Removes all entries from ITZ internal virtual to physical address cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_v2pcache_flush(Vm * _vm){
//TODO
}

//    Adds one entry to ITZ's internal virtual to physical address cache.
void vmi_v2pcache_add(Vm * _vm, uint64_t va, uint64_t dtb, uint64_t pa){
//TODO

}

//    Removes all entries from ITZs internal kernel symbol to virtual address cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_symcache_flush(Vm * _vm){
//TODO

}

//    Adds one entry to ITZ's internal symbol to virtual address cache.
void vmi_symcache_add(Vm * _vm, uint64_t base_addr, vmi_pid_t pid, char * sym, uint64_t va){
//TODO


}

//    Removes all entries from ITZs internal RVA to symbol cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_rvacache_flush(Vm * _vm){
//TODO

}

//    Adds one entry to ITZ's internal RVA to symbol cache.
void vmi_rvacache_add(Vm * _vm, uint64_t base_addr, vmi_pid_t pid, uint64_t rva, char * sym){
//TODO

}

//    Removes all entries from ITZs internal pid to directory table base cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_pidcache_flush(Vm * _vm){
//TODO

}

//    Adds one entry to ITZs internal pid to directory table base cache.
void vmi_pidcache_add(Vm * _vm, vmi_pid_t pid, uint64_t dtb){
//TODO

}

// Additional Functions

//Use this to get symbol name from address
const char* firstCharAfterSpace(const char* input) {
   const char* starting;
   starting = input;
   while (*starting++ != ' ');

   return starting;
} 

//Split into pair address/symbol
char** strsplit(const char* s, const char* del) {
  void* data;
  char* _s;
  _s = (char*)s;
  const char** ptrs;
  unsigned int ptrsSize;
  unsigned int  sLen;
  sLen = Genode::strlen(s);
  unsigned int delLen;
  delLen = Genode::strlen(del);
  unsigned int nbWords;
  nbWords = 1;

  while ((_s = strstr(_s, del))){
    _s += delLen;
    ++nbWords;
  }

  ptrsSize = (nbWords + 1) * sizeof(char*);
  data = malloc(ptrsSize + sLen + 1);
  ptrs = (const char**)data;
  if (data) {
    *ptrs = _s = strcpy(((char*)data) + ptrsSize, s);
    if (nbWords > 1) {
      while ((_s = strstr(_s, del))) {
        *_s = '\0';
        _s += delLen;
        *++ptrs = _s;
      }
    }
    *++ptrs = NULL;
  }
  return (char**)data;
}

void md5(void * string) {
  PDBG("MD5 \n");
  unsigned char digest[16];

  MD5_CTX ctx;
  MD5_Init( & ctx);
  MD5_Update( & ctx, string, sizeof(string));
  MD5_Final(digest, & ctx);

  char mdString[33];
  int i;
  for (i = 0; i < 16; i++)
    sprintf( & mdString[i * 2], "%02x", (unsigned int) digest[i]);

  Genode::printf("MD5 digest: %s\n", mdString);
  PDBG("Exit From MD5\n");

}

void sha1(void * string) {
  PDBG("SHA1 \n");

  unsigned char digest[SHA_DIGEST_LENGTH];

  SHA1((unsigned char * ) & string, sizeof(string), (unsigned char * ) & digest);

  char mdString[SHA_DIGEST_LENGTH * 2 + 1];

  for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    sprintf( & mdString[i * 2], "%02x", (unsigned int) digest[i]);

  Genode::printf("SHA1 digest: %s\n", mdString);
  PDBG("Exit From SHA1\n");

}

void sha256(void * string) {
  PDBG("SHA256 \n");

  unsigned char digest[SHA256_DIGEST_LENGTH];

  SHA256_CTX ctx;
  SHA256_Init( & ctx);
  SHA256_Update( & ctx, string, sizeof(string));
  SHA256_Final(digest, & ctx);

  char mdString[SHA256_DIGEST_LENGTH * 2 + 1];
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf( & mdString[i * 2], "%02x", (unsigned int) digest[i]);

  Genode::printf("SHA256 digest: %s\n", mdString);
  PDBG(" SHA256\n");

}

void sha512(void * string) {
  PDBG("SHA512 \n");

  unsigned char digest[SHA512_DIGEST_LENGTH];

  SHA512((unsigned char * ) & string, sizeof(string), (unsigned char * ) & digest);

  char mdString[SHA512_DIGEST_LENGTH * 2 + 1];

  for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    sprintf( & mdString[i * 2], "%02x", (unsigned int) digest[i]);

  Genode::printf("SHA512 digest: %s\n", mdString);
  PDBG("Exit From SHA512 \n");

}

void hashselect(char * hashname, void * hashcontent) {
  if (Genode::strcmp("md5", hashname) == 0) {
    md5(hashcontent);
  } else if (Genode::strcmp("sha1", hashname) == 0) {
    sha1(hashcontent);
  } else if (Genode::strcmp("sha256", hashname) == 0) {
    sha256(hashcontent);
  } else if (Genode::strcmp("sha512", hashname) == 0) {
    sha512(hashcontent);
  }
}