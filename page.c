#include <hash.h>
#include <stdio.h>
#include <string.h>
#include "threads/thread.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "lib/kernel/hash.h"
#include "vm/page.h"
						     

static unsigned hash_get (const struct hash_elem *e){
  struct vm_entry *p = hash_entry(e, struct vm_entry, elem);

  return hash_int(p->vaddr);
}

static bool cmp_addr (const struct hash_elem *e1, const struct hash_elem *e2){
  struct vm_entry *t1 = hash_entry(e1, struct vm_entry, elem);
  struct vm_entry *t2 = hash_entry(e2, struct vm_entry, elem);

  if (t2->vaddr > t1->vaddr) return true;
  else return false;
}

void vm_init (struct hash *vm){
  hash_init(vm, hash_get, cmp_addr, NULL);
}

struct vm_entry *find_vme (void *vaddr){
  struct vm_entry vme;
  struct hash_elem *e;

  void *pg_num = pg_round_down (vaddr);
  vme.vaddr = pg_num;
  e = hash_find(&(thread_current()->vm), &vme.elem);
  
  if (e) return hash_entry(e, struct vm_entry, elem);
  else return;
}


bool insert_vme (struct hash *vm, struct vm_entry *vme){
  struct hash_elem *e = hash_insert(vm, &vme->elem);
  bool result = false;
  
  if (!e) result = true;
  return result;
}


bool delete_vme (struct hash *vm, struct vm_entry *vme){
  struct hash_elem *e = hash_delete(vm, &vme->elem);
  bool result = false;

  if (e) result = true;
  free(vme);
  return result;
}

void vm_destroy (struct hash *vm){
  hash_destroy(vm, vm_free_func);
}

static void vm_free_func(struct hash_elem *e, void *aux UNUSED){
  struct vm_entry *e1 = hash_entry(e, struct  vm_entry, elem);
    
  if (!e1) return;
  
  free(e1);
}

bool file_load (void *p, struct vm_entry *vme){
  unsigned int t = file_read_at(vme->file, p, vme->read_bytes, vme->offset);

  if(t == vme->read_bytes){
    memset(p + vme->read_bytes, 0, vme->zero_bytes);
    return true;
  }
  else return false;
}
