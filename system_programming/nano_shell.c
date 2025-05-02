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

typedef struct ShellVar
{
    char *name;
    char *value;
    struct ShellVar *next;
} ShellVar;

ShellVar *local_vars = NULL;

void set_local_var(const char *name, const char *value)
{
    ShellVar *curr = local_vars;
    while (curr)
    {
        if (strcmp(curr->name, name) == 0)
        {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }
    ShellVar *new_var = (ShellVar *)malloc(sizeof(ShellVar));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = local_vars;
    local_vars = new_var;
}

char *get_local_var(const char *name)
{
    ShellVar *curr = local_vars;
    while (curr)
    {
        if (strcmp(curr->name, name) == 0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}

void export_var(const char *name)
{
    char *val = get_local_var(name);
    if (val)
        setenv(name, val, 1);
}

void free_local_vars()
{
    ShellVar *curr = local_vars;
    while (curr)
    {
        ShellVar *tmp = curr;
        curr = curr->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}

void substitute_vars(char **args, int arg_count)
{
    for (int i = 0; i < arg_count; i++)
    {
        char *start = args[i];
        while ((start = strchr(start, '$')))
        {
            char *end = strchr(start, ' '); // Find the end of the variable name
            if (!end)
            {
                end = start + strlen(start); // End at the string's end
            }

            char *var_name = start + 1; // Skip the '$'
            size_t var_len = end - var_name;
            char *val = get_local_var(var_name);
            if (!val)
            {
                val = getenv(var_name); // Fall back to environment variables
            }

            // If a variable was found, substitute it
            if (val)
            {
                size_t new_len = strlen(args[i]) - var_len + strlen(val);
                char *new_str = (char *)malloc(new_len + 1);
                if (new_str)
                {
                    // Copy the part before the variable
                    strncpy(new_str, args[i], start - args[i]);
                    new_str[start - args[i]] = '\0';
                    // Append the variable value
                    strcat(new_str, val);
                    // Append the part after the variable
                    strcat(new_str, end);
                    free(args[i]);
                    args[i] = new_str;
                }
            }
            else
            {
                // No variable found, replace with an empty string
                size_t new_len = strlen(args[i]) - var_len + 1;
                char *new_str = (char *)malloc(new_len + 1);
                if (new_str)
                {
                    // Copy up to the '$' symbol
                    strncpy(new_str, args[i], start - args[i]);
                    new_str[start - args[i]] = '\0';
                    // Append an empty string
                    strcat(new_str, "");
                    // Append the rest of the string
                    strcat(new_str, end);
                    free(args[i]);
                    args[i] = new_str;
                }
            }
            start = end;
        }
    }
}

int is_assignment(const char *str)
{
    const char *eq = strchr(str, '=');
    if (!eq || eq == str || *(eq + 1) == '\0')
        return 0;
    // Ensure no spaces in the assignment
    for (const char *p = str; *p; p++)
    {
        if (*p == ' ')
            return 0;
    }
    return 1;
}

int nanoshell_main(int argc, char *argv[])
{
    char prompt[MAXI];
    size_t len = 0;
    char **args = NULL;
    char *token = NULL;
    int arg_count = 0;
    int _status = 0;

    while (TRUE)
    {
        printf("Nano-Shell>> ");
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

        // Handle assignment (no variable substitution here)
        if (is_assignment(args[0]))
        {
            if (arg_count > 1)
            {
                printf("Invalid command\n");
            }
            else
            {
                char *eq = strchr(args[0], '=');
                *eq = '\0';
                const char *var = args[0];
                const char *val = eq + 1;
                set_local_var(var, val);
            }
        }
        else
        {
            // variable substitution for all other commands
            substitute_vars(args, arg_count);

            if (strcmp(args[0], "export") == 0)
            {
                if (arg_count < 2)
                    fprintf(stderr, "export: missing variable name\n");
                else
                    export_var(args[1]);
            }
            else if (strcmp(args[0], "exit") == 0)
            {
                printf("Good Bye\n");
                break;
            }
            else if (strcmp(args[0], "cd") == 0)
            {
                if (arg_count < 2 || !args[1])
                    fprintf(stderr, "cd: missing operand\n");
                else if (chdir(args[1]) != 0)
                {
                    printf("cd: %s: No such file or directory\n", args[1]);
                    exit(1);
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
        }

        for (int i = 0; i < arg_count; i++)
            free(args[i]);
        free(args);
        args = NULL;
    }

    free_local_vars();
    return _status;
}