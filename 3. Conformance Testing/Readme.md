# (Optional Assignment) Shell Programming(Conformance tests)
Write a set of conformance tests to test if your operating system conforms to prescribed behaviour. This is to be done using SHell programming and C programs.

Example of how to write a  conformance test:

Given below is a C program.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return 1;
    return 0;    
}

All that this program does is it opens a file, specified by argv[1] and returns 0 if successfully opened.

This program is compiled using

$ cc testopen.c -o testopen

Give below a shell script (runtest.sh) that tests this program.

#!/bin/bash
touch /tmp/file1
chmod 644 /tmp/file1
./testopen /tmp/file1
if [ $? -eq 0 ]
then
    echo "testopen Pass"
else
    echo "testopen Fail"
fi


When you run the above program

$ bash ./runtest.sh

It should print

"testopen Pass"

The above program tests whether the "open" system call is able to successfully open a file (return proper fd) on a file which is assumed to have read permissions on it.  Here "testopen" is a name of the test (test name).

The program tests whether the OS conforms to one expected behaviour from the open() system call.  There are system calls for many facilities offered by OS, but you are required to write tests only for file system related system calls as you are aware of these functions.

You are expected to write a set of C programs (one program or many programs, does not matter) and one single shell script to test whether your operating system conforms to the following described behaviour of the system calls:

Open a new file for writing in /tmp/ folder and write 100 bytes of data to it. Ensure that the data specified was written properly. Here the C code should only be opening file and writing data, and the rest of the testing should be done using a shell script. You can use commands like cat, echo, diff to do the test. Test name: read100
Open an existing file for read-write, read it's byte number 5 to 10. You can use commands like sed, awk, cat, echo for this. Test name: read510
Open an existing file for read-write and write 10 bytes at the end of the file.   (Ensure that data read is correct and data are written went to the end of the file). Command like cat, echo, diff may be useful. Test names:  write10end
Create a copy of an existing file. Check using diff if copy was correct. Testname: copy
Rename an existing file using a C program and the link () or rename() system call. Test using shell code if the renaming worked. Commands like diff should be sufficient. Testname: rename
The shell program file should be called test.sh

The expected output of the shell program is simply like this

 

read100 pass
read510 pass
write10end pass
copy pass
rename pass
Note that your skill lies in writing one or more C programs that are parameterised enough and calling those C programs using shell scripts.
