#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE (1)
#define END (0)
#define FAILURE (-1)
#define SIZE (1024)
#define SIZE_LOW (256)
#define MAX_VARIABLES (1000)

typedef struct
{
    char *name;
    char *value;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;
char* var = NULL;

void save_variable(char *name, char *value)
{
    /* Update variable */
    for (int i = 0; i < var_count; ++i)
    {
        if (!strcmp(variables[i].name, name))
        {
            free(variables[i].value);
            variables[i].value = strdup(value);
            return;
        }
    }

    /* Add new variable */
    if (var_count < MAX_VARIABLES)
    {
        variables[var_count].name = strdup(name);
        variables[var_count].value = strdup(value);
        ++var_count;
    }
    else
    {
        printf("Max variables reached\n");
    }
}

char *get_variable(char *name)
{
    for (int i = 0; i < var_count; ++i)
    {
        if (!strcmp(variables[i].name, name))
        {
            return variables[i].value;
        }
    }

    return (NULL);
}

void export_variable(char *name)
{
    for (int i = 0; i < var_count; ++i)
    {
        if (!strcmp(variables[i].name, name))
        {
            if (!setenv(variables[i].name, variables[i].value, 1))
                printf("Exported: %s\n", variables[i].name);

            return;
        }
    }

    printf("Variable %s Not Found!\n", name);
}

void exit_cmd(void)
{
    printf("Nano Shell Terminated\n");
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
        printf("Invalid command\n");
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

void replaceVar(char* input, const char* target, const char *replacement)
{
    char buffer[SIZE];
    char *pos = strstr(input, target);

    if(pos != NULL)
    {
        strncpy(buffer, input, pos - input);
        buffer[pos - input] = 0;

        strcat(buffer, replacement);

        strcat(buffer, pos + strlen(target));

        strcpy(input, buffer);
    }
}

void nano_shell(void)
{
    while (TRUE)
    {
        char prompt[SIZE];
        char *args[SIZE];

        char *token;
        int args_count = 0;

        printf("Nano:%s>> ", get_wd());

        fgets(prompt, sizeof(prompt), stdin);

        prompt[strcspn(prompt, "\n")] = 0; /* Removes '\n' from the buffer */

        if(strchr(prompt, '$'))     /* '$' Exists */
        {
            var = strdup(strchr(prompt, '$') + 1);
            
            for(int i = 0 ; var[i] != '\0' ; ++i)
            {
                if(*(var + i) == ' ')
                {
                    *(var + i) = '\0';
                    break;
                }
            }

            // TODO:: Replace the variable in the prompt

            char offset[SIZE_LOW] = "$";
            strcat(offset, var);

            replaceVar(prompt, offset, get_variable(var));

            free(var);
        }

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
            printf("%s\n", get_wd());
        }
        else if (!strcmp(args[0], "cd"))
        {
            if (!chdir(args[1]))
                printf("New directory: %s\n", get_wd());
            else
                printf("Can't change directory\n");
        }
        else if (!strcmp(args[0], "export"))
        {
            for(int i = 1 ; i < args_count ; ++i)
            {
                export_variable(args[i]);
            }
        }
        else if (strchr(args[0], '='))
        {
            char *target = strchr(args[0], '=');

            if ((target > args[0]) && (*(target + 1) != '\0'))
            {

                *target = '\0';

                char *name = args[0];
                char *value = target + 1;

                save_variable(name, value);
            }
            else
            {
                printf("Invalid command\n");
            }
        }
        else
        {
            exec_command(args);
        }
    }
}

int main(void)
{
    nano_shell();
    return (END);
}