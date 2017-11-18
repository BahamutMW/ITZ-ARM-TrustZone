#include "vmi.h"


//Read from a given virtual address

//IN: 
// vmi instance (IGNORED) 
// Virtual address to read - 
// Pid of the virtual address space (0 for kernel)

//OUT
// Value from memory

status_t vmi_read_addr_va (vmi_instance_t vmi, addr_t vaddr, vmi_pid_t pid, addr_t *value){

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
	Genode::printf("Content : %s\n", (char*)_buf);
	return 0;

	//TODO
}

//Read 32 bits from a given virtual address
status_t vmi_read_32_va (vmi_instance_t vmi, addr_t vaddr, vmi_pid_t pid, uint32_t *value){
	//TODO
}

//Read string from memory starting at given virtual address
status_t vmi_read_str_va (vmi_instance_t vmi, addr_t vaddr, vmi_pid_t pid){
	//TODO
}

//Translate kernel symbol to virtual address
addr_t vmi_translate_ksym2v (vmi_instance_t vmi, const char *symbol){
	//TODO
}
