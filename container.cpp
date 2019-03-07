#include <iostream> // Input output stream
#include <sched.h>  // Contain clone syscall for cloning a process
#include <unistd.h> // Contain execvp function
#include <wait.h>   // Contain ANSI C signal handeling like SIGCHILD

char *stack_memory()
{
    const int stackSize = 65536;
    auto *stack = new (std::nothrow) char[stackSize];

    if (stack == nullptr)
    { // Can't allocate memory from heap hence `new` will return null pointer
        printf("Memory allocation failed !! \n");
        exit(1);
    }

    return stack + stackSize; // As stack grows backward so we have to start from end of stack   {(end of stack)| *stack|-------> stackSize(start of stack)}
}

template <typename P>                   
int run(P params)
{
    //basically generating the arguments array at compile time.
    char *args[] = {(char *)params, (char *)0};
    return execvp(args[0], args);
}

int jail(void *args)
{ // Function JAIL that is container
    printf("Namastey from JAIL \n");
    run("/bin/sh");
    return 0;
}

int main(int argc, char **argv)
{
    printf("Namastey from main(), Waiting for JAIL.....\n");
    clone(jail, stack_memory(), SIGCHLD, 0);    // Clone syscall to clone precoess (`$ man fork` for more details)
    wait(nullptr);                              // After cloning a process wait until all the child processes created to complete(CHILD process return *nullptr after completion)
    printf("Exitting from container.... \n");
    return 0;
}