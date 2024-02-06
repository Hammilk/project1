Author: David Pham

COMPILATION STEPS:

To compile and run the project, ensure that directory has all required files:
1. oss.c (Parent file)
2. user.c (Child Executable file)
3. header.h (Header file)
4. Makefile (Makefile to make project)

To make the project, run the command "make" in command line.
The project takes the following system inputs:
-h Help command
-n Sets total children to run
-s Sets how many concurrent processes can run at a time
-t Sets how many iterations each child executable program runs

If inputs are not used, defaults of 1 are used for each value.

Example command:

$ ./oss -n 5 -s 3 -t 2

This will run 5 child processes with a maximum of 3 concurrent processes with 2 iterations running per child process.

Use the command "make clean" to remove .o files and executables.

Outstanding issues:

None known



