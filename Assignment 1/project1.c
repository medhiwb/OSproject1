// Medhi Welsh - Brito
// U47469189
// Operating Systems: Project 1

#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define len 1000

int main (int argc, char * argv[]) {

    char str[len], *cmd[len], *param[len];
    int count = 0; 

    printf("Enter a command: ");
    scanf("%[^\n]%*c", str); // allows everything entered (including spaces) to be stored in variable

    char * token = strtok (str, "^[ !@#$%^&* )(]{2,20}$"); //regex expression, splits string for every hypen detected

    cmd[0] = token;
    printf("%s \n", cmd[0]); // prints first argument
    while (token != NULL) { // stores every argument into param array
        param[count++] = token;
        token = strtok(NULL, "^[ !@#$%^&* )(]{2,20}$");
    }

    int call = fork(); // creates new process

    if (call < 0) { // fork failure, exits...
        exit(1);
    } else if (call == 0) { // child process
        char * args[count];
        args[0] = strdup(cmd[0]); //copies our first argument into args[] array
        int i;
            for (i = 1; i < count; i++) { //copies parameters into args[] array
                args[i] = strdup(param[i]);
            }
        args[count] = NULL; // last element of parameters has to be NULL
        execvp(args[0], args); // execute cmd with "args" being the parameters
    } else { // parent process
        int rest = wait(NULL); // waits for child process to finish execution
        printf("\n++++\n"); //prints ++++ on a new line
    }

    return 0;
} 
