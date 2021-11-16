#ifndef OS_TASK_SCHEDULE_SCHED_H
#define OS_TASK_SCHEDULE_SCHED_H

#include <sys/types.h>
#include <sys/time.h>
#include "list.h"

#define TASK_RUNNING 0
#define TASK_STOPPING 1
#define TASK_KILLED -1

#define CURRENT_TASK()

struct task_struct {
    volatile long state;  //说明了该进程是否可以执行,还是可中断等信息
    unsigned long flags;  //Flag是进程号,在调用fork()时给出
    long nice;       //进程的基本时间片
    unsigned long sleep_time;  //进程的睡眠时间
    struct list_head tasks; //用于将系统中所有的进程连成一个双向循环链表, 其根是init_task
    pid_t pid;    //进程标识符,用来代表一个进程
    long priority; //优先级

//it_virt_value，it_virt_value用于Virtual定时器，单位为jiffies。当进程运行时，不管在何种
//状态下，每个tick都使it_virt_value值减一当减到0时，向进程发送信号SIGVTALRM，根据
//it_virt_incr重置初值。
    unsigned long virt_value;
    unsigned long virt_incr;
//    struct tms times;      //记录进程消耗的时间
    unsigned long start_time;  //进程创建的时间
};

void schedule();

void print_info(struct task_struct *);

#endif //OS_TASK_SCHEDULE_SCHED_H
