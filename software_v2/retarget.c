#include  <errno.h>
#include  <stdio.h>
#include  <sys/unistd.h>
#include "drivers/uart/uart.h"

#undef errno
extern int errno;

int _read(int file, char *data, int len)
{
    int bytes_read;

    if (file != STDIN_FILENO)
    {
        errno = EBADF;
        return -1;
    }

    for (bytes_read = 0; bytes_read < len; bytes_read++)
    {
        *data = read_uart(1000U);
        data++;
    }

    return bytes_read;
}

int _write(int file, char *data, int len)
{
    int bytes_written;

    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
        errno = EBADF;
        return -1;
    }

    for (bytes_written = 0; bytes_written < len; bytes_written++)
    {
        print(*data);
        data++;
    }

    print('\r');
    print('\n');

    return bytes_written;
}