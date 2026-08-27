*This project has been created as part of the 42 curriculum by lpaiva.*

---

## Description

**Codexion** is a multi-threaded concurrency simulation project inspired by the classic **Dining Philosophers Problem**. It models a scenario where multiple coders compete for shared dongles (resources) to compile their code while respecting strict timing constraints and burnout deadlines.

### Project Goal

The primary goal is to implement a **concurrent resource allocation system** that:
- Manages contention among N threads (coders) competing for N/2 shared resources (dongles)
- Prevents classical concurrency issues: **deadlocks, race conditions, starvation, and priority inversions**
- Enforces strict scheduling disciplines: **FIFO (First-In-First-Out)** or **EDF (Earliest Deadline First)**
- Guarantees each coder completes a required number of compile cycles before their burnout deadline expires
- Maintains **thread-safe logging** without interleaved output
- Handles dongle cooldown periods (to simulate hardware recovery time)

### Key Features

- **Flexible Scheduling**: Switch between FIFO and EDF schedulers via command-line arguments
- **Precise Burnout Detection**: Monitor thread detects when coders exceed their time-to-burnout threshold
- **Priority Queue Management**: Min-heap structure maintains coder priority based on scheduler type
- **Race Condition Prevention**: Global `sim_lock` mutex + atomic operations protect all critical sections
- **Extensive Testing Suite**: Argument validation, memory leak detection (valgrind), data race detection (helgrind), concurrency verification

---

## Instructions

### Compilation

```bash
cd coders
make
```

The binary `codexion` will be generated in the `coders/` directory.

To clean object files and rebuild:
```bash
make re
```

### Execution

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> \
           <time_to_debug> <time_to_refactor> <number_of_compiles_required> \
           <dongle_cooldown> <scheduler>
```

#### Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| `number_of_coders` | Number of threads (coders) to spawn | `3` |
| `time_to_burnout` | Milliseconds before a coder burns out if not compiling | `800` |
| `time_to_compile` | Milliseconds for each compilation cycle | `200` |
| `time_to_debug` | Milliseconds spent debugging after compilation | `100` |
| `time_to_refactor` | Milliseconds spent refactoring after debugging | `100` |
| `number_of_compiles_required` | Target compilations per coder before completion | `5` |
| `dongle_cooldown` | Milliseconds dongles must rest after release before re-acquisition | `50` |
| `scheduler` | Scheduling algorithm: `fifo` or `edf` | `fifo` |

#### Example Executions

```bash
# FIFO scheduling: 3 coders, 800ms burnout, 200ms compile, 50ms dongle cooldown
./codexion 3 800 200 100 100 5 50 fifo

# EDF scheduling: 4 coders, 3000ms burnout, 150ms compile, 100ms cooldown
./codexion 4 3000 150 100 100 3 100 edf

# Single coder (edge case)
./codexion 1 3000 100 100 100 3 50 fifo

# Large scale stress test (200 coders, minimal contention)
./codexion 200 60000 60 60 60 5 60 fifo
```

### Testing

#### Run All Tests (Args, Memory Leak, Data Race Detection)
```bash
make TESTS
```

#### Individual Test Suites
```bash
make ARGS-TEST      # Argument validation & simulation tests
make MEM-TEST       # Memory leak detection (valgrind --leak-check=full)
make HELGRIND-TEST  # Data race & deadlock detection (helgrind)
```

#### Custom Concurrency Verification
```bash
bash codexion_tester.sh
bash tests_conc.sh
```

---

## Blocking Cases Handled

### 1. **Deadlock Prevention**

**Coffman's Conditions Addressed:**

| Condition | Implementation |
|-----------|-----------------|
| **Mutual Exclusion** | Dongles can only be held by one coder at a time (`is_taken` flag under `sim_lock`) |
| **Hold and Wait** | Coders never wait while holding a dongle; they request both simultaneously or not at all (`ft_grab_dongles` is atomic) |
| **No Preemption** | Dongles are only released voluntarily at end of compile cycle (`release_dongles`) |
| **Circular Wait** | Ring topology ensures ordering: even-indexed coders grab left-first, odd-indexed grab right-first (alternating pattern prevents cycles) |

**Key Mechanism:**  
The `ft_dicede_dongle()` function enforces a strict acquisition order per coder ID, breaking the circular dependency chain. Combined with atomic `ft_grab_dongles()` under mutex protection, this guarantees no two threads can form a wait-for cycle.

---

### 2. **Starvation Prevention**

**Priority Queuing:**
- Coders are stored in a **min-heap priority queue** managed by `ft_heappush()`, `ft_heappop()`, and heap shift operations
- **FIFO mode**: Coders ordered by `request_time` (arrival order) with ID as tiebreaker
- **EDF mode**: Coders ordered by deadline `(last_compile_start + time_to_burnout)` with ID as tiebreaker

**Global Priority Check:**  
The `check_global_priority()` function ensures coders only grab dongles if no higher-priority thread needs them:
```c
int check_global_priority(t_coder *coder, t_dongle *dongle)
{
    // Scan queue for threats (coders needing this dongle)
    // If threat has HIGHER priority, yield and retry
}
```

**Fairness:**  
When a coder releases dongles, only the highest-priority waiting thread proceeds, preventing indefinite blocking of lower-priority threads.

---

### 3. **Priority Inversion Prevention**

**Problem Scenario:**  
In EDF mode, if a coder's `last_compile_start` is not set when added to the queue, it gets an artificially early deadline, causing a high-priority thread to starve a low-priority one.

**Solution:**  
In `routine.c::ft_compile()`, `last_compile_start` is set **immediately upon queue insertion**, not after dongles are acquired. This ensures accurate deadline calculation from the moment a coder enters the waiting state.

---

### 4. **Precise Burnout Detection**

**Monitor Thread Logic** (`monitor.c`):
```c
if (coders[i].nbr_of_compiles < nbr_of_comp 
    && time - coders[i].last_compile_start >= data->time_to_burnout)
{
    log_action(&coders[i], "burned out", RED);
    return (1);  // Stop simulation
}
```

- Checks every 1ms (via `usleep(1000)`)
- Compares elapsed time **since the last successful compile started** against `time_to_burnout`
- Accounts for compilation in progress (time includes the full compile + debug + refactor cycle)
- Stops entire simulation immediately upon detection, preventing cascading burnouts

---

### 5. **Cooldown Handling**

**Dongle Recovery Period:**  
After release, a dongle cannot be re-acquired until its cooldown period elapses. This simulates hardware reinitialization.

**Verification in `ft_grab_dongles()`:**
```c
if (first->is_taken || time - first->last_release < first->time_to_cooldown)
    return (1);  // Can't grab yet
```

- Checks: `(current_time - last_release_time) < cooldown_period`
- Dongles initialized with `last_release = -cooldown` to allow immediate first acquisition
- Prevents lock-free race where a coder grabs a dongle before cooldown expires

---

### 6. **Race Condition Prevention**

**Shared Resource Protection:**

| Resource | Protector | Scope |
|----------|-----------|-------|
| Priority queue (`t_heap`) | `sim_lock` | All heap operations (push, pop, remove) |
| Dongle states (`is_taken`, `last_release`) | `sim_lock` | All reads/writes to dongle fields |
| Simulation active flag (`sim_active`) | `sim_lock` | All checks for graceful shutdown |
| Coder compile count (`nbr_of_compiles`) | `sim_lock` | Increment after each compilation |
| Terminal output | `write_lock` | All `log_action()` calls (separate lock for performance) |

**Atomic Operations:**  
- `ft_grab_dongles()` is called **only under `sim_lock`** and atomically sets both `first->is_taken` and `second->is_taken`
- No thread can observe a state where one dongle is taken and the other free
- Release operations (`release_dongles()`) similarly atomic under lock

---

### 7. **Log Serialization**

**Problem:**  
Multiple threads printing simultaneously causes interleaved output like:
```
0 1 is0 2compiling is compiling
```

**Solution:**  
The `write_lock` (separate from `sim_lock`) serializes all terminal I/O:
```c
void log_action(t_coder *coder, const char *msg, const char *color)
{
    pthread_mutex_lock(&coder->data->write_lock);
    printf("%lld %i%s %s%s\n", time, id, color, msg, RESET);
    pthread_mutex_unlock(&coder->data->write_lock);
}
```

**Test Validation:**  
The test suite checks: `grep -E "([0-9]+ [0-9] ...){2,}"` to detect interleaved logs (two complete messages on one line).

---

## Thread Synchronization Mechanisms

### 1. **Mutex-Based Synchronization**

#### `sim_lock` (Primary Synchronization Primitive)

**Purpose:**  
Global lock protecting all simulation state.

**Protected Resources:**
- Priority queue structure and operations
- Dongle acquisition/release (flags and timestamps)
- Simulation active flag
- Coder compile counters and state

**Usage Pattern:**
```c
// Requesting dongles
pthread_mutex_lock(&coder->data->sim_lock);
if (check_global_priority(coder, first) && check_global_priority(coder, second)) {
    if (ft_grab_dongles(coder, first, second) == 0) {
        // Acquired successfully
        pthread_mutex_unlock(&coder->data->sim_lock);
        break;
    }
}
pthread_mutex_unlock(&coder->data->sim_lock);
usleep(500);  // Back off before retry
```

**Why Single Global Lock?**
- Simplifies deadlock analysis (no lock ordering complexity)
- Eliminates circular wait by design
- Performance acceptable for this scale (benchmark: 200 coders @ 60s)

#### `write_lock` (Output Serialization)

**Purpose:**  
Dedicated lock for terminal output to prevent log interleaving.

**Protected Resources:**
- All `printf()` calls in `log_action()`

**Design Rationale:**
- Separated from `sim_lock` to allow logging during long operations (compile/debug/refactor) without blocking resource arbitration
- Improves responsiveness and reduces contention on critical path

---

### 2. **Heap-Based Priority Queue**

**Structure:**
```c
typedef struct s_heap {
    struct s_coder **coders;  // Dynamic array (min-heap)
    int max_leng;             // Capacity
    int size;                 // Current occupancy
    int type;                 // EDF or FIFO
    struct s_data *data;      // Back-reference for scheduler config
} t_heap;
```

**Operations (All Under `sim_lock`):**

| Operation | Function | Time Complexity | Purpose |
|-----------|----------|-----------------|---------|
| Insert | `ft_heappush()` | O(log N) | Add coder to waiting queue |
| Remove Top | `ft_heappop()` | O(log N) | N/A (currently unused; threads remove themselves) |
| Remove Arbitrary | `ft_remove_queue()` | O(N) | Remove coder after acquiring dongles |
| Shift Up | `heap_shift_up()` | O(log N) | Restore heap invariant after insertion |
| Shift Down | `heap_shift_down()` | O(log N) | Restore heap invariant after deletion |

**Comparison Function:**
```c
int compare_priority(t_coder *coder_a, t_coder *coder_b, t_data *data)
{
    if (data->scheduler == EDF) {
        // EDF: Earlier deadline wins
        long long deadline_a = coder_a->last_compile_start + data->time_to_burnout;
        long long deadline_b = coder_b->last_compile_start + data->time_to_burnout;
        if (deadline_a == deadline_b)
            return (coder_a->id < coder_b->id);  // ID tiebreaker
        return (deadline_a < deadline_b);
    } else {
        // FIFO: Earlier request time wins
        if (coder_a->request_time == coder_b->request_time)
            return (coder_a->id < coder_b->id);  // ID tiebreaker
        return (coder_a->request_time < coder_b->request_time);
    }
}
```

**Example: EDF Scheduling Under Contention**

Scenario: 3 coders needing the same dongle at T=500ms

| Coder | Request Time | Deadline | Priority |
|-------|--------------|----------|----------|
| Coder 1 | 100ms | 900ms (100+800) | ✓ **HIGHEST** (earliest deadline) |
| Coder 2 | 200ms | 950ms (200+750 remaining) | Medium |
| Coder 3 | 500ms | 1300ms (500+800) | **LOWEST** |

- Heap root = Coder 1
- `check_global_priority(Coder 2)` returns `0` (wait, Coder 1 is ahead)
- `check_global_priority(Coder 1)` returns `1` (grab, no threats)
- Coder 1 acquires dongle; others retry after 500µs

---

### 3. **Atomic Dongle Acquisition**

**Key Code:**
```c
static int ft_grab_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
    long long time = get_time() - coder->data->start_time;
    
    // Check BOTH dongles before committing to EITHER
    if (first->is_taken || time - first->last_release < first->time_to_cooldown)
        return (1);
    if (second->is_taken || time - second->last_release < second->time_to_cooldown)
        return (1);
    
    // Atomic: Under sim_lock, both state changes happen together
    first->is_taken = 1;
    second->is_taken = 1;
    return (0);
}
```

**Why Atomic?**
- Called only under `sim_lock`
- Checks both resources before modifying either
- Prevents partial acquisition (one free, one held)
- Ensures if this function returns 0, **both dongles are guaranteed acquired**

**Race Prevention:**
- Without lock: Two threads could read both dongles free, then both write taken=1
- Result: Both threads think they own both dongles (data corruption)
- With lock: Only one thread in `ft_grab_dongles()` at a time

---

### 4. **Graceful Shutdown Coordination**

**Monitor-Driven Termination:**
```c
void monitor(t_data *data)
{
    while (1) {
        usleep(1000);
        pthread_mutex_lock(&data->sim_lock);
        if (is_coder_burnout(data, coders) || all_coders_finish(data, coders)) {
            data->sim_active = 0;  // Signal shutdown
            pthread_mutex_unlock(&data->sim_lock);
            return;
        }
        pthread_mutex_unlock(&data->sim_lock);
    }
}
```

**Coder Response to Shutdown:**
```c
// In ft_request_dongles(), check each loop:
pthread_mutex_lock(&coder->data->sim_lock);
if (!coder->data->sim_active)
    return (pthread_mutex_unlock(&coder->data->sim_lock), 1);
pthread_mutex_unlock(&coder->data->sim_lock);
```

**Shutdown Sequence:**
1. Monitor detects condition (burnout or all done)
2. Sets `sim_active = 0` under lock
3. All coder threads see flag change, cleanly exit
4. Main thread `pthread_join()` all coders
5. Cleanup and exit

---

### 5. **Timing & Burnout Tracking**

**Deadline Calculation (EDF):**
```c
long long deadline = coder->last_compile_start + data->time_to_burnout;
long long time_since_start = get_time() - data->start_time;
long long time_since_compile_began = time_since_start - coder->last_compile_start;

if (time_since_compile_began >= data->time_to_burnout) {
    // Burnout detected
}
```

**Dongle Cooldown Check:**
```c
long long elapsed_since_release = time_current - dongle->last_release;
if (elapsed_since_release < dongle->time_to_cooldown) {
    // Still cooling, can't acquire
}
```

**Initialization (Ensures Immediate First Acquisition):**
```c
// In ft_init_dongles():
data->dongles[i].last_release = -data->dongle_cooldown;  // Pre-aged
```

Without this, dongle 0 at T=0 would check:
- `0 - (-50) = 50ms elapsed`
- `50 < 50` → FALSE, **allowed to grab immediately** ✓

---

## Technical Choices & Rationale

### Why Global Mutex Over Per-Dongle Locks?

**Alternative (Per-Dongle Locks):**
- Each dongle has its own `pthread_cond_t` + `pthread_mutex_t`
- Coders signal specific conditions when releasing
- **Problem:** Complex synchronization logic, harder to verify deadlock-freedom

**Chosen Approach (Global `sim_lock`):**
- Single lock protects all state
- Simpler, easier to reason about
- No lock ordering issues (Coffman condition eliminated)
- Sufficient performance for simulation scope

---

### Why Polling + usleep() Over Condition Variables?

**Initial Design (Condition Variables):**
- Coders `pthread_cond_wait()` when dongles unavailable
- Monitor or releasing thread signals via `pthread_cond_signal()`
- **Problem Encountered:** Spurious wakeups, missed signals, complex state management

**Current Implementation (Polling):**
- Coders poll priority with 500µs backoff
- Simpler, more predictable behavior
- Sufficient for this scale (3–200 coders)
- Easier to test and debug

---

### Why Separate `write_lock`?

**Performance Optimization:**
- `sim_lock` held briefly for resource arbitration (< 1ms)
- `write_lock` held for terminal I/O (1–10ms depending on output buffering)
- Separating them prevents slow I/O from blocking resource acquisition
- Trade-off: Minimal added complexity for better responsiveness

---

## Resources

### Documentation & References

1. **POSIX Threads (Pthreads)**
   - [The Linux Programming Interface - Michael Kerrisk (Chapter 29-33)](https://man7.org/linux/man-pages/man7/pthreads.7.html)
   - [pthread_mutex_t & pthread_cond_t Manual](https://pubs.opengroup.org/onlinepubs/9699919799/)

2. **Concurrency & Deadlock Theory**
   - Coffman, E. G., et al. (1971). "System Deadlocks" *ACM Computing Surveys* - Foundational resource on deadlock conditions and prevention
   - [Race Condition & Mutual Exclusion](https://en.wikipedia.org/wiki/Race_condition)
   - [Starvation vs Deadlock](https://en.wikipedia.org/wiki/Starvation_(computer_science))

3. **Synchronization Patterns**
   - Tanenbaum, A. S., & Bos, H. (2014). *Modern Operating Systems* (4th ed.) - Chapter on Process Synchronization
   - [Producer-Consumer & Monitor Patterns](https://en.wikipedia.org/wiki/Monitor_(synchronization))

4. **Testing & Verification Tools**
   - [Valgrind: Dynamic Analysis Tool](https://valgrind.org/)
   - [Helgrind: Data Race Detector](https://valgrind.org/docs/manual/hg-manual.html)

5. **42 School Resources**
   - Course materials on multithreading and concurrency
   - Philosophers project (predecessor to this project)

---

## AI Usage

AI was utilized to support development in the following areas:

### Code Analysis & Debugging (Primary Use)
- **Task:** Root cause analysis of starvation and burnout issues
- **Application:** 
  - Identified three critical bugs in priority checking logic, deadline initialization, and cooldown math
  - Generated detailed bug reports with line-by-line annotations
  - Provided corrected code blocks for each issue
  - Validated fixes against test scenarios

### Documentation & Explanation (Secondary Use)
- **Task:** README structure and technical explanation generation
- **Application:**
  - Drafted comprehensive "Blocking Cases Handled" section with Coffman's conditions analysis
  - Outlined thread synchronization mechanisms with code examples
  - Created tables and diagrams for technical clarity
  - Formatted examples and instructions for readability

### Test Suite Review
- **Task:** Verification of test logic
- **Application:**
  - Analyzed Makefile test targets for coverage gaps
  - Reviewed shell script test cases for edge cases
  - Suggested improvements to concurrency verification (sliding-window algorithm)

### NOT Used For
- Core algorithm design (custom, handwritten)
- Actual implementation (written by project author)
- Architectural decisions (author-driven)

---

## Build & Run Summary

```bash
# Clone and enter directory
cd coders

# Compile
make

# Run example
./codexion 3 800 200 100 100 5 50 fifo

# Run full test suite
make TESTS

# Clean
make fclean
```

---

## Author

**lpaiva** (42 Porto student)

---

**Last Updated:** August 27, 2026
