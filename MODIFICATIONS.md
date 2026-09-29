# DeskLib32 - every change from upstream DeskLib, and why

This document lists **every** difference between the source in `src/` and
upstream DeskLib. For each change it gives the problem, the evidence, what was
changed, why it was done that way, what it means for programs using the
library, and how it was checked. It is meant to let someone who was not
involved follow, and challenge, each decision.

The exact source changes are also in `patches/desklib32-vs-upstream.diff`
(unified diff: 8 modified files and 1 added file).

---

## 1. What the source is based on

| Part of `src/` | Taken from | Version |
|---|---|---|
| `include/`, `oldinclude/` | upstream `!DeskLib/include`, `!DeskLib/oldinclude` | master `f7469f4` (2026-04-04) |
| `Libraries/**/*.c`, C-only `*.h` | upstream `!DLSources/Libraries` | master `f7469f4` |
| `Libraries/**/*.s`, `Macros.h`, `RegDefs.h`, `*/SwiNos.h` | upstream `!DLSources/Libraries` | **`aof` branch** `7cea4a6` (2008-01-16) - see section 3 |

Upstream repository: https://github.com/riscos-dot-info/desklib

### File inventory (638 files)

| Status | Files |
|---|---|
| Byte-identical to upstream master `f7469f4` | 348 |
| Byte-identical to upstream `aof` branch `7cea4a6` (assembler sources and their include files) | 281 |
| Modified (listed individually below) | 8 |
| Added (C1, `Libraries/Compat/Printf.c`) | 1 |
| Removed relative to upstream | 0 |

The 8 modified files and the 1 added file are:

| File | Compared against | Change |
|---|---|---|
| `include/Wimp.h` | master | M1 `wimp_colourflags` layout, M2 `wimp_point` semicolon |
| `include/Environment.h` | master | M5 trailing comma in `enum` |
| `Libraries/Template/Clone.c` | master | M3 compile-time layout guard |
| `Libraries/Debug/DebugDefs.h` | master | M4 stray `static` prototype removed |
| `Libraries/Environment/OSCLI.s` | aof | A1 export name |
| `Libraries/Tinct/PlotScaled.s` | aof | A2 register fix |
| `Libraries/Tinct/PlotScaledAlpha.s` | aof | A2 register fix, export name and SWI |
| `Libraries/BackTrace/GetPC2.s` | aof | A3 extra export name |
| `Libraries/Compat/Printf.c` | **new file** | C1 bounded `snprintf`/`vsnprintf` built from C89 calls; DeskLib's own calls are redirected to it by build flags, so programs avoid the "SWI &5DC34 not known" start-up failure |

The modified files carry no added copyright notices; the original authors'
copyright lines are untouched. The one new file, `Libraries/Compat/Printf.c`,
names its author in the usual DeskLib header. DeskLib's licence is in `LICENCE`.

**To reproduce the inventory:** clone upstream, then compare each file in `src/`
with the upstream file at the same path. For `include/` and `oldinclude/`, use
`git show f7469f4:'!DeskLib/<path>'`. For `Libraries/`, compare against
`git show f7469f4:'!DLSources/Libraries/<path>'`, and for the files that differ,
against `git show 7cea4a6:'!DLSources/Libraries/<path>'`.

---

## 2. Changes to C source and headers

### M1. `include/Wimp.h` - `wimp_colourflags` layout under Norcroft

**Problem.** The Wimp's window block contains an 8-byte colour/flags block:
seven colour bytes, then a byte of extra window flags (24-bit colour, extended
scroll requests, 3D border control, return shaded icons). Upstream declares it
like this:

```c
unsigned char titlefore, titleback, workfore, workback,
              scrollouter, scrollinner, titlefocus;   /* bytes 0-6 */
unsigned int  fullcolour:1, extendedscroll:1, never3d:1,
              always3d:1, returnshaded:1, padding:3;  /* meant to be byte 7 */
```

GCC puts those `unsigned int` bit-fields into byte 7, so the struct is 8 bytes.
Norcroft 5.x won't share a bit-field container with the plain `char` members
in front of it. It starts a new 4-byte word at offset 8 instead, so the struct
becomes **12 bytes**. Every later field of `window_block` then moves by 4 bytes
(`workarearect`, `titleflags`, ... `numicons` and the icon blocks).

