#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  settickets(100);

  int pid = fork();
  if (pid == 0) {
    
    settickets(1000);
  } else {
    while (1);
  }

  exit(0);
}
