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
static int tm;
static int dosyscall; 

void alarm_signal_handler(int unused_var)
{
  printf("CPU%d: %d context switches/s\n", cpu, curr_counter - prev_counter);
  prev_counter = curr_counter;

  alarm(1);
}

void usr1_signal_handler(int unused_var)
{
  printf("usr1 signal handler!");
}

void touch(void)
{
  if (tm == YES) {
    asm volatile ("tabort. 0;");
  } 

  if (dosyscall == YES) {
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
 
  if (argc >= 2) {
    cpu = atoi(argv[1]);
  } else {
    cpu = 0;
  }

  tm = NO;
  dosyscall = NO;
  if (argc >= 3) {
    if (!strncmp(argv[2], "--touch-tm", 10)) {
      printf("Touching TM...\n");
      tm = YES;
    } 

    if (!strncmp(argv[3], "--do-syscall", 12)) {
       printf("Doing syscall...\n");
       dosyscall = YES;
    }
  }
       
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
