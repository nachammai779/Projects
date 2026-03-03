# Operating Systems Projects

![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)

> Six C programming projects covering core operating systems concepts including process management, inter-process communication (IPC), scheduling, and concurrency.

---

## 📂 Projects Overview

| Project | Topic | Key Concepts | Files |
|---------|-------|-------------|-------|
| [Project 1](Project1/) | Systems Programming | File I/O, C basics | `assignment1-finalversion.c` |
| [Project 2](Project2/) | Log Parsing & Subset Extraction | File parsing, string processing | `logParse.c`, `subset.c` |
| [Project 3](Project3/) | OS Concepts | Process fundamentals | — |
| [Project 4](Project4/) | Message Queue IPC | POSIX IPC, OSS simulator | `oss.c`, `user.c`, `msqreader.c`, `msqwriter.c` |
| [Project 5](Project5/) | OSS Scheduling Simulation | Process scheduling, shared memory | `oss.c`, `user.c` |
| [Project 6](Project6/) | Master-User Process Management | Queue management, process coordination | `master.c`, `user.c`, `queue.c` |

---

## 🔹 Project 1 — Systems Programming

**Goal:** Foundational C systems programming covering file I/O and process operations.

**File:** `assignment1-finalversion.c`

---

## 🔹 Project 2 — Log Parsing & Subset Extraction

**Goal:** Parse log files and extract meaningful subsets using C string manipulation and file I/O.

**Files:** `logParse.c`, `subset.c`, `input.dat`, `output.dat`, `Makefile`

**Build:** `make`

---

## 🔹 Project 3 — OS Concepts

**Goal:** Exploration of core OS concepts (see project folder for details).

---

## 🔹 Project 4 — Message Queue IPC

**Goal:** Implement inter-process communication using POSIX message queues. An OSS (Operating System Simulator) coordinates user processes via message passing.

**Key Concepts:** Message queues, IPC, parent-child process coordination, shared memory

**Files:** `oss.c`, `user.c`, `msqreader.c`, `msqwriter.c`, `test01.c`

**Build:** `gcc oss.c -o oss`

---

## 🔹 Project 5 — OSS Scheduling Simulation

**Goal:** Simulate a CPU scheduling algorithm within an OSS framework using shared memory and semaphores.

**Key Concepts:** Process scheduling, shared memory, fork/exec, signal handling

**Files:** `oss.c`, `oss.h`, `user.c`

**Build:** `gcc oss.c -o oss && gcc user.c -o user`

---

## 🔹 Project 6 — Master-User Process Management with Queue

**Goal:** Implement a master process that manages user processes through a custom queue, simulating OS-level resource management.

**Key Concepts:** Queue data structures, process lifecycle management, process synchronization

**Files:** `master.c`, `user.c`, `queue.c`, `makefile`

**Build:** `make`

---

## ⚙️ Requirements

- GCC compiler
- Linux/Unix environment (POSIX support required for IPC projects)

```bash
gcc -o program program.c
# Or use make where a Makefile is provided
```
