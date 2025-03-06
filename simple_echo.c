#include <stdio.h>
#include <stdlib.h>

#define ZERO        (0)
#define SUCCESS     (ZERO)
#define ARG_ERR     (-1)

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        /* Error */
        printf("Usage: %s file-name\n", argv[ZERO]);
        exit(ARG_ERR);
    }

    for(int i = 1; i < argc; ++i)
    {
        printf("%s ",argv[i]);
    }

    printf("\n");

    return (SUCCESS);
}