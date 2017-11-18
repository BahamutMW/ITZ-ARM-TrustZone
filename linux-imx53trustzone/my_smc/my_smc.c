///****************************************Share Memory overhead*********************
#include <linux/kernel.h> 
#include <linux/syscalls.h> 
//#include <asm/cacheflush.h> 
#include <asm/system.h> 
#include <asm/page.h> 
#include <asm/io.h> 
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <net/tcp.h>
#include <linux/dma-mapping.h>

#define MYMODNAME "FindModule"
#define SIZE (128)
asmlinkage long sys_my_smc(char *data)
{ 
    
   //OFFSET TOOL LIBVMI
   struct task_struct *p = NULL;
   unsigned long commOffset;
   unsigned long tasksOffset;
   unsigned long mmOffset;
   unsigned long pidOffset;
   unsigned long pgdOffset;
   unsigned long addrOffset;

   //printk(KERN_ALERT "Module %s loaded.\n\n", MYMODNAME);
   p = current;

   if (p != NULL) {
       commOffset = (unsigned long) (&(p->comm)) - (unsigned long) (p);
       tasksOffset =
       (unsigned long) (&(p->tasks)) - (unsigned long) (p);
       mmOffset = (unsigned long) (&(p->mm)) - (unsigned long) (p);
       pidOffset = (unsigned long) (&(p->pid)) - (unsigned long) (p);
       pgdOffset =
       (unsigned long) (&(p->mm->pgd)) - (unsigned long) (p->mm);
       addrOffset =
       (unsigned long) (&(p->mm->start_code)) -
       (unsigned long) (p->mm);

       printk(KERN_ALERT "[domain name] {\n");
       printk(KERN_ALERT "    ostype = \"Linux\";\n");
       printk(KERN_ALERT "    sysmap = \"[insert path here]\";\n");
       printk(KERN_ALERT "    linux_name = 0x%x;\n",
        (unsigned int) commOffset);
       printk(KERN_ALERT "    linux_tasks = 0x%x;\n",
           (unsigned int) tasksOffset);
       printk(KERN_ALERT "    linux_mm = 0x%x;\n",
        (unsigned int) mmOffset);
       printk(KERN_ALERT "    linux_pid = 0x%x;\n",
        (unsigned int) pidOffset);
       printk(KERN_ALERT "    linux_pgd = 0x%x;\n",
        (unsigned int) pgdOffset);
       printk(KERN_ALERT "}\n");
   }
   else {
       printk(KERN_ALERT
        "%s: found no process to populate task_struct.\n",
        MYMODNAME);
   }
   
	//OFFSET TOOL END

	//EXTRA CODE    	

   unsigned long procnetoffset;
   unsigned long subdiroffset;
   unsigned long nameoffset;
   unsigned long nextoffset;
   unsigned long dataoffset;
   unsigned long showoffset;

   struct tcp_seq_afinfo *tcp_afinfo = NULL;

   //printk(KERN_ALERT "Module %s loaded.\n\n", MYMODNAME);

   procnetoffset = (unsigned long)(&(init_net.proc_net)) - (unsigned long)(&(init_net));
   subdiroffset = (unsigned long)(&(init_net.proc_net->subdir)) - (unsigned long)(&(*(init_net.proc_net)));
   nameoffset = (unsigned long)(&(init_net.proc_net->name)) - (unsigned long)(&(*(init_net.proc_net)));
   nextoffset = (unsigned long)(&(init_net.proc_net->next)) - (unsigned long)(&(*(init_net.proc_net)));
   dataoffset = (unsigned long)(&(init_net.proc_net->data)) - (unsigned long)(&(*(init_net.proc_net)));

   tcp_afinfo = (struct tcp_seq_afinfo *) (init_net.proc_net->data);
   showoffset = (unsigned long)(&((tcp_afinfo->seq_ops.show))) - (unsigned long)(&(*tcp_afinfo));
   

   printk("procnetoffset = 0x%x\n", (unsigned int)procnetoffset);
   printk("subdiroffset = 0x%x\n", (unsigned int)subdiroffset);
   printk("nameoffset = 0x%x\n", (unsigned int)nameoffset);
   printk("nextoffset = 0x%x\n", (unsigned int)nextoffset);
   printk("dataoffset = 0x%x\n", (unsigned int)dataoffset);
   printk("showoffset = 0x%x\n", (unsigned int)showoffset);

	//END OF EXTRA CODE


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

     
        memcpy(data, genblk_buf, SIZE); // to receive data from the secure world
        

        return data; 
    } 



