#include <stdio.h>    // for printf()
#include <unistd.h>   // for pause()
#include <signal.h>   // for signal()
#include <string.h>   // for memset()
#include <sys/time.h> // struct itimeral. setitimer()

void printMsg(int num) {
    printf("%s", "Hello World!!\n");
}

int main() {
    // Get system call result to determine successful or failed
    int res = 0;
    // Register printMsg to SIGALRM
    signal(SIGALRM, printMsg);
    struct itimerval tick;
    // Timeout to run function first time
    tick.it_value.tv_sec = 5;  // sec
    tick.it_value.tv_usec = 0; // micro sec.
    // Interval time to run function
    tick.it_interval.tv_sec = 1;
    tick.it_interval.tv_usec = 0;
    // Set timer, ITIMER_REAL : real-time to decrease timer,
    //                          send SIGALRM when timeout
    res = setitimer(ITIMER_REAL, &tick, NULL);
    if (res) {
        printf("Set timer failed!!/n");
    }
    // Always sleep to catch SIGALRM signal
    while (1) {
        pause();
    }
    return 0;
}