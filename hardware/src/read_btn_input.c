#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    int fd = open("/tmp/btn_input_fifo",O_RDONLY);

    if (fd == -1) {
        perror("Error opening FIFO");
        return 1;
    }

    char ch;
    while( read(fd,&ch,1)!=0){
        if (ch != '\n') {
            printf("Button pressed: >> %c\n",ch);
            fflush(stdout);
        }
    }
    close(fd);
}