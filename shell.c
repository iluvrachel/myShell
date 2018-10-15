#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<pwd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>


char *commands[1024];
//char current_path[1024];//TODO

void read_cmd();
void judge_cmd();
int is_Pipe();
void parse_Pipe();
void do_Pipe();

//char **parse_cmd(char *str);

int main(int argc,char **argv){
	//char *cmd;
	int n;
	while(1){
		printf("LinShell¬$");
		fflush(stdout);
		read_cmd();

		int ret1 = is_Pipe();
		//printf("%d\n", ret1);
        if(ret1 > 0)
        {
            //if is pipe
            char* argv1[10];
            char* argv2[10];
            parse_Pipe(argv1,argv2);
            do_Pipe(argv1,argv2);
        }
        else
        {
            judge_cmd();
        }

		
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
		}else if(pid==0){
			int res;
			res = execvp(commands[0],commands);
			if(res<0){
				printf("command not found\n");
			}
		}
	}
}


/*pipe implement*/
int is_Pipe()
{                                                                                                                                       
    int i = 0;
    while(commands[i] != NULL)
    {
    	//printf("debug\n");
        if(strcmp(commands[i], "^") == 0)
        {
            return i+1;     //i is pipe symbol，so i+1 is the second command
        }
        ++i;
    }
    return 0;
}

void parse_Pipe(char* output1[],char* output2[])                   

{
    int i = 0;
    int size1 = 0;
    int size2 = 0;
    int ret = is_Pipe();   

    while(strcmp(commands[i], "^")!=0)
    {
        output1[size1++] = commands[i++];
    }
    output1[size1] = NULL;  //split the command into two command, *char, end with NULL

    int j = ret;
    while(commands[j] != NULL)
    {
        output2[size2++] = commands[j++];
    }           
    output2[size2] = NULL;
}

void do_Pipe(char* argv1[],char* argv2[])  //command before pipe & command after pipe
{
    pid_t pid = fork();
    if(pid == -1)                                                                                                                       
    {                                                                                                                           
        perror("fork()");
    }
    else if(pid == 0)
    {
        //child, create a pipe
        int fd[2];
        int ret = pipe(fd);

        if(fork() == 0 )
        {
            //create a grandchild process for execute the command before pipe
            close(fd[0]); //grandchild close the reading end of pipe
            dup2(fd[1],1);  //the output is the input of pipe
            execvp(argv1[0],argv1);
        }
        wait(NULL);
        close(fd[1]); //child close writing end

        dup2(fd[0],0);   //read the output of pipe as input
        execvp(argv2[0],argv2);
        //execvp failed
        perror("execvp()");
        exit(0);    
    }
    else
    {
        //parent
        wait(NULL);                                                                                                                     
    }
}
