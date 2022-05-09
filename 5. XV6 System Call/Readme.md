# (Optional Assignment) xv6 system call
This assignment will be seriously checked for plagiarism.

Add a system call to xv6 kernel that does the following:

returns a count of {number of processes forked so far, current count of RUNNABLE processes, current count of SLEEPING processes, current count of ZOMBIE processes, total number of times scheduling was done for processes, number of keyboard interrupts so far, number of total traps so far}

Add an application program that does the above system call and prints the results. Name the application program as newsyscall.c

Submit a "git diff" patch with the master branch. Filename should be MISID.patch
