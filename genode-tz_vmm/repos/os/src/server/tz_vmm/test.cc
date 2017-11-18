
 ///LIBVMI MODIFIED
///LIBVMI MODIFIED

              size_t vmi_read_addr_va (Vm *_vm, uint64_t vaddr, int32_t pid, void *buf, size_t count){

                void *_buf;
                Genode::size_t _buf_size;
                Ram *  ram;


                Genode::addr_t buf_base = vaddr;
                _buf_size = sizeof(buf_base);

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
                Genode::printf("Content of the virtual address: %s\n", (char*)_buf);
                return 0;

      //TODO Test Function
              }

    //Read 32 bits from a given virtual address
              status_t vmi_read_8_va (uint64_t vaddr, int32_t pid, uint32_t *value){
                return VMI_SUCCESS;

      //TODO
              }
    //Read 32 bits from a given virtual address
              status_t vmi_read_16_va (uint64_t vaddr, int32_t pid, uint32_t *value){
                return VMI_SUCCESS;

      //TODO
              }
    //Read 32 bits from a given virtual address
              status_t vmi_read_32_va (uint64_t vaddr, int32_t pid, uint32_t *value){
                return VMI_SUCCESS;

      //TODO
              }

    //Read 32 bits from a given virtual address
              status_t vmi_read_64_va (uint64_t vaddr, int32_t pid, uint32_t *value){
                return VMI_SUCCESS;

      //TODO
              }

    //Read string from memory starting at given virtual address
              char* vmi_read_str_va (uint64_t vaddr, int32_t pid){
                return 0;

      //TODO
              }

    //Reads count bytes from memory located at the physical address paddr and stores the output in buf.
              size_t vmi_read_pa (uint64_t paddr, void *buf, size_t count){
      //return buf;               
               return 0;

             }


    //Translate kernel symbol to virtual address
             uint64_t vmi_translate_ksym2v (const char *symbol){
              return 0;

      //TODO
            }

    //Translate physical address to virtual address
            uint64_t vmi_translate_p2uv (uint64_t phy_addr, int32_t pid){
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

            uint64_t vmi_translate_uv2p(uint64_t vaddr, int32_t pid){
              Genode::addr_t phys_addr_SW = _vm->va_to_pa(vaddr);
              Genode::printf("Corresponding Physical Address: %lu\n", phys_addr_SW);
              return phys_addr_SW;

            };

    //Translate normal world virtual address to secure world physical address
    //Genode::addr_t phys_addr_SW = _vm->va_to_pa(virt_addr_NW);

    //Translate secure world physical address to secure world virtual address
    //Genode::addr_t virt_addr_SW  =  ram->va(phys_addr_SW);


    //Writes count bytes to memory located at the virtual address vaddr from buf.
            size_t vmi_write_va (uint64_t vaddr, int32_t pid, void *buf, size_t count){
                return 0;


            }


    //Writes count bytes to memory located at the physical address paddr from buf.
            size_t vmi_write_pa (uint64_t paddr, void *buf, size_t count){
                              return 0;

            }

    //Writes 8 bits to memory, given a virtual address.
            status_t vmi_write_8_va (uint64_t vaddr, int32_t pid, uint8_t *value){
                return VMI_SUCCESS;

            }
//Writes 8 bits to memory, given a virtual address.
            status_t vmi_write_16_va (uint64_t vaddr, int32_t pid, uint8_t *value){
                return VMI_SUCCESS;

            }//Writes 8 bits to memory, given a virtual address.
            status_t vmi_write_32_va (uint64_t vaddr, int32_t pid, uint8_t *value){
                return VMI_SUCCESS;

            }
            //Writes 8 bits to memory, given a virtual address.
            status_t vmi_write_64_va (uint64_t vaddr, int32_t pid, uint8_t *value){
                return VMI_SUCCESS;

            }
    //Writes 8 bits to memory, given a physical address.
            status_t vmi_write_8_pa (uint64_t paddr, uint8_t *value){
                return VMI_SUCCESS;

            }

    //Writes 8 bits to memory, given a physical address.
            status_t vmi_write_16_pa (uint64_t paddr, uint8_t *value){
                return VMI_SUCCESS;

            }

    //Writes 8 bits to memory, given a physical address.
            status_t vmi_write_32_pa (uint64_t paddr, uint8_t *value){
                return VMI_SUCCESS;

            }

    //Writes 8 bits to memory, given a physical address.
            status_t vmi_write_64_pa (uint64_t paddr, uint8_t *value){
                return VMI_SUCCESS;

            }

    //Prints out the hex and ascii version of a chunk of bytes from virtual address
            void vmi_print_hex_va (uint64_t vaddr, int32_t pid, size_t length){
              Genode::addr_t phys_addr_SW = _vm->va_to_pa(vaddr);
              vmi_print_hex_pa(phys_addr_SW,length);

            }
