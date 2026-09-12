# DeskLib32 - Changelog

A 32-bit build of DeskLib 2.80 (the FreeWare RISC OS C library) compiled with
Norcroft cc 5.18 on build.riscos.online, with a struct-layout fix and a
debugging toolkit. Dates are ISO (YYYY-MM-DD).

---

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

- DeskLib 2.80 - the FreeWare C library for RISC OS, (C) its original authors.
  Canonical source: https://www.riscos.info/index.php/DeskLib
