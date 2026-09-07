#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <lgpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "console_config.h"

void handle_error(int handle, const char *msg, int exit_code){
    fprintf(stderr, "Error: %s\n",msg);
    if (handle >= 0) {
        lgGpiochipClose(handle);
    }
    exit(exit_code);
}

static volatile sig_atomic_t keep_running = 1;

void sigint_handler(int signum) {
    keep_running = 0;
}

typedef struct {
    int pin;
    const char *name;
    int last_state;
    const char protocol_codename; //for transmitting to fifo
} ButtonDef;
//error codes definitions:

// 1 - error at accessing board chip
// 2 - error at accessing button pin
// 3 - error at reading/writing fifo
int main(void){

    signal(SIGINT, sigint_handler);
    
    int handle;

    handle = lgGpiochipOpen(0);

    if(handle<0){
        handle_error(-1, "Couldn't access board's chip",1);
    }

    //toate butoanele sunt neapasate la inceput
    ButtonDef buttons [] = {
        {PIN_BTN_UP, "UP",1,'U'},
        {PIN_BTN_DOWN, "DOWN",1,'D'},
        {PIN_BTN_LEFT, "LEFT",1,'L'},
        {PIN_BTN_RIGHT, "RIGHT",1,'R'},
        {PIN_BTN_A, "A",1,'A'},
        {PIN_BTN_B, "B",1,'B'},
        {PIN_BTN_START, "START",1,'S'},
        {PIN_BTN_SELECT, "SELECT",1,'E'} //sElect, ca lowercase sa fie cand isi ia release
        // {PIN_BTN_POWER, "POWER",1}  da eroare daca nu e comentat, prin dtoverlay=gpio-shutdown kernel-ul placutei il ia si se ocupa de el
    };

    int num_buttons = sizeof(buttons)/sizeof(buttons[0]);

    for(int i=0;i<num_buttons;i++){
        if(lgGpioClaimInput(handle,LG_SET_PULL_UP,buttons[i].pin)<0){
            char error_msg[128]; //construim dinamic mesajul de eroare
            snprintf(error_msg, sizeof(error_msg), "Couldn't access PIN %d associated to %s button", buttons[i].pin, buttons[i].name);
            handle_error(handle, error_msg, 2);
        }
    }
    int fifo_fd;
    if(-1  ==  mkfifo("/tmp/btn_input_fifo",0600)){
        if(errno==EEXIST){
            //handle_error(handle,"FIFO for button input already exists",3);
        } else {
            handle_error(handle,"couldn't creating FIFO for button input",3);
        }
    }

    if ( (fifo_fd=open("/tmp/btn_input_fifo",O_WRONLY))==-1){
        handle_error(handle,"couldn't open FIFO for button input",3);
    }
    while(keep_running){
        for(int i=0;i<num_buttons;i++){
            int current_state = lgGpioRead(handle,buttons[i].pin);


            if(current_state==0 && buttons[i].last_state==1){
                // printf("HARDWARE DETECT >> Button PRESSED: %s\n",buttons[i].name);
                // fflush(stdout);

                if(write(fifo_fd,&buttons[i].protocol_codename,1)<0){
                    handle_error(-1,"couldn't send button press to FIFO",3);
                }
            } else if(current_state == 1 && buttons[i].last_state == 0){
                //printf(">> Button RELEASED: %s\n", buttons[i].name);
                const char lowercase = buttons[i].protocol_codename-'A'+'a';
                if(write(fifo_fd,&lowercase,1)<0){
                    handle_error(-1,"couldn't send button release to FIFO",3);
                }
            }

            buttons[i].last_state = current_state;
        }

        usleep(20000);
    }

    printf("\nExiting and cleaning up GPIO...\n");
    lgGpiochipClose(handle);
    close(fifo_fd);
    unlink("/tmp/btn_input_fifo");
    return 0;
}
