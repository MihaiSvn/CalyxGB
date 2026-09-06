#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <lgpio.h>
#include <unistd.h>
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
} ButtonDef;
//error codes definitions:

// 1 - error at accessing board chip
// 2 - error at accessing button pin
int main(void){

    signal(SIGINT, sigint_handler);
    
    int handle;

    handle = lgGpiochipOpen(0);

    if(handle<0){
        handle_error(-1, "Couldn't access board's chip",1);
    }

    ButtonDef buttons [] = {
        {PIN_BTN_UP, "UP"},
        {PIN_BTN_DOWN, "DOWN"},
        {PIN_BTN_LEFT, "LEFT"},
        {PIN_BTN_RIGHT, "RIGHT"},
        {PIN_BTN_A, "A"},
        {PIN_BTN_B, "B"},
        {PIN_BTN_START, "START"},
        {PIN_BTN_SELECT, "SELECT"},
        {PIN_BTN_POWER, "POWER"}
    };

    int num_buttons = sizeof(buttons)/sizeof(buttons[0]);

    for(int i=0;i<num_buttons;i++){
        if(lgGpioClaimInput(handle,LG_SET_PULL_UP,buttons[i].pin)<0){
            char error_msg[128]; //construim dinamic mesajul de eroare
            snprintf(error_msg, sizeof(error_msg), "Couldn't access PIN %d associated to %s button", buttons[i].pin, buttons[i].name);
            handle_error(handle, error_msg, 2);
        }
    }

    while(keep_running){
        for(int i=0;i<num_buttons;i++){
            if(lgGpioRead(handle,buttons[i].pin) == 0){
                printf(">> Button pressed: %s\n",buttons[i].name);
                break;
            }
        }

        usleep(100000);
    }
    printf("\nExiting and cleaning up GPIO...\n");
    lgGpiochipClose(handle);
    return 0;
}
