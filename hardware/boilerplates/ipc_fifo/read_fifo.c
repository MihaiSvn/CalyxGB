#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "fifo";
    int fd = open(fifo_path, O_RDONLY);

    if (fd == -1) {
        perror("Error opening FIFO for reading");
        return 1;
    }

    char ch;
    while (read(fd, &ch, 1) > 0) {
        printf("%c", ch);
        fflush(stdout);
    }
    
    close(fd);
    return 0;
}