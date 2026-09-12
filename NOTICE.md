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

The only functional source change from stock DeskLib 2.80 is in
`src/include/Wimp.h`: the `wimp_colourflags` bitfield group is replaced with a
single `unsigned char extflags;` so the struct keeps its intended 8-byte layout
under modern Norcroft (see README.md and CHANGELOG.md). The build kit (`build/`)
and the debugging toolkit (`debug/`) are new tooling, not part of DeskLib.

These additions and the one-line struct fix are offered under the same FreeWare
terms as DeskLib, preserving the original authors' copyright. They carry a
modification notice **(C) 2026 Andrew Youll** where a notice is appropriate.
