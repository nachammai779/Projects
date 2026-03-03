# Operating Systems — Project 1: System Programming in C

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![OS](https://img.shields.io/badge/Topic-System%20Programming-blue)

> Introduction to system-level programming in C, covering process management, file I/O, and basic Unix system calls.

---

## Overview

This project introduces fundamental **system-level programming in C** using Unix/Linux system calls. The assignment covers process creation, file operations, and interaction with the operating system kernel through system call interfaces.

---

## Files

| File | Description |
|------|-------------|
| assignment1-finalversion.c | System programming assignment in C |

---

## Key Concepts

- Unix system calls (fork, exec, wait, open, read, write, close)
- Process creation and management with fork()
- Parent-child process relationships
- File I/O using low-level system call interface
- Error handling with errno and perror()
- Process termination and exit status codes

---

## Build and Run

```bash
gcc -o assignment1 assignment1-finalversion.c
./assignment1
```
