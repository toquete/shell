#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 100

void type_prompt()
{
    char hostName[MAX_LEN], userName[MAX_LEN], curDir[MAX_LEN];

    gethostname(hostName, MAX_LEN);
    getlogin_r(userName, MAX_LEN);
    getcwd(curDir, MAX_LEN);

    fprintf(stdout, "[MySh] ");
    fprintf(stdout, userName);
    fprintf(stdout, "@");
    fprintf(stdout, hostName);
    fprintf(stdout, ":");
    fprintf(stdout, curDir);
    fprintf(stdout, "$ ");
}

void read_command(char* command, char** params)
{
    int i = 0;
    char* word;

    fgets(command,MAX_LEN ,stdin);
    command[strlen(command)-1] = '\0';

    if (!(strcmp(command, "exit")))
        exit(1);

    word = strtok(command, " ");
    strcpy(command,word);

    while (word != NULL){
        word = strtok(NULL, " ");
        params[i] = word;
        i++;
    }
}

int main(void)
{
    char command[MAX_LEN];
    char params[MAX_LEN][MAX_LEN];
    int status;
    pid_t pid;

    while(1){
        type_prompt();
        read_command(command,params);

        pid = fork();
        if(pid < 0){
           fprintf(stderr, "Unable to fork");
           continue;
        }

        if(pid != 0){
            waitpid(-1, &status, 0);
        }else{
            execvp(command, params);
            fprintf (stderr, "An error ocurred calling execvp\n");
            abort();
        }

    }
    return 0;
}

