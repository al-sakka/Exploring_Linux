#include <stdio.h>
#include <string.h>

#define TRUE (1)
#define EXIT (0)
#define SIZE (1024)

int main(void)
{
    char prompt[SIZE] = {0};
    
    while (TRUE)
    {

        printf("Femto-Shell>> ");
        fflush(stdout);

        if (fgets(prompt, sizeof(prompt), stdin) == NULL)
        {
            break;
        }

        prompt[strcspn(prompt, "\n")] = 0; /* Removes '\n' from the buffer */
        
        if(prompt[0] == 0) continue;

        if (!strcmp(prompt, "exit"))
        {
            printf("Good Bye\n");
            break;
        }
        else if (!strncmp(prompt, "echo ", 5))
        {
            printf("%s\n", prompt + 5);
        }
        else
        {
            printf("Invalid command\n");
        }
    }

    return (EXIT);
}