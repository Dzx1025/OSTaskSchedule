# OSTaskSchedule

这是关于学校操作系统的系统调度实验

## 构建（使用cmake）:

Linux: 
```
./build-option.sh 算法名
```

Windows:
```
cmake -D DEFINE_FCFS=OFF -D DEFINE_SJF=OFF -D DEFINE_RR=OFF -D DEFINE_PRIOR=OFF build
cmake -D "DEFINE_算法名"=ON build
cmake --build build
```
## 运行:
```
build/OSTaskSchedule
```
