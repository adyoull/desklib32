# DeskLib32 - 32-bit DeskLib for RISC OS 5 (Norcroft build)

A **32-bit build of DeskLib 2.80** - the FreeWare C library for RISC OS - compiled
with the **build.riscos.online** service's own **Norcroft cc 5.18 (JRF)** so the
result is ABI- and runtime-compatible with 32-bit Norcroft-linked applications.
It carries one small source fix (below) needed for correct Wimp struct layout
under modern Norcroft.

This was produced to link the 32-bit port of **!RDPClient**
(https://github.com/adyoull/riscos-rdpclient). The official prebuilt DeskLib is
GCC-built and references symbols (`__divsi3`, `__ctype_*` and similar) that
Norcroft's stubs do not provide, so a Norcroft-built DeskLib is required to link
a Norcroft application cleanly.

## The fix

`src/include/Wimp.h` - the `wimp_colourflags` type.

The original declared a group of `unsigned int : 1` bitfields after some `char`
fields. Modern Norcroft word-aligns the bitfield unit, which makes the struct
**12 bytes instead of 8** and shifts `numicons`/`icons` by 4. The wrong
`numicons` offset makes `Template_Clone`/template loading request a bogus
allocation - the app dies on launch with *"not enough memory to copy template"*.

The bitfield group is replaced with a single `unsigned char extflags;`, which
keeps the struct at 8 bytes and matches the Wimp window-block format. No code
references the individual flags. The change is commented inline in `Wimp.h`.

## Repository layout

```
src/            The modified DeskLib 2.80 source (source of truth):
                  include/    DeskLib public headers (the extflags fix is here)
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

`builddesklib.py` compiles all ~516 DeskLib objects (C + assembler) with
`-apcs 3/32bit` in wall-clock-capped slices, carrying the object directory
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
fix here is offered under the same FreeWare terms, preserving the original
authors' copyright. See `NOTICE.md`.
