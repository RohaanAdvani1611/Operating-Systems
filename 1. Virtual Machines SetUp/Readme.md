# Assignment: Virtual Machines Setup

Install at least 3 virtual machines using the ISO images or some other technique, using VirtualBox software, in Triple boot mode.  You can take help from the Wiki pages on COEP Wiki   https://foss.coep.org.in/coepwiki/index.php/Operating_Systems 

One of the 3 should be a GNU/Linux distribution, one Should be a variant of Windows, and the other can be anything (better to use some variant of Unix).

You should have a

After the installation is done, reboot and take a screenshot of the GRUB screen (showing the triple boot options). Call it  1grub.png

Boot using Linux System.

Inside it, mount all partitions of the two other operating systems.

Now run

$ df -h

Take a screenshot called  2df.png

The above should show free space remaining in all partitions of all operating systems.

Run ls on the partition of the Windows system and take a screenshot. Call it 3windows.png

Note that for the above to work you need to "mount" the Windows partition and only then you can see the files inside. Learn mounting.

Run ls on all the partitions of the third operating system and take a screenshot. Call it 4thirdos.png

Run cat /proc/partitions on Linux and take a screenshot. Call it 5partitions.png

Boot using Windows

Go to disk administration in Control panel showing the list of partitions. Take a screenshot. Call it  6partitions.png

Boot using the third OS

Figure out how to see the list of partitions on this OS. Take a screenshot and call it  7partitions.png

Submission

Submit a single MISID.tar.gz file with the above 7 files in it.  DO NOT SUBMIT a  .zip file, or a .rar file or any file that is not actually a tarred and gzipped file.

Wrongly named files will get zero marks.

Notes:

1) If you face any problems in the installation process, share them on the Discussion Forum.

On Linux, advice: try various possible "disk partitioning" options. It offers you options like "Use Entire disk", "Guided", "Manual", etc. options. You should try all of them, for learning purpose. It is highly useful in the study of file systems. During submission, submit only one!

2) Possible list of OSes

Various variants of GNU/Linux (Debian, Ubuntu, Fedora, ArchLinux, Redhat EL, Kali, Mint, Hamara, etc. )

Unixes (HP-UX, IBM-AIX, Open Solaris, Sun Solaris, Free BSD, MINIX, MAC-OS(?),  )

Windows Variants (XP, 7, 10, NT, Server, etc. )

Other (Android, Chromium OS, etc. )

DOS can be used, but only as a 4th operating system.

3) ISO images are available at https://foss.coep.org.in/isos/
