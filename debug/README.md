# debug/ - toolkit files

The full method is in **[../DEBUGGING.md](../DEBUGGING.md)** (a field guide to
diagnosing Norcroft 32-bit / RISC OS 5 crashes: data aborts and heap
corruption). This folder holds the tools it refers to.

- **`Die_crash_capture.c`** - startup postmortem capture: redirects the C
  runtime's error output to `<Wimp$ScrapDir>.<app>.Error` and leaves SIGFPE/
  SIGILL/SIGSEGV uncaught so a crash records registers, the aborting address and
  a backtrace. Reference copy from !RDPClient's `c/Die`.
- **`dumpasm.py`** - recompile one source to ARM assembly (`-S`) on
  build.riscos.online, to map a fault offset to the exact instruction. Reuses a
  `buildapp.py`-style driver (`import buildapp`), so run it from an app build kit
  and edit the compile line for your file.
- **`dumpasm_rdp.py`** - the same, pre-targeted at `rdesktop/c/rdp`.
- **`experiments/`** - heap-instrumented sources kept as an example. See the
  heisenbug caveat in DEBUGGING.md section 5: a `malloc/free` probe can *mask*
  heap corruption; prefer markers-only instrumentation.
