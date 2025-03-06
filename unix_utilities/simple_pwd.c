#include <stdio.h>
#include <unistd.h>

#define ZERO        (0)
#define SUCCESS     (ZERO)
#define FAILURE     (1)
#define COUNT       (100)

int main(void)
{
    char buf[COUNT];

    char* path = getcwd(buf, COUNT);

    if(path == NULL)
    {
        printf("Operation returned NULL value.");
        return (FAILURE);
    }
    
    printf("Current working directory: %s\n", path);

    return (SUCCESS);
}