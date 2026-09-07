#!/bin/bash

echo "Compling FIFO programs..."

gcc read_fifo.c -o read_fifo

if [ $? -eq 0 ]; then
    echo "read_fifo compiled"
else
    echo "read_fifo was NOT compiled"
fi

gcc write_fifo.c -o write_fifo

if [ $? -eq 0 ]; then
    echo "read_fifo compiled"
else
    echo "read_fifo was NOT compiled"
fi

echo "You can test by running ./write_fifo in a terminal and ./read_fifo in another"