#include <stdio.h>
#include <string.h>

#define TRUE (1)
#define EXIT (0)
#define SIZE (100)

int main(void)
{
    while (TRUE)
    {
        char prompt[SIZE];
        char *token;

        printf("Femto-Shell>> ");

        if (fgets(prompt, sizeof(prompt), stdin) == NULL)
        {
            printf("Good Bye :)\n");
            break;
        }

        prompt[strcspn(prompt, "\n")] = 0; /* Removes '\n' from the buffer */

        token = strtok(prompt, " ");       /* Split the prompt */

        if (token == NULL)
        {
            printf("Good Bye :)\n");
            break;
        }

        if (!strcmp(token, "exit"))
        {
            printf("Good Bye :)\n");
            break;
        }
        else if (!strcmp(token, "echo"))
        {
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                printf("%s ", token);
                token = strtok(NULL, " ");
            }
            printf("\n");
        }
        else
        {
            printf("Invalid command\n");
        }
    }

    return (EXIT);
}