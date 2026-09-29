# DeskLib32 - 32-bit DeskLib for RISC OS 5 (Norcroft build)

A **32-bit build of current upstream DeskLib** - the FreeWare C library for RISC
OS, from https://github.com/riscos-dot-info/desklib (master `f7469f4`,
2026-04-04) - compiled with the **build.riscos.online** service's own **Norcroft
cc 5.18 (JRF)** so the result is ABI- and runtime-compatible with 32-bit
Norcroft-linked applications. It carries two small header fixes (below), both
of which keep DeskLib's API unchanged.

This was produced to link the 32-bit port of **!RDPClient**
(https://github.com/adyoull/riscos-rdpclient). The official prebuilt DeskLib is
GCC-built and references symbols (`__divsi3`, `__ctype_*` and similar) that
Norcroft's stubs do not provide, so a Norcroft-built DeskLib is required to link
a Norcroft application cleanly.

## Changes from upstream DeskLib

**Every change, with the problem, evidence, reasoning, compatibility impact and
verification, is documented in [MODIFICATIONS.md](MODIFICATIONS.md).** The
exact edits are in [patches/desklib32-vs-upstream.diff](patches/desklib32-vs-upstream.diff).

In summary, `src/` is upstream DeskLib master `f7469f4`: 348 files are
byte-identical to it, 281 assembler files and their includes are byte-identical
to upstream's `aof` branch (master's are in GNU `as` syntax, which `objasm`
cannot read), 8 files are modified, and 1 file is added:

| | File | Change |
|---|---|---|
| M1 | `include/Wimp.h` | `wimp_colourflags`: for non-GCC compilers, bytes 4-7 are one `unsigned int` bit-field container, so the struct is 8 bytes under Norcroft (12 before). All member names are unchanged, and GCC sees upstream's declaration |
| M2 | `include/Wimp.h` | missing `;` after `int x` in `wimp_point` restored |
| M3 | `Libraries/Template/Clone.c` | compile-time check that `wimp_colourflags` is 8 bytes and `window_block` is 88 |
| M4 | `Libraries/Debug/DebugDefs.h` | `static` prototype removed from a shared header (a Norcroft error) |
| M5 | `include/Environment.h` | trailing comma after the last `sysvar_type` value removed (not allowed in C89) |
| A1 | `Libraries/Environment/OSCLI.s` | exports `Environment__OS_CLI`, as the header declares (ported from master) |
| A2 | `Libraries/Tinct/PlotScaled.s`, `PlotScaledAlpha.s` | argument registers fixed; `PlotScaledAlpha` now has its own name and SWI (ported from master) |
| A3 | `Libraries/BackTrace/GetPC2.s` | also exports `BackTrace_GetPC2`, the name the header declares |
| C1 | `Libraries/Compat/Printf.c` (new) | bounded `snprintf`/`vsnprintf` built from C89 calls; DeskLib's own calls are redirected to it with `-D` flags, so programs don't need SharedCLibrary stub chunk 5 (avoids "SWI &5DC34 not known" at start-up, seen on a Pi with the Norcroft 5.18 build) |

Build flags (`-apcs 3/32bit`, `-za1`, and the C1 `-D` redirection, on top of upstream's own Norcroft flags)
live in `build/dlplan.json`, not in the source; see MODIFICATIONS.md section 4.

**History:** earlier DeskLib32 releases were based on DeskLib 2.80 (2007) and
replaced the named colour flags with a single `extflags` byte. That broke the
API for other programs (e.g. WinEd) and has been withdrawn. Programs that used
`cols.extflags` should use `vals.extra`.

## Repository layout

```
src/            Upstream DeskLib source plus the changes in MODIFICATIONS.md:
                  include/    DeskLib public headers
                  oldinclude/ compatibility headers (on DeskLib$Path, as upstream)
                  Libraries/  the library sources (C + ARM assembler)
build/          Reproducible build kit:
                  builddesklib.py   chunked build driver (build.riscos.online)
                  dlplan.json       per-object compile command lines + libfile
                  desklib_src.zip   the build INPUT the service unpacks
                  package.py        regenerates desklib_src.zip from ../src
DeskLib32       The prebuilt 32-bit library (drop-in; RISC OS type Data)
debug/          The RISC OS 5 / Norcroft debugging toolkit (see debug/README.md)
MODIFICATIONS.md  Every change from upstream and the reason for it
patches/        The same changes as a unified diff against upstream
LICENCE         DeskLib's licence, credits and contact (verbatim from upstream)
NOTICE.md       What the licence means for this repository
CHANGELOG.md    Change history for this 32-bit build
```

`src/` is the readable source of truth; `build/desklib_src.zip` is the exact
bundle the build service consumes. After editing anything under `src/`, run
`build/package.py` to regenerate the zip before building.

## Build from source

Needs Python 3, plain internet access to `wss://build.riscos.online`, and the
`websocket-client` package. No RISC OS hardware or cross-compiler is required to
build - only to run the result.

```
cd build
python3 -m pip install websocket-client     # once
python3 package.py                          # ../src -> desklib_src.zip (if you edited src)
python3 builddesklib.py                     # compile + libfile on the service
```

`builddesklib.py` compiles all 530 DeskLib objects (C + assembler) with
`-apcs 3/32bit` (C objects additionally `-za1`, for alignment-safe codegen so
the library runs with CPU alignment checking ON — see CHANGELOG) in
wall-clock-capped slices, carrying the object directory
(`build/deskwork/o`) forward between runs, then `libfile`s them into the final
library and writes **`build/DeskLib32`**. It is resumable - re-run to continue;
delete `build/deskwork/` to start clean.

## Using it

Link it in place of DeskLib's `o.DeskLib` when building a 32-bit Norcroft
application. The !RDPClient build (`buildapp.py`) picks up a `DeskLib32` placed
beside it automatically.

## Debugging

See **[DEBUGGING.md](DEBUGGING.md)** for a field guide to diagnosing Norcroft
32-bit / RISC OS 5 crashes (type-20 data aborts and heap corruption), and
`debug/` for the tools it uses.

## Licence

DeskLib is under **its own licence**, not the GPL. It is reproduced verbatim in
[`LICENCE`](LICENCE), taken from upstream's `!DeskLib/Docs/TextHelp`.
Copyright remains with DeskLib's authors, and each source file carries their
notices. The licence's conditions are:

- distribute the copyright messages and conditions intact;
- distribute unaltered copies, and send alterations to the moderator;
- make no profit from distribution;
- acknowledge DeskLib in the distribution of any software built with it.

See [`NOTICE.md`](NOTICE.md) for how this repository relates to those
conditions.
