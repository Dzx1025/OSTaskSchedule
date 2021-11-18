#ifndef OS_TASK_SCHEDULE_SCHED_H
#define OS_TASK_SCHEDULE_SCHED_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include "list.h"

extern struct task_struct t0;

extern void execute_int(struct task_struct *ts); //可中断的运行

extern void execute(struct task_struct *ts);//任务执行

extern struct timeval original_time;

extern long now_time();

#define TASK_RUNNING 0
#define TASK_INTERRUPTED 1
#define TASK_KILLED (-1)

//Debug
#define _SJF_
//----------------------

struct list_head *head;

#define CURRENT_TASK()  (list_entry((head)->next, struct task_struct, tasks))
#define EFFECTIVE_TASK(ts) ((ts)->create_time <= (now_time() / 100))

struct task_struct {
    volatile long state;        //说明了该进程是否可以执行,还是可中断等信息
    unsigned int time_slice;    //进程的时间片的大小
    struct list_head tasks;     //用于将系统中所有的进程连成一个双向循环链表, 其根是init_task
    pid_t pid;                  //进程标识符,用来代表一个进程
    long priority;              //优先级
    unsigned long run_time;     //运行消耗时间
    unsigned long left_time;    //剩余运行时间
    unsigned long create_time;  //进程创建的时间
};

void print_info(struct task_struct *);

void print_queue();

void switch_to(struct task_struct *);

int schedule();

#endif //OS_TASK_SCHEDULE_SCHED_H
