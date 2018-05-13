#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
struct lock procLock;

struct thread_info
{
  struct list_elem felem;
  struct file *f;
  int *descriptor;
};

void
syscall_init (void) 
{
  lock_init(&procLock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int fd ;
  void *buffer;
  unsigned size;

  switch(*(int *) f->esp) {
      case SYS_HALT :
         printf("Tries to halt\n " );
         break;
      case SYS_EXIT :
         fd = *(int *)(f->esp + 4);
         exit(fd);
         break;
      case SYS_EXEC :
         printf("Tries to exec\n" );
         break;
      case SYS_WAIT :
         printf("Tries to wait\n" );
         break;
       case SYS_CREATE :
         printf("Tries to create\n " );
         break;
       case SYS_REMOVE :
         printf("Tries to remove\n " );
         break;
       case SYS_OPEN :
         printf("Tries to open \n " );
         break;
       case SYS_FILESIZE :
         printf("Tries to filesize\n " );
         break;
       case SYS_READ :
         printf("Tries to read \n " );
         break;

       case SYS_WRITE :
        // printf("Tries to write \n " );
	 fd = *(int *)(f->esp + 4);
	 buffer = *(char **)(f->esp + 8);
	 size = *(unsigned *)(f->esp + 12);
         f->eax = write (fd, buffer,size);

	 break;
       case SYS_SEEK :
         printf("Tries to seek \n " );
         break;
       case SYS_TELL :
         printf("Tries to tell \n " );
         break;
       case SYS_CLOSE :
         printf("Tries to close \n " );
         break;

      default :
         printf("Invalid Input\n" );
   }

 // printf ("%d",*(int *) f->esp);
 // printf ("system call!\n");
 // thread_exit ();
}

int write (int fd, const void *buffer, unsigned size)
{
 // printf("Made it to write function\n");
  struct thread *cur = thread_current ();
  struct list_elem *e;
  lock_acquire(&procLock);

  // Check if stdout(1)
  if (fd == 1)
  {
  // printf("Ran part putbuf, size was:  \n");
   // printf("%d\n",(int)size);
    putbuf((char * )buffer, (size_t) size);
    lock_release(&procLock);
    return (int) size;
  }

  // Check if there is nothing there or for stdin(0)
  if (fd == 0 || list_empty(&cur->file_descriptor))
  {
   // printf("Ran part 2\n");
    lock_release(&procLock);
    return 0;
  }

  // Scan thread's file descriptor and look for fd
  for (e = list_front (&cur->file_descriptor); e != NULL; e->next)
  {
    struct thread_info *temp = list_entry (e, struct thread_info, felem);
    if (temp->descriptor == fd)
    {
      int written_size  = (int) file_write (temp->f, buffer, size);
     // printf("Ran part 3\n");
      lock_release(&procLock);
      return written_size;
    }
  }
  lock_release(&procLock);

  return 0;
}

void exit(int exit){
printf("%s: exit(%d)\n", thread_current()->name, exit);
thread_exit();
}

