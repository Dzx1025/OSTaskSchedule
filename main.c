#include <unistd.h>
#include <malloc.h>
#include "list.h"
#include "sched.h"

#define MS(time) (time*100*1000)

struct task_struct t0;  //init_process
static int uni_pid = 0;
struct timeval original_time;
static int INT_FLAG = 0;

void print_time();

static void alloc_pid(int *);

void init_process();

void create_process(unsigned long, unsigned long, int, long, long);

int do_exit(struct task_struct *ts); //任务结束

void task_with_int(struct task_struct *ts);    //中断

int main(int argc, char *argv[]) {
    setbuf(stdout, 0);
    //初始化
    init_process();
    //创建进程
    create_process(0, 4, TASK_INTERRUPTED, 5, 1);
    create_process(2, 3, TASK_INTERRUPTED, 4, 1);
    create_process(3, 5, TASK_INTERRUPTED, 3, 1);
    create_process(3, 2, TASK_INTERRUPTED, 2, 1);
    create_process(2, 2, TASK_INTERRUPTED, 2, 1);

    print_queue();
    //运行调度
    printf("--------------------------------------------------------\n");
    printf("Start Schedule:\n");

    gettimeofday(&original_time, NULL);

    while (1) {
        int res = schedule();
        if (res != 0) {
            printf("Task queue empty.\n");
            exit(0);
        }
        if (INT_FLAG == 1) {    //处理中断
            INT_FLAG = 0;
            schedule(); //再次调度
        }
    }
    exit(0);
}

long get_now_time() {
    long time;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time = ((tv.tv_sec - original_time.tv_sec) * 1000 +
            (tv.tv_usec - original_time.tv_usec) / 1000);   // 毫秒
    return time;
}

void print_time() {
    printf("time: %ld\t\t", get_now_time());    // 毫秒
}

static void alloc_pid(int *pid) {
    *pid = uni_pid++;
}

void init_process() {
    INIT_LIST_HEAD(&t0.tasks);
    head = &t0.tasks;
    alloc_pid(&t0.pid);
    printf("Creating requests...\n");
    printf("Pid\tCreateTime\tRunTime\tState\tPriority\n");
}

void create_process(unsigned long createTime,  //时间均以100毫秒为单位
                 unsigned long runTime,
                    int state, //1:Stop  0:Run
                 long prior,
                    long slice) {
    struct task_struct *ts = malloc(sizeof(struct task_struct));
    ts->state = state;

#ifdef _PRIOR_
    slice = 4;  //优先级调度每400ms进行调整
#endif
    ts->time_slice = slice;

    alloc_pid(&ts->pid);
    ts->priority = prior;
    ts->run_time = runTime;
    ts->left_time = runTime;
    ts->create_time = createTime;

//    print_info(ts);
    //加入任务队列
    list_add_tail(&ts->tasks, &t0.tasks);
}

int do_exit(struct task_struct *ts) {
    list_del(&ts->tasks);
    ts->state = TASK_KILLED;
    int deadPid = ts->pid;
    long lastState = ts->state;
    free(ts);
    print_time();
    printf("task %d stopped\n", deadPid);
    return lastState != TASK_KILLED;
}

void task_with_int(struct task_struct *ts) {
    print_time();
    printf("task %d interrupted\n", ts->pid);
    ts->state = TASK_INTERRUPTED;
#ifdef _RR_
    list_move_tail(&ts->tasks, head);   //移至队尾
#endif
    INT_FLAG = 1;   //中断标志置位
}

void execute_with_int(struct task_struct *ts) {
    print_time();
    printf("task %d is running\n", ts->pid);

    unsigned long leftTime = ts->left_time;
    if (leftTime <= ts->time_slice) {   //任务在中断前就结束运行
        usleep(MS(leftTime));
        ts->left_time = 0;
        if (do_exit(ts)) {
            printf("Exit Error!!!\n");
            exit(0);
        }
    } else {    //进入中断
        usleep(MS(ts->time_slice)); //执行一个时间片后进入中断
#ifdef _PRIOR_
        ts->priority <<= 1; //aged
#endif
        ts->left_time = leftTime - ts->time_slice;  //减去运行时间
        task_with_int(ts);
    }
}

void execute(struct task_struct *ts) {
    print_time();
    printf("task %d is running\n", ts->pid);
    usleep(MS(ts->run_time));   //do something
    if (do_exit(ts)) {
        printf("Exit Error!!!\n");
        exit(0);
    }
}