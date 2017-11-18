#ifndef _TZ_VMM__INCLUDE__ITZ_H_
#define _TZ_VMM__INCLUDE__ITZ_H_

typedef enum status {
    VMI_SUCCESS,  /**< return value indicating success */

    VMI_FAILURE   /**< return value indicating failure */
} status_t;


typedef enum page_mode {

    VMI_PM_UNKNOWN, /**< page mode unknown */

    VMI_PM_LEGACY,  /**< x86 32-bit paging */

    VMI_PM_PAE,     /**< x86 PAE paging */

    VMI_PM_IA32E,   /**< x86 IA-32e paging */

    VMI_PM_AARCH32, /**< ARM 32-bit paging */

    VMI_PM_AARCH64  /**< ARM 64-bit paging */

} page_mode_t;


/**
 * typedef for forward compatibility with 64-bit guests
 */
 typedef uint64_t addr_t;

/**
 * type def for consistent pid_t usage
 */

 typedef int32_t vmi_pid_t;


/**
 * Stub Functions (They do absolutely nothing!) 
 * Descriptions are according to LibVMI API
 */

//Initializes access to a specific VM or file given a name. 
status_t vmi_init (Vm *_vm, uint32_t flags, const char *name);

//Initializes access to a specific VM with a custom configuration source.
status_t vmi_init_custom (Vm *_vm, uint32_t flags, const char config);

//Completes initialization. Call this after calling vmi_init with VMI_INIT_PARTIAL.
status_t vmi_init_complete (Vm *_vm, const char *config);

//Completes initialization. Call this after calling vmi_init or vmi_init_custom with VMI_INIT_PARTIAL. 
status_t vmi_init_complete_custom (Vm *_vm, uint32_t flags, const char config);

//Initialize or reinitialize the paging specific functionality of LibVMI.
//TODO FIX ME
//page_mode_t vmi_init_paging (Ṽm *_vm, uint8_t force_reinit);

//Destroys an instance by freeing memory and closing any open handles.
status_t vmi_destroy (Vm *_vm);


/**
 * Translating Functions
 */

//Performs the translation from a kernel virtual address to a physical address.
 uint64_t vmi_translate_kv2p (Vm *_vm, uint64_t vaddr);

//Translate kernel symbol to virtual address
 uint64_t vmi_translate_ksym2v(Vm *_vm, const char * symbol);

//Translate physical address to virtual address
 uint64_t vmi_translate_p2uv(Vm *_vm, uint64_t phy_addr, int32_t pid);

//Translate virtual address to physical address
 uint64_t vmi_translate_uv2p(Vm *_vm,  uint64_t vaddr, int32_t pid);


/**
 * Reading Functions
 */


//Reads count bytes from memory located at the virtual address vaddr and stores the output in buf.
 size_t vmi_read_va(Vm *_vm, uint64_t vaddr, int32_t pid, void * buf, size_t count);

//Read X bits from a given virtual address
 status_t vmi_read_8_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t * value);

 status_t vmi_read_16_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t * value);

 status_t vmi_read_32_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t * value);

 status_t vmi_read_64_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t * value);

//Read string from memory starting at given virtual address
 void * vmi_read_str_va(Vm *_vm, uint64_t vaddr, int32_t pid);

//Reads count bytes from memory located at the physical address paddr and stores the output in buf.
 size_t vmi_read_pa(Vm *_vm, uint64_t paddr, void * buf, size_t count);

//Read X bits from a given physical address
 status_t vmi_read_8_pa(Vm *_vm, uint64_t paddr, uint32_t * value);

 status_t vmi_read_16_pa(Vm *_vm, uint64_t paddr, uint32_t * value);

 status_t vmi_read_32_pa(Vm *_vm, uint64_t paddr, uint32_t * value);

 status_t vmi_read_64_pa(Vm *_vm, uint64_t paddr, uint32_t * value);


//Reads count bytes from memory located at the kernel symbol sym and stores the output in buf.
 size_t vmi_read_ksym (Vm *_vm, char *sym, void *buf, size_t count);

//Reads 8 bits from memory, given a kernel symbol.
 status_t vmi_read_8_ksym (Vm *_vm, char *sym, uint8_t *value);

//Reads 16 bits from memory, given a kernel symbol.
 status_t vmi_read_16_ksym (Vm *_vm, char *sym, uint16_t *value);

//Reads 32 bits from memory, given a kernel symbol.
 status_t vmi_read_32_ksym (Vm *_vm, char *sym, uint32_t *value);

//Reads 64 bits from memory, given a kernel symbol.
 status_t vmi_read_64_ksym (Vm *_vm, char *sym, uint64_t *value);

//Reads an address from memory, given a kernel symbol. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 status_t vmi_read_addr_ksym (Vm *_vm, char *sym, uint32_t *value);

//Reads a null-terminated string from memory, starting at the given kernel symbol. The returned value must be freed by the caller.
 char* vmi_read_str_ksym (Vm *_vm, char *sym);

//Reads an address from memory, given a virtual address. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 status_t vmi_read_addr_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t *value);

//Reads an address from memory, given a physical address. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 status_t vmi_read_addr_pa (Vm *_vm, uint64_t paddr, uint32_t *value);

//Reads a nul terminated string from memory, starting at the given physical address. The returned value must be freed by the caller.
 char* vmi_read_str_pa (Vm *_vm, uint64_t paddr);



/**
 * Writing Functions
 */


//Writes count bytes to memory located at the virtual address vaddr from buf.
 size_t vmi_write_va(Vm *_vm, uint64_t vaddr, int32_t pid, void * buf, size_t count);

//Writes X bits to memory, given a virtual address.
 status_t vmi_write_8_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint8_t * value);
 status_t vmi_write_16_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint16_t * value);
 status_t vmi_write_32_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint32_t * value);
 status_t vmi_write_64_va(Vm *_vm, uint64_t vaddr, int32_t pid, uint64_t * value);


//Writes count bytes to memory located at the physical address paddr from buf.
 size_t vmi_write_pa(Vm *_vm, uint64_t paddr, void * buf, size_t count);

//Writes X bits to memory, given a physical address.
 status_t vmi_write_8_pa(Vm *_vm, uint64_t paddr, uint8_t * value);
 status_t vmi_write_16_pa(Vm *_vm, uint64_t paddr, uint16_t * value);
 status_t vmi_write_32_pa(Vm *_vm, uint64_t paddr, uint32_t * value);
 status_t vmi_write_64_pa(Vm *_vm,  uint64_t paddr, uint64_t * value);

//Writes count bytes to memory located at the kernel symbol sym from buf.
 size_t vmi_write_ksym (Vm *_vm, char *sym, void *buf, size_t count);

//Writes X bits to memory, given a kernel symbol.
 status_t vmi_write_8_ksym (Vm *_vm, char *sym, uint8_t *value);
 status_t vmi_write_16_ksym (Vm *_vm, char *sym, uint16_t *value);
 status_t vmi_write_32_ksym (Vm *_vm, char *sym, uint32_t *value);
 status_t vmi_write_64_ksym (Vm *_vm, char *sym, uint64_t *value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.

 status_t vmi_write_addr_ksym (Vm *_vm, char *sym, uint8_t *value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 status_t vmi_write_addr_va (Vm *_vm, uint64_t vaddr, int32_t pid, uint8_t *value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 status_t vmi_write_addr_pa (Vm *_vm, uint64_t paddr, uint8_t *value);


/**
 * Printing Functions
 */

//Prints out the hex and ascii version of a chunk of bytes from virtual address
 void vmi_print_hex_va(Vm *_vm, uint64_t vaddr, int32_t pid, size_t length);

//Prints out the hex and ascii version of a chunk of bytes from physical address
 void vmi_print_hex_pa(Vm *_vm, uint64_t paddr, size_t length);

//Prints out the hex and ascii version of a chunk of bytes.
 void vmi_print_hex_ksym (Vm *_vm, char *sym, size_t length);

//Prints out the hex and ascii version of a chunk of bytes.
 void vmi_print_hex (unsigned char *data, unsigned long length);

/**
 * Other Functions
 */

//Gets the name of the VM that ITZ is accessing.
 char* vmi_get_name (Vm *_vm);






#endif 
