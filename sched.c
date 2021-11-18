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

unsigned long get_now_time() {
    long time;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time = ((tv.tv_sec - original_time.tv_sec) * 1000 +
            (tv.tv_usec - original_time.tv_usec) / 1000);   // 毫秒
    return time;
}

void switch_to(struct task_struct *next) {
    //将任务移到头部
    next->state = TASK_RUNNING;
    list_move(&next->tasks, head);  //移至队头
//调用不同版本的execute函数
#if defined (_RR_) || defined (_HRRN_)
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
#elif defined(_HRRN_)  // 响应比高者优先调度算法
    unsigned long nextNice = current->nice;
    list_for_each_entry(tp, head_p, tasks) {
        if (tp->state != TASK_RUNNING && EFFECTIVE_TASK(tp)) {
            tp->nice = 1 + (long) ((get_now_time() - tp->last_int_time) / tp->run_time);     //nice=1+作业等待时间/作业运行时间
            if (tp->nice > nextNice) {
                next = tp;
                nextNice = tp->nice;
            }
        }
    }
#else
    printf("NOT DEFINED SCHEDULE!!!\n");
#endif

    switch_to(next);    //上下文切换
    return 0;
}

