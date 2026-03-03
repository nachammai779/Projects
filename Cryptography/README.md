# Cryptography Projects

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Status](https://img.shields.io/badge/status-academic-blue)

> C++ implementations of pseudorandom number generators and cryptographic stream cipher functions used in classical cryptography.

---

## 📂 Files Overview

| File | Description | Algorithm |
|------|-------------|-----------|
| `bbs.cpp` | Blum Blum Shub PRNG | Quadratic residue-based PRNG |
| `lcg.cpp` | Linear Congruential Generator | Classic LCG formula |
| `task2.cpp` | Cryptographic task | Stream cipher or key generation |
| `ssfun.cpp.txt` | Shared support functions | Helper utilities |
| `ssfun.h.txt` | Header file for support functions | Function declarations |

---

## 🔹 Blum Blum Shub (BBS) — `bbs.cpp`

**Algorithm:** The BBS generator uses the formula: **x(n+1) = x(n)² mod M**, where M = p × q (product of two large primes).

**Security:** Considered cryptographically secure — the sequence is hard to predict without knowing the factorization of M.

**Output:** Generates a pseudorandom bit sequence.

---

## 🔹 Linear Congruential Generator (LCG) — `lcg.cpp`

**Algorithm:** Uses the recurrence: **x(n+1) = (a × x(n) + c) mod m**

**Parameters:** Multiplier (a), increment (c), modulus (m), and seed.

**Use Case:** Fast and simple PRNG, not cryptographically secure but widely used for simulations.

---

## 🔹 Support Functions — `ssfun.cpp` / `ssfun.h`

Shared utility functions used across cryptographic implementations (e.g., modular arithmetic, prime checking).

---

## ⚙️ Build & Run

```bash
g++ bbs.cpp -o bbs && ./bbs
g++ lcg.cpp -o lcg && ./lcg
g++ task2.cpp ssfun.cpp -o task2 && ./task2
```
