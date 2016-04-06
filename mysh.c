#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_LEN 100
#define SPACE 32

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

void read_command(char** command, char*** params)
{
    int count = 0;
    char* tmp;
    char** result;
    const char delim[] = " ";

    tmp = (char*)malloc(sizeof(char));

    fgets(tmp, MAX_LEN, stdin);
    tmp[strlen(tmp)-1] = '\0';

    if (!(strcmp(tmp, "exit")))
        exit(1);

    *command = tmp;

    while (*tmp)
    {
        if ((*tmp) == SPACE)
          count++;
        tmp++;
    }

    count += 2;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(*(command), delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(NULL, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    *params = result;
}

int main(void)
{
    char* command;
    char** params;
    int status;
    pid_t pid;

    while(1){
        type_prompt();
        read_command(&command, &params);

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
