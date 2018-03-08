#ifndef _TZ_VMM__INCLUDE__ITZ_H_
#define _TZ_VMM__INCLUDE__ITZ_H_

typedef enum status {
  VMI_SUCCESS,
  /**< return value indicating success */

  VMI_FAILURE /**< return value indicating failure */
}
status_t;

typedef enum page_mode {

  VMI_PM_UNKNOWN,
  /**< page mode unknown */

  VMI_PM_LEGACY,
  /**< x86 32-bit paging */

  VMI_PM_PAE,
  /**< x86 PAE paging */

  VMI_PM_IA32E,
  /**< x86 IA-32e paging */

  VMI_PM_AARCH32,
  /**< ARM 32-bit paging */

  VMI_PM_AARCH64 /**< ARM 64-bit paging */

}
page_mode_t;


typedef enum os {

    VMI_OS_UNKNOWN,  /**< OS type is unknown */

    VMI_OS_LINUX,    /**< OS type is Linux */

    VMI_OS_WINDOWS   /**< OS type is Windows */
} os_t;

/**
 * Windows version enumeration. The values of the enum
 * represent the size of KDBG structure up to Windows 8.
 * At Windows 10 the KDBG based scan is no longer supported
 * and thus at that point the value itself has no magic value.
 */
typedef enum win_ver {

    VMI_OS_WINDOWS_NONE,    /**< Not Windows */
    VMI_OS_WINDOWS_UNKNOWN, /**< Is Windows, not sure which */

    VMI_OS_WINDOWS_2000     = 0x0208U, /**< Magic value for Windows 2000 */
    VMI_OS_WINDOWS_XP       = 0x0290U, /**< Magic value for Windows XP */
    VMI_OS_WINDOWS_2003     = 0x0318U, /**< Magic value for Windows 2003 */
    VMI_OS_WINDOWS_VISTA    = 0x0328U, /**< Magic value for Windows Vista */
    VMI_OS_WINDOWS_2008     = 0x0330U, /**< Magic value for Windows 2008 */
    VMI_OS_WINDOWS_7        = 0x0340U, /**< Magic value for Windows 7 */
    VMI_OS_WINDOWS_8        = 0x0360U, /**< Magic value for Windows 8 */
    VMI_OS_WINDOWS_10,
} win_ver_t;

typedef enum page_size {

    VMI_PS_UNKNOWN  = 0ULL,         /**< page size unknown */

    VMI_PS_1KB      = 0x400ULL,     /**< 1KB */

    VMI_PS_4KB      = 0x1000ULL,    /**< 4KB */

    VMI_PS_16KB     = 0x4000ULL,    /**< 16KB */

    VMI_PS_64KB     = 0x10000ULL,   /**< 64KB */

    VMI_PS_1MB      = 0x100000ULL,  /**< 1MB */

    VMI_PS_2MB      = 0x200000ULL,  /**< 2MB */

    VMI_PS_4MB      = 0x400000ULL,  /**< 4MB */

    VMI_PS_16MB     = 0x1000000ULL, /**< 16MB */

    VMI_PS_32MB     = 0x2000000ULL, /**< 32MB */

    VMI_PS_512MB    = 0x2000000ULL,  /**< 512MB */

    VMI_PS_1GB      = 0x4000000ULL,  /**< 1GB */

} page_size_t;

#define VMI_INVALID_DOMID ~0ULL /**< invalid domain id */

typedef uint64_t reg_t;

/**
 * The following definitions are used where the API defines
 * either reg_t or registers_t.
 *
 * x86_* registers
 */
#define EAX              0
#define EBX              1
#define ECX              2
#define EDX              3
#define EBP              4
#define ESI              5
#define EDI              6
#define ESP              7

#define EIP              8
#define EFLAGS           9

#define RAX        EAX
#define RBX        EBX
#define RCX        ECX
#define RDX        EDX
#define RBP        EBP
#define RSI        ESI
#define RDI        EDI
#define RSP        ESP

#define RIP        EIP
#define RFLAGS     EFLAGS

#define R8               10
#define R9               11
#define R10              12
#define R11              13
#define R12              14
#define R13              15
#define R14              16
#define R15              17

#define CR0              18
#define CR2              19
#define CR3              20
#define CR4              21
#define XCR0             22

#define DR0              23
#define DR1              24
#define DR2              25
#define DR3              26
#define DR6              27
#define DR7              28

#define CS_SEL           29
#define DS_SEL           30
#define ES_SEL           31
#define FS_SEL           32
#define GS_SEL           33
#define SS_SEL           34
#define TR_SEL           35
#define LDTR_SEL         36

#define CS_LIMIT         37
#define DS_LIMIT         38
#define ES_LIMIT         39
#define FS_LIMIT         40
#define GS_LIMIT         41
#define SS_LIMIT         42
#define TR_LIMIT         43
#define LDTR_LIMIT       44
#define IDTR_LIMIT       45
#define GDTR_LIMIT       46

#define CS_BASE          47
#define DS_BASE          48
#define ES_BASE          49
#define FS_BASE          50
#define GS_BASE          51
#define SS_BASE          52
#define TR_BASE          53
#define LDTR_BASE        54
#define IDTR_BASE        55
#define GDTR_BASE        56

#define CS_ARBYTES       57
#define DS_ARBYTES       58
#define ES_ARBYTES       59
#define FS_ARBYTES       60
#define GS_ARBYTES       61
#define SS_ARBYTES       62
#define TR_ARBYTES       63
#define LDTR_ARBYTES     64

#define SYSENTER_CS      65
#define SYSENTER_ESP     66
#define SYSENTER_EIP     67

#define SHADOW_GS        68
#define TSC              69

#define MSR_FLAGS        70
#define MSR_LSTAR        71
#define MSR_CSTAR        72
#define MSR_SYSCALL_MASK 73
#define MSR_EFER         74
#define MSR_TSC_AUX      75

#define MSR_STAR                    119
#define MSR_SHADOW_GS_BASE          120
#define MSR_MTRRfix64K_00000        121
#define MSR_MTRRfix16K_80000        122
#define MSR_MTRRfix16K_A0000        123
#define MSR_MTRRfix4K_C0000         124
#define MSR_MTRRfix4K_C8000         125
#define MSR_MTRRfix4K_D0000         126
#define MSR_MTRRfix4K_D8000         127
#define MSR_MTRRfix4K_E0000         128
#define MSR_MTRRfix4K_E8000         129
#define MSR_MTRRfix4K_F0000         130
#define MSR_MTRRfix4K_F8000         131
#define MSR_MTRRdefType             132
#define MSR_IA32_MC0_CTL            133
#define MSR_IA32_MC0_STATUS         134
#define MSR_IA32_MC0_ADDR           135
#define MSR_IA32_MC0_MISC           136
#define MSR_IA32_MC1_CTL            137
#define MSR_IA32_MC0_CTL2           138
#define MSR_AMD_PATCHLEVEL          139
#define MSR_AMD64_TSC_RATIO         140
#define MSR_IA32_P5_MC_ADDR         141
#define MSR_IA32_P5_MC_TYPE         142
#define MSR_IA32_TSC                143
#define MSR_IA32_PLATFORM_ID        144
#define MSR_IA32_EBL_CR_POWERON     145
#define MSR_IA32_EBC_FREQUENCY_ID   146
#define MSR_IA32_FEATURE_CONTROL    147
#define MSR_IA32_SYSENTER_CS        148
#define MSR_IA32_SYSENTER_ESP       149
#define MSR_IA32_SYSENTER_EIP       150
#define MSR_IA32_MISC_ENABLE        151
#define MSR_HYPERVISOR              152

/**
 * Special generic case for specifying arbitrary MSRs not formally listed above.
 */
#define MSR_UNDEFINED 153


/**
 * Special generic case for handling MSRs, given their understandably
 * generic treatment for events in Xen and elsewhere. Not relevant for
 * vCPU get/set of register data.
 */
#define MSR_ALL          76

/**
 * ARM32 Registers
 */
#define SCTLR            77
#define CPSR             78

#define TTBCR            79
#define TTBR0            80
#define TTBR1            81

#define R0               82
#define R1               83
#define R2               84
#define R3               85
#define R4               86
#define R5               87
#define R6               88
#define R7               89

/* R8-R15 already defined */

#define SPSR_SVC         90
#define SPSR_FIQ         91
#define SPSR_IRQ         92
#define SPSR_UND         93
#define SPSR_ABT         94

#define LR_IRQ           95
#define SP_IRQ           96

#define LR_SVC           97
#define SP_SVC           98

#define LR_ABT           99
#define SP_ABT           100

#define LR_UND           101
#define SP_UND           102

#define R8_FIQ           103
#define R9_FIQ           104
#define R10_FIQ          105
#define R11_FIQ          106
#define R12_FIQ          107

#define SP_FIQ           108
#define LR_FIQ           109

#define PC               118

/**
 * Compatibility naming
 */
#define SP_USR      R13
#define LR_USR      R14
#define PC32        PC

/**
 * ARM64 register
 */
#define SP_EL0           110
#define SP_EL1           111
#define ELR_EL1          112

/**
 * Many ARM64 registers are architecturally mapped over ARM32 registers
 */
#define X0          R0
#define X1          R1
#define X2          R2
#define X3          R3
#define X4          R4
#define X5          R5
#define X6          R6
#define X7          R7
#define X8          R8
#define X9          R9
#define X10         R10
#define X11         R11
#define X12         R12
#define X13         R13
#define X14         R14
#define X15         R15
#define X16         LR_IRQ
#define X17         SP_IRQ
#define X18         LR_SVC
#define X19         SP_SVC
#define X20         LR_ABT
#define X21         SP_ABT
#define X22         LR_UND
#define X23         SP_UND
#define X24         R8_FIQ
#define X25         R9_FIQ
#define X26         R10_FIQ
#define X27         R11_FIQ
#define X28         R12_FIQ
#define X29         SP_FIQ
#define X30         LR_FIQ

#define PC64        PC
#define SPSR_EL1    SPSR_SVC
#define TCR_EL1     TTBCR

/*
 * Commonly used x86 registers
 */
typedef struct x86_regs {
    uint64_t rax;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rflags;
    uint64_t dr7;
    uint64_t rip;
    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;
    uint64_t sysenter_cs;
    uint64_t sysenter_esp;
    uint64_t sysenter_eip;
    uint64_t msr_efer;
    uint64_t msr_star;
    uint64_t msr_lstar;
    uint64_t fs_base;
    uint64_t gs_base;
    uint32_t cs_arbytes;
    uint32_t _pad;
} x86_registers_t;

typedef struct arm_registers {
    uint64_t ttbr0;
    uint64_t ttbr1;
    uint64_t ttbcr;
    uint64_t pc;
    uint32_t cpsr;
    uint32_t _pad;
} arm_registers_t;

typedef struct registers {
    union {
        x86_registers_t x86;
        arm_registers_t arm;
    };
} registers_t;



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
status_t vmi_init(Vm * _vm, uint32_t flags,
  const char * name);

//Initializes access to a specific VM with a custom configuration source.
status_t vmi_init_custom(Vm * _vm, uint32_t flags,
  const char config);

//Completes initialization. Call this after calling vmi_init with VMI_INIT_PARTIAL.
status_t vmi_init_complete(Vm * _vm,
  const char * config);

//Completes initialization. Call this after calling vmi_init or vmi_init_custom with VMI_INIT_PARTIAL. 
status_t vmi_init_complete_custom(Vm * _vm, uint32_t flags,
  const char config);

//Initialize or reinitialize the paging specific functionality of LibVMI.
page_mode_t vmi_init_paging (Vm *_vm, uint8_t force_reinit);

//Destroys an instance by freeing memory and closing any open handles.
status_t vmi_destroy(Vm * _vm);

/**
 * Translating Functions
 */

//Performs the translation from a kernel virtual address to a physical address.
uint64_t vmi_translate_kv2p(Vm * _vm, uint64_t vaddr);

//Translate kernel symbol to virtual address
uint64_t vmi_translate_ksym2v(Vm * _vm,
  const char * symbol);

