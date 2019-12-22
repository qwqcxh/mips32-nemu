#include "proc.h"
#include <elf.h>
#include "fs.h"
#include "memory.h"

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename,0,0);
  Elf_Ehdr elfhdr;
  fs_read(fd,&elfhdr,sizeof(elfhdr));
  int ph_offset=elfhdr.e_phoff;
  int ph_num=elfhdr.e_phnum;
  int ph_entry_size=elfhdr.e_phentsize;
  Elf_Phdr phdr;
  for(int i=0;i<ph_num;i++){
    fs_lseek(fd,ph_offset+i*ph_entry_size,SEEK_SET);
    fs_read(fd,&phdr,sizeof(Elf_Phdr));
    if(phdr.p_type!=PT_LOAD) continue;
    uint32_t p_offset=phdr.p_offset;
    uint32_t p_filesz=phdr.p_filesz;
    int p_memsz=phdr.p_memsz;
    void*    p_vaddr=(void*)phdr.p_vaddr;
    while(p_memsz > 0){
      void* page = new_page(1);
      memset(page,0,PGSIZE);
      uint32_t readsz = p_filesz < PGSIZE ? p_filesz : PGSIZE;
      if(p_filesz){
        fs_lseek(fd,p_offset,SEEK_SET);
        fs_read(fd,page,readsz);
        p_offset += readsz;
        p_filesz -= readsz;
      }
      _map(&pcb->as,p_vaddr,page,1);
      p_vaddr += PGSIZE;
      p_memsz -= PGSIZE;
      printf("memsz is %x now\n",p_memsz);//debug
    }
  }
  return elfhdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
