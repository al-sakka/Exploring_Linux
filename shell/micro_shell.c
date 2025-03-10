#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define TRUE            (1)
#define END             (0)
#define FAILURE         (-1)
#define SIZE            (1024)
#define SIZE_LOW        (256)
#define MAX_VARIABLES   (128)

typedef struct
{
    char *name;
    char *value;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;

void replaceVar(char *input, char *target, char *replacement)
{
    char buffer[SIZE];
    char *pos = strstr(input, target);

    if (pos != NULL)
    {
        strncpy(buffer, input, pos - input);
        buffer[pos - input] = 0;

        strcat(buffer, replacement);

        strcat(buffer, pos + strlen(target));

        strcpy(input, buffer);
    }
}

void save_variable(char *name, char *value)
{
    /* Update variable first if exist */
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
            return (variables[i].value);
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

void handle_variables(char *token)
{
    char *var = strdup(token + 1);

    for (int i = 0; var[i] != '\0'; ++i)
    {
        if (*(var + i) == ' ')
        {
            *(var + i) = '\0';
            break;
        }
    }

    char *replacement = get_variable(var);

    char offset[SIZE_LOW] = "$";
    strcat(offset, var);

    if (replacement != NULL)
    {
        replaceVar(token, offset, replacement);
    }
    else
    {
        replaceVar(token, offset, " ");
    }

    free(var);
}

void exit_cmd(void)
{
    printf("Micro Shell Terminated.\n");
    exit(END);
}

void execute_command(char **args, char *input_file, char *output_file, char *error_file)
{
    pid_t pid = fork();

    if (pid > 0)
    {
        int child_status;
        wait(&child_status);
    }
    else if (pid == 0)
    {
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                printf("the shell could not open the file for reading/writing.\n");
                exit(FAILURE);
            }
            dup2(fd, STDIN_FILENO /* 0 */);
            close(fd);
        }

        if (output_file)
        {
            int fd = open(output_file, (O_CREAT | O_WRONLY | O_TRUNC), 0644);
            if (fd < 0)
            {
                printf("the shell could not open the file for reading/writing.\n");
                exit(FAILURE);
            }
            dup2(fd, STDOUT_FILENO /* 1 */);
            close(fd);
        }

        if (error_file)
        {
            int fd = open(error_file, (O_CREAT | O_WRONLY | O_TRUNC), 0644);
            if (fd < 0)
            {
                printf("the shell could not open the file for reading/writing.\n");
                exit(FAILURE);
            }
            dup2(fd, STDERR_FILENO /* 2 */);
            close(fd);
        }

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

void prompt_tokenization(char *input, char **args, int *args_count, char **input_file, char **output_file, char **error_file)
{
    char *token;

    token = strtok(input, " \t\r\n");

    while (token != NULL)
    {
        if (!strcmp(token, "<"))
        {
            token = strtok(NULL, " \t\r\n");
            if (token)
                *input_file = token;
        }
        else if (!strcmp(token, ">"))
        {
            token = strtok(NULL, " \t\r\n");
            if (token)
                *output_file = token;
        }
        else if (!strcmp(token, "2>"))
        {
            token = strtok(NULL, " \t\r\n");
            if (token)
                *error_file = token;
        }
        else if (strchr(token, '$'))
        {
            handle_variables(token);
            args[(*args_count)++] = token; /* Replaced with the new variable */
        }
        else
        {
            args[(*args_count)++] = token;
        }
        token = strtok(NULL, " \t\r\n");
    }
    args[*args_count] = NULL;
}

void micro_shell(void)
{
    while (TRUE)
    {
        char prompt[SIZE];
        char *args[SIZE];

        char *input_file = NULL;
        char *output_file = NULL;
        char *error_file = NULL;

        int args_count = 0;

        printf("Micro:%s>> ", get_wd());

        fgets(prompt, sizeof(prompt), stdin);

        prompt[strcspn(prompt, "\n")] = 0; /* Removes '\n' from the buffer */

        prompt_tokenization(prompt, args, &args_count, &input_file, &output_file, &error_file);

        if (args[0] == NULL)    continue;

        if (!strcmp(args[0], "exit"))
        {
            exit_cmd();
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
            for (int i = 1; i < args_count; ++i)
            {
                export_variable(args[i]);
            }
        }
        else if (strchr(args[0], '='))
        {
            char *target = strchr(args[0], '=');

            if ((target > args[0]) && (*(target + 1) != '\0') && (args_count == 1))
            {

                *target = '\0';

                char *name = args[0];
                char *value = target + 1;

                save_variable(name, value);
            }
            else
            {
                printf("Invalid command.\n");
            }
        }
        else
        {
            execute_command(args, input_file, output_file, error_file);
        }
    }
}



int main(void)
{
    micro_shell();
    return (END);
}