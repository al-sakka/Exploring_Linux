#include <stdio.h>
#include <fcntl.h>  /* For open */
#include <unistd.h> /* For read & write & unlink */
#include <stdlib.h> /* For exit */

#define ZERO        (0)
#define SUCCESS     (ZERO)
#define ARG_ERR     (-1)
#define OPEN_ERR    (-2)
#define WRITE_ERR   (-3)
#define MODE        (0644)
#define COUNT       (100)

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        /* Error */
        printf("Usage: %s source-file destination-file\n", argv[ZERO]);
        exit(ARG_ERR);
    }

    char buf[COUNT];
    int num_read;

    char *source_path       = argv[1];
    char *destination_path  = argv[2];

    int src_fd = open(source_path, (O_RDONLY));
    int des_fd = open(destination_path, (O_CREAT | O_WRONLY), MODE);

    if ((src_fd < ZERO) || (des_fd < ZERO))
    {
        /* Error */
        printf("Couldn't open file\n");
        exit(OPEN_ERR);
    }

    while ((num_read = read(src_fd, buf, COUNT)) > ZERO) /* Will return 0 at the end of the file */
    {
        if (write(des_fd, buf, num_read) < ZERO)
        {
            /* Error */
            printf("Couldn't create new file\n");
            exit(WRITE_ERR);
        }
    }

    unlink(source_path);
    close(src_fd);

    /* Ensure data is flushed */
    fsync(des_fd);
    
    close(des_fd);

    return (SUCCESS);
}