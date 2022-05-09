#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>


void readCommand(char cmd[], char *par[]) {
	char line[1024];
	int count = 0, i = 0, j = 0;
	char *array[100], *temp;

	for( ;; ){
		int c = fgetc(stdin); //Get the line char by char
		line[count++] = (char) c;
		if (c == '\n') break;
	}
	if (count == 1) return;
	temp = strtok(line, " \n");

	while (temp != NULL) {
		array[i++] = strdup(temp);
		temp = strtok(NULL, " \n");
	}
	//Update command and parameters
	strcpy(cmd, array[0]);

	for(int j=0; j<i; j++)
		par[j] = array[j];
	par[i] = NULL;
}


void prompt(char a[]) {
	static int x = 1;
	if(x) {
		const char* CLEAR_SCREEN = " \e[1;1H\e[2J";
		write(STDOUT_FILENO,CLEAR_SCREEN,12);
		x = 0;
	}
	//Print the custom prompt
	printf("%s$", a);
}


void handle_cd(char *par[], char a[100]){
	char temp[100] = "";
	int i, j, k,l , n;
	DIR *dr;
	//Case 1: cd ..
	if(par[1][0] == '.' && par[1][1] == '.'){
		i = strlen(a);
		while(a[i] != '/')
			i--;
		for(j = 0; j < i; j++)
			temp[j] = a[j];
		printf("%s ", temp);
		strcpy(a, temp);
		for(i = j; i < 100; i++)
			a[i] = '\0';
		return;
	}
	//Case 2: cd /usr/bin
	if(par[1][0] == '/') {
		strcpy(temp, par[1]);
		dr = opendir(temp);
		if(dr) {
			strcpy(a, temp);
			return;
		}
		else {
			printf("error");
			return;
		}
	}
	//Case 3: cd dirname
	else {
		strcpy(temp, a);
		strcat(temp, "/");
		strcat(temp, par[1]);
		dr = opendir(temp);
		if(dr) {
			strcpy(a, temp);
			return;
		}
		else {
			printf("error");
			return;
		}
	}
	
}


void handle_ps1(char *par[], char a[100]){
	if (par[1][0] == '/' && par[1][1] == 'w' && par[1][2] == '$') //Revert Feature
		getcwd(a,100);
	else
		strcpy(a,par[1]);
	return;
}


void update_path(char envp[], char *par[]){
	//printf("%s",par[1]);
	char temp[100] = "";
	DIR *dr;
	if(par[1][0] == '/') {
		strcpy(temp, par[1]);
		printf("%s",temp);
		dr = opendir(temp);
		if(dr) {
			strcpy(envp, temp);
			return;
		}
		else {
			printf("error");
			return;
		}
	}
	else {
		printf("error");
		return;
	}
}

//void history(char hist[][], char *par[]){
//	int i=0, j=1;
//	if(par[0]){
//		hist[i] = par[0];
//		hist[j] = par[1];
//		i = i + 2;
//		j = j + 2;
//	}
//}

int main() {
	char cmd[100], command[100], *parameters[20], line[100];
	// Environment Variable
	char *envp[] = { (char *) "PATH=/usr/bin", 0 };
	char a[100];
	char hist[100][100];
        getcwd(a,100);
        //char *buffer[100];
        printf("______________________________________________________________________________________________________\n");
        printf("WELCOME TO MY SHELL:\n");
        printf("---------------------\n");
        printf("The following can be done on it :\n");
        printf("1. Run prompt in loop\n");
        printf("2. Run all system call commands without complete path with arguments\n");
        printf("3. Handle cd command functionality\n");
        printf("NOTE : Type command as shown in square brackets put path name instead of pathname");
        printf("4. Handle [PS1 pathname] command functionality\n");
        printf("5. Handle update enviormental [PATH pathname] for system calls\n");
        printf("6. Exit the shell on typing exit\n");
        printf("7. Error handling for all the above functions\n");
        printf("______________________________________________________________________________________________________\n");
        int i=0, j=1;
	while(1) { //Loop prompt continuously
		prompt(a);
		//UNCOMMENT THE BELOW 3 LINES FOR HISTORY FUNCTION WAS NOT ABLE TO MAKE IT COMPATIBLE WITH MY CODE
		//buffer = readline(a);
		//if(strlen(buffer)>0)
		//	add_history(buffer);
		readCommand(command, parameters); //Read the comm and arguments
		if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't') //exit
			break;
		if(command[0] == 'P' && command[1] == 'A' && command[2] == 'T' && command[3] == 'H')
			update_path(*envp, parameters); //update_path function
		if (command[0] == 'c' && command[1] == 'd'){
			handle_cd(parameters, a); //Handle cd
		}
		if (command[0] == 'P' && command[1] == 'S' && command[2] == '1'){
			handle_ps1(parameters, a); //Handle PS1
		}
		if(fork() != 0)
			wait(NULL); //Let child processess finish
		else{
			strcpy(cmd, "/usr/bin/");
			strcat(cmd, command);
			execve(cmd, parameters, envp); //Execute command with parameters from envp directry executiable
			
		}
		
		//history(hist[][], parameters);
		//i=i+2;
		//j=j+2;
		//printf("%s",hist[i]);
	}
	return 0;
}
