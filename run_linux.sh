#!/bin/bash
mkdir build 2>/dev/null
gcc example_linux.c athan_calculator.c -lm -o build/example

if [ $? -eq 0 ]; then      # Checks if the build is successful
	chmod +x build/example # Sets the output file as executable
	build/example          # Runs the output file
fi
