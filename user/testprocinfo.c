#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

int main() {
  struct pstat st;

  if (getpinfo(&st) < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }

  printf("PID\tInUse\tOrigTix\tCurrTix\tTimeSlices\n");
  for (int i = 0; i < 64; i++) {
    if (st.inuse[i]) {
      printf("%d\t%d\t%d\t%d\t%d\n",
             st.pid[i],
             st.inuse[i],
             st.tickets_original[i],
             st.tickets_current[i],
             st.time_slices[i]);
    }
  }

  exit(0);
}
