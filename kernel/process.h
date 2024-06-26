#ifndef _PROCESS_H
#define _PROCESS_H

#include "def.h"
#include "list.h"
#include "context.h"

#define NR_OPEN 16

enum ProcState {
    Ready,
    Running,
    Exited,
};

/**
 * 进程控制块
 */
struct ProcessControlBlock {
    int pid;
    enum ProcState state;
    // 进程与 Trap 上下文必须紧邻放置，在进程切换时
    // 会使用到它们之间的关系
    struct ProcessContext process_cx;
    struct TrapContext trap_cx;
    usize kstack;
    // ustack 保存用户栈在内核里的虚拟地址
    // 在用户态时会被映射到固定的虚拟地址
    usize ustack;
    struct MemoryMap *mm;
    struct ProcessControlBlock *parent;
    // 进程链表（调度队列）
    struct list_head list;
    // 儿子节点链表
    struct list_head children;
    // 兄弟节点链表
    struct list_head sibling;
    struct File *files[NR_OPEN];
};

#endif