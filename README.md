*This project has been created as part of the 42 curriculum by dmonseur.*

# Codexion

## Description

Codexion is a multithreaded simulation of resource contention.
A group of coders sit in a circular co-working hub sharing
USB dongles: compiling quantum code requires a coder to hold two dongles at once (the one
on their left and the one on their right). Coders cycle endlessly through three states —
**compiling**, **debugging**, and **refactoring** — and must regularly return to compiling
before running out of time, or they **burn out**.

The goal of the project is to model this cycle correctly using threads and mutexes:
each coder is a thread, each dongle is a shared resource guarded by mutexes,
and a dedicated monitor thread watches every coder for burnout in near
real time. The simulation stops as soon as a coder burns out, or once every coder has
completed the required number of compiles.

Two arbitration policies decide who gets a contested dongle:
- **fifo** — dongles are granted strictly in request order.
- **edf** — dongles are granted to whichever coder has the earliest burnout deadline
  (`last_compile_start + time_to_burnout`), with request order used as a deterministic
  tie-breaker when two deadlines coincide.

## Instructions

### Compilation

```bash
make
```

This builds the `codexion` binary at the root of the project using
`-Wall -Wextra -Werror -pthread`.

Other targets:
```bash
make clean   # remove object files
make fclean  # remove object files and the binary
make re      # rebuild from scratch
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
|---|---|
| `number_of_coders` | Number of coders (and dongles) sitting at the table |
| `time_to_burnout` | Time in ms a coder can go without starting a compile before burning out |
| `time_to_compile` | Time in ms a compile takes |
| `time_to_debug` | Time in ms spent debugging after a compile |
| `time_to_refactor` | Time in ms spent refactoring before trying to compile again |
| `number_of_compiles_required` | Simulation stops once every coder has reached this many compiles |
| `dongle_cooldown` | Time in ms a dongle stays unavailable after being released |
| `scheduler` | Arbitration policy for contested dongles: `fifo` or `edf` |

All arguments are mandatory and must be non-negative integers (except `scheduler`).
Invalid input (negative numbers, non-numeric arguments, an unknown scheduler, or the
wrong argument count) is rejected with an explanatory error message.

Example:
```bash
./codexion 5 800 200 200 200 4 50 edf
```

### Example output

Each line is `timestamp_in_ms coder_id message`; the number in parentheses is the dongle taken.

```
0 1 has taken a dongle (5)
0 1 has taken a dongle (1)
0 1 is compiling
0 3 has taken a dongle (2)
0 3 has taken a dongle (3)
0 3 is compiling
200 3 is debugging
201 1 is debugging
...
```

### Known limits

With an odd number of coders, fewer coders can compile at the same time (at most
`(n - 1) / 2`), so `time_to_burnout` needs more margin. For example, 9 coders with a
200 ms compile need about 450 ms between two compiles at best, so `9 500 200 100 100`
cannot be sustained, while `200 500 200 100 100` can.

## Resources

- POSIX Threads Programming (LLNL) — reference for `pthread_create`, `pthread_mutex_t`,
  and `pthread_cond_t` usage.
- `man pthread_cond_timedwait`, `man gettimeofday` — timing and conditional waiting.
- Earliest Deadline First (EDF) scheduling — real-time scheduling theory behind the
  `edf` arbitration policy.
- Coffman's conditions for deadlock — used as a checklist while designing the dongle
  acquisition logic (see below).

**AI usage:** AI assistance was used to help understand the thread and mutex theory
and of course to help draft this README.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** a coder always requests both of its
  dongles together while holding the shared `dongles_mutex`, and never holds one dongle
  while blocking indefinitely for the other — it releases the mutex and retries via
  `pthread_cond_timedwait` instead, which removes the "hold and wait" condition that
  classically causes deadlock in the dining philosophers problem.
- **Starvation prevention:** each dongle keeps a queue of pending requests, and a coder
  may only take its two dongles when it is at the head of the queue on both of them,
  according to the chosen scheduler (arrival order for `fifo`, nearest burnout deadline
  for `edf`). A coder is never overtaken by a later request, even if its neighbours are
  not yet ready. Ties are broken by request ticket. All first requests are registered
  before any thread starts, with odd-numbered coders ahead of even-numbered ones, so the
  start order does not depend on thread scheduling and the table splits into two
  alternating groups that can compile in parallel.
- **Synchronized start:** all threads are created first and released together. The
  simulation clock and every coder's burnout timer start at that moment, so thread
  creation time never counts against a coder.
- **Cooldown handling:** a released dongle records a `ready_at` timestamp
  (`release time + dongle_cooldown`) and is not considered available again until that
  time has passed, enforced before any coder is allowed to pick it back up.
- **Precise burnout detection:** a dedicated monitor thread continuously polls every
  coder's last compile timestamp against `time_to_burnout` at a fine-grained interval,
  so a burnout is logged within the required precision of the actual event.
- **Log serialization:** every state-change message is printed while holding a single
  `print_mutex`, so two log lines can never interleave on the terminal.

## Thread synchronization mechanisms

- **`pthread_mutex_t dongles_mutex`** protects all shared dongle state (availability,
  cooldown timestamps, request queues) and the simulation's shared `stop` flag. Any
  coder thread wanting to check or change a dongle's state must hold this mutex first,
  which is what prevents two coder threads from ever believing they both hold the same
  dongle at once.
- **`pthread_cond_t dongles_ready`**, used together with `dongles_mutex`, lets a coder
  thread sleep instead of busy-waiting while its two dongles are unavailable or it is
  not yet its turn under the active scheduler. Whenever a dongle is released, the
  releasing thread calls `pthread_cond_broadcast` so every waiting coder re-checks the
  conditions instead of missing a wake-up. `pthread_cond_timedwait` is used (rather than
  a plain wait) so a sleeping coder still re-evaluates its burnout deadline periodically.
- **`pthread_mutex_t print_mutex`** is a separate lock dedicated purely to output,
  keeping logging decoupled from the dongle arbitration logic while still guaranteeing
  that no two state-change messages are printed concurrently.
- **Monitor thread:** a thread independent from the coders repeatedly checks each
  coder's last known compile time against the burnout limit. Because it only reads
  timestamps and does not contend for dongles, it can run at high frequency without
  interfering with coder throughput, giving fast, low-latency burnout detection.

Together, these primitives ensure that dongle state changes, print output, and burnout
detection are each protected by the appropriate lock, and that coder threads communicate
indirectly — through shared, mutex-guarded state and condition variable signals — rather
than directly with one another, in line with the subject's "coders do not communicate"
constraint.
