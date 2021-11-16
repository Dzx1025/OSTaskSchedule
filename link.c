#include <linux/module.h>
#include <stdio.h>
#include<linux/sched.h>
#include<linux/sem.h>
#include <stdlib.h>
#include "list.h"

#define N 10

typedef struct info {
    char id;
    char character;
    char parent_id;
    struct list_head member;
} info_s;

int List_Demo_init(void) {
    int i = 0, ret = 0;
    struct info info_head = {
            .id = 0,
            .character = 'a',
            .parent_id = 1,
            .member = LIST_HEAD_INIT(info_head.member),
    };
    //插入N个元素
    for (i = 0; i < N; i++) {
        struct info *info_tmp;
        info_tmp = malloc(sizeof(struct info));
        if (info_tmp == NULL)
            return -1;
        info_tmp->id = i;
        info_tmp->character = (char) (64 + i);
        info_tmp->parent_id = i + 1;
        list_add(&info_tmp->member, &info_head.member);
    }
    //遍历N个元素
    struct info *info_p, *p;
    struct list_head *pos;
    int count = 0;
    info_p = &info_head;
    list_for_each(pos, &info_p->member) {
        p = list_entry(pos, struct info, member);
        count++;
        printf("ID - %d -- Name = %c\nparent_id - %d.\n", p->id, p->character, p->parent_id);
    }

    //删除元素
    list_del(&p->member);
    count = 0;

    //遍历N-1剩余元素
    list_for_each(pos, &info_p->member) {
        p = list_entry(pos,
                       struct info, member);
        count++;
        printf("ID - %d -- Name = %c\nparent_id - %d.\n", p->id, p->character, p->parent_id);
    }
    return 0;
}

static void List_Demo_Exit(void) {
    printf("Exit\n");
}

module_init(List_Demo_init);
module_exit(List_Demo_Exit);
MODULE_AUTHOR("Keven");
MODULE_DESCRIPTION("List Head Description");
MODULE_LICENSE("GPL");