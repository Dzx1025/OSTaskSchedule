#include "sched.h"

struct task_struct *current;    //存储当前任务

void print_info(struct task_struct *ts) {
    printf("%d\t\t%lu\t\t%d\t\t%d\t\t%d\n", ts->pid, ts->create_time, 0, 0, 0);
}

inline void print_queue() {
    struct list_head *head_p = &t0.tasks;
    struct task_struct *tp;
    list_for_each_entry(tp, head_p, tasks) {
        print_info(tp);
    }
}

void switch_to(struct task_struct *next) {
    //将任务移到头部
    next->state = TASK_RUNNING;
    list_move(&next->tasks, head);  //移至队头
//调用不同版本的execute函数
#if defined (_RR_) || defined (_PRIOR_)
    execute_with_int(next);
#else
    execute(next);
#endif
}

int schedule() {
    current = CURRENT_TASK();
    if (list_empty(head))   //任务队列空
        return -1;
    struct task_struct *next = current;

    struct list_head *head_p = head;
    struct task_struct *tp;

#ifdef _FCFS_   // 先来先到服务算法
    list_for_each_entry(tp, head_p, tasks) {
        if (tp->state != TASK_RUNNING && EFFECTIVE_TASK(tp)) {
            next = tp;
            break;
        }
    }
#elif defined(_SJF_)    // 短作业优先调度算法
    unsigned long nextCreateTime = current->create_time, nextRunTime = current->run_time;
    list_for_each_entry(tp, head_p, tasks) {
        if (tp->state != TASK_RUNNING && EFFECTIVE_TASK(tp)) {
            if (tp->create_time < nextCreateTime) {
                next = tp;
                nextRunTime = tp->run_time;
                nextCreateTime = tp->create_time;
            } else if (tp->create_time == nextCreateTime && tp->run_time < nextRunTime) {
                next = tp;
                nextRunTime = tp->run_time;
            }
        }
    }
#elif defined(_RR_) // 时间片轮转调度算法
    list_for_each_entry(tp, head_p, tasks) {
        if (tp->state != TASK_RUNNING && EFFECTIVE_TASK(tp)) {
            next = tp;
            break;
        }
    }
#elif defined(_PRIOR_)  // 优先级调度算法
    unsigned long nextCreateTime = current->create_time, nextPrior = current->run_time;
    list_for_each_entry(tp, head_p, tasks) {
        if (tp->state != TASK_RUNNING && EFFECTIVE_TASK(tp)) {
            if (tp->create_time < nextCreateTime) {
                next = tp;
                nextCreateTime = tp->create_time;
                nextPrior = tp->priority;
            } else if (tp->create_time == nextCreateTime && tp->priority < nextPrior) {
                next = tp;
                nextPrior = tp->priority;
            }
        }
    }
#else
    printf("NOT DEFINED SCHEDULE!!!\n");
#endif

    switch_to(next);    //上下文切换
    return 0;
}

