    /*
     * \brief  Virtual Machine Monitor
     * \author Stefan Kalkowski
     * \date   2012-06-25
     */

    /*
     * Copyright (C) 2008-2012 Genode Labs GmbH
     *
     * This file is part of the Genode OS framework, which is distributed
     * under the terms of the GNU General Public License version 2.
     */

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
    //#include <newlibvmi/vmi.h>

    /* external includes */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <openssl/md5.h>
    #include <openssl/sha.h>

     /* libvmi includes */

    //#include <libvmi.h>
    //#include <../config.h>

    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <sys/mman.h>
    #include <inttypes.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <itz.h>
    //#include <test.h>




     using namespace Genode;

    #define SHMSZ     27

    #define MAXSIZE 1000
    #define MINDIFF 2.25e-308                   // use for convergence check
     enum {
      KERNEL_OFFSET    = 0x8000,
      MACH_TYPE_TABLET = 3011,
      MACH_TYPE_QSB    = 3273,
      BOARD_REV_TABLET = 0x53321,
    };


    static const char* cmdline_tablet = "console=ttymxc0,115200";

    void on_vmm_entry();
    void on_vmm_exit();

      char str1[15];
            char str2[15];
            char str3[15];
            char str4[15];


    namespace Vmm {
      class Vmm;
    }


    class Vmm::Vmm : public Thread<8192>
    {
    private:

      Signal_receiver           _sig_rcv;
      Signal_context            _vm_context;
      Vm                       *_vm;
      Io_mem_connection         _m4if_io_mem;
      M4if                      _m4if;
      Serial                    _serial;
      Block                     _block;


      void _handle_hypervisor_call()
      {
       enum {
        FRAMEBUFFER = 0,
        INPUT       = 1,
        SERIAL      = 2,
        BLOCK       = 3,
        TEST        = 4,
      };  

      switch (_vm->smc_arg_0()) {
        case FRAMEBUFFER:                      break;
        case INPUT:                            break;
        case SERIAL:      _serial.handle(_vm); break;
        case BLOCK:       _block.handle(_vm);  break;
        case TEST:
        //Genode::printf("%s\n","Secure World"); 
        //size_t size_nw;
        //size_t size_sw;
        //uint64_t addr_nw;
        //uint64_t addr_sw;


            //size_nw = Trustzone::NONSECURE_RAM_SIZE; //the size of the normal world memory T
            //addr_nw = (uint64_t)Trustzone::NONSECURE_RAM_BASE; //the base address of the normal world memory

            //size_sw = Trustzone::SECURE_RAM_SIZE; //the size of the secure world memory
            //addr_sw = (uint64_t)Trustzone::SECURE_RAM_BASE; //the base address of the secure world memory
            
            //TEST ZONE
            
            //hexDump(addr_nw, 128);
            /*
            void* function;
            function = receiver();
            
            char* identifier;
            identifier = (char*)function;
            
            int funcID;
            funcID = 3;

            switch(funcID) {
              
              //Read
              case 0 :
              Genode::printf("%s\n", "\n TEST READ FROM VIRTUAL ADDRESS");
              vmi_read_va (addr_nw, NULL, NULL, 32);
              break;
              
              //Write
              case 1 :
              Genode::printf("%s\n", "TEST WRITE TO MEMORY");
              break;
              
              //Translate
              case 2 :
              Genode::printf("%s\n", "\n TEST TRANSLATE PHYSICAL TO VIRTUAL");
              uint64_t virtaddr;
              virtaddr = vmi_translate_p2uv(addr_nw,0);   

              //Genode::printf("%s\n", "\n TEST TRANSLATE VIRTUAL TO PHYSICAL");
              // vmi_translate_uv2p(virtaddr,0);           
              break;

              //Print
              case 3 :
              Genode::printf("%s\n", "TEST PRINT FROM PHYSICAL ADDR");
              vmi_print_hex_pa(addr_nw,32); 


              //Genode::printf("%s\n", "TEST PRINT FROM VIRTUAL ADDR");
              // vmi_print_hex_va(virtaddr,0,32);             
              break;

              //Hash
              case 4 :
              Genode::printf("%s\n", "TEST HASHING CONTENT FROM MEMORY");

              md5(function);
              break;

              default :
              Genode::printf("Invalid input! Shame on you! \n" );
              break;

            }


            char *osname;
            osname = "l4linux";

            

            

            


            //vmi_read_va (NULL, 0xe8, 0, NULL);
            
            //snapshotlibvmi();
            //sender(addr_nw);

            //introspect_process_list(osname);

            //dumper((void*)_vm->_ram.local());       

            //dumptofile(addr_nw,size_nw);

                //END OF TEST ZONE*/


            //NEW TEST AREA
            //introspect_process_list("Process");
          
            strcpy(str1, "md5");
            strcpy(str2, "sha1");

            strcpy(str3, "sha256");
            strcpy(str4, "sha512");
            //splitter();
            //_vm->dump();
            //introspect_kernel_check(str1);
            //introspect_kernel_check(str2);
            introspect_kernel_check(str3);
            //introspect_kernel_check(str4);
            //introspect_process_list();
            //introspect_procfs_check("ias");
            //receiver();
            //Genode::printf("%s\n", "TEST PRINT FROM PHYSICAL ADDR");
            //vmi_print_hex_pa(_vm, addr_nw,32);  
            break;

            default:
            PERR("unknown hypervisor call!");
            _vm->dump();
          };
        }

        bool _handle_data_abort()
        {
          _vm->dump();
          return false;
        }

        bool _handle_vm()
        {
          /* check exception reason */
          switch (_vm->state()->cpu_exception) {
           case Cpu_state::DATA_ABORT:
           if (!_handle_data_abort()) {
            PERR("could not handle data-abort will exit!");
            return false;
          }
          break;
          case Cpu_state::SUPERVISOR_CALL:
          _handle_hypervisor_call();
          break;
          default:
          PERR("curious exception occured");
          _vm->dump();
          return false;
        }
        return true;
      }

    protected:

      void entry()
      {
       _vm->sig_handler(_sig_rcv.manage(&_vm_context));
       _vm->start();
       _vm->run();

       while (true) {
        Signal s = _sig_rcv.wait_for_signal();
        on_vmm_entry();
        if (s.context() == &_vm_context) {
         if (_handle_vm())
          _vm->run();
      } else {
       PERR("invalid context");
       continue;
     }
     on_vmm_exit();
   }
 };

public:

  Vmm(Vm *vm)
  : Thread<8192>("vmm"),
  _vm(vm),
  _m4if_io_mem(Board_base::M4IF_BASE, Board_base::M4IF_SIZE),
  _m4if((Genode::addr_t)env()->rm_session()->attach(_m4if_io_mem.dataspace()))
  {
   _m4if.set_region0(Trustzone::SECURE_RAM_BASE,
    Trustzone::SECURE_RAM_SIZE);
 }


    //Code to send to the normal world
 void sender(uint64_t addr)
 { 

  void *_buf;
  Genode::size_t _buf_size;
  Ram *  ram;

      //Test string
  const char str[32]="Secure World says hello!";
  Genode::addr_t buf_base = (Genode::addr_t)_vm->state()->r2;

  _buf_size = (Genode::size_t)_vm->state()->r3;

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

  Genode::memcpy(_buf, str, sizeof(str));

  char *data = (char*)_buf;     
  Genode::printf("Written: %s\n", data);
}


    //Code to receive data from the normal world
void* receiver()
{ 

 void *_buf;
 Genode::size_t _buf_size;
 Ram *  ram;

 Genode::addr_t buf_base = (Genode::addr_t)_vm->state()->r2;
 _buf_size = (Genode::size_t)_vm->state()->r3;

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
Genode::printf("Received : %s\n", (char*)_buf);
return _buf;

}

    //Code to dump memory
void hexDump(void *addr, int len) 
{
  int i;
  unsigned char buff[17];
  unsigned char *pc = (unsigned char*)addr;

        // Process every byte in the data.
  for (i = 0; i < len; i++) {
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

        // And print the final ASCII bit.
  Genode::printf("  %s\n", buff);
}


void dumptofile(void *addr, int size){
  FILE *file = fopen("memdump.dd", "w+b");
  if(file !=NULL)
    fwrite(addr,1,size,file);
}

/*void dumper(void *data)
{
 PDBG("Enter into dumper code\n");
 size_t size = _vm->_ram.size();
           //void *data = malloc(size);
 if(data != NULL)
 {
  FILE *out = fopen("test.dd", "w+b");
  if(out != NULL)
  {
    size_t to_go = size;
    Genode::printf("Base Address in tz_vmm  %p\n", (void*)_vm->_ram.base());
    Genode::printf("Local Address in tz_vmm  %p\n", (void*)_vm->_ram.local());
              //while(to_go > 0)
                // {
                      //const size_t wrote = 
            //fwrite(data,1, to_go, out);
                                   //  if(wrote == 0)
                                   //      break;
                                   //    to_go -= wrote;
                            // }
                     //   fseek(out, 0L,SEEK_END);
                          //    long double length=ftell(out);
                          //    PDBG("the file's length is %Lf\n",length);
                    //    fclose(out);
  }
  else
    PDBG("the file couldn't open\n");
                           //free(data);
}
}*/


void md5(void* string) {
  PDBG("MD5 \n");
  unsigned char digest[16];

  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, string, sizeof(string));
  MD5_Final(digest, &ctx);

  char mdString[33];
  int i;
  for ( i = 0; i < 16; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

  Genode::printf("MD5 digest: %s\n", mdString);
  PDBG("Exit From MD5\n");

}

void sha1(void* string){
  PDBG("SHA1 \n");

  unsigned char digest[SHA_DIGEST_LENGTH];

  SHA1((unsigned char*)&string, sizeof(string), (unsigned char*)&digest);    

  char mdString[SHA_DIGEST_LENGTH*2+1];

  for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
   sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
 
 Genode::printf("SHA1 digest: %s\n", mdString);
 PDBG("Exit From SHA1\n");

}

void sha256(void* string){
  PDBG("SHA256 \n");

  unsigned char digest[SHA256_DIGEST_LENGTH];

  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  SHA256_Update(&ctx, string, sizeof(string));
  SHA256_Final(digest, &ctx);

  char mdString[SHA256_DIGEST_LENGTH*2+1];
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

  Genode::printf("SHA256 digest: %s\n", mdString);
  PDBG(" SHA256\n");

}

void sha512(void* string){
  PDBG("SHA512 \n");

  unsigned char digest[SHA512_DIGEST_LENGTH];

  SHA512((unsigned char*)&string, sizeof(string), (unsigned char*)&digest);    

  char mdString[SHA512_DIGEST_LENGTH*2+1];

  for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
   sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
 
 Genode::printf("SHA512 digest: %s\n", mdString);
 PDBG("Exit From SHA512 \n");

}

void hashselect(char* hashname, void* hashcontent){
  if (Genode::strcmp("md5",hashname)==0){
    md5(hashcontent);
  } else if (Genode::strcmp("sha1",hashname)==0){
    sha1(hashcontent);
  } else if (Genode::strcmp("sha256",hashname)==0){
    sha256(hashcontent);
  } else if (Genode::strcmp("sha512",hashname)==0){
    sha512(hashcontent);
  }
}
    /*void dumper(void *data)
        {
           PDBG("Enter into dumper code\n");
           size_t size = _vm->_ram.size();
          //void *data = malloc(size);
        if(data != NULL)
          {
            FILE *out = fopen("test.dd", "w+b");
              if(out != NULL)
                {
                size_t to_go = size;
          Genode::printf("Base Address in tz_vmm  %p\n", (void*)_vm->_ram.base());
          Genode::printf("Local Address in tz_vmm  %p\n", (void*)_vm->_ram.local());
              //while(to_go > 0)
                // {
                      //const size_t wrote = 
            //fwrite(data,1, to_go, out);
                                   //  if(wrote == 0)
                                   //      break;
                                   //    to_go -= wrote;
                            // }
                     //   fseek(out, 0L,SEEK_END);
                          //    long double length=ftell(out);
                          //    PDBG("the file's length is %Lf\n",length);
                    //    fclose(out);
                            }
              else
          PDBG("the file couldn't open\n");
                           //free(data);
                        }
        }*/



      /*int introspect_process_list (char *name) {
          unsigned long tasks_offset;
          unsigned long pid_offset;
          unsigned long name_offset;



          vmi_instance_t vmi;
          long long unsigned int list_head = 0, next_list_entry = 0, current_process = 0;
          int32_t pid = 0;
          char *procname = NULL;

          if (VMI_FAILURE ==
            vmi_init_complete(&vmi, name, VMI_INIT_DOMAINNAME, NULL,
              VMI_CONFIG_GLOBAL_FILE_ENTRY, NULL, NULL))
          {
            Genode::printf("Failed to init LibVMI library.\n");
            return 1;
          }

          vmi_pause_vm(vmi);*/

         /**
         * get offsets of the kernel data structures
         * get the head of the task_struct 
         */

         /* tasks_offset = 0xac;
          name_offset = 0x1d0;
          pid_offset = 0xe8;


          list_head = vmi_translate_ksym2v(vmi, "init_task") + tasks_offset;
          next_list_entry = list_head;*/


        /** 
         * traverse the task lists and print out each process 
         */
         /*do {
          current_process = next_list_entry - tasks_offset;
          vmi_read_32_va(vmi, current_process + pid_offset, 0, (uint32_t*)&pid);
          procname = vmi_read_str_va(vmi, current_process + name_offset, 0);
          if (!procname) {
            Genode::printf("Failed to find procname\n");
            goto exit;
          }

          Genode::printf("[%5d] %s\n", pid, procname);

          free(procname);
          procname = NULL;

            if (vmi_read_va(vmi, next_list_entry, 0, &next_list_entry) == VMI_FAILURE) {
                Genode::printf("Failed to read next pointer in loop at %"PRIx64"\n", next_list_entry);
                goto exit;
            }

              } while(next_list_entry != list_head);

              exit:
              vmi_resume_vm(vmi);
              vmi_destroy(vmi);

              return 0;
            }*/



          //Read from a given virtual Address


              //AUXILIARY FUNCTIONS FOR TESTS
              double sqroot(double square)
              {
                double root=square/3, last, diff=1;
                if (square <= 0) return 0;
                do {
                  last = root;
                  root = (root + square / root) / 2;
                  diff = root - last;
                } while (diff > MINDIFF || diff < -MINDIFF);
                return root;
              }

              float pow(float e, float b)
              {
                int t=1;
                while(b==t)
                {
                  e=e*b;
                  t++;
                }
                return e;
              } 
              
              /*  linux_name = 0x1d0;
    linux_tasks = 0xac;
    linux_mm = 0xc8;
    linux_pid = 0xe8;
    linux_pgd = 0x24;
procnetoffset = 0x1c
subdiroffset = 0x30

nameoffset = 0x8
nextoffset = 0x28
dataoffset = 0x34
showoffset = 0x7c*/

int introspect_process_list () {

    Genode::addr_t list_head;
    list_head = 0;
    Genode::addr_t next_list_entry;
    next_list_entry = 0;
    Genode::addr_t current_process;
    current_process = 0;
    int32_t pid;
    pid = 0;

    char *procname = NULL;

    
     //get offsets of the kernel data structures
     //get the head of the task_struct 
     

 
     Genode::addr_t tasks_offset;
    tasks_offset = 0xac;
    
     Genode::addr_t name_offset;
    name_offset = 0x8;
    
     Genode::addr_t pid_offset;
    pid_offset = 0xe8;

    //vmi_translate_uv2p(_vm,0x0c0871668,0);
    list_head = 0xC0871714;

    next_list_entry = 0xC0871714;

    // 
     // traverse the task lists and print out each process 
     //
    do {
        current_process = next_list_entry - tasks_offset;
        Genode::printf("%lu\n", current_process );
        vmi_read_32_va(_vm, current_process + pid_offset, 0, (uint32_t*)&pid);
        vmi_read_va(_vm, current_process + name_offset, 0, procname, 8);
        Genode::printf("%s\n", procname );
        //procname = vmi_read_str_va(_vm, current_process + name_offset, 0);
        if (!procname) {
            Genode::printf("Failed to find procname\n");
        }

        Genode::printf("[%5d] %s\n", pid, procname);

        
        /*if (vmi_read_va(_vm, next_list_entry, 0, &next_list_entry) == VMI_FAILURE) {
            Genode::printf("Failed to read next pointer in loop at %x08\n", next_list_entry);
        }*/

    } while(next_list_entry != list_head);
    return 0;
}

int introspect_kernel_check(char *hashname) {
    /*Genode::addr_t kernel_start;
    Genode::addr_t kernel_end;
    Genode::addr_t kerneladd[10000];*/
    uint64_t pt[51];

    pt[0]=0xc083a3f5;
    pt[1]=0xc083a405;
    pt[2]=0xc083a416;
    pt[3]=0xc083a428;
    pt[4]=0xc083a43b;
    pt[5]=0xc083a44d;


    pt[6]=0xc083a45d;
    pt[7]=0xc083a46b;
    pt[8]=0xc083a47a;
    pt[9]=0xc083a493;
    pt[10]=0xc083a4aa;

    pt[11]=0xc083a4c2;
    pt[12]=0xc083a4d8;
    pt[13]=0xc083a4f1;
    pt[14]=0xc083a50a;
    pt[15]=0xc083a51d;

    pt[16]=0xc083a4c2;
    pt[17]=0xc083a4d8;
    pt[18]=0xc083a4f1;
    pt[19]=0xc083a50a;
    pt[20]=0xc083a51d;

    pt[21]=0xc083a4c2;
    pt[22]=0xc083a4d8;
    pt[23]=0xc083a4f1;
    pt[24]=0xc083a50a;
    pt[25]=0xc083a51d;

    pt[26]=0xc083a4c2;
    pt[27]=0xc083a4d8;
    pt[28]=0xc083a4f1;
    pt[29]=0xc083a50a;
    pt[30]=0xc083a51d;
    
    pt[31]=0xc083a4c2;
    pt[32]=0xc083a4d8;
    pt[33]=0xc083a4f1;
    pt[34]=0xc083a50a;
    pt[35]=0xc083a51d;

    pt[36]=0xc083a4c2;
    pt[37]=0xc083a4d8;
    pt[38]=0xc083a4f1;
    pt[39]=0xc083a50a;
    pt[40]=0xc083a51d;

    pt[41]=0xc083a4c2;
    pt[42]=0xc083a4d8;
    pt[43]=0xc083a4f1;
    pt[44]=0xc083a50a;
    pt[45]=0xc083a51d;

    pt[46]=0xc083a4c2;
    pt[47]=0xc083a4d8;
    pt[48]=0xc083a4f1;
    pt[49]=0xc083a50a;
    pt[50]=0xc083a51d;


    char buf[512];
    int i;

    /**
     * get kernel function boundary
     */


    //kernel_start = 0xc0008000;
    //kernel_end = 0xc0847000;  


    if (Genode::strcmp(str1,hashname)==0){

      MD5_CTX c;
      
      unsigned char out[MD5_DIGEST_LENGTH];

      MD5_Init(&c);

      for (i=0; i<51; i+= 1) {
          vmi_read_va(_vm, pt[i], 0, buf, 8);
          MD5_Update(&c, buf, 8);
      }

      MD5_Final(out, &c);

      Genode::printf("kernel section hash value: ");
      for (i=0; i<MD5_DIGEST_LENGTH; i++)
         Genode::printf("%x ", out[i]);
      Genode::printf("\n");

    } else if (Genode::strcmp(str2,hashname)==0) {

      SHA_CTX c;
      
      unsigned char out[SHA_DIGEST_LENGTH];

      SHA_Init(&c);

      for (i=0; i<51; i+= 1) {
          vmi_read_va(_vm, pt[i], 0, buf, 8);
          SHA_Update(&c, buf, 8);
      }

      SHA_Final(out, &c);

      Genode::printf("kernel section hash value: ");
      for (i=0; i<SHA_DIGEST_LENGTH; i++)
         Genode::printf("%x ", out[i]);
      Genode::printf("\n");  

    } else if(Genode::strcmp(str3,hashname)==0) {

      SHA256_CTX c;
      
      unsigned char out[SHA256_DIGEST_LENGTH];

      SHA256_Init(&c);

      for (i=0; i<51; i+= 1) {
          vmi_read_va(_vm, pt[i], 0, buf, 8);
          SHA256_Update(&c, buf, 8);
      }

      SHA256_Final(out, &c);

      Genode::printf("kernel section hash value: ");
      for (i=0; i<SHA256_DIGEST_LENGTH; i++)
         Genode::printf("%x ", out[i]);
      Genode::printf("\n");  

  } else if (Genode::strcmp(str4,hashname)==0){

      SHA512_CTX c;
      
      unsigned char out[SHA512_DIGEST_LENGTH];

      SHA512_Init(&c);

      for (i=0; i<51; i+= 1) {
          vmi_read_va(_vm, pt[i], 0, buf, 8);
          SHA512_Update(&c, buf, 8);
      }

      SHA512_Final(out, &c);

      Genode::printf("kernel section hash value: ");
      for (i=0; i<SHA512_DIGEST_LENGTH; i++)
         Genode::printf("%x ", out[i]);
      Genode::printf("\n");
    }

    return 0;
  }

        };

        int main()
        {
          static Vm vm("linux", cmdline_tablet,
           Trustzone::NONSECURE_RAM_BASE, Trustzone::NONSECURE_RAM_SIZE,
           KERNEL_OFFSET, MACH_TYPE_QSB);
          static Vmm::Vmm vmm(&vm);

          Genode::printf("%s\n", "Start virtual machine ...");
          vmm.start();

          sleep_forever();
          return 0;
        }




















