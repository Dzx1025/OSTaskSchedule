#!/usr/bin/env bash
cmake -D DEFINE_FCFS=OFF -D DEFINE_SJF=OFF -D DEFINE_RR=OFF -D DEFINE_PRIOR=OFF build
cmake -D "DEFINE_$1"=ON build
cmake --build build