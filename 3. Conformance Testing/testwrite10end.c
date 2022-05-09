# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>

int main(int argc, char *argv[]){
	int fd; //file descriptor 
	fd = open(argv[1],O_RDWR | O_CREAT, 0644); // Open file as read/write
	int value;
	char *buffer = (char*)malloc(sizeof(char)*100);
	char msg[10] = "abcdefghij"; //Size of msg is 10
	if (fd != -1){
		lseek(fd, 0, SEEK_END); // Go to end
		write(fd, msg, sizeof(msg)); // Write msg in file
		lseek(fd, -11, SEEK_END); // Go back 10 char
		read(fd, buffer, sizeof(msg)); // Read to ensure that data is written properly
		//printf("\n%s\n",buffer);
		value = strcmp(msg,buffer);
		if (value != 0) // If data not written properly return error
			return 1;
		close(fd);
	}
	return 0;
}
