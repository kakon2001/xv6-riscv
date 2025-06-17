#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
    int pid[NPROC];
    int inuse[NPROC];
    int tickets_original[NPROC];
    int tickets_current[NPROC];
    int time_slices[NPROC];
};

#endif
