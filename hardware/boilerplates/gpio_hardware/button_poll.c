#include <stdio.h>
#include <stdlib.h>
#include <lgpio.h>
#include <unistd.h>
#include <signal.h>

#define BUTTON_PIN 20

static volatile sig_atomic_t keep_running = 1;

void sigint_handler(int signum) {
    keep_running = 0;
}

int main(void) {
    signal(SIGINT, sigint_handler);
    
    int handle = lgGpiochipOpen(0);
    if (handle < 0) {
        fprintf(stderr, "Error: Can't access GPIO chip.\n");
        return 1;
    }

    if (lgGpioClaimInput(handle, LG_SET_PULL_UP, BUTTON_PIN) < 0) {
        fprintf(stderr, "Error: Can't claim pin %d for button.\n", BUTTON_PIN);
        lgGpiochipClose(handle);
        return 1;
    }

    printf("Waiting for touches. Press Ctrl+C for exit...\n");

    int last_state = 1; // 1 means unpressed (pull up)
    
    while (keep_running) {
        int current_state = lgGpioRead(handle, BUTTON_PIN);

        // goes from unpressed(1) to pressed(0)
        if (current_state == 0 && last_state == 1) {
            printf("Button pressed!\n");
        } 
        // goes from pressed(0) to unpressed(1)
        else if (current_state == 1 && last_state == 0) {
            printf("Buton released!\n");
        }

        last_state = current_state;
        
        // avoid double press
        usleep(20000); 
    }

    printf("\nExiting...\n");
    lgGpiochipClose(handle);
    return 0;
}