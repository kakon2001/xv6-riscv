#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"
#include "syscall.h"

extern struct proc proc[NPROC];
extern struct spinlock proc_lock;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// ------------------------------------
uint64
sys_settickets(void)
{
  int n;
  if(argint(0, &n), n < 1)
    return -1;

  struct proc *p = myproc();
  p->tickets_original = n;
  p->tickets_current = n;
  return 0;
}

uint64
sys_getpinfo(void)
{
  uint64 addr;
  argaddr(0, &addr);

  struct pstat ps;

  acquire(&proc_lock);

  for (int i = 0; i < NPROC; i++) {
    struct proc *p = &proc[i];
    ps.inuse[i] = (p->state != UNUSED);
    ps.pid[i] = p->pid;
    ps.tickets_original[i] = p->tickets_original;
    ps.tickets_current[i] = p->tickets_current;
    ps.time_slices[i] = p->time_slices;
  }

  release(&proc_lock);

  if (copyout(myproc()->pagetable, addr, (char *)&ps, sizeof(ps)) < 0)
    return -1;

  return 0;
}
