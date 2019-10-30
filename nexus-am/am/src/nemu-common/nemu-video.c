#include <am.h>
#include <amdev.h>
#include <nemu.h>
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
      for(int i=0;i<ctl->h;i++){
        //assert(ctl->y+i<SCREEN_H);
        for(int j=0;j<ctl->w;j++){
          //assert(ctl->x+j<SCREEN_W);
          outl(FB_ADDR+SCREEN_W*(ctl->y+i)+ctl->x+j,*ctl->pixels);
        }
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
}
