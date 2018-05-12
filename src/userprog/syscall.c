#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
struct lock sysLock;

struct thread_info
{
  struct list_elem *elem;
  struct file *f;
  int *descriptor;
};

void syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  is_valid((const void *) f->esp);
  int args[3];
  void *physical_page;

  switch (*(int *) f->esp)
  {
    case SYS_WAIT:
	int fd = *(int *)(f->esp + 4);
	void *buffer = *(char **)(f->esp + 8);
	unsigned size = *(unsigned *)(f->esp + 12);

	buffer_check(buffer, size);

	physical_page = pagedir_get_page(thread_current()->pagedir, const void buffer);
	if (physical_page == NULL)
	{
	  exit(-1);
	}
	buffer =(int) physical_page;
	f->eax = wait (fd, buffer, size);
	break;

  default :
	exit(-1);
  }
  //thread_exit ();
}

void exit (int status)
{
  thread_current()->exitNum = status;
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit ();
}

int write (int fd, const void *buffer, unsigned size)
{
  struct thread *cur = thread_current ();
  struct list_elem *e;
  // Check if stdout(1)
  if (fd == 1)
  {
    putbuf((char * )buffer, (size_t) size);
    return (int) size;
  }
  // Check if there is nothing there or for stdin(0)
  if (fd == 0 || list_empty(&cur->file_descriptor)
  {
    return 0;
  }
  // Scan thread's file descriptor and look for fd
  for (e = list_begin (&cur->file_descriptor); e != NULL; e->next)
  {
    struct thread_info *temp = list_entry (e, struct thread_info, elem);
    if (temp->descriptor == fd)
    {
      int written_size  = (int) file_write (temp->f, buffer, size);
      return written_size;
    }
  }
  return 0;
}

void is_valid (const void *ptr)
{
  if (!is_user_vadder(*ptr) || ptr == NULL || ptr < (void *) 0x08048000)
  {
    exit(-1);
  }
}
void buffer_check (void * buffer, unsigned size)
{
  unsigned i;
  char *b = (char *) buffer;
  for (i = 0; i < size; i++)
  {
    is_valid((const void *) b);
    b++;
  }
}
