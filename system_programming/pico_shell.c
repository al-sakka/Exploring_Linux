#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define TRUE 1
#define EXIT 0
#define MAXI 1024

void launch_external(char **args)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            printf("%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int picoshell_main(int argc, char *argv[])
{
    char prompt[MAXI];
    size_t len = 0;
    char **args = NULL;
    char *token = NULL;
    int arg_count = 0;
    int _status = 0;

    while (TRUE)
    {
        printf("Pico-Shell>> ");
        fflush(stdout);

        if (fgets(prompt, sizeof(prompt), stdin) == NULL)
        {
            break;
        }

        // Remove trailing newline
        prompt[strcspn(prompt, "\n")] = '\0';

        // Skip empty input
        if (prompt[0] == '\0')
            continue;

        // Tokenize input
        arg_count = 0;
        token = strtok(prompt, " ");
        while (token)
        {
            char **new_args = (char **)realloc(args, sizeof(char *) * (arg_count + 2));
            if (!new_args)
                break;

            args = new_args;
            args[arg_count++] = strdup(token);
            token = strtok(NULL, " ");
        }

        if (args)
            args[arg_count] = NULL;

        if (args == NULL || args[0] == NULL)
            continue;

        // Built-in commands
        if (strcmp(args[0], "exit") == 0)
        {
            printf("Good Bye\n");
            break;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (arg_count < 2 || !args[1])
            {
                fprintf(stderr, "cd: missing operand\n");
                _status = 1;
            }
            else if (chdir(args[1]) != 0)
            {
                printf("cd: %s: No such file or directory\n", args[1]);
                _status = 1;
            }
            else
            {
                _status = 0;
            }
        }
        else
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                printf("%s: command not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    _status = WEXITSTATUS(status);
                else
                    _status = 1;
            }
            else
            {
                perror("fork");
                _status = 1;
            }
        }

        for (int i = 0; i < arg_count; i++)
            free(args[i]);
        free(args);
        args = NULL;
    }

    return (_status);
}

