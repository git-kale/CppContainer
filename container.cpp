#include <iostream> // Input output stream
#include <sched.h>  // Contain clone syscall for cloning a process
#include <unistd.h> // Contain execvp function
#include <wait.h>   // Contain ANSI C signal handeling like SIGCHILD
#include <sys/mount.h>

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
template <typename Function>
void clone_process(Function &&function, int flags)
{
    auto pid = clone(function, stack_memory(), flags, 0);

    wait(nullptr);
}

void setup_vars();
void setup_root(const char *root);

int jail(void *args)
{

    printf("Namastey from JAIL \n");
    printf("jail pid : %d\n", getpid());
    sethostname("Devil's Nest", 13);
    setup_vars();

    setup_root("./root");

    mount("proc", "/proc", "proc", 0, 0);

    auto runThis = [](void *args) -> int { run("/bin/sh"); };

    clone_process(runThis, SIGCHLD);

    umount("/proc");
    return 0;
}

void setup_root(const char *folder)
{
    chroot(folder);
    chdir("/");
}

void setup_vars()
{
    clearenv();
    setenv("TERM", "xterm-256color", 0);
    setenv("PATH", "/bin/:/sbin/:usr/bin:/usr/sbin", 0);
}

int main(int argc, char **argv)
{

    printf("Namastey from main(), Waiting for JAIL.....\n");
    printf("parent pid : %d\n", getpid());

    clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);
    printf("Exitting from container.... \n");

    return 0;
}
