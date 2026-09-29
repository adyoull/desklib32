# NOTICE - DeskLib and this 32-bit build

## DeskLib's licence

DeskLib is **"The FreeWare C library for RISC OS machines."** Many authors have
written it over many years, and copyright remains with them. Each source file
carries its authors' copyright notice, and `LICENCE` lists the contributors.

DeskLib is under its own licence, which upstream keeps in
`!DeskLib/Docs/TextHelp`. It is **reproduced verbatim in `LICENCE`** in this
repository, because its first condition is that the copyright messages and
conditions are distributed intact with every copy. In summary (the `LICENCE`
text is authoritative):

- DeskLib may be copied and distributed, provided that:
  - the copyright messages and conditions stay intact;
  - only original, unaltered copies are distributed (alterations are to be sent
    to the moderator so they can become official updates);
  - no profit is made from the distribution.
- Software built with DeskLib must acknowledge, in its distribution, that
  DeskLib (or parts of it) was used.
- There is no warranty.

DeskLib is **not** under the GPL. This matters for programs such as !RDPClient:
the program is GPL, but the DeskLib library it links stays under DeskLib's own
licence.

## Modifications in this repository

DeskLib32 is upstream DeskLib (https://github.com/riscos-dot-info/desklib,
master `f7469f4`, with assembler sources from upstream's `aof` branch). Eight
files are modified and one is added (`Libraries/Compat/Printf.c`), so that it
builds with Norcroft and to fix upstream bugs. Every change and its reason is
in `MODIFICATIONS.md`, and the exact edits are in
`patches/desklib32-vs-upstream.diff`.

DeskLib's licence asks that altered copies are not passed on, but sent back so
they can become official updates. In line with that, these changes are
documented file by file and are being offered to the upstream maintainers.

The build kit (`build/`), the debugging toolkit (`debug/`) and the
documentation are new tooling and are not part of DeskLib. The DeskLib source
files themselves carry only their original authors' copyright notices.
