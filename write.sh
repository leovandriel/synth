#!/bin/bash

set -e

gcc src/write.c -o bin/run -Wall -Wextra -O3
./bin/run > output/run.wav