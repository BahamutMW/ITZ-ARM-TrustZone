#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <openssl/md5.h>
//#include <libvmi.h>
//#include <events.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

/**
 * @brief LibVMI Instance.
 *
 * This struct holds all of the relavent information for an instance of
 * LibVMI.  Each time a new domain is accessed, a new instance must
 * be created using the vmi_init function.  When you are done with an instance,
 * its resources can be freed using the vmi_destroy function.
 */
typedef struct vmi_instance *vmi_instance_t;


typedef enum status {
    VMI_SUCCESS,  /**< return value indicating success */

    VMI_FAILURE   /**< return value indicating failure */
} status_t;


/* task_struct offsets */
unsigned long tasks_offset;
unsigned long pid_offset;
unsigned long name_offset;

/**
 * typedef for forward compatibility with 64-bit guests
 */
typedef uint64_t addr_t;

/**
 * type def for consistent pid_t usage
 */

typedef int32_t vmi_pid_t;


/**
 * Reads an address from memory, given a virtual address.  The number of
 * bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_addr_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    addr_t *value);


/**
 * Reads 32 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_32_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint32_t * value);


/**
 * Reads a null terminated string from memory, starting at
 * the given virtual address.  The returned value must be
 * freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address for start of string
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @return String read from memory or NULL on error
 */
char *vmi_read_str_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid);

/**
 * Performs the translation from a kernel symbol to a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] symbol Desired kernel symbol to translate
 * @return Virtual address, or zero on error
 */
addr_t vmi_translate_ksym2v(
    vmi_instance_t vmi,
    const char *symbol);


