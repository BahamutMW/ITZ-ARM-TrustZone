#ifndef _TZ_VMM__INCLUDE__ITZ_H_
#define _TZ_VMM__INCLUDE__ITZ_H_


typedef enum status {
    VMI_SUCCESS,  /**< return value indicating success */

    VMI_FAILURE   /**< return value indicating failure */
} status_t;


/**
 * typedef for forward compatibility with 64-bit guests
 */
typedef uint64_t addr_t;

/**
 * type def for consistent pid_t usage
 */

typedef int32_t vmi_pid_t;

//Prints out the hex and ascii version of a chunk of bytes from physical address
void vmi_print_hex_pa(uint64_t paddr, size_t length);

#endif 
