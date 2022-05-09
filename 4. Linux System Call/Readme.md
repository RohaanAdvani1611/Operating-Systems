# (Mandatory Assignment) Adding a System call to Linux Kernel
You are required to add a system call to Linux kernel and demonstrate calling the system call using a C program. The system call should take a PID as argument, and print the PIDs of entire ancestry of the process.  There are 7 marks for adding a dummy system call and 3 marks for adding the system-call to print PID ancestry.

Notes:
Adding a system call to Linux kernel is a straightforward job. It involves following steps

1) Writing code of the system call in appropriate files in the Linux kernel code.

2) Compiling, installing and rebooting using the new kernel

3) Writing a C program to call the newly added system call

The process of adding a system call to Linux kernel is documented in the this file and also in these videos.

Submission:

Submit two files:

A diff file showing differences with "diff -c" for the  table file, header file, source code files(s), and userland application program. Filename:  diff.txt

single screenshot showing execution of user program, and output of dmesg (Filename :output.png)
