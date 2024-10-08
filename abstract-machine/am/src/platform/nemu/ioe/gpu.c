#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

AM_GPU_CONFIG_T cfg;
void __am_gpu_config(AM_GPU_CONFIG_T *cfg);

void __am_gpu_init() {
  __am_gpu_config(&cfg);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = ((short *)VGACTL_ADDR)[1],
    .height = ((short *)VGACTL_ADDR)[0],
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  for (int i = 0; i < ctl->w; ++i) {
    for (int j = 0; j < ctl->h; ++j) {
      ((uint32_t *)FB_ADDR)[(ctl->y + j) * cfg.width + (ctl->x + i)] = ((uint32_t *)ctl->pixels)[j * ctl->w + i];
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
