#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE (1)
#define END (0)
#define FAILURE (-1)
#define SIZE (1024)

void exit_cmd(void)
{
    printf("Good Bye :)\n");
    exit(END);
}

void echo_cmd(int args_count, char **args)
{
    for (int k = 1; k < args_count; ++k)
    {
        printf("%s ", args[k]);
    }
    printf("\n");
}

void exec_command(char **args)
{
    pid_t pid = fork();

    if (pid > 0)
    {
        int child_status;
        wait(&child_status);
    }
    else if (pid == 0)
    {
        execvp(args[0], args);
        printf("Execution Failed\n");
        exit(FAILURE);
    }
}

char *get_wd(void)
{
    char *buf = malloc(SIZE);
    if (buf == NULL)
        return NULL;
    return getcwd(buf, SIZE);
}

void pico_shell(void)
{
    while (TRUE)
    {
        char prompt[SIZE];
        char *args[SIZE];

        char *token;
        int args_count = 0;

        char *cwd = get_wd();
        printf("Pico:%s>> ", cwd);
        free(cwd);

        if(fgets(prompt, sizeof(prompt), stdin) == NULL)
            continue;

        prompt[strcspn(prompt, "\n")] = 0; /* Removes '\n' from the buffer */

        token = strtok(prompt, " \t\r\n"); /* Split the prompt */

        if (token == NULL)
            continue;

        /* Tokenization of the prompt */
        while (token != NULL)
        {
            args[args_count++] = token;
            token = strtok(NULL, " \t\r\n");
        }
        args[args_count] = NULL;

        if (!strcmp(args[0], "exit"))
        {
            exit_cmd();
        }
        else if (!strcmp(args[0], "echo"))
        {
            echo_cmd(args_count, args);
        }
        else if (!strcmp(args[0], "pwd"))
        {
            char *cwd = get_wd();
            printf("%s\n", cwd);
            free(cwd);
        }
        else if (!strcmp(args[0], "cd"))
        {
            if (!chdir(args[1]))
            {
                char *cwd = get_wd();
                printf("New directory: %s\n", cwd);
                free(cwd);
            }

            else
                printf("Can't change directory\n");
        }
        else
        {
            exec_command(args);
        }
    }
}

int main(void)
{
    pico_shell();
    return (END);
}