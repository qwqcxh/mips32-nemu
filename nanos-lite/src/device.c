#include "common.h"
#include <amdev.h>
#include "fs.h"

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i=0;i<len;i++) _putc(((char*)buf)[i]);
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
    int key = read_key();
    int down = 0;
    if (key & 0x8000) {
      key ^= 0x8000;
      down = 1;
    }
    if (key != _KEY_NONE) {
      return snprintf(buf,len,"%s %s\n",down ? "kd" : "ku" ,keyname[key]);
    }
    else return snprintf(buf,len,"t %u\n",uptime());
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  printf("dispinfo read offset len : %u %u\n",offset,len);//debug
  int tail = strlen(dispinfo);
  if(offset >= tail) return 0;
  int ret = len < tail - offset ? len : tail - offset;
  memcpy(buf,dispinfo+offset,ret);
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int width  = screen_width();
  int height = screen_height();
  int x = offset % (width-1);
  int y = offset / width;
  draw_rect((uint32_t*)buf,x,y,width,height);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
}

// extern int fs_open(const char *pathname, int flags, int mode);
// extern size_t fs_read(int fd, void *buf, size_t len);
// extern size_t fs_lseek(int fd, size_t offset, int whence);
// extern int fs_close(int fd);

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  // int fd = fs_open("/proc/dispinfo",0,0);
  // int sz = fs_lseek(fd,0,SEEK_END);
  // fs_lseek(fd,0,SEEK_SET);
  // fs_read(fd,dispinfo,sz);
  // fs_close(fd);
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n\0",screen_width(),screen_height());
}
