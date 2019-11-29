probe kernel.function("tm_unavailable").inline {
  ip = task_current()->thread->regs->nip;
  instruction = user_int32(ip);
  if (instruction == 0x7c00051d /* tbegin. */) {
    printf("tm_unavailable() due to tbegin. in '%s' process\n", execname());
  }
}
