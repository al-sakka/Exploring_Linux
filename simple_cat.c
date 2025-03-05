#include <stdio.h>
#include <fcntl.h>  /* For open */
#include <unistd.h> /* For read & write */
#include <stdlib.h> /* For exit */

#define ZERO        (0)
#define SUCCESS     (ZERO)
#define ARG_ERR     (-1)
#define OPEN_ERR    (-2)
#define WRITE_ERR   (-3)
#define COUNT       (100)

int main(int argc, char** argv)
{
    char buf[COUNT];
    int num_read;

    if(argc < 2)
    {
        printf("Usage: %s file-name\n", argv[ZERO]);
        exit(ARG_ERR);
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd < ZERO)
    {
        /* Error */
        printf("Couldn't open file\n");
        exit(OPEN_ERR);
    }

    while ((num_read = read(fd, buf, COUNT)) > ZERO)   /* Will return 0 at the end of the file */
    {
        if (write(1 /* STD_OUT */, buf, num_read) < ZERO)
        {
            /* Error */
            printf("Couldn't write to file\n");
            exit(WRITE_ERR);
        }
    }
    
    close(fd);

    return (SUCCESS);
}