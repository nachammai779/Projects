# Operating Systems — Project 4: Inter-Process Communication with Message Queues

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![OS](https://img.shields.io/badge/Topic-IPC%20Message%20Queues-blue)

> Master-worker process coordination using POSIX message queues (IPC) in C, implementing an Operating System Simulator (oss).

---

## Overview

This project implements **inter-process communication (IPC)** using **message queues** in C. An Operating System Simulator (oss) acts as the master process, spawning multiple worker processes that communicate through System V message queues. The project demonstrates process synchronization and communication patterns.

---

## Files

| File | Description |
|------|-------------|
| oss.c | Master process / OS simulator |
| user.c | Worker/user process |
| msqreader.c | Message queue reader utility |
| msqwriter.c | Message queue writer utility |
| test01.c | Test program for IPC mechanism |

---

## Key Concepts

- System V Message Queues (msgget, msgsnd, msgrcv, msgctl)
- Master-worker (producer-consumer) process architecture
- Process spawning with fork() and exec()
- Shared memory and IPC mechanisms
- Process synchronization without race conditions
- Cleanup of IPC resources on termination

---

## Build and Run

```bash
gcc -o oss oss.c
gcc -o user user.c
./oss
```
