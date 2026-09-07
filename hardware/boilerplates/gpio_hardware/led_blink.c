#include <stdio.h>
#include <stdlib.h>
#include <lgpio.h>
#include <unistd.h>
#include <signal.h>

#define LED_PIN 21

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

    // lgGpioClaimOutput(handle, flags, pin, initial_level)
    // initial_level = 0 means led will be off on run
    if (lgGpioClaimOutput(handle, 0, LED_PIN, 0) < 0) {
        fprintf(stderr, "Error: Can't claim pin %d for LED.\n", LED_PIN);
        lgGpiochipClose(handle);
        return 1;
    }

    printf("Starting blink program. Press Ctrl+C to shut it off...\n");

    int led_state = 0;
    while (keep_running) {
        led_state = !led_state; // Switch state
        
        // lgGpioWrite writes value (1 = 3.3V, 0 = 0V) on pin
        lgGpioWrite(handle, LED_PIN, led_state);
        
        usleep(500000); 
    }

    printf("\nExiting...\n");
    // shutting off LED before ending, or it will be left on
    lgGpioWrite(handle, LED_PIN, 0); 
    
    lgGpiochipClose(handle);
    return 0;
}