# NOTICE - DeskLib and this 32-bit build

## DeskLib (base library)

DeskLib is **"The FreeWare C library for RISC OS machines."** It is the work of
many authors over many years, including (from the source headers) John Winters,
Jason Williams, Tim Browse, Cy Booker, Sergio Monesi, Julian Smith and John
Tytgat, among other DeskLib contributors. Copyright remains with those authors.

Every DeskLib source file carries its authors' copyright notice and the banner:

> Please refer to the accompanying documentation for conditions of use.

DeskLib is distributed as **general-purpose freeware**. There is no single
licence file in the upstream source tree; the conditions of use live in the
DeskLib documentation. The canonical DeskLib project and source repository are
at:

  https://www.riscos.info/index.php/DeskLib

If you intend to redistribute DeskLib (modified or not), please consult the
upstream documentation for the authoritative conditions of use.

## Modifications in this repository

The source is upstream DeskLib (https://github.com/riscos-dot-info/desklib,
master `f7469f4`) with two header fixes in `src/include/Wimp.h` - the
`wimp_colourflags` flags declared as `unsigned char` bit-fields so the struct
keeps its 8-byte layout under Norcroft, with all member names unchanged, and
the missing semicolon in `wimp_point` restored - plus a compile-time layout
check in `src/Libraries/Template/Clone.c` (see README.md and CHANGELOG.md). The
build kit (`build/`) and the debugging toolkit (`debug/`) are new tooling, not
part of DeskLib.

These additions and the small header fixes are offered under the same FreeWare
terms as DeskLib, preserving the original authors' copyright. They carry a
modification notice **(C) 2026 Andrew Youll** where a notice is appropriate.
