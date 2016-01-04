
#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "pwm.h"

int kmain(void) {

  while (1) {
    audio_test(1);
  }
  sys_exit(0);

  return 0;
}
