# DeskLib32 - Changelog

A 32-bit build of DeskLib (the FreeWare RISC OS C library) compiled with
Norcroft cc 5.18 on build.riscos.online, with the fixes Norcroft needs and a
debugging toolkit. Since 2026-09-29 it is based on upstream DeskLib master
`f7469f4` (2026-04-04); before that it was based on DeskLib 2.80. See
MODIFICATIONS.md for every change. Dates are ISO (YYYY-MM-DD).

---

## 2026-09-29

### Fixed (after the first build)
- **"SWI &5DC34 not known" at start-up in programs linking DeskLib32.**
  Upstream DeskLib calls the C99 `snprintf`/`vsnprintf` (23 files). These are
  are linked through SharedCLibrary stub chunk 5 by the Norcroft 5.18
  toolchain, and on the test Pi that chunk is not initialised at start-up.
  Whether that is a quirk of the 5.18 toolchain or a ROM limit is not
  established; avoiding the functions works either way. A new `Libraries/Compat/Printf.c` provides a bounded
  replacement built from C89 calls. Every other C file is compiled with
  `-Dsnprintf=DeskLib__snprintf -Dvsnprintf=DeskLib__vsnprintf`, and
  `builddesklib.py` now refuses a library that still imports either function.
  The first build (529 objects) is withdrawn. The build plan is now 530
  objects (271 C + 259 assembler). See MODIFICATIONS.md, C1.

Built clean: 530 objects plus `libfile`, and the chunk-5 guard passed. The
repository-root `DeskLib32` (323,048 bytes) is this build. RDPClient 0.93.3
built against it starts and runs on a Raspberry Pi.

Every change from upstream, with its reasoning and verification, is now
documented in `MODIFICATIONS.md`, and the exact edits are in
`patches/desklib32-vs-upstream.diff`.

### Changed
- **Rebased on current upstream DeskLib** (https://github.com/riscos-dot-info/desklib,
  master `f7469f4`, 2026-04-04) instead of DeskLib 2.80 (2007). This brings in 19
  years of upstream work, including the Ursula `numeric` icon flag,
  `Pointer_Get/SetPosition`, `Icon_FindValidationStringCommand`,
  `Error_CheckSilent`, `Resource_InitialiseAuto`, the `Environment` module, and
  fixes to `EventMsg_DispatchMessage`, buffer termination, Tinct veneers and
  `Menu_FullDispose`. Five older headers (`ColourMenu.h`, `Sound.h`,
  `StringCR.h`, `Validation.h`, `WAssert.h`) are now in `src/oldinclude/`, which
  the build adds to `DeskLib$Path` as upstream's `!Boot` does. The build plan
  covered 529 objects (270 C + 259 assembler); 530 with the C1 fix.
- **Assembler sources come from upstream's `aof` branch.** Upstream master
  converted its `.s` files to GNU `as` syntax in 2007, which `objasm` cannot
  assemble. The `aof` branch keeps objasm-syntax equivalents of all 259 files
  (plus `Macros.h`, `RegDefs.h` and the 24 `SwiNos.h` constant files); these are
  used, with the later master fixes ported (see below). The exported symbol set
  matches master's except where master's is wrong.

### Fixed
- **`wimp_colourflags` layout, without changing the API.** GCC sees upstream's
  declaration unchanged. For other compilers, bytes 4-7 (`scrollouter`,
  `scrollinner`, `titlefocus` and the flags) are one `unsigned int` bit-field
  container. Norcroft rejects `unsigned char` bit-fields as non-ANSI, so they
  could not be used. All member names are as upstream, the struct is 8 bytes
  under both compilers, and the flags sit in bits 0-4 of the 8th byte. The swapped
  `never3d`/`always3d` comments are corrected.
- **`wimp_point` missing semicolon** (upstream, April 2026 header reformat):
  `int x` restored to `int x;`, without which nothing including `Wimp.h`
  compiles.
- **Assembler veneers ported from master to the `aof` sources:**
  `Environment/OSCLI.s` exports `Environment__OS_CLI` (the name `Environment.h`
  declares; `Environment_Command` is the C function in `Command.c`);
  `Tinct/PlotScaled.s` and `Tinct/PlotScaledAlpha.s` pass the width/height in the
  right registers, and `PlotScaledAlpha` exports and calls
  `Tinct_PlotScaledAlpha` rather than duplicating `Tinct_PlotAlpha`.
- **Assembler exports that do not match the headers (upstream master):**
  `Environment/GSTrans.s` keeps `Environment_ExpandString` (master exports
  `OS_GSTrans`, which the header defines as a macro) and `Font/Font09.s` keeps
  `Font_ConvertToPoints` (master has `Font_ConvertTopoints`).
  `BackTrace/GetPC2.s` now also exports `BackTrace_GetPC2`, the name
  `BackTrace.h` declares, alongside the old `Desk_BackTrace_GetPC2`.
- **`Debug/DebugDefs.h`: `static` prototype in a shared header** (upstream
  2008). `static FILE *Debug__OpenPipeFile(void);` was declared in a header
  included by five files but defined only in `UniquePipe.c`. Norcroft rejects
  this ("static function not defined"). The prototype is removed; the
  function is defined before use.
- **`Environment.h`: trailing comma in `enum sysvar_type`** (upstream 2008).
  C89 does not allow it, and Norcroft stops with "Superfluous ',' in 'enum'
  declaration". Removed.
- Added a compile-time layout guard in `Template/Clone.c`
  (`sizeof(wimp_colourflags) == 8`, `sizeof(window_block) == 88`).

### Removed
- Debugging instrumentation that had been left in `Template/Clone.c` in the
  2.80-based build (diagnostic text in place of the library's normal
  "not enough memory" errors), and the character-encoding damage to the
  copyright line in the two files edited there. Both came from the 2.80-based
  tree and are gone with the rebase.

---

## 2026-09-14

### Changed
- **Whole-program alignment-safe build (`-za1`).** All 256 C compiles in
  `build/dlplan.json` now pass `-za1`, disabling the compiler's use of unaligned
  word loads/stores (it emits `LDRB`/`LDRH` sequences instead). This is the
  accepted equivalent of `-memaccess -L22-S22-L41`, which the online Norcroft
  5.18 rejects, and keeps the library safe to run with CPU alignment checking ON
  (the RISC OS / ARMv7 default). The `objasm` (assembler) compiles are unchanged
  (`-za` is a C-compiler option). Rebuilt clean (516 objects, cc rc=0, libfile
  rc=0) and the committed `DeskLib32` regenerated. Separate from the `Wimp.h`
  struct-layout fix below (a different bug; still required).

## 2026-09-12

### Added
- **Reproducible 32-bit build kit** (`build/`): `builddesklib.py` (chunked build
  driver for build.riscos.online), `dlplan.json` (per-object compile lines +
  libfile step), `desklib_src.zip` (the build input) and `package.py`
  (regenerates the zip from `src/`). Builds all ~516 objects with
  `-apcs 3/32bit` and produces `DeskLib32`.
- **Prebuilt `DeskLib32`** library committed for drop-in use.
- **`debug/` toolkit** - the RISC OS 5 / Norcroft debugging aids used to find the
  32-bit port's data-abort faults: `Die_crash_capture.c` (postmortem capture to
  file), `dumpasm.py` / `dumpasm_rdp.py` (per-file `-S` assembly dumps to map a
  fault address to the exact instruction), and `experiments/` (heap-instrumented
  source used to hunt an 8-bit heap overrun). See `debug/README.md`.
- **`DEBUGGING.md`** - field guide: the two crash families, the address ->
  function -> instruction workflow, and a catalogue of Norcroft codegen gotchas
  (unaligned 16-bit reads, int-bitfield struct layout, stream-macro alignment
  and endianness, 16-bit write-back) with fixes.

### Fixed
- **`wimp_colourflags` struct layout** (`src/include/Wimp.h`). The
  `unsigned int : 1` bitfield group is word-aligned by modern Norcroft, making
  the struct 12 bytes instead of 8 and corrupting the `numicons` offset, which
  breaks template loading (*"not enough memory to copy template"* on launch).
  Replaced the bit-field group with a single byte field, bringing the struct
  back to 8 bytes. (Superseded on 2026-09-29 by a fix that keeps upstream's
  flag names; see above.)

### Notes
- Built specifically so a **Norcroft**-linked application (the 32-bit !RDPClient
  port) can link against DeskLib; the official prebuilt DeskLib is GCC-built and
  references symbols Norcroft's stubs do not provide.

---

## Base

- Upstream DeskLib, master `f7469f4` (2026-04-04) - the FreeWare C library for
  RISC OS, (C) its original authors. Source: https://github.com/riscos-dot-info/desklib
  (project page https://www.riscos.info/index.php/DeskLib). Releases before
  2026-09-29 were based on DeskLib 2.80 (2007).
