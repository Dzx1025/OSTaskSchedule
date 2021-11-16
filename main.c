#include <signal.h>
#include <malloc.h>
#include "list.h"
#include "sched.h"

struct task_struct t0;  //1号任务
static int uni_pid = 0;

static int get_pid() {
    return uni_pid++;
}

void init_task() {
    INIT_LIST_HEAD(&t0.tasks);
    t0.pid = get_pid();
    printf("Creating threads...\n");
    printf("pid\tcreateTime\trunTime\tstate\tpriority\n");
}

void create_task(unsigned long createTime,
                 unsigned long runTime,
                 int state,   //1:停止  0:运行
                 long prior) {
    struct task_struct *ts = malloc(sizeof(struct task_struct));
    ts->state = state;
    ts->nice;
    ts->pid = get_pid();
    ts->priority = prior;
    ts->virt_value;
    ts->virt_incr;
//    struct tms times;
    ts->start_time = createTime;

//    print_info(ts);
    //加入任务队列
    list_add_tail(&ts->tasks, &t0.tasks);
}

void print_queue() {
    struct list_head *head_p = &t0.tasks;
    struct task_struct *tp;
    list_for_each_entry(tp, head_p, tasks) {
        print_info(tp);
    }
}
//void print() { printf("test\n"); }
//
//void init_sigaction() {
//    struct sigaction act;
//    act.sa_handler = print;
//    act.sa_flags = 0;
//    sigemptyset(&act.sa_mask);
//    sigaction(SIGPROF, &act, NULL); //设置信号 SIGPROF 的处理函数为 print_info
//}
//
//void init_time() {
//    struct itimerval value;
//    value.it_value.tv_sec = 2; //每隔1秒
//    value.it_value.tv_usec = 0;
//    value.it_interval = value.it_value;
//    setitimer(ITIMER_PROF, &value, NULL); //初始化 timer，到期发送 SIGPROF 信号
//}

int main(int argc, char *argv[]) {
    //初始化
    init_task();
    //创建进程
    create_task(0, 4, 0, 5);
    create_task(1, 3, 0, 5);
    create_task(2, 5, 0, 5);
    create_task(3, 2, 0, 5);
    create_task(4, 4, 0, 5);
    print_queue();
    //运行调度
    printf("--------------------------------------------------------\n");
    printf("Start Schedule:\n");
    schedule();

//    init_sigaction();
//    init_time();
//    while (1);
    return 0;
}


