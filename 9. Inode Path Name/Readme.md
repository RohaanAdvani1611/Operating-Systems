# (Optional Assignment) Print Inode or file data, using path name
Write a program which given the complete path name, will print the

(a) inode of the file/directory specified, from an ext2 file system.

(b) contents of the file or directory

Submit file, with name

ext2.c

Program runs like this:

./a.out /dev/sdb1 /test/t.c   inode

(format: ./a.out  device-file-name  path-on-partition  inode/data)

<inode printed here>

./a.out  /dev/sdc2 /test/t.c   data

<data printed here, listing of files if it's a directory>

Notes:

Before you start working on this, you need to first completely understand the ext2 file system layout. The notes available at http://wiki.osdev.org/Ext2 should be of help.  https://www.nongnu.org/ext2-doc/ext2.html is also useful.

On linux systems the hard disk partitions are accessible thorought the file interface provided by files like /dev/sda1, /dev/sdb2, etc.  You can call open("/dev/sda2", O_RDONLY) and start reading the partition /dev/sda2 as if it was a file. You need superuser privileges to make this open call. So you can try this in a virtualbox environment, by adding an extra disk to your Ubuntu virtual machine and formatting the disk with ext2 layout.

Try reading the file system incrementally, that is superblock, then group descriptors, and so on to make sure that you are on the right path. An important task lies in writing a recursive/iterative function which will split the components of the pathname into file/directory names and locate it on disk. You can think of writing (1) a function which takes two arguments: directory inode and name of a file in that directory, and returns inode number of the file (2) a function which given an inode number, returns that inode.

Some hints:

Superblock starts at 1024 bytes offset

Group descriptors start at 1 file-system-block offset from the beginning of partition

Size of inode is stored in super block

See the online diagrams of data structures for directory entries

Use the 64 bit variants of the file handling library functions (e.g. lseek64)

Try to modularise your code to simplify the work

Use http://wiki.osdev.org

You need to install e2fslibs-dev to get the header files.
