#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int sys_waitx(void)
{
  int *wtime;
  int *rtime;
  int flag1 = argptr(0, (void *)&wtime, sizeof(wtime));
  int flag2 = argptr(1, (void *)&rtime, sizeof(rtime));
  if ( flag1 < 0)
    return -1;
  if ( flag2 < 0)
    return -1;
  return waitx(wtime, rtime);
}
int sys_set_priority(void)
{
  int new_priority;
  int pid;
  int flag1 = argint(0, &new_priority);
  int flag2 = argint(1, &pid);
  if (flag1 < 0)
    return -1;
  if (flag2 < 0 || pid < 0 || pid > 100)
    return -1;
  return set_priority(new_priority, pid);
}
int
sys_ps(void)
{
  return ps();
}
