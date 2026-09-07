#!/bin/bash

gcc -Wall led_blink.c -o led_blink -llgpio
if [ $? -eq 0 ]; then
    echo "led_blink compiled"
else
    echo "led_blink was NOT compiled"
fi

gcc -Wall button_poll.c -o button_poll -llgpio

if [ $? -eq 0 ]; then
    echo "button_poll compiled"
else
    echo "button_poll was NOT compiled"
fi

echo "You can test by running ./led_blink in a terminal or ./button_poll, given you correctly did their circuits."
