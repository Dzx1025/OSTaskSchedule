#ifndef OS_TASK_SCHEDULE_LIST_H
#define OS_TASK_SCHEDULE_LIST_H

#include "stddef.h"

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

static inline void my_list_add(struct list_head *new,
                               struct list_head *prev,
                               struct list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    my_list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    my_list_add(new, head->prev, head);
}

static inline void my_list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head *entry) {
    my_list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

static inline void list_move(struct list_head *list, struct list_head *head) {
    my_list_del(list->prev, list->next);
    list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head) {
    my_list_del(list->prev, list->next);
    list_add_tail(list, head);
}

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos->next, pos != (head); \
            pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define container_of(ptr, type, member) ( { \
    const typeof( ((type*)0)->member ) *__mptr=(ptr); \
    (type*)( (char*)__mptr - offsetof(type,member) );} )

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define list_for_each_entry(pos, head, member)                \
    for (pos = list_entry((head)->next, typeof(*pos), member);    \
         pos->member.next, &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_reverse(pos, head, member)            \
    for (pos = list_entry((head)->prev, typeof(*pos), member);    \
         prefetch(pos->member.prev), &pos->member != (head);    \
         pos = list_entry(pos->member.prev, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)            \
    for (pos = list_entry((head)->next, typeof(*pos), member),    \
        n = list_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))

#endif //OS_TASK_SCHEDULE_LIST_H
