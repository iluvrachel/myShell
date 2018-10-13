#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

char *commands[1024];
//char current_path[1024];//TODO

void read_cmd();
void judge_cmd();
//char **parse_cmd(char *str);

int main(int argc,char **argv){
	//char *cmd;
	int n;
	while(1){
		printf("LinShell¬$");
		fflush(stdout);
		read_cmd();
		judge_cmd();
	}
	return 0;
}

void read_cmd(){
	char c;
	char *buff = malloc(sizeof(char)*1024);
	int position = 0;
	int count = 0;
	int size = 1024;
	char *parser;
	while(1){
		c = getchar(); 
		if(c == '\n'){
			buff[position] = '\0';
			break;
		} else{
			buff[position] = c;
		}
		position++;
		if(position >= size){
			printf("Sorry, your command is too complicated to be handled!\n" );
		}
	}
	parser = strtok(buff," ");
	while(parser != NULL){
		commands[count] = parser;
		count++;
		parser = strtok(NULL," ");
	}
	commands[count] = NULL;
}

void judge_cmd(){
	//char **buff[1024];
	//strcpy(buff,cmd);
	//buff = parse_cmd(cmd);
	if(strcmp(commands[0],"help") == 0){
		printf("Welcome to the fake shell produced by Lin Zhizhao\n");
		printf("This shell just implement a few internal commands like\n");
		printf("help cd and exit\n");
		//TODO
	}
	else if(strcmp(commands[0],"cd") == 0){
		if(commands[1] == NULL){
			printf("ERROR: no path was declared!\n");
		}else{
			if(chdir(commands[1]) == -1){
				printf("No such file or directory!\n");
			}
			else{
				//strcpy(current_path,commands[1]);//TODO
			}
		}
	}
	else if(strcmp(commands[0],"exit") == 0){
		printf("Goodbye\n");
		sleep(1);
		exit(0);
	}
	else{

		pid_t pid = fork();
		if(pid>0){
			int st;
			while(wait(&st) != pid);
		}else if(pif==0){
			int res;
			res = execvp(commands[0],commands);
			if(res<0){
				printf("command not found\n");
			}
		}
	}
}
/*
char** parse_cmd(char *str){
	int position = 0;
	char *cmd;
	char **cmds = malloc(sizeof(char*)*1024);

	cmd = strtok(str," ");
	while(cmd != NULL){
		cmds[position] = cmd;
		position++;
		cmd = strtok(NULL," ");
	}
	cmds[position] = NULL;
	return cmds;
}
*/