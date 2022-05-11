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

  if(argint(0, &pid) < 0)
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

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int
sys_signal(void){
  int signal_no;
  char *x;
  if (argint(0, &signal_no) < 0 || argptr(1, &x, sizeof(sighandler_t)) < 0)
	  return -1;
 sighandler_t handler = (sighandler_t)x;
  return signal(signal_no, handler);
}

int
sys_sigret(void){
  return sigret();
}

int
sys_kill_signal(void)
{
  int pid, signal_no;
  if (argint(0, &pid) < 0 || 
      argint(1, &signal_no) < 0)
	  return -1;
  return kill_signal(pid, signal_no);
}


int
sys_pause(void){
  return pause();
}

int sys_sigprocmask(void){
  uint sigmask;
  if (argint(0, (int*)&sigmask) < 0){
    return -1;
  }
  return sigprocmask(sigmask);
}

int sys_sigaction(void){
  uint signum;
  struct sigaction* act;
  struct sigaction* oldact;
  if (argint(0, (int*)(&signum)) < 0 || argptr(1, (char**)(&act), 16) < 0 || argptr(2, (char**)(&oldact), 16)){
    return -1;
  }
  return sigaction(signum, act, oldact);
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
