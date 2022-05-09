# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>

int main(int argc, char *argv[]){
	int fd, fd1; //file descriptor 
	fd = open(argv[1],O_RDWR | O_CREAT, 0644); // Open file as read/write
	fd1 = open("copy.txt", O_RDWR | O_CREAT, 0644); // Create Copy File
	int value;
	char buffer[100];
	if (fd != -1){
		read(fd, buffer, 100); // Read data from original file
		write(fd1, buffer, 100); // Write data to copied file
		value = rename(argv[1], "Renamed.txt"); // Rename original file
		if (value)
			return 1;
		close(fd);
		close(fd1);
	}
	return 0;
}
