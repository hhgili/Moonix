#ifndef _LIST_H
#define _LIST_H

/* 更改自 linux2.6 */

/**
 * 结构体成员变量在该结构体的偏移
 *
 * @param type 结构体类型
 * @param member 该成员在结构体内的变量名
 */
#define offsetof(type, member) ((size_t) & ((type *)0)->member)

/**
 * 将结构体成员指针转换为该结构体的指针
 *
 * @param ptr 结构体成员的指针
 * @param type 结构体的类型
 * @param member 该成员在结构体内的变量名
 */
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) ); })

/**
 * 单向链表
 */
struct list_head
{
    struct list_head *next;
};

static inline void list_add(struct list_head *new, struct list_head *head)
{
    new->next = head->next;
    head->next = new;
}

/**
 * 获取包含链表 list_head 的结构体
 *
 * @param ptr 指向 list_head 的指针
 * @param type 结构体的类型
 * @param member 链表在结构体内的变量名
 * @return 指向结构体的指针
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * 遍历结构体链表
 *
 * @param pos 循环变量
 * @param head 链表头指针
 * @param member 链表在结构体内的变量名
 */
#define list_for_each_entry(pos, head, member)                 \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);                               \
         pos = list_entry(pos->member.next, typeof(*pos), member))

#endif