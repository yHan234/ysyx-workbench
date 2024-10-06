#include <am.h>
#include <nemu.h>
// #include <stdio.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t am_scancode = *(uint32_t *)KBD_ADDR;
  // printf("%x\n", am_scancode);
  kbd->keydown = !!(am_scancode & KEYDOWN_MASK);
  kbd->keycode = am_scancode & ~KEYDOWN_MASK;
}
