#include <stdio.h>
#include "sched.h"

void schedule() {
#ifdef _FCFS_   // 先来先服务算法
    printf("FCFS\n");
#endif
#ifdef _SJF_    // 短作业优先调度算法
    printf("SJF\n");
#endif
#ifdef _RR_ // 时间片轮转调度算法
    printf("RR\n");
#endif
#ifdef _PRIOR_  // 优先级调度算法
    printf("_PRIOR_\n");
#endif
//将任务由头部移到尾部
//    list_move(&tt->tasks, &t0.tasks);
//将任务移到头部
//    struct task_struct *p;
//    struct list_head *head_p = &t0.tasks, *pos, *tmp;
//    list_for_each_safe(pos, tmp, &t0.tasks) {
//        p = list_entry(pos, struct task_struct, tasks);
//        if (p->pid == 3) {
//            list_move(&p->tasks, &t0.tasks);
//        }
//    }
}

void print_info(struct task_struct *ts) {
    printf("%d\t\t%lu\t\t%d\t\t%d\t\t%d\n", ts->pid, ts->start_time, 0, 0, 0);
}

