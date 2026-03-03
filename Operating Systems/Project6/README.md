# Operating Systems — Project 6: Memory Management with Page Replacement

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![OS](https://img.shields.io/badge/Topic-Memory%20Management-blue)

> Simulated memory management system with a custom queue data structure, implementing page replacement algorithms in C.

---

## Overview

This project implements a **memory management simulator** in C. A master process coordinates multiple user processes that generate memory access requests. The simulator uses a custom queue data structure to manage page frames and implements page replacement to handle page faults.

---

## Files

| File | Description |
|------|-------------|
| master.c | Master/OS process managing memory simulation |
| user.c | User process generating memory access requests |
| queue.c | Custom queue data structure implementation |
| makefile | Build configuration |
| navTemp.txt | Navigation/test data file |
| navTemp2.txt | Navigation/test data file (variant) |

---

## Key Concepts

- Virtual memory and page tables
- Page faults and page replacement algorithms (LRU, FIFO, etc.)
- Frame allocation strategies
- Custom queue/linked-list data structures in C
- Memory access simulation (reads and writes)
- Page frame management and tracking
- Dirty bit and reference bit tracking

---

## Build and Run

```bash
make
./master
```
