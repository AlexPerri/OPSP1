#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
struct lock sysLock;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  
  thread_exit ();
}
void exit (int status)
{
  thread_current()->exitNum = status;
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit ();
}
int write (int fd, const void *buffer, unsigned size)
{/*
  struct thread *cur = thread_current();
  if (fd == STDOUT_FILENO)
  { 
    //writes byte size
      putbuf(buffer, size);
      return size;
  }
  if (fd == 0 || list_empty(&cur->fileL))
  {
    return 0;
  }
  for (struct list_elem *scan = list_front(&cur->fileL);scan != NULL; scan = scan->next)
  {
      struct theFile *f = list_entry (scan, struct theFile, felem);
      if (f->num == fd)
      {
        int bytes = (int) file_write(f->address, buffer, size);
        return bytes;
      }
  }*/ 
}
