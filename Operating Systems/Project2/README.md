# Operating Systems — Project 2: Log Parsing and String Operations

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![OS](https://img.shields.io/badge/Topic-File%20Processing-blue)

> Log file parsing and subset selection utility in C, processing structured text data from input files and generating filtered output.

---

## Overview

This project implements **log file parsing** and **subset extraction** in C. It reads structured log data from an input file, parses each entry according to a defined format, and writes filtered or processed results to an output file.

---

## Files

| File | Description |
|------|-------------|
| logParse.c | Log file parser implementation |
| subset.c | Subset selection/extraction utility |
| Makefile | Build configuration |
| input.dat | Sample input data file |
| output.dat | Sample output data file |

---

## Key Concepts

- File I/O with fopen, fread, fwrite, fclose
- String parsing with strtok, sscanf, strstr
- Structured data processing from text files
- Makefile-based build system
- Command-line argument parsing (argc, argv)
- Input validation and error handling

---

## Build and Run

```bash
make
./logParse input.dat output.dat
```
