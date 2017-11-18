
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

    #include <errno.h>
    #include <sys/mman.h>
    #include <stdio.h>

    #include <inttypes.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <itz.h>

using namespace Vmm;
using namespace Genode;


///LIBVMI MODIFIED

/**
 * Stub Functions (They do absolutely nothing!) 
 * Descriptions are according to LibVMI API
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

//Initialize or reinitialize the paging specific functionality of LibVMI.
/*page_mode_t vmi_init_paging (Ṽm *_vm, uint8_t force_reinit){
    return VMI_PM_AARCH32;

}*/

//Destroys an instance by freeing memory and closing any open handles.
status_t vmi_destroy (Vm *_vm){
    return VMI_SUCCESS;

}



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
  Genode::printf("Content of the virtual address: %s\n", (char*)_buf);
  return count;

      //TODO Test Function
}

    //Read 32 bits from a given virtual address
status_t vmi_read_8_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of 8 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_16_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of 16 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_32_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value){
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
  value = (uint32_t*)_buf;
  Genode::printf("Content of 32 bits of the virtual address: %s\n", (char*)_buf);
  return VMI_SUCCESS;
}

    //Read 32 bits from a given virtual address
status_t vmi_read_64_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of 64 bits of the virtual address: %s\n", (char*)_buf);
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

      //TODO
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
  Genode::printf("Content of the physical address: %s\n", (char*)_buf);
  return 0;

}


    //Reads 8 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_8_pa (Vm *_vm, uint64_t paddr, uint32_t * value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of 8 bits of the physical address: %s\n", (char*)_buf);
  return VMI_SUCCESS;

}


    //Reads 16 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_16_pa (Vm *_vm, uint64_t paddr, uint32_t * value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of 16 bits of the physical address: %s\n", (char*)_buf);
  return VMI_SUCCESS;

}


    //Reads 32 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_32_pa (Vm *_vm, uint64_t paddr, uint32_t * value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of of the physical address: %s\n", (char*)_buf);
  return VMI_SUCCESS;

}


    //Reads 64 bytes from memory located at the physical address paddr and stores the output in buf.
status_t vmi_read_64_pa (Vm *_vm, uint64_t paddr, uint32_t * value){
  void *_buf;
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
  _buf = (void *)(ram->local() + buf_off);
  Genode::printf("Content of of the physical address: %s\n", (char*)_buf);
  return VMI_SUCCESS;

}

//Performs the translation from a kernel virtual address to a physical address.
 uint64_t vmi_translate_kv2p (Vm *_vm, uint64_t vaddr){
        //TODO
  return 0;
 }

    //Translate kernel symbol to virtual address
uint64_t vmi_translate_ksym2v (Vm *_vm, const char *symbol){
  return 0;

      //TODO
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

  Genode::memcpy(_buf, value, 8);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a virtual address.
status_t vmi_write_16_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint8_t *value){
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

  Genode::memcpy(_buf, value, 16);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

            } 


//Writes 8 bits to memory, given a virtual address.
status_t vmi_write_32_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint8_t * value) {
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

  Genode::memcpy(_buf, value, 32);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
    return VMI_SUCCESS;

  }

  //Writes 8 bits to memory, given a virtual address.
status_t vmi_write_64_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint8_t * value) {
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

  Genode::memcpy(_buf, value, 64);

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
   void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

  Genode::addr_t buf_base = paddr;

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

  Genode::memcpy(_buf, value, 8);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_16_pa(Vm * _vm, uint64_t paddr, uint8_t * value) {
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

  Genode::memcpy(_buf, value, 16);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_32_pa(Vm * _vm, uint64_t paddr, uint8_t * value) {
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

  Genode::memcpy(_buf, value, 32);

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
  return VMI_SUCCESS;

}

//Writes 8 bits to memory, given a physical address.
status_t vmi_write_64_pa(Vm * _vm, uint64_t paddr, uint8_t * value) {
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

  Genode::memcpy(_buf, value, 64);

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