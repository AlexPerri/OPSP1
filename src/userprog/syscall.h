#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;

void syscall_init (void);
pid_t exec (const char *file);
int write (int, const void*, unsigned);
void exit_sys(int status);
#endif /* userprog/syscall.h */
