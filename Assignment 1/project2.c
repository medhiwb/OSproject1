// Medhi Welsh - Brito
// U47469189
// Operating Systems: Project 2

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define len 1000

int main (int argc, char * argv[]) {

    char str[len];
    printf("Enter commands in the form of cmd1[params1]| cmd2[params2]: ");
    scanf("%[^\n]%*c", str); // allows everything entered (including spaces) to be stored in variable

    char * token = strtok (str, "|");

    char * string1[len], * string2[len]; // left and right side of commands separated by "|"
    char * cmd1[len], * cmd2[len]; // the cmd
    char * params1[len], * params2[len]; // the parameters of cmd

    string1[0] = token;
    token = strtok(NULL, "|");
    string2[0] = token;

    // tokenizes first string
    char * newtoken1 = strtok (* string1, "^[ !@#$%^&* )(]{2,20}$"); //regex expression to catch hyphens

    cmd1[0] = newtoken1;
    int count1 = 0, count2 = 0;

    while (newtoken1 != NULL) { // stores every argument into param array 
        params1[count1++] = newtoken1;
        newtoken1 = strtok(NULL, "^[ !@#$%^&* )(]{2,20}$");
    }

    // tokenizes second string
    char * newtoken2 = strtok (* string2, "^[ !@#$%^&* )(]{2,20}$"); 

    cmd2[0] = newtoken2;

    while (newtoken2 != NULL) { // stores every argument for string2 into param array 
        params2[count2++] = newtoken2;
        newtoken2 = strtok(NULL, "^[ !@#$%^&* )(]{2,20}$");
    }

    int fd[2]; // file descriptors for pipe()

    if (pipe(fd) == -1) { // pipe failure, exits...
        return 1; 
    }

    int pid1 = fork();

    if (pid1 < 0) { // fork failure
        return 1;
    } else if (pid1 == 0) { // child process of pid1
        char * args[count1];
        args[0] = strdup(cmd1[0]);
        int i;
        for (i = 1; i < count1; i++)
        {
            args[i] = strdup(params1[i]);
        }
        args[count1] = NULL;
        dup2(fd[1], STDOUT_FILENO); // duplicates file descriptor
        close(fd[0]);
        close(fd[1]);
        execvp(args[0], args);
    }

    int pid2 = fork();

    if (pid2 < 0) { // fork failure, exit
        return 3;
    } else if (pid2 == 0) { // child process of pid2
        char * args2[count2];
        args2[0] = strdup(cmd2[0]);
        int a;
        for (a = 1; a < count2; a++)
        {   // copies parameters into args[] array
            args2[a] = strdup(params2[a]);
        }
        args2[count2] = NULL; // last element of parameters has to be NULL
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]); // closes file descriptor so later it may be reused
        close(fd[1]);
        execvp(args2[0], args2);
    }
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("\n++++\n"); 

    return 0;
}