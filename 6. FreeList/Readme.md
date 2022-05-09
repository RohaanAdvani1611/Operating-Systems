# (Mandatory Assignment) Change free list management in xv6
XV6 uses a very simple data structure to maintain the list of frames.

The frames that are free are linked together using the first 4 bytes in the frames themselves.

This is done in functions kinit1(), kinit2() and then later used in kalloc() and kfree().

Replace this code to use some other data structure. For example - an array containing the list of free frames, a separate linked list containing the list of free frames, etc.

Note: This assignment is likely to take time because reading and understanding xv6 will take time. You will have to revise a lot of concepts related to linking, loading memory managent to understand what you are doing.

Submission :

You need to submit a patch file created using "git diff.

A patch file is nothing but the output of "diff" (either "diff" or "git diff").

You need to create a branch locally (say branch name is "xv6-free-list").  Run the following command , while being on the new branch, to get the patch file

$ git diff master  > /tmp/MIS.patch

Submit the MIS.patch file