**Evidence.** Under Norcroft, loading templates failed at start-up with "not
enough memory to copy template". `Template_Clone` was reading `numicons` 4
bytes too far along and got a nonsense icon count. With the struct back at 8
bytes, templates load correctly.

**Change.**
- **Under GCC** (`#if defined(__GNUC__)`) the declaration is upstream's,
  unchanged.
- **Under every other compiler** (`#else`), bytes 4-7 are declared as one
  `unsigned int` bit-field container: `scrollouter:8`, `scrollinner:8`,
  `titlefocus:8`, the five flags, then `padding:3`. The container starts at
  offset 4, which is word-aligned. The three colours fill bytes 4, 5 and 6, and
  the flags fill bits 0-4 of byte 7.
- The comments on `never3d` and `always3d` were swapped upstream. They are
  corrected in both branches.

**Why this way.** Two approaches were tried:

1. *Declare the flags as `unsigned char` bit-fields.* This is the neatest fix:
   the names stay the same and the struct is 8 bytes under GCC. Norcroft 5.18
   rejects it, though: `Error: ANSI C forbids bit field type 'char'`
   (Wimp.h, all six flag lines; build of 2026-09-29). ANSI C only allows `int`
   bit-fields.
2. *One `unsigned int` container for bytes 4-7* (**chosen**). It is valid ANSI
   C, keeps every member name and gives 8 bytes on both compilers. It is only
   applied to non-GCC compilers, so GCC users see exactly upstream's
   declaration and nothing changes for them.

**Effect on programs using the library.**
- Source that reads or writes any member by name compiles and behaves the same
  as before.
- Under Norcroft only, `scrollouter`, `scrollinner` and `titlefocus` are now
  8-bit bit-fields, so a program cannot take their address
  (`&w->colours.cols.titlefocus`). Nothing in DeskLib does this.
- The `vals` view (`vals.colours[7]`, `vals.extra`) is unchanged.

**Verification.**
- With GCC in 32-bit mode, both branches were compiled (the non-GCC one by
  undefining `__GNUC__`). Each gives `sizeof(wimp_colourflags) == 8` and
  `sizeof(window_block) == 88`.
- Setting `scrollouter`=0x11, `scrollinner`=0x22 and `titlefocus`=0x33 gives
  bytes 4-6 = `11 22 33`.
- Setting `fullcolour`, `never3d` and `returnshaded` gives `vals.extra` = `0x15`
  (bits 0, 2 and 4).
- Under Norcroft, the size is checked at build time by M3.
- **Not yet checked on Norcroft: bit order within the container.** The ARM
  procedure-call standard allocates bit-fields from the least significant bit
  on little-endian targets, which gives the layout above. This still has to be
  confirmed on RISC OS, with a window that sets one of the flags.

**Upstream status.** Not yet reported.

### M2. `include/Wimp.h` - missing semicolon in `wimp_point`

