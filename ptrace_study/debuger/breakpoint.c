#include "breakpoint.h"

void breakpoint_set(pid_t pid, unsigned long vaddr)
{
    getdata(pid, vaddr, 4, backup_machine_code);
    putdata(pid, vaddr, 4, breakpoint);
}

void breakpoint_delete(pid_t pid, unsigned long vaddr)
{
    putdata(pid, vaddr, 4, backup_machine_code);
}
