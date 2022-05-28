#include <hash.h>
#include <stdio.h>
#include <string.h>
#include "threads/thread.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "lib/kernel/hash.h"


struct vm_entry {

 void *vaddr;
 bool wr_state;
 bool load_state;

 size_t read_bytes;
 size_t zero_bytes;
 size_t offset;

 struct hash_elem elem;
 struct file *file;

};


static unsigned hash_get (const struct hash_elem *e);

static bool cmp_addr (const struct hash_elem *e1, const struct hash_elem *e2);

void vm_init (struct hash *vm);

bool insert_vme (struct hash *vm, struct vm_entry *vme);

struct vm_entry *find_vme (void *vaddr);

bool delete_vme (struct hash *vm, struct vm_entry *vme);

void vm_destroy (struct hash *vm);

static void vm_free_func(struct hash_elem *e, void *aux UNUSED);

bool file_load (void *p, struct vm_entry *vme);


