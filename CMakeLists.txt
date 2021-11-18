cmake_minimum_required(VERSION 3.16)
project(OSTaskSchedule C)

set(CMAKE_C_STANDARD 99)

OPTION(DEFINE_FCFS  "Option description" OFF) # Disabled by default
OPTION(DEFINE_SJF   "Option description" OFF) # Disabled by default
OPTION(DEFINE_RR    "Option description" OFF) # Disabled by default
OPTION(DEFINE_HRRN  "Option description" OFF) # Disabled by default

IF (DEFINE_FCFS)
    ADD_DEFINITIONS(-D_FCFS_)
ELSEIF (DEFINE_SJF)
    ADD_DEFINITIONS(-D_SJF_)
ELSEIF (DEFINE_RR)
    ADD_DEFINITIONS(-D_RR_)
ELSEIF (DEFINE_HRRN)
    ADD_DEFINITIONS(-D_HRRN_)
ENDIF ()

add_executable(OSTaskSchedule main.c sched.h list.h sched.c)