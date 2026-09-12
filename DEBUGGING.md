# Debugging guide - DeskLib32 / Norcroft 32-bit on RISC OS 5

A field guide to diagnosing crashes in applications built with **Norcroft cc
5.18** for **32-bit RISC OS 5** (Raspberry Pi and friends), against DeskLib32.
It is written from the real faults hit porting !RDPClient, every one of which
was invisible in the original 26-bit binary and only appeared once the code was
recompiled 32-bit with a modern Norcroft. The companion tools live in `debug/`.

---

## 1. The two crash families

**Data abort - "type 20".**
```
Internal error: abort on data transfer at &0002AE94
  2ae94 in function mcs_connect
  ...  (type 20)
```
Almost always an **unaligned memory access** with the CPU's alignment checking
ON. RISC OS 5 defaults alignment checking ON, including on ARMv7 (Pi 4). Turning
it OFF in `!Config -> CPU` is only a workaround - fix the code instead.

**Heap corruption.**
```
*** Unrecoverable error: in runtime system, malloc failed (heap overwritten)
```
A buffer overran its allocation and trampled heap metadata. These often produce
*no* error-log entry, because the runtime's own error path uses the corrupt
heap.

---

## 2. Make crashes self-reporting (do this first)

Copy the pattern in `debug/Die_crash_capture.c` into your app's death handler.
At startup, before `Event_Poll`:

```c
OS_CLI("CDIR <Wimp$ScrapDir>.YourApp");
freopen("<Wimp$ScrapDir>.YourApp.Error", "w", stderr);
setbuf(stderr, NULL);
```

and in the loop that installs `signal()` handlers, **skip** the hardware-fault
signals so the C runtime prints its own postmortem instead of you swallowing it:

```c
for (sig = 1; sig <= signal_MAX; sig++) {
    if (sig == SIGFPE || sig == SIGILL || sig == SIGSEGV)  /* 2, 3, 5 */
        continue;
    signal(sig, die_message);
}
```

Now a crash writes registers, the **aborting address** and a **function
backtrace** to `<Wimp$ScrapDir>.YourApp.Error`, which the Filer can open on
death. That address is the thread you pull next.

(Revert to normal signal handling for a shipping build once you are done - this
state deliberately lets faults through.)

---

## 3. Address -> function -> instruction

1. **Function** - add `-map` to the linker via-file and relink. The link map
   places the aborting address inside one object (e.g. `&2AE94` -> `rd_mcs` ->
   `rdesktop/c/mcs`).

2. **Instruction** - recompile just that source to assembly with the same flags
   but `-S` instead of `-c`, and read the listing near the offset. `debug/dumpasm.py`
   does this against the build service (edit the compile line for your file).
   You are looking for a load/store that touches a non-word-aligned address.

---

## 4. Norcroft codegen gotchas found in this port (with fixes)

### 4a. Unaligned word load of a 16-bit value  (type 20)
Norcroft sometimes reads a `uint16`/`short` with a **word** `LDR` at a halfword
offset instead of `LDRH`:
```
LDR  a1,[a1,#2] ; MOV a1,a1,LSR #16     ; a 16-bit GLOBAL
LDR  a1,[sp,#2] ; MOV a1,a1,LSR #16     ; a 16-bit STACK STRUCT FIELD
```
`+2`/`sp+2` is not word-aligned, so it aborts with alignment checking on.
**Fix:** widen the offending value to a full word (`uint32`/`int`). It stays
word-aligned and is read with a plain `LDR`. Safe when the value is only ever
16 bits on the wire or an in-memory count. Examples from !RDPClient: the MCS
user id and server RDP version (globals), and `COLOURMAP.ncolours` (a struct
field). The DDE flag that would otherwise make the compiler emit alignment-safe
access, **`-memaccess -L22-S22-L41`**, is **not supported by Norcroft 5.18** on
build.riscos.online - so the fix must be in the source.

Also avoid **`-Otime`**: it encourages folding byte reads into unaligned word
loads. This port compiles without it.

### 4b. Struct layout from `int` bitfields  (wrong offsets -> crashes)
`unsigned int : 1` bitfields are word-aligned by modern Norcroft, so a group of
them can push the struct size up (DeskLib's `wimp_colourflags` went 8 -> 12
bytes) and move every following field. If that struct mirrors an OS block (a Wimp
window/icon block), the offsets no longer match and things like template loading
fail ("not enough memory to copy template"). **Fix:** don't use `int` bitfields
for OS-format structs; use explicit-width fields (`unsigned char`, etc.). This is
exactly the DeskLib32 `Wimp.h` fix.

### 4c. Endianness + alignment in stream/parse macros  (garbage fields)
"Fast path" parse macros that read protocol fields with `*(uint16*)p` /
`*(uint32*)p` are both **unaligned** (rotated garbage on ARM at odd offsets) and,
if a big-endian variant is selected on a little-endian machine, **byte-swapped**.
In !RDPClient this silently corrupted the MCS channel id (blank screen) and the
RDPSND format records (no audio for years - every advertised format was rejected
because `wFormatTag` never read back as `WAVE_FORMAT_PCM`). **Fix:** force
unconditional **byte-by-byte, endian-correct** reads.

### 4d. 16-bit out-parameter not written back  (optimiser)
A `uint16 *` out-parameter (and even a plain `uint16` local) was not reliably
stored back across a call boundary under the optimiser. **Fix:** carry the value
through a file-scope **`volatile int`** and reload it where needed; `volatile`
forces a real memory access and `int` avoids the miscompiled 16-bit path.

---

## 5. Chasing heap corruption

- Get the allocation size right first: a fixed-size buffer written with a
  data-dependent length is the classic cause (in !RDPClient an 8-bit pixel
  translation table overran a fixed 256-byte buffer; the real size is
  `2^(source depth)` bytes).
- **Do not** debug it with a `malloc(N)/free` probe around the suspect code. The
  extra allocation changes the heap layout, the overrun lands somewhere
  harmless, and the bug vanishes while instrumented - a heisenbug. See
  `debug/experiments/` for exactly this trap.
- Prefer **markers-only** instrumentation: write a known sentinel immediately
  before and after a buffer, run, then check the sentinels are intact. This
  observes corruption without allocating or freeing, so it does not move the
  bug.

---

## 6. Quick checklist

- Crash on launch, "not enough memory to copy template" -> struct layout / int
  bitfields (4b).
- "type 20" data abort, cured by turning CPU alignment OFF -> unaligned access;
  find it via section 3 and widen the value (4a).
- Connects/runs but draws nothing, or a protocol feature silently does nothing
  -> parse macro alignment/endianness (4c).
- "heap overwritten", no error log -> heap overrun; size the buffer correctly and
  use markers-only instrumentation (section 5).
- Reproduce with alignment ON (the RISC OS default) - never "fix" by leaving it
  off.

See `debug/README.md` for the toolkit files.
