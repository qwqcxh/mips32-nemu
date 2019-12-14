#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      uint32_t screen = inl(SCREEN_ADDR);
      info->width = screen>>16;
      info->height = screen & 0xffff;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

static inline int min(int x, int y) {
  return (x < y) ? x : y;
}
size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
      uint32_t *pixels = ctl->pixels;
      uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
      int cp_bytes = sizeof(uint32_t) * min(w, screen_width() - x);
      for (int j = 0; j < h && y + j < screen_height(); j ++) {
        memcpy(&fb[(y + j) * screen_width() + x], pixels, cp_bytes);
        pixels += w;
      }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = 10000;
  draw_sync();
}
