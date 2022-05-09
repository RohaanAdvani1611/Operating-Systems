# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>

int main(int argc, char *argv[]){
	int fd; //file descriptor 
	fd = open(argv[1],O_RDWR | O_CREAT, 0644); // Open file as read/write
	int value;
	char buffer[101];
	char msg[101] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv"; //Size of msg is 100 
	if (fd != -1){
		write(fd, msg, sizeof(msg)); // Write msg in file
		lseek(fd, 0, SEEK_SET); // Go back to first char
		read(fd, buffer, sizeof(msg)); // Read to ensure that data is written properly
		value = strcmp(msg,buffer);
		//printf("%s",buffer);
		if (value != 0) // If data not written properly return error
			return 1;
		close(fd);
	}
	return 0;
}
