# Operating Systems — Project 5: Process Management with Shared Memory

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![OS](https://img.shields.io/badge/Topic-Shared%20Memory-blue)

> Advanced process management using shared memory IPC and an Operating System Simulator (oss) with header file organization.

---

## Overview

This project extends the OS Simulator (oss) with **shared memory** for efficient inter-process communication. The project is modularized with a header file (oss.h) defining shared data structures used by both the master (oss.c) and worker (user.c) processes.

---

## Files

| File | Description |
|------|-------------|
| oss.c | Master process / OS simulator |
| oss.h | Shared header with data structures and constants |
| user.c | Worker/user process |

---

## Key Concepts

- Shared memory segments (shmget, shmat, shmdt, shmctl)
- Semaphores for synchronization (semget, semop, semctl)
- Modular C code with header files
- Process lifecycle management
- Simulated clock and logical time in shared memory
- Race condition prevention in concurrent processes

---

## Build and Run

```bash
gcc -o oss oss.c
gcc -o user user.c
./oss
```
