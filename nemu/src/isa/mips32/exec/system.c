#include "cpu/exec.h"

make_EHelper(syscall){
    rtl_mv(&cpu.epc,&cpu.pc);
    rtl_ori(&cpu.cause,&cpu.cause,0x20);//set cause.execode
    rtl_ori(&cpu.status,&cpu.status,0x2);
    rtl_j(0x80000180);
}