**Problem.** Upstream commit `a70977a` ("Modify the headers to work with
haddoc") split `int x, y;` into two lines. The first line lost its semicolon:

```c
typedef struct { int x
                 int y; } wimp_point;
```

The commit was written in March 2022 and merged on 2026-04-03.

**Evidence.** Any file that includes `Wimp.h` fails to compile. That covers
most of DeskLib and most programs that use it.

**Change.** `int x` → `int x;`

**Effect.** None beyond making the header compile.

**Upstream status.** Not yet reported.

### M3. `Libraries/Template/Clone.c` - compile-time layout guard

**Problem.** M1 went unnoticed because nothing checks the struct sizes. A wrong
layout compiles cleanly and only fails at run time, in a way that is hard to
diagnose.

**Change.** Two `typedef`s were added after the includes:

```c
typedef char desklib_check_colourflags[(sizeof(wimp_colourflags) == 8) ? 1 : -1];
typedef char desklib_check_windowblock[(sizeof(window_block) == 88) ? 1 : -1];
```

If either size is wrong, the array size becomes -1 and the build stops with an
error in this file.

**Why here.** `Template_Clone` is the function that copies window blocks, so it
is where a wrong layout does its damage. The check produces no code or data.
88 is the size of the window block header defined by the Wimp.

**Effect.** None at run time. On a 64-bit host `window_block` contains pointers
and is larger, so this check is only meaningful for 32-bit RISC OS targets,
which is all DeskLib builds for.

### M4. `Libraries/Debug/DebugDefs.h` - `static` prototype in a shared header

**Problem.** `DebugDefs.h` is included by five files in `Libraries/Debug/`, and
it declares

```c
static FILE *Debug__OpenPipeFile(void);
```

A `static` declaration in a shared header gives every one of those files its
own private function, but only `UniquePipe.c` defines it.

**Evidence.** Norcroft 5.18 treats this as an error in `Debug.c`: `Error: static
function 'Debug__OpenPipeFile' not defined - treated as extern` (line 146).
GCC only warns ("declared 'static' but never defined"). That is presumably why
it went unnoticed upstream (it arrived in commit `9aa54fc`, 2008-01-31).

**Change.** The prototype line is removed from the header. `UniquePipe.c`
defines the function before it is first used, so it needs no prototype.

**Effect.** None. The function was private to `UniquePipe.c` before and still
is.

### M5. `include/Environment.h` - trailing comma in `enum sysvar_type`

**Problem.** The last value in the `sysvar_type` enum has a comma after it
(`sysvar_LITERAL,`). C99 allows that, but ANSI C89 does not, and Norcroft
compiles C89. The comma has been there since commit `9aa54fc` (2008-01-31).

**Evidence.** `"DeskLib:h.Environment", line 184: Error: Superfluous ',' in
'enum' declaration` when compiling `Libraries.Debug.c.Reporter`. It would stop
every file, and every program, that includes `Environment.h`.

**Change.** The comma is removed: `sysvar_LITERAL,` → `sysvar_LITERAL`.

**Effect.** None; the enum values are unchanged.

**Checks for similar problems.** After M4 and M5, all 270 library C files were
compiled with `gcc -std=c89 -Wpedantic`, using stand-in RISC OS `kernel.h`
declarations. The headers were also compiled one at a time. Nothing else in
the headers the library uses breaks C89. The remaining header problems are all
in headers the library does not include (see section 5).

### C1. `snprintf` / `vsnprintf` redirected to `Libraries/Compat/Printf.c` (new file + build flags)

**Problem.** Upstream DeskLib calls the C99 functions `snprintf` and
`vsnprintf` in 23 source files. These include `Error_Report`, `Icon_printf`,
`Msgs_Report`, the `Resource` and `Template` loaders, `File_printf`, and the
`Debug` and `Environment` modules. With the Norcroft 5.18 toolchain on
build.riscos.online, those two functions are linked through a separate
SharedCLibrary stub chunk (chunk 5). On the Raspberry Pi used for testing,
that chunk is not initialised at start-up. A program that links any DeskLib
member calling them fails to start.

**The underlying cause is not established.** It may be something odd about
Norcroft 5.18 or its stubs, or a limit of that ROM's shared C library.
Avoiding the two functions works in either case, so C1 does not depend on
which it is.

The DeskLib 2.80 that earlier DeskLib32 versions were based on used
`vsprintf`/`sprintf` only. That is why the problem first appeared with the
rebase.

**Evidence.** The first RDPClient built against the rebased library stopped at
launch with **`SWI &5DC34 not known`**. That is the same failure RDPClient had
on 2026-09-18, when its own code called `snprintf`; see RDPClient's crash
notes. A scan of the library found imports of `snprintf` and `vsnprintf`. The
2.80-based library had none. No other C99-only library function is imported.

**Change.**
- A new file, `Libraries/Compat/Printf.c`, provides `DeskLib__snprintf` and
  `DeskLib__vsnprintf`, built only from C89 library calls.
- Every other DeskLib C file is compiled with
  `-Dsnprintf=DeskLib__snprintf -Dvsnprintf=DeskLib__vsnprintf`, so upstream's
  calls reach those functions. The upstream sources themselves are not edited.
- `Printf.c` is compiled without those flags.
- `build/builddesklib.py` now refuses a library that still imports
  `snprintf`/`vsnprintf`. It renames the output to `DeskLib32.UNSAFE` and
  fails.
- RDPClient's `buildapp.py` has a chunk-5 guard, which now scans the DeskLib
  library as well as RDPClient's own objects.

**How the replacement stays bounded.** The format string is handled one
conversion at a time:
- Plain text, `%s` and `%c` are copied by hand, with width and precision, so
  their length never matters.
- Each numeric conversion (`%d %i %o %u %x %X %e %E %f %g %G %p`) is rendered
  by C89 `sprintf` into a scratch buffer sized from its own width and
  precision. Up to 400 extra characters are allowed for floating point, so it
  cannot overflow. The result is then copied into the caller's buffer with
  truncation. `*` widths and precisions, the `h`, `l` and `L` modifiers and
  `%n` are supported.
- The behaviour follows C99:
  - at most `size-1` characters are stored;
  - the result is always terminated when `size > 0`;
  - the return value is the length the complete output would have had;
  - `(NULL, 0)` measures the output.
  - It returns a negative value only if a large scratch buffer cannot be
    allocated.

**Why this way.**
- *Replacing each call with `vsprintf` into a fixed buffer*, as RDPClient's
  own `kh_snprintf` does, would mean editing 23 upstream files. It would also
  bring back the overflow risk upstream removed in 2007 when it moved to
  `vsnprintf`.
- *Requiring a newer SharedCLibrary* would break DeskLib programs on machines
  that are otherwise supported.
- *Defining functions called `snprintf` inside DeskLib* does not work. The
  linker takes the C library stubs' definition first, so chunk 5 is still
  pulled in.

**Effect on programs using the library.**
- DeskLib itself no longer needs stub chunk 5.
- Programs that call `snprintf` themselves are not affected by this change.
  With this toolchain they still hit the same start-up failure unless they
  avoid those functions or redirect them the same way.
- Output from DeskLib's formatting functions is unchanged.

**Verification.**
- `Printf.c` compiles cleanly with `gcc -std=c89 -pedantic -Wall -Wextra`.
- 198 test cases were compared byte for byte, and on return value, with the C
  library's `snprintf`, at buffer sizes 0, 1, 2, 5, 8, 16, 64, 300 and 1000.
  They cover every conversion, the flags, `*` width and precision, `%n`, long
  strings, very wide fields, `%.400f`, and the format strings DeskLib itself
  uses. All matched except a format ending in a lone `%`, which is undefined
  behaviour in C.
- The builddesklib guard catches the imports in the previous library and
  ignores the new `DeskLib__` names.
- **Confirmed on RISC OS (2026-09-29):** the rebuilt library passed the
  guard. RDPClient (the 0.93.3 code) linked against it passed its own chunk-5 guard, and
  it starts and runs on the Pi. Before C1, RDPClient failed at start-up there
  with "SWI &5DC34 not known".

---

## 3. Assembler sources

### Why the `aof` branch is used for all `.s` files

Upstream master converted its assembler veneers to GNU `as` syntax in commit
`ae6bc90` (2007-01-01), when the library moved to ELF format. That syntax uses `.global`,
`.include` and labels with colons. Norcroft's assembler, `objasm`, cannot read
it. The first build of the rebased tree stopped at the first assembler file:

```
A Label was found which was in no AREA at line 3 in file Libraries.BackTrace.s.GetPC
```

Upstream's own Makefile still has a `COMPILER=norcroft` setting, but on master
it cannot build for this reason.

Upstream's `aof` branch keeps the objasm-syntax sources (`EXPORT`, `AREA`, `GET`,
the `PREAMBLE`/`STARTCODE` macros from `Macros.h`). It has an equivalent of all
259 master `.s` files. It also has one extra file, `Dispatch/Dispatch.s`, which
master does not have; it is not used.

DeskLib32 takes all 259 `.s` files from `aof`, together with the files they
include (`Macros.h`, `RegDefs.h` and the 24 `SwiNos.h` files, 285 files in
all). None of these include files are used by the C code.

**Checks made before switching:**
- After stripping comments and syntax, each `aof` file carries out the same
  instruction sequence as its master counterpart. The only exceptions are the
  two Tinct veneers master fixed later (A2) and the export-name differences
  covered in A1, A3 and the "Kept as in `aof`" table.
- Every SWI number in the `aof` `SwiNos.h` files matches master.
- The set of exported symbols is the same as master's, except for the fixes in
  A1-A3 and the two master naming bugs described in "Kept as in `aof`" below.
  No symbol is exported twice.
- There are no 26-bit-only instructions (`MOVS pc`, `LDM ... ^`, `TEQP`) except
  in comments.

### A1. `Environment/OSCLI.s` - export name (ported from master)

`aof` exports the OS_CLI veneer as `Environment_Command`. Upstream later made
`Environment_Command` a printf-style C function (`Environment/Command.c`) that
calls the veneer. `Environment.h` declares the veneer as
`Environment__OS_CLI` (commits `1f9a0b0` and `181790a`, February 2008). With the
`aof` name, the library would contain two `Environment_Command` symbols and no
`Environment__OS_CLI` at all.

