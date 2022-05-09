# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>

int main(int argc, char *argv[]){
	int fd; //file descriptor 
	fd = open(argv[1],O_RDWR | O_CREAT, 0644); // Open file as read/write
	char buffer[6];
	if (fd != -1){
		lseek(fd, 4, SEEK_SET); // Go to 5th char
		read(fd, buffer, 6); // Read next 6 char
		if (strlen(buffer) != 6) // If data not read properly return error
			return 1;
		close(fd);
	}
	return 0;
}
