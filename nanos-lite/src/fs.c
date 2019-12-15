#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len) ;
extern size_t fb_write(const void *buf, size_t offset, size_t len) ;
extern size_t fbsync_write(const void *buf, size_t offset, size_t len) ;
extern size_t dispinfo_read(void *buf, size_t offset, size_t len) ;
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/events",0,0,0,events_read, invalid_write},
  {"/dev/fb", 0, 0 ,0 ,invalid_read, fb_write},
  {"/dev/fbsync", 0 , 0 , 0 , invalid_read, fbsync_write},
  {"/proc/dispinfo", 0 ,0 ,0 , dispinfo_read , invalid_write},
  {"/dev/tty" , 0 , 0 , 0 , invalid_read , serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[4].size = screen_height()*screen_width();
}

/***********work**************/
int fs_open(const char *pathname, int flags, int mode){
  for(int i=0;i<NR_FILES;i++){
    if(strcmp(pathname,file_table[i].name)==0){
      file_table[i].open_offset = 0;
      return i;
    }
  }
  return -1;
}

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t fs_read(int fd, void *buf, size_t len){
  size_t real_off= file_table[fd].disk_offset + file_table[fd].open_offset;
  if(fd==58) printf("fs read fd is %d offset is %u len is %u\n",fd,real_off,len);//debug
  size_t file_sz = file_table[fd].size ;
  size_t ret;
  if(file_table[fd].read) ret = file_table[fd].read(buf,real_off,len);
  else {
    size_t maxread = file_sz - file_table[fd].open_offset;
    ret = len < maxread ? len : maxread ;
    ramdisk_read(buf, real_off, ret);
  }
  file_table[fd].open_offset+=ret;
  return ret;
}

extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_write(int fd, const void *buf, size_t len){
  size_t real_off= file_table[fd].disk_offset + file_table[fd].open_offset;
  size_t file_sz = file_table[fd].size;
  size_t ret;
  if(file_table[fd].write) ret = file_table[fd].write(buf,real_off,len);
  else {
    size_t maxwrite = file_sz - file_table[fd].open_offset;
    ret = len < maxwrite ? len : maxwrite ;
    ret = ramdisk_write(buf, real_off, len); 
  }
  file_table[fd].open_offset+=ret;
  return ret;
}

int fs_close(int fd){
  file_table[fd].open_offset=0;
  return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  if(whence == SEEK_SET ) file_table[fd].open_offset = offset;
  else if(whence == SEEK_CUR) file_table[fd].open_offset+=offset;
  else file_table[fd].open_offset=file_table[fd].size+offset;
  return file_table[fd].open_offset;
}