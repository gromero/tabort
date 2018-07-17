probe kernel.function("tm_unavailable").inline {
  printf("tm_unavailable() -> %s\n", execname());
}
