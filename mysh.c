#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN 100
#define SPACE 32

void removeSubstring(char *s,const char *toremove)
{
  while(s = strstr(s,toremove))
      memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void replace_home_dir(char *command) {
    //if (strcspn(command, "~") == strlen(command))
    //    return;

    char homedir[100];
    strcpy(homedir, getenv("HOME"));

    char *pch;
    pch = strstr(command, homedir);
    if(pch != NULL){
        removeSubstring(command,homedir);
        strcpy(homedir,"~");
        strcat(homedir,command);
        strcpy(command,homedir);
    }
}

void type_prompt()
{
    char hostName[MAX_LEN], userName[MAX_LEN], curDir[MAX_LEN];

    gethostname(hostName, MAX_LEN);
    getlogin_r(userName, MAX_LEN);
    getcwd(curDir, MAX_LEN);

    replace_home_dir(curDir);

    fprintf(stdout, "[MySh] ");
    fprintf(stdout, userName);
    fprintf(stdout, "@");
    fprintf(stdout, hostName);
    fprintf(stdout, ":");
    fprintf(stdout, curDir);
    fprintf(stdout, "$ ");

    fflush(stdout);
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

void signal_handler(int signo)
{
    printf("\n");
    type_prompt();
}

int main(void)
{
    char* command;
    char** params;
    int status;
    pid_t pid;
    int ret;

    signal(SIGINT, &signal_handler);
    signal(SIGTSTP, &signal_handler);

    while(1){
        type_prompt();

        /*if((ret = getchar()) == EOF){
            printf("\n");
            exit(1);
        }*/

        read_command(&command, &params);

        if (strcmp("cd", command) == 0) {
            ret = chdir(params[1]);

            if (ret) {
                fprintf(stderr, "mysh: cd: %s: No such file or directory\n", params[1]);
            }

            continue;
        }

        pid = fork();
        if(pid < 0){
           fprintf(stderr, "Unable to fork");
           continue;
        }

        if(pid != 0){
            waitpid(-1, &status, 0);
        }else{
            ret = execvp(command, params);

            if (ret)
                fprintf(stderr, "%s: command not found\n", command);
        }

    }
    return 0;
}
