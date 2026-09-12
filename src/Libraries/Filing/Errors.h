#ifndef filing_ERRORBASE

#define filing_ERRORBASE 0x00801C00
/* This should be allocated to Acorn 6502 Emulator! */

#define filing_ERROR_OutOfMemory          (0x00+filing_ERRORBASE)
#define filing_ERROR_DirectoryIsFile      (0x01+filing_ERRORBASE)
#define filing_ERROR_DirectoryDoesntExist (0x02+filing_ERRORBASE)

#endif
