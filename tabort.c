#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>

#define YES 1
#define NO  0

static int curr_counter = 0;
static int prev_counter = 0;
static int cpu;
static int do_touch_tm;
static int do_syscall;

void alarm_signal_handler(int unused_var)
{
  printf("CPU%d: %d context switches/s\n", cpu, curr_counter - prev_counter);
  fflush(0);
  prev_counter = curr_counter;

  alarm(1);
}

void usr1_signal_handler(int unused_var)
{
  printf("usr1 signal handler!");
}

void touch(void)
{
  if (do_touch_tm == YES) {
    asm volatile ("tabort. 0;");
  } 

  if (do_syscall == YES) {
    getpid();
  }
}

void* ping__(void* unused_var)
{
  signal(SIGALRM, alarm_signal_handler);
  alarm(1);

  while (1) {
    touch();
    curr_counter += 1;
    sched_yield();
  }

  return NULL;    
}

void* pong__(void *unused_var)
{
  while (1) {
    touch();
    sched_yield();
  }

  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t t0;
  pthread_t t1;
  cpu_set_t cpuset;
  pthread_attr_t attr;

  int option;

  cpu = 0;
  do_touch_tm = NO;
  do_syscall = NO;

  while ( (option = getopt(argc, argv, "tsc:")) != -1) {
    switch (option) {
      case 't':
        do_touch_tm = YES;
        break;
      case 's':
        do_syscall = YES;
        break;
      case 'c':
        cpu = atoi(optarg);
        break;
      default:
        printf("Usage: %s [-s (do syscall), default: NO] [-t (touch TM), default: NO] [-c CPUID, default: 0]\n", argv[0]); 
        exit(1);
    }
  }
       
  if (do_touch_tm)
    printf(" - Touch TM using 'tabort. 0'\n");

  if(do_syscall)
    printf(" - Do syscall\n");

  CPU_ZERO(&cpuset);
  CPU_SET(cpu, &cpuset);
 
  pthread_attr_init(&attr);
  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset); 
 
  signal(SIGUSR1, usr1_signal_handler); 
 
  pthread_create(&t0, &attr, &ping__, NULL);
  pthread_create(&t1, &attr, &pong__, NULL);
 
  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
}
