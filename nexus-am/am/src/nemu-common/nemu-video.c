#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>
#define SCREEN_H 300
#define SCREEN_W 400
static uint32_t video_readsize;

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      video_readsize = inl(SCREEN_ADDR);      
      info->width = video_readsize >> 16;
      info->height = video_readsize & 0xffff;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
      uint32_t *pixels = ctl->pixels;
      int cplen = sizeof(uint32_t)*w;
      for(int i=0;i<h;i++){
        //assert(ctl->y+i<SCREEN_H);
        memcpy((void *)(FB_ADDR+sizeof(uint32_t)*(SCREEN_W*(y+i)+x)),pixels,cplen);
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

  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (int i = 0; i < size; i ++) fb[i] = 0xa61b29;
  draw_sync();
}