//Translate physical address to virtual address
uint64_t vmi_translate_p2uv(Vm * _vm, uint64_t phy_addr, int32_t pid);

//Translate virtual address to physical address
uint64_t vmi_translate_uv2p(Vm * _vm, uint64_t vaddr, int32_t pid);

/**
 * Reading Functions
 */

//Reads count bytes from memory located at the virtual address vaddr and stores the output in buf.
size_t vmi_read_va(Vm * _vm, uint64_t vaddr, int32_t pid, void * buf, size_t count);

//Read X bits from a given virtual address
status_t vmi_read_8_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint8_t * value);

status_t vmi_read_16_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint16_t * value);

status_t vmi_read_32_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint32_t * value);

status_t vmi_read_64_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint64_t * value);

//Read string from memory starting at given virtual address
void * vmi_read_str_va(Vm * _vm, uint64_t vaddr, int32_t pid);

//Reads count bytes from memory located at the physical address paddr and stores the output in buf.
size_t vmi_read_pa(Vm * _vm, uint64_t paddr, void * buf, size_t count);

//Read X bits from a given physical address
status_t vmi_read_8_pa(Vm * _vm, uint64_t paddr, uint8_t * value);

status_t vmi_read_16_pa(Vm * _vm, uint64_t paddr, uint16_t * value);

status_t vmi_read_32_pa(Vm * _vm, uint64_t paddr, uint32_t * value);

status_t vmi_read_64_pa(Vm * _vm, uint64_t paddr, uint64_t * value);

//Reads count bytes from memory located at the kernel symbol sym and stores the output in buf.
size_t vmi_read_ksym(Vm * _vm, char * sym, void * buf, size_t count);

//Reads 8 bits from memory, given a kernel symbol.
status_t vmi_read_8_ksym(Vm * _vm, char * sym, uint8_t * value);

//Reads 16 bits from memory, given a kernel symbol.
status_t vmi_read_16_ksym(Vm * _vm, char * sym, uint16_t * value);

//Reads 32 bits from memory, given a kernel symbol.
status_t vmi_read_32_ksym(Vm * _vm, char * sym, uint32_t * value);

//Reads 64 bits from memory, given a kernel symbol.
status_t vmi_read_64_ksym(Vm * _vm, char * sym, uint64_t * value);

//Reads an address from memory, given a kernel symbol. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
status_t vmi_read_addr_ksym(Vm * _vm, char * sym, uint32_t * value);

//Reads a null-terminated string from memory, starting at the given kernel symbol. The returned value must be freed by the caller.
char * vmi_read_str_ksym(Vm * _vm, char * sym);

//Reads an address from memory, given a virtual address. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
status_t vmi_read_addr_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint32_t * value);

//Reads an address from memory, given a physical address. The number of bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
status_t vmi_read_addr_pa(Vm * _vm, uint64_t paddr, uint32_t * value);

//Reads a nul terminated string from memory, starting at the given physical address. The returned value must be freed by the caller.
char * vmi_read_str_pa(Vm * _vm, uint64_t paddr);

/**
 * Writing Functions
 */

//Writes count bytes to memory located at the virtual address vaddr from buf.
size_t vmi_write_va(Vm * _vm, uint64_t vaddr, int32_t pid, void * buf, size_t count);

//Writes X bits to memory, given a virtual address.
status_t vmi_write_8_va(Vm * _vm, uint64_t vaddr, int8_t pid, uint8_t * value);
status_t vmi_write_16_va(Vm * _vm, uint64_t vaddr, int16_t pid, uint16_t * value);
status_t vmi_write_32_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint32_t * value);
status_t vmi_write_64_va(Vm * _vm, uint64_t vaddr, int64_t pid, uint64_t * value);

//Writes count bytes to memory located at the physical address paddr from buf.
size_t vmi_write_pa(Vm * _vm, uint64_t paddr, void * buf, size_t count);

//Writes X bits to memory, given a physical address.
status_t vmi_write_8_pa(Vm * _vm, uint64_t paddr, uint8_t * value);
status_t vmi_write_16_pa(Vm * _vm, uint64_t paddr, uint16_t * value);
status_t vmi_write_32_pa(Vm * _vm, uint64_t paddr, uint32_t * value);
status_t vmi_write_64_pa(Vm * _vm, uint64_t paddr, uint64_t * value);

//Writes count bytes to memory located at the kernel symbol sym from buf.
size_t vmi_write_ksym(Vm * _vm, char * sym, void * buf, size_t count);

//Writes X bits to memory, given a kernel symbol.
status_t vmi_write_8_ksym(Vm * _vm, char * sym, uint8_t * value);
status_t vmi_write_16_ksym(Vm * _vm, char * sym, uint16_t * value);
status_t vmi_write_32_ksym(Vm * _vm, char * sym, uint32_t * value);
status_t vmi_write_64_ksym(Vm * _vm, char * sym, uint64_t * value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.

status_t vmi_write_addr_ksym(Vm * _vm, char * sym, uint8_t * value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
status_t vmi_write_addr_va(Vm * _vm, uint64_t vaddr, int32_t pid, uint8_t * value);

//Writes the address to memory. The number of bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
status_t vmi_write_addr_pa(Vm * _vm, uint64_t paddr, uint8_t * value);

/**
 * Printing Functions
 */

//Prints out the hex and ascii version of a chunk of bytes from virtual address
void vmi_print_hex_va(Vm * _vm, uint64_t vaddr, int32_t pid, size_t length);

//Prints out the hex and ascii version of a chunk of bytes from physical address
void vmi_print_hex_pa(Vm * _vm, uint64_t paddr, size_t length);

//Prints out the hex and ascii version of a chunk of bytes.
void vmi_print_hex_ksym(Vm * _vm, char * sym, size_t length);

//Prints out the hex and ascii version of a chunk of bytes.
void vmi_print_hex(unsigned char * data, unsigned long length);

/**
 * Get and Set Functions
 */

//Gets the name of the VM that ITZ is accessing.
char * vmi_get_name(Vm * _vm);

//Gets the id of the VM that LibVMI is accessing.
unsigned long vmi_get_vmid(Vm * _vm);

// Gets the current access mode for LibVMI, which tells what resource is being using to access the memory (e.g., VMI_XEN, VMI_KVM, or VMI_FILE).
uint32_t vmi_get_access_mode(Vm * _vm);

// Gets the current page mode for LibVMI, which tells what type of address translation is in use (e.g., VMI_PM_LEGACY, VMI_PM_PAE, or VMI_PM_IA32E).
page_mode_t vmi_get_page_mode(Vm * _vm);

// Gets the current address width for the given vmi_instance_t
uint8_t vmi_get_address_width(Vm * _vm);

// Get the OS type that LibVMI is currently accessing. This is simple windows or linux (no version information).
os_t vmi_get_ostype(Vm * _vm);

// Get the version of Windows that LibVMI is currently accessing. This is the simple Windows version - no service pack or patch level is provided.
win_ver_t vmi_get_winver(Vm * _vm);

// Get string represenatation of the version of Windows that LibVMI is currently accessing.
const char * vmi_get_winver_str(Vm * _vm);

// Get the version of Windows based on the provided KDVB address. This is the simple Windows version - no service pack or patch level is provided.
win_ver_t vmi_get_winver_manual(Vm * _vm, addr_t kdvb_pa);

//Get the memory offset associated with the given offset_name. Valid names include everything in the /etc/libvmi.conf file.
uint64_t vmi_get_offset(Vm * _vm, char * offset_name);

// Gets the memory size of the guest or file that LibVMI is currently accessing. This is effectively the max physical address that you can access in the system.
uint64_t vmi_get_memsize(Vm * _vm);

//    Gets the memory size of the guest that LibVMI is accessing. This information is required for any interaction with of VCPU registers.
unsigned int vmi_get_num_vcpus(Vm * _vm);

//    Gets the current value of a VCPU register. This currently only supports control registers. When LibVMI is accessing a raw memory file, this function will fail.
status_t vmi_get_vcpureg(Vm * _vm, reg_t * value, registers_t reg, unsigned long vcpu);

//    Sets the current value of a VCPU register. This currently only supports control registers. When LibVMI is accessing a raw memory file, this function will fail. Operating upon an unpaused VM with this function is likely to have unexpected results.
status_t vmi_set_vcpureg(Vm * _vm, reg_t value, registers_t reg, unsigned long vcpu);

//Pauses the VM. Use vmi_resume_vm to resume the VM after pausing it. If accessing a memory file, this has no effect.
status_t vmi_pause_vm(Vm * _vm);

//Resumes the VM.
status_t vmi_resume_vm(Vm * _vm);

/**
 * Cache Functions
 */

// Removes all entries from LibVMI internal virtual to physical address cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_v2pcache_flush(Vm * _vm);

//    Adds one entry to LibVMI's internal virtual to physical address cache.
void vmi_v2pcache_add(Vm * _vm, addr_t va, addr_t dtb, addr_t pa);

//    Removes all entries from LibVMIs internal kernel symbol to virtual address cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_symcache_flush(Vm * _vm);

//    Adds one entry to LibVMI's internal symbol to virtual address cache.
void vmi_symcache_add(Vm * _vm, addr_t base_addr, vmi_pid_t pid, char * sym, addr_t va);

//    Removes all entries from LibVMIs internal RVA to symbol cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_rvacache_flush(Vm * _vm);

//    Adds one entry to LibVMI's internal RVA to symbol cache.
void vmi_rvacache_add(Vm * _vm, addr_t base_addr, vmi_pid_t pid, addr_t rva, char * sym);

//    Removes all entries from LibVMIs internal pid to directory table base cache. This is generally only useful if you believe that an entry in the cache is incorrect, or out of date.
void vmi_pidcache_flush(Vm * _vm);

//    Adds one entry to LibVMIs internal pid to directory table base cache.
void vmi_pidcache_add(Vm * _vm, vmi_pid_t pid, addr_t dtb);

/**
 * Event Functions
 */

//    Callback receives one event as input. Callback is invoked while within the event listener loop, so actions taken by the callback must take into account that other events may have been delivered and not yet processed. This is especially important when events have been configured in an asyncronous manner (i.e., events delivered are not necessarily in lockstep with the VM state).
//status_t vmi_register_event(Vm * _vm, vmi_event_t * event);

//    Clear the event specified by the vmi_event_t object.
//status_t vmi_clear_event(Vm * _vm, vmi_event_t * event);

//    Return the pointer to the vmi_event_t if one is set on the given register.
//vmi_event_t * vmi_get_reg_event(Vm * _vm, registers_t reg);

//    Return the pointer to the vmi_event_t if one is set on the given page.
//vmi_event_t * vmi_get_mem_event(Vm * _vm, addr_t physical_address, vmi_memevent_granularity_t granularity);

//    Setup single-stepping to register the given event after the specified number of steps.
//status_t vmi_step_event(Vm * _vm, vmi_event_t * event, uint32_t vcpu_id, uint64_t steps, event_callback_t cb);

//    Listen for events until one occurs or a timeout. If the timeout is given as 0, it will process leftover events in the ring-buffer (if there are any).
//status_t vmi_events_listen(Vm * _vm, uint32_t timeout);

//    Set wether to pause the domain if the event listener is no longer present.
//status_t vmi_event_listener_required(Vm * _vm, int required);

//    Check if there are events pending to be processed.
//int vmi_are_events_pending(Vm * _vm);

/**
 * SingleStep Functions
 */

//    Return the pointer to the vmi_event_t if one is set on the given vcpu.
//vmi_event_t * vmi_get_singlestep_event(Vm * _vm, uint32_t vcpu);

//    Disables the MTF single step flag from a vcpu as well as the libvmi event objects bitfield position. This does not disable single step for the whole domain.
//status_t vmi_stop_single_step_vcpu(Vm * _vm, vmi_event_t * event, uint32_t vcpu);

//Cleans up any domain wide single step settings. This should be called when the caller is completely finished with single step, as it implicitly disables single-step on all VM VCPUs.
//status_t vmi_shutdown_single_step(Vm * _vm);

#endif
