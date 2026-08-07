Here is a detailed, objective breakdown of the threading tools you must use, why they are mandatory for your architecture, and how they will alter your structures and logic.1. What You Need to UseTo manage your shared priority queue in a multithreaded environment, you must use two specific POSIX thread mechanisms mandated by the project subject:  Mutexes (pthread_mutex_t): Think of a mutex as a physical padlock on your queue.Condition Variables (pthread_cond_t): Think of a condition variable as a pager or a waiting room bell.2. Why You Need ThemThe Mutex (Preventing Memory Corruption):If Coder 1 and Coder 2 both finish refactoring at the exact same millisecond, both of their threads will call ft_heappush.Without a mutex, both threads will read queue->size simultaneously (e.g., they both read 0). Both will write their pointer to queue->coders[0]. One will overwrite the other, destroying your data and permanently losing a coder. A mutex ensures that only one thread can access or modify the heap at any given time.The Condition Variable (Preventing CPU Burnout):
When a coder pushes themselves into the queue because the dongles are taken, they have to wait.
If you use a simple while (waiting) loop to continuously check if it is their turn, that thread will spin infinitely, consuming 100% of a CPU core. This is called "busy-waiting." A condition variable allows the thread to safely go to sleep (consuming 0 CPU) until another thread explicitly rings the "bell" to wake them up when dongles are ready.  3. How It Translates to Your StructuresYour structs need to house these new tools. You currently have a sim_lock and a write_lock in your t_data struct, but tying the queue strictly to sim_lock might create bottlenecks if the monitor thread is constantly checking other simulation states.  You should add dedicated synchronization variables to manage the waiting coders. You have two structural choices for the condition variable:Option A (Centralized): Add a pthread_mutex_t queue_lock and a single pthread_cond_t queue_cond inside your t_heap struct. All waiting coders sleep on this single condition variable. When dongles are freed, you wake up all of them (pthread_cond_broadcast), they all check the top of the queue, and only the winner takes the dongles; the rest go back to sleep.Option B (Targeted): Add a pthread_mutex_t queue_lock to t_heap, but keep the pthread_cond_t wait variable you already placed inside your t_coder struct. When dongles are freed, the releasing thread pops the winning coder from the heap and sends a signal specifically to that coder's condition variable.  Option B is mathematically cleaner and strictly enforces your EDF/FIFO rules without waking up threads unnecessarily.4. How It Translates to Your Code LogicYou will not simply call ft_heappush anymore. You will wrap it in a sequence of locking and signaling operations. Here is the conceptual workflow:When a Coder Needs Dongles (The Entry Phase):Lock: The thread locks the queue_lock. (No other thread can touch the queue now).Push: The thread calls ft_heappush to add itself to the heap.Sleep: The thread calls pthread_cond_wait.Crucial mechanic: Calling pthread_cond_wait automatically and instantly unlocks the mutex and puts the thread to sleep in one atomic step. This allows other coders to push themselves into the queue while this thread is sleeping.Wake up: When signaled, the thread wakes up and automatically re-locks the mutex.Proceed: The thread (now awake and holding the lock) takes the physical dongles, unlocks the queue_lock, and begins compiling.When a Coder Finishes Compiling (The Release Phase):Lock: The releasing thread locks the queue_lock.Release: It puts the physical dongles back on the table.Pop: It checks if the queue has waiting coders (queue->size > 0). If yes, it calls ft_heappop to extract the highest-priority coder.Signal: It calls pthread_cond_signal aimed at the specific condition variable of the popped coder.Unlock: It unlocks the queue_lock and goes to debug.This is the architectural blueprint for your concurrent queue. Between Option A (Centralized broadcasting) and Option B (Targeted signaling to individual coders), which approach makes the most logical sense to you to implement, and why?



The coder_routine is the autonomous engine of your simulation. Once a thread is spawned, it must enter a continuous loop that dictates how the coder interacts with the shared environment (the dongles and the terminal) without crashing the program or causing a deadlock.

Building this phase requires translating real-world actions into a strict, thread-safe state machine. Here is an objective, detailed breakdown of the logic and the structural hurdles you will need to resolve, keeping the focus entirely on concepts rather than a full code implementation.
1. The Core Lifecycle (The State Machine)

Inside the coder_routine, the thread will run an infinite loop (e.g., while (1)). Inside this loop, the coder cycles through distinct phases in a strict order:

    Phase A: Requesting to Compile (The Queue)
    Before touching any dongles, the coder must realize they need to compile. They capture the current system time and store it in their request_time variable. If you are implementing your priority queue, this is the exact moment the coder locks the queue_lock, pushes themselves into the heap using ft_heappush, and waits for the scheduler to grant them permission to proceed.

    Phase B: Compiling (Execution and Reset)
    Once granted permission (or once both left and right dongles are successfully locked), the coder has secured the resources.
    Crucially, this is when the coder's "starvation" timer resets. You must capture the current time again and update their last_compile_start variable. The thread then sleeps for the time_to_compile duration. Upon waking, the coder increments their nbr_of_compiles counter and unlocks both dongles to return them to the table.

    Phase C: Debugging and Refactoring
    Immediately after releasing the dongles, the coder enters the debugging phase (sleeping for time_to_debug), followed by the refactoring phase (sleeping for time_to_refactor). Once these are complete, the loop restarts, and the coder requests to compile again.

2. The Custom Sleep Mechanism (Mitigating OS Drift)

Relying purely on the standard C usleep() function is a critical failure point in time-sensitive simulations.

The operating system's thread scheduler is not perfectly precise. If you tell a thread to usleep(50000) (50 milliseconds), the CPU might get busy and actually sleep the thread for 55 or 60 milliseconds. Over hundreds of cycles, this "drift" accumulates, causing coders to unfairly miss their burnout deadlines and die.

To solve this, you must write a custom wrapper function (e.g., ft_usleep). The logic for this function involves:

    Capturing the exact start time using gettimeofday.

    Calculating the target end time (start time + desired sleep duration).

    Entering a while loop that continuously checks the current time against the target end time.

    Inside that loop, executing tiny micro-sleeps (e.g., usleep(100)) so the thread does not consume 100% of the CPU while it waits.

This guarantees the sleep duration is measured by the absolute system clock, not the unreliable thread scheduler.
3. Thread-Safe Terminal Logging

Every time a coder changes state (e.g., takes a dongle, starts compiling, dies), they must print a timestamped message to the terminal.

Because multiple threads are running simultaneously, if two coders try to use printf at the exact same microsecond, the terminal output will become scrambled, overlapping strings and breaking the formatting.

You must create a dedicated logging function that utilizes your write_lock. The logical flow is:

    Lock the write_lock.

    Check the sim_active flag. (This is critical: if a coder died in another thread a microsecond ago, you do not want this thread to print "Coder is compiling" after the death message).

    Calculate the current simulation timestamp.

    Print the formatted message.

    Unlock the write_lock.

Given that the priority queue acts as the gateway to the dongles, how do you plan to structure the logic inside the routine so that a thread knows it is mathematically "their turn" to pop themselves from the queue and lock the mutexes