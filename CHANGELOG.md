# DeskLib32 - Changelog

A 32-bit build of DeskLib (the FreeWare RISC OS C library) compiled with
Norcroft cc 5.18 on build.riscos.online, with a struct-layout fix and a
debugging toolkit. Based on upstream DeskLib from 2026-09-29 (2.80 before). Dates are ISO (YYYY-MM-DD).

---

## 2026-09-29

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
  now covers 529 objects (270 C + 259 assembler).

### Fixed
- **`wimp_colourflags` layout, without changing the API.** The previous fix
  replaced the five named extra-window flags with one `extflags` byte, which
  broke programs that use them (e.g. WinEd). The flags are now `unsigned char`
  bit-fields: all member names are as upstream, the struct is 8 bytes under both
  GCC and Norcroft, and the flags sit in bits 0-4 of the 8th byte. The swapped
  `never3d`/`always3d` comments are corrected. Code that used `cols.extflags`
  should use `vals.extra`.
- **`wimp_point` missing semicolon** (upstream, April 2026 header reformat):
  `int x` restored to `int x;`, without which nothing including `Wimp.h`
  compiles.
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
  Replaced with a single `unsigned char extflags;` - struct back to 8 bytes,
  matching the Wimp window-block format. No code referenced the individual flags.

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
