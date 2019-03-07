#include <iostream>         // Input output stream
#include <sched.h>          // Contain clone syscall for cloning a process
#include <signal.h>         // Contain ANSI C signal handeling like SIGCHILD   

char* stack_memory() {
  const int stackSize = 65536;
  auto *stack = new (std::nothrow) char[stackSize];

  if (stack == nullptr) {                       // Can't allocate memory from heap hence `new` will return null pointer
    printf("Memory allocation failed !! \n");
    exit(1);
  }  

  return stack+stackSize;  // As stack grows backward so we have to start from end of stack   {(end of stack)| *stack|-------> stackSize(start of stack)}
}

int jail(void *args) {                              // Function JAIL that is container 
  printf("Namastey from JAIL \n");
  return 0;
}

int main(int argc, char** argv) {                                           
  printf("Namastey from main(), Waiting for JAIL.....\n");
  clone(jail, stack_memory(), SIGCHLD, 0);                          // Clone syscall to clone precoess (`$ man fork` for more details)

  return 0;
}