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

1. **`src/include/Wimp.h` - `wimp_colourflags` layout.** Upstream declares the
   extra window flags (`fullcolour`, `extendedscroll`, `never3d`, `always3d`,
   `returnshaded`, `padding`) as `unsigned int` bit-fields after seven `char`
   fields. GCC packs them into the 8th byte, but Norcroft 5.x starts a new word
   at offset 8, making the struct **12 bytes instead of 8** and shifting
   `numicons`/`icons` in `window_block` by 4 - template loading then fails
   (*"not enough memory to copy template"*). They are now declared as
   `unsigned char` bit-fields: **every member name is unchanged**, the struct is
   8 bytes on both compilers, and the flags sit in bits 0-4 of the 8th byte as
   the Wimp expects. (The never3d/always3d comments, which were swapped, are
   corrected.)
2. **`src/include/Wimp.h` - `wimp_point`.** Upstream's April 2026 header
   reformat left `int x` without its semicolon, so any file including `Wimp.h`
   fails to compile. Restored to `int x;`.
3. **`src/Libraries/Template/Clone.c` - compile-time layout guard.** Two
   `typedef`s that fail the build if `wimp_colourflags` is not 8 bytes or
   `window_block` is not 88 bytes, so a layout regression cannot slip through
   silently. No runtime code change.

Everything else is upstream, byte for byte. Build-plan choices (`-apcs 3/32bit`,
`-za1`) live in `build/dlplan.json`, not in the source.

**History:** earlier DeskLib32 releases were based on DeskLib 2.80 (2007) and
replaced the named colour flags with a single `extflags` byte. That broke the
API for other programs (e.g. WinEd) and has been withdrawn in favour of the
fix above. Programs that used `cols.extflags` should use `vals.extra`.

## Repository layout

```
src/            Upstream DeskLib source plus the fixes above (source of truth):
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
NOTICE.md       FreeWare attribution and conditions of use
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

`builddesklib.py` compiles all 529 DeskLib objects (C + assembler) with
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

DeskLib is general-purpose **FreeWare**, (C) its original authors (John Winters,
Jason Williams, Tim Browse, Cy Booker, Sergio Monesi, Julian Smith, John Tytgat
and the DeskLib contributors). There is no single licence file upstream; each
source file carries its authors' copyright and the banner *"Please refer to the
accompanying documentation for conditions of use."* The canonical source is the
DeskLib project at https://www.riscos.info/index.php/DeskLib . The 32-bit build
fixes here are offered under the same FreeWare terms, preserving the original
authors' copyright. See `NOTICE.md`.
