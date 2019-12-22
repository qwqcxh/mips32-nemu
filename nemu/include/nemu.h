#ifndef __NEMU_H__
#define __NEMU_H__

#include "common.h"
#include "memory/memory.h"
#include "isa/reg.h"

typedef struct{
    uint32_t EntryHi;
    uint32_t EntryLo0;
    uint32_t EntryLo1;
} TLBentry;
#define TLBSIZE 16
#define PVN(x) (x>>13)
#define PFN(x) (x>>12)
extern CPU_state cpu;

#endif
