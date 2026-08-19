# CSE 314 — Operating Systems (xv6-riscv)

A series of five OS modules built on top of the MIT **xv6-riscv** kernel, progressively extending it from shell tooling all the way to kernel-level threading.

---

## Module Overview

```mermaid
flowchart LR
    SRC(["🖥️ Base\nxv6-riscv"])

    subgraph M1["Module 1 · Shell & Autograder"]
        SH["Bash Script\nAutograder"]
    end

    subgraph M2["Module 2 · System Calls"]
        SC["trace() · sysinfo()\nhistory !!"]
    end

    subgraph M3["Module 3 · CPU Scheduler"]
        direction TB
        Q0["Queue 0\nLottery"]
        Q1["Queue 1\nRound Robin"]
        Q0 --> Q1
    end

    subgraph M4["Module 4 · IPC"]
        direction TB
        PT["pthreads"]
        SEM["Semaphores\nMutexes · CondVars"]
        PT --> SEM
    end

    subgraph M5["Module 5 · Kernel Threads"]
        direction TB
        TC["thread_create()"]
        TJ["thread_join()"]
        TX["thread_exit()"]
        SP["Spinlock · Mutex"]
        TC --> TJ --> TX --> SP
    end

    SRC --> M1 --> M2 --> M3 --> M4 --> M5

    style SRC fill:#1e3a5f,color:#fff
    style M1  fill:#065f46,color:#fff
    style M2  fill:#1e40af,color:#fff
    style M3  fill:#b45309,color:#fff
    style M4  fill:#6d28d9,color:#fff
    style M5  fill:#991b1b,color:#fff
```

---

## Module 1 — Shell Scripting & Autograder

A fully automated grading system written in Bash. It validates, extracts, compiles, runs and scores any number of student submissions and produces a CSV report.

```mermaid
flowchart TD
    IN["input_file.txt\n11-line config"]

    IN --> VI["validate_input()\nline count · formats\nlanguages · penalties"]

    VI -->|valid| EX["Extract Submissions\nunzip / tar / rar"]
    VI -->|invalid| ERR["Exit 1"]

    EX --> COM["Compile\nC → gcc\nC++ → g++\nPy → python3\nSh → bash"]

    COM --> RUN["Run vs Test Cases\ndiff output"]

    RUN --> SC["Score\n− wrong answer\n− missing file\n− submission penalty"]

    SC --> PLAG["Plagiarism Check\ncompare all outputs\npair-wise md5"]

    PLAG --> CSV["marks.csv\nstudent_id , score"]

    style IN   fill:#1e3a5f,color:#fff
    style VI   fill:#065f46,color:#fff
    style COM  fill:#1e40af,color:#fff
    style RUN  fill:#b45309,color:#fff
    style PLAG fill:#991b1b,color:#fff
    style CSV  fill:#065f46,color:#fff
```

**Config file format (`input_file.txt`):**

| Line | Value |
|------|-------|
| 1 | `true` / `false` — archive mode |
| 2 | Allowed archive formats (`zip rar tar`) |
| 3 | Allowed source languages (`c cpp py sh`) |
| 4–11 | Penalty and test-case settings |

**Run:**
```bash
cd "Offline1(Shell-Autograder)"
bash Offline1_2005046.sh -i input_file.txt
```

---

## Module 2 — System Calls

Three new kernel interfaces added to xv6, mirroring Linux tracing and monitoring utilities.

```mermaid
flowchart TD
    US["User Space"]

    US -->|"trace(mask)"| TR["kernel/sysproc.c\nsys_trace()\nsets p->tracemask"]
    US -->|"sysinfo()"| SI["kernel/sysproc.c\nsys_sysinfo()\ncollects nproc · freemem"]
    US -->|"!!"| HI["user/sh.c\nhistory buffer\nreplay last / nth cmd"]

    TR --> KT["Kernel intercepts\nevery syscall\nprints if masked"]
    SI --> KM["kernel/kalloc.c\nfreecount()"]
    SI --> KP["kernel/proc.c\nnproc()"]

    KT --> LOG["stdout trace log\ne.g. pid=3 syscall=write"]
    KM & KP --> UFR["struct sysinfo\n{nproc, freemem}"]

    style US  fill:#1e3a5f,color:#fff
    style TR  fill:#1e40af,color:#fff
    style SI  fill:#1e40af,color:#fff
    style HI  fill:#065f46,color:#fff
    style LOG fill:#b45309,color:#fff
    style UFR fill:#b45309,color:#fff
```

**Apply & run:**
```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
git apply ../Offline2\(System-call\)/Offline2_2005046.patch
make qemu
```

---

## Module 3 — CPU Scheduler (MLFQ + Lottery)

The default Round-Robin scheduler is replaced with a **two-level Multi-Level Feedback Queue**. Queue 0 uses probabilistic Lottery Scheduling; Queue 1 falls back to Round Robin.

```mermaid
flowchart TD
    NP["New Process\nenters Queue 0\nwith default tickets"]

    NP --> Q0

    subgraph Q0["Queue 0 — Lottery Scheduling (High Priority)"]
        LT["Draw random ticket\nwinner gets CPU slice"]
    end

    subgraph Q1["Queue 1 — Round Robin (Low Priority)"]
        RR["Fixed time-slice\nfair rotation"]
    end

    LT -->|"used full\ntime slice"| DEM["Demote → Queue 1"]
    LT -->|"voluntary\nyield / I/O"| LT

    DEM --> Q1
    RR  -->|"aged too long\n(starvation guard)"| PRO["Promote → Queue 0"]
    PRO --> Q0

    Q0 & Q1 --> SC["kernel/proc.c\nscheduler()"]

    SC --> NEW["settickets(n)\ngetpinfo()"]

    style Q0  fill:#1e40af,color:#fff
    style Q1  fill:#065f46,color:#fff
    style DEM fill:#991b1b,color:#fff
    style PRO fill:#40916c,color:#fff
    style SC  fill:#b45309,color:#fff
```

| System Call | Purpose |
|-------------|---------|
| `settickets(int n)` | Assign `n` lottery tickets to the calling process |
| `getpinfo(struct pstat*)` | Dump scheduler stats (queue, ticks, tickets) for all processes |

**Apply & run:**
```bash
git apply ../Offline3\(Kernel-Scheduler\)/Offline3_2005046.patch
make qemu
```

---

## Module 4 — Inter-Process Communication (IPC)

A **Museum Visit** concurrency simulation solved with POSIX threads, semaphores, mutexes and condition variables. Demonstrates Reader-Writer variants and deadlock-free synchronization.

```mermaid
flowchart TD
    MAIN["main()\nspawns visitor threads"]

    MAIN --> PV["Premium Visitors\nhigher priority\npreempt standard queue"]
    MAIN --> SV["Standard Visitors\nwait for capacity slot"]

    PV & SV --> ENT["Museum Entry\ncapacity semaphore\nmax N concurrent"]

    ENT --> PB["Photo Booth\nReader-Writer mutex\n1 photographer at a time"]
    ENT --> EX["General Exhibits\nshared · concurrent OK"]

    PB & EX --> EXIT["Visitor exits\nreleases capacity slot"]

    EXIT --> REP["Stats Reporter thread\nprints counts · waits"]

    style MAIN fill:#1e3a5f,color:#fff
    style ENT  fill:#1e40af,color:#fff
    style PB   fill:#991b1b,color:#fff
    style EX   fill:#065f46,color:#fff
    style REP  fill:#b45309,color:#fff
```

**Synchronization primitives used:**

| Primitive | Role |
|-----------|------|
| `sem_t` (semaphore) | Museum capacity limit |
| `pthread_mutex_t` | Photo booth exclusion |
| `pthread_cond_t` | Priority queuing for premium visitors |
| `pthread_barrier_t` | All threads ready before simulation starts |

**Build & run:**
```bash
cd "Offline4(IPC)"
unzip 2005046.zip
g++ -pthread -o museum museum.c
./museum <visitors> <capacity>
```

---

## Module 5 — Kernel-Level Threads & Synchronization

Thread support and synchronization primitives implemented **inside the xv6 kernel**. Threads share the parent's page table via `uvmmirror()` (no new physical pages allocated).

```mermaid
flowchart TD
    TC["thread_create(fcn, arg, stack)"]

    TC --> FORK["fork()-like clone\nbut NO address space copy"]
    FORK --> UVM["uvmmirror()\nmappages() same PTE\nno kalloc()"]
    FORK --> STK["Replace user stack\np→trapframe→sp = stack\ncopy arg onto new stack"]
    FORK --> EPC["p→trapframe→epc = fcn\nfake return PC 0xFFFFFFFF"]

    UVM & STK & EPC --> RUN["Kernel schedules\nnew thread"]

    RUN --> SYNC

    subgraph SYNC["Synchronization"]
        direction LR
        SL["Spinlock\natomic test-and-set\nbusy-wait"]
        MX["Mutex\natomic test-and-set\nyield() on contention"]
    end

    RUN --> TJ["thread_join(tid)\nwait() variant\nmatches mem_id"]
    RUN --> TX["thread_exit()\nexit() variant\ndo NOT free page table\n(shared with parent)"]

    style TC   fill:#1e3a5f,color:#fff
    style FORK fill:#1e40af,color:#fff
    style UVM  fill:#6d28d9,color:#fff
    style SYNC fill:#065f46,color:#fff
    style TJ   fill:#b45309,color:#fff
    style TX   fill:#991b1b,color:#fff
```

**Key `struct proc` additions:**

```c
struct proc {
    // ... existing fields ...
    struct spinlock memlock;  // guard shared page table
    int is_thread;            // 1 if this proc is a thread
    int mem_id;               // all threads of same process share this ID
};
```

**Apply & run:**
```bash
git apply ../Offline5\(Threading\)/Offline5_2005046.patch
make qemu
# In xv6 shell:
$ threads
```

---

## Supported Features

| Feature | Module | Implemented |
|---------|--------|-------------|
| Bash autograder (multi-language) | 1 | ✅ |
| Plagiarism detection | 1 | ✅ |
| `trace()` syscall (strace-like) | 2 | ✅ |
| `sysinfo()` — nproc / freemem | 2 | ✅ |
| Shell history `!!` | 2 | ✅ |
| Lottery Scheduling (Queue 0) | 3 | ✅ |
| Round-Robin (Queue 1) | 3 | ✅ |
| MLFQ promotion / demotion | 3 | ✅ |
| Aging (starvation prevention) | 3 | ✅ |
| `settickets` / `getpinfo` | 3 | ✅ |
| POSIX thread Museum simulation | 4 | ✅ |
| Priority visitor queuing | 4 | ✅ |
| Deadlock-free semaphore design | 4 | ✅ |
| `thread_create/join/exit` | 5 | ✅ |
| Shared address space (`uvmmirror`) | 5 | ✅ |
| Page table sync across threads | 5 | ✅ |
| User-space Spinlock | 5 | ✅ |
| User-space Mutex (sleeping lock) | 5 | ✅ |

---

## Project Structure

```
CSE-314-Operating-System/
│
├── Offline1(Shell-Autograder)/
│   ├── Offline1_2005046.sh      # Autograder script
│   └── Offline1_test-cases.zip
│
├── Offline2(System-call)/
│   └── Offline2_2005046.patch   # trace · sysinfo · history
│
├── Offline3(Kernel-Scheduler)/
│   └── Offline3_2005046.patch   # MLFQ · Lottery · RR · aging
│
├── Offline4(IPC)/
│   └── 2005046.zip              # Museum simulation (pthreads)
│
├── Offline5(Threading)/
│   ├── Offline5_2005046.patch   # Kernel threads · spinlock · mutex
│   └── Offline_5_Threading.md  # Assignment spec
│
└── Resources/
    ├── book-riscv-rev1.pdf      # xv6 RISC-V book
    └── Shell_Commands_Sept4Update.pdf
```

---

## How to Build & Run

> **Prerequisites:** `gcc-riscv64-linux-gnu`, `qemu-system-misc`, `make`
>
> Install on Ubuntu/Debian:
> ```bash
> sudo apt install gcc-riscv64-linux-gnu qemu-system-misc make
> ```

### Applying Any Patch (Modules 2–5)

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
git apply /path/to/OfflineN_2005046.patch
make qemu
```

### Module 1 — Autograder only

```bash
cd "Offline1(Shell-Autograder)"
bash Offline1_2005046.sh -i input_file.txt
```

---

## Data Flow Summary

```mermaid
sequenceDiagram
    participant U  as User/Shell
    participant K  as xv6 Kernel
    participant S  as Scheduler
    participant M  as Memory Manager

    U->>K: syscall (trace / sysinfo / thread_create)
    K->>K: validate args · update proc struct
    K->>S: scheduler() selects next runnable thread
    S->>S: Lottery draw (Queue 0) or RR tick (Queue 1)
    S->>K: swtch() to chosen thread
    K->>M: uvmmirror() — share page table for threads
    M-->>K: mappages() — same PTE, no kalloc
    K-->>U: return value in a0 register
```

---

*Course: CSE 314 — Operating Systems | Student ID: 2005046*