**Change:** `STARTCODE Environment_Command` → `STARTCODE Environment__OS_CLI`,
which matches master.

### A2. `Tinct/PlotScaled.s`, `Tinct/PlotScaledAlpha.s` - registers, name and SWI (ported from master)

The C prototype is
`Tinct_PlotScaled(sprite, x, y, width, height, flags)`. APCS passes the first
four arguments in r0-r3 and the rest on the stack. The Tinct SWIs expect
R2 = sprite, R3 = x, R4 = y, R5 = width, R6 = height, R7 = flags.

The `aof` veneers put `width` (r3) into R7 and load `height, flags` from the
stack into R5, R6. That is the wrong order. `PlotScaledAlpha.s` also exports
the name `Tinct_PlotAlpha`, which duplicates `PlotAlpha.s`, and calls the
unscaled `SWI_Tinct_PlotAlpha`.

Master fixed all of this (commits `d501ec9`, 2008, and `c258d1d`, 2009,
"Fix Tinct veneers").

**Change:** in both files, `MOV r7, r3` → `MOV r5, r3` and
`LDMIA ip, {r5, r6}` → `LDMIA ip, {r6, r7}`. In `PlotScaledAlpha.s`, the export
also becomes `Tinct_PlotScaledAlpha` and the SWI becomes
`SWI_Tinct_PlotScaledAlpha`. The result is instruction-for-instruction the
same as master.

### A3. `BackTrace/GetPC2.s` - extra export name

`BackTrace.h` declares `BackTrace_GetPC2`, but both `aof` and master export
only `Desk_BackTrace_GetPC2`. That name is a leftover from the `Desk_`-prefixed
DeskLib 3 line. A program calling `BackTrace_GetPC2()` fails to link.

**Change:** a second label, `BackTrace_GetPC2`, is exported at the same
address. The old name is kept, so anything that used it still links.

### Kept as in `aof` (master is wrong here)

| File | `aof` / DeskLib32 export | master export | Header declares |
|---|---|---|---|
| `Environment/GSTrans.s` | `Environment_ExpandString` | `OS_GSTrans` | `Environment_ExpandString` (with `OS_GSTrans` a macro for it) |
| `Font/Font09.s` | `Font_ConvertToPoints` | `Font_ConvertTopoints` | `Font_ConvertToPoints` |

With master's names, a program calling these functions fails to link.

---

## 4. Build configuration (not source changes)

The build runs on build.riscos.online with Norcroft `cc` 5.18 and `objasm` 3.32.
The command for each object is listed in `build/dlplan.json`: 530 objects,
271 C and 259 assembler.

| Flag | Where it comes from | Why |
|---|---|---|
| `-ffahi -throwback -IC:` (cc) | upstream `!DLSources/Makefile`, `COMPILER=norcroft` | unchanged upstream settings |
| `-ILibraries -I<dir> -throwback` (objasm) | upstream Makefile | unchanged |
| `-apcs 3/32bit` (cc and objasm) | added | builds for the 32-bit procedure-call standard used by RISC OS 5 |
| `-Dsnprintf=DeskLib__snprintf -Dvsnprintf=DeskLib__vsnprintf` (cc, every file except `Compat/Printf.c`) | added 2026-09-29 | keeps the library off SharedCLibrary stub chunk 5; see C1 |
| `-za1` (cc only) | added 2026-09-14 | stops the compiler from using unaligned word loads/stores, so code stays safe with the CPU's alignment checking on (the RISC OS 5 / ARMv7 default); this is the equivalent of `-memaccess -L22-S22-L41`, which this compiler version rejects |

The build sets `DeskLib$Path` to `include` then `oldinclude`, matching the
search path upstream's `!Boot` file sets up. `-za1` changes only the code the compiler generates; it does not
change struct layout, so it does not replace M1.

---

## 5. Upstream problems seen but deliberately not changed

Each of these is left alone because it does not stop the library building, or
because fixing it would change behaviour. They are recorded here so they can be
reported upstream.

| Where | Problem | Why left |
|---|---|---|
| `include/PCI.h` | `pci_address` has two members named `flags`; line 166 has `exetern` for `extern` | nothing in DeskLib includes it; it only fails in programs that do |
| `include/PCI.h` | `PCI_ConfigurationRead` is declared twice with different parameter types | as above |
| `include/USB.h` | `devicefs_request` is both a typedef and a function name | as above |
| `include/Tinct.h` | `tinct_compressed` ends in `unsigned char data[0]`; C89 has no zero-length arrays, so Norcroft needs `-Ez` for programs that include it | only affects programs that use Tinct from C; changing it would change the struct's API |
| `Debug/UniquePipe.c` line 55 | `Error_ReportFatal(1, "... '%s'")` has no argument for `%s` | error path only; left as upstream |
| `Debug/Debug.c` | Norcroft warns that `debug__outputtype` and `debug__filename` are "extern not declared in header" | warnings only |
| `Sprite/43ReadMask[P].s`, `Sprite/44WriteMas[P].s`, `Font/Font10.s` | `Sprite_ReadMaskPixel[P]`, `Sprite_WriteMaskPixel[P]` and `Font_SetScaleFactor` are exported but not declared in any header | harmless; same in master |
| upstream `Makefile` | `COMPILER=norcroft` cannot build master (GNU `as` sources) | DeskLib32 has its own build kit |

---

## 6. Earlier DeskLib32 versions (withdrawn)

Until 2026-09-29, DeskLib32 was based on **DeskLib 2.80** (2007), the copy that
came with the original RDPClient source. That version had problems which a
forum reviewer rightly pointed out:

- a `Wimp.h` layout fix that removed the named colour flags, which broke the
  API;
- headers 19 years behind upstream (for example no Ursula `numeric` icon flag);
- debugging text left in `Template/Clone.c` in place of the library's normal
  "not enough memory" errors;
- character-encoding damage to the copyright line in the two files edited.

All of these went away with the rebase. None of that code is in the current
tree. The last 2.80-based source is kept outside the repository for reference.

---

## 7. Still to be done

- ~~A full clean Norcroft build with C1.~~ **Done 2026-09-29:** all 530
  objects compiled, `libfile` rc=0, and the chunk-5 guard and the M3 size check
  passed. The library is 323,048 bytes and is the repository-root `DeskLib32`.
  The earlier 529-object build, which imported `snprintf`/`vsnprintf`, is
  withdrawn.
- ~~Start-up test on the Pi.~~ **Done:** RDPClient (the 0.93.3 code) linked against this
  library builds, starts and runs on the Pi, and its windows (templates and
  `KnownHosts`) open normally. That confirms the M1 struct size and template
  loading at run time.
- Still open: a run-time check of the M1 flag **bit order**, using a window
  that sets one of the extra flags (RDPClient only clears the byte).
- Reporting M1, M2, M4, M5, A1-A3, the master export bugs and section 5 upstream,
  and pointing out upstream's dependence on stub chunk 5 (C1).
