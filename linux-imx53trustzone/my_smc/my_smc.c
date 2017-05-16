///****************************************Share Memory overhead*********************
#include <linux/kernel.h> 
#include <linux/syscalls.h> 
//#include <asm/cacheflush.h> 
#include <asm/system.h> 
#include <asm/page.h> 
#include <asm/io.h> 
#include <linux/linkage.h>
#include <linux/slab.h>
//#include <linux/time.h>
#include <linux/string.h>

#include <linux/dma-mapping.h>


#define SIZE (128)
asmlinkage long sys_my_smc(char *data)
{ 
        
	    	
	static dma_addr_t genblk_buf_phys;
	static void *genblk_buf;
	
	enum { GENBLK_BUF_SIZE = SIZE };
        
	genblk_buf = dma_alloc_coherent(NULL, GENBLK_BUF_SIZE, &genblk_buf_phys, GFP_KERNEL);
        
   	memcpy(genblk_buf, data, SIZE); //to send data to the secure world
        
       asm(".arch_extension sec\n\t");  
       asm volatile("mov r0, #4     	\n" 
		    "mov r2, %[value]   \n"
		    "mov r3, %[size]    \n"
		    "dsb            	\n" 
		    "dmb           	\n" 
		    "smc   #0       	\n" 
		    :: [value] "r" (genblk_buf_phys), [size] "r" (GENBLK_BUF_SIZE): "r0", "r2", "r3"); 

 
        memcpy(data, genblk_buf, SIZE); // to recieve data from the secure world
       
    return data; 
} 



