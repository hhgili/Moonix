#ifndef _DEF_H
#define _DEF_H

#include "types.h"

struct Buddy;
struct Segment;
struct MemoryMap;
struct TrapContext;
struct ProcessContext;
struct ProcessControlBlock;
struct Inode;
struct File;
enum SegmentType;

/* buddy_system_allocator.c */
void init_buddy(struct Buddy *);
void add_to_buddy(struct Buddy *, void *, void *);
void *buddy_alloc(struct Buddy *, uint64);
void buddy_dealloc(struct Buddy *, void *, uint64);

/* elf.c */
struct MemoryMap *from_elf(char *);

/* fs.c */
void init_fs();
struct Inode *lookup(char *);
int readall(struct Inode *, char *);
void dealloc_files(struct File **);
int sys_open(char *, int);
int sys_close(int);
int sys_read(int, char *, int);
int sys_write(int, char *, int);

/* kerneltrap.S */
void __trap_entry();
void __restore(struct TrapContext *current_trap_cx);

/* memory.c */
void init_memory();
void *alloc(usize);
void dealloc(void *, usize);
usize alloc_frame();
void dealloc_frame(usize);

/* mapping.c */
struct Segment *new_segment(usize, usize, usize, enum SegmentType);
void map_segment(usize, struct Segment *, char *, usize);
void map_pages(usize, usize, usize, int, usize);
struct MemoryMap *new_kernel_memory_map();
struct MemoryMap *remap_kernel();
void dealloc_memory_map(struct MemoryMap *);
struct MemoryMap *copy_mm(struct MemoryMap *);
void activate_pagetable(usize);

/* printf.c */
void printf(char *, ...);
void panic(char *, ...) __attribute__((noreturn));

/* sbi.c */
void console_putchar(usize);
usize console_getchar();
void shutdown() __attribute__((noreturn));
void set_timer(usize);

/* syscall.c */
usize syscall(usize, usize[3]);

/* switch.S */
void __switch(struct ProcessContext *current_process_cx,
              struct ProcessContext *next_process_cx);

/* trap.c */
void init_trap();

/* process.c */
void add_process(struct ProcessControlBlock *);
void init_process();
void exit_current();
int sys_fork();
int sys_wait();
int sys_exec(char *);
void yield();

/* timer.c */
void init_timer();
void set_next_timeout();

#endif