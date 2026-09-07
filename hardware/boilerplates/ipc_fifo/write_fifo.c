#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "fifo";
    
    mkfifo(fifo_path, 0600);

    int fd = open(fifo_path, O_RDWR);
    if (fd == -1) {
        perror("Error opening FIFO for writing");
        return 1;
    }

    char ch;
    while ((ch = getchar()) != EOF) {
        write(fd, &ch, 1);
    }

    close(fd);
    
    unlink(fifo_path); 
    
    return 0;
}