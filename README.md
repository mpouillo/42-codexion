*This project has been created as a part of the 42 curriculum by mpouillo.*

# Codexion

[![Norminette](https://github.com/mpouillo/42-codexion/actions/workflows/norminette.yml/badge.svg?branch=main)](https://github.com/mpouillo/42-codexion/actions/workflows/norminette.yml)
[![Compilation](https://github.com/mpouillo/42-codexion/actions/workflows/compilation.yml/badge.svg?branch=main)](https://github.com/mpouillo/42-codexion/actions/workflows/compilation.yml)

## Instructions

Compile the program using the provided Makefile:

```shell
# In the /coders directory
$> make all
```

Run the program:

```shell
$> ./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```


## Description:

Codexion is a project written in C that requires orchestrating multiple threads (represented as coders) competing for limited shared resources (represented as USB dongles). This project is an introduction to POSIX threads (pthreads), mutexes, and smart scheduling aimed at synchronizing resource usage.

POSIX Threads, as defined in the POSIX standards, are subdivisions of a single process that each share the same global memory (data and heap segments) but exist on their own stack. In other words, they compete for resources while executing individually of each other.

The goal of Codexion is to implement thread-safe management of resources.

### Overview

- One or more coders sit in a circular inclusive co-working hub. In the center, there is a shared Quantum Compiler.
- The coders alternatively compile, debug, or refactor. While compiling, they are not debugging nor refactoring; while debugging, they are not compiling nor refactoring; and, of course, while refactoring, they are not compiling nor debugging.
- There are USB dongles on the table. There are as many dongles as coders.
- Compiling quantum code requires two dongles plugged in simultaneously, one in each hand: a coder takes their left and right dongles to compile.
- When a coder finishes compiling, they put both dongles back on the table and start debugging. Once debugging is done, they start refactoring. The simulation stops when a coder burns out due to lack of compiling.
- Every coder needs to compile regularly and should never burn out.
- Coders do not communicate with each other.
- Coders do not know if another coder is about to burn out.
- Needless to say, coders should avoid burnout!

### Program arguments

- `number_of_coders`: The number of coders and also the number of dongles. Must be a valid positive integer (`>1`).
- `time_to_burnout` (in milliseconds): If a coder did not start compiling within `time_to_burnout` milliseconds since the beginning of their last compile or the
beginning of the simulation, they burn out. Must be a valid positive integer.
- `time_to_compile` (in milliseconds): The time it takes for a coder to compile. During that time, they must hold two dongles. Must be a valid positive integer.
- `time_to_debug` (in milliseconds): The time a coder will spend debugging. Must be a valid positive integer.
- `time_to_refactor` (in milliseconds): The time a coder will spend refactoring. After completing the refactoring phase, the coder will immediately attempt to acquire dongles and start compiling again. Must be a valid positive integer.
- `number_of_compiles_required`: If all coders have compiled at least this
many times, the simulation stops. Otherwise, it stops when a coder burns out.
- `dongle_cooldown` (in milliseconds): After being released, a dongle is unavailable until its cooldown has passed. Must be a valid positive integer.
- `scheduler`: The arbitration policy used by dongles to decide who gets them when multiple coders request them. The value must be exactly one of: `fifo` or `edf`. `fifo` means First In, First Out: the dongle is granted to the coder whose request arrived first. `edf` means Earliest Deadline First with deadline = last_compile_start + time_to_burnout

### Blocking cases handled

#### Coffman's conditions

A deadlock represents a situation in which multiple processes (threads) prevent each other from accessing a shared resource. In Codexion, this could be the case where two coders share two dongles, and each have priority on one dongle over the other one. Since they need both to compile, they are both stuck waiting for both dongles to be available.

There are four conditions which must hold for deadlock to occur as classically defined. These are known as Coffman's conditions:
- Mutual exclusion. There must be some resource that can't be shared between processes.
- Hold and wait. Some process must be holding one resource while waiting for another.
- No preemption. Only the process holding a resource can release it.
- Circular wait. There must be some cycle of waiting processes P1, P2, ..., Pn such that each process Pi is waiting for a resource held by the next process in the cycle. (Note that this implies hold-and-wait.)

In the case of Codexion, dongles are the resource that can't be shared between processes. In that end, they are each represented by a mutex (pthread_mutex_t), a device that can never be owned by two different threads simultaneously. The "hold and wait" situation would represent coders acquiring a dongle while waiting to obtain the second one, preventing other coders from accessing it since only the coder (thread) that acquired a dongle (mutex) can unlock it. Finally, each coder always share a dongle with two other coders, creating a circle of waiting threads. Since Coffman's conditions are all completed, this means a deadlock is possible and likely to occur if not actively prevented.

#### Deadlock prevention

To prevent deadlocks, coders with an even ID are started after a delay (equal to half the compilation time). Since they all share the same compilation time, this gives a chance for every other coder to grab both dongles and start compiling, preventing coders from blocking more than one other coder from accessing dongles.

Another prevention method used is to only try to acquire both dongles at once. In other words, if a coder can only obtain one dongle, they will release it before attempting to acquire them again. This gives a chance to another coder to acquire it instead.

#### Starvation prevention

In Codexion, waiting time is limited by a burnout deadline. To minimize burnouts, `edf` scheduling prioritizes coders closest to burnout when they are both waiting for a dongle's cooldown to end.

#### Cooldown handling

While dongles are on cooldown, they cannot be accessed by coders. To guarantee fair arbitration at the end of the cooldown, each dongle has a queue representing the order in which coders requested access. At the end of the cooldown, the dongle is given to either the first coder to request access (under `fifo`) or the one with the closest burnout deadline (under `edf`).

#### Precise burnout detection

To detect and stop the program as soon as a coder burns out, a separate monitor thread constantly checks the `last_compile` time for each coder. As soon as `time_to_burnout` milliseconds have elapsed since a coder last compiled, the monitor prints that a burnout occurred and stops the simulation. Having a separate thread dedicated to that job allows for precise monitoring of burnouts.

#### Log serialization

In order to prevent two threads from logging their status (compiing, debugging, refactoring...) at the same time and having their messages interlace, a shared mutex is used to protect calls to printf. This assures two threads can never output at the same time.

### Thread synchronization mechanisms

#### pthread_mutex_t

A mutex is a MUTual EXclusion device, and is useful for protecting shared data structures from concurrent modifications, and implementing critical sections and monitors.

A mutex has two possible states: unlocked (not owned by any thread), and locked (owned by one thread). A mutex can never be owned by two different threads simultaneously. A thread attempting to lock a mutex that is already locked by anotherthread is suspended the owning thread unlocks the mutex first.

In Codexion, each dongle is represented with a mutex so that they can only be acquired (locked) by a single thread. This forces implementing synchronization methods to guarantee only one coder can access a dongle.

#### Custom event implementation

Coders all begin at the same time by waiting in a loop until a shared resource (`is_running` variable) is updated. This allows to control when they begin execution. Without using a condition variable (pthread_cond_t).

#### Coordination of access to shared resources

To access dongles, coders add themselves to a queue and then ask the dongle if they are next in line. This allows fair arbitration and exclusive access to dongles.

As for logging, access to a shared mutex is required, preventing two coders from logging their status at the same time.

#### Thread-safe communication

A race condition is a situation where multiple threads are simultaneously trying to access or modify shared data. To prevent this, whenever a shared resource is accessed by a thread, a shared mutex is used to prevent other threads from accessing it at the same time (race condition). Since mutexes can only be acquired by a single thread at a time, this forces other threads to wait for the mutex to be unlocked in order to access shared resources. This effectively synchronizes threads and forces their coordination when accessing shared resources.

This system is used to guarantee that the monitor thread has a real-time view on each of the coders when it checks their values, as coders can not update the observed values during that time (if access is properly protected with mutexes).

## Resources:

- [Project link](https://projects.intra.42.fr/projects/codexion) (requires access to the 42 intra)
- [Mastering Concurrency in C with Pthreads: A Comprehensive Guide](https://dev.to/emanuelgustafzon/mastering-concurrency-in-c-with-pthreads-a-comprehensive-guide-56je)
- [Linux Tutorial: POSIX Threads](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html)
- [pthreads (Wikipedia)](https://en.wikipedia.org/wiki/Pthreads)
- [Jacob Sorber - Programming with Threads (Youtube Playlist)](https://www.youtube.com/playlist?list=PL9IEJIKnBJjFZxuqyJ9JqVYmuFZHr7CFM)
- [Enumeration (or enum) in C](https://www.geeksforgeeks.org/c/enumeration-enum-c/)
- [Posix Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [CIS 307: Threads: Implementing Monitors using locks and condition variables](https://cis.temple.edu/~giorgio/old/cis307s97/readings/pbuffer.html)
- [pthread_mutex_init(3) - Linux manual page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)
- AI was not used an any point during this project.
