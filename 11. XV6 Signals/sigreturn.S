#include "syscall.h"
#include "traps.h"

.globl sigreturn_start;
.globl sigreturn_end;

sigreturn_start:
        movl $SYS_sigret, %eax;
        int $T_SYSCALL;
        ret
sigreturn_end:

