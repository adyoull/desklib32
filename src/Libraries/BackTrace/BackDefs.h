#ifndef __BackTrace_Defs_h
#define __BackTrace_Defs_h

/* PCMask returns a value which can be used to mas out the status and mode bits
   to give just the program counter.
   This works in both 32-bit and 26-bit modes
 */

unsigned int BackTrace_PCMask( void);


/* The save code pointer points 3 words into the function */
#define SaveCodePointerOffset 2


/* We need to search back from the instruction that created a frame until we
 * find the name information word. NameInfoSearchWordLimit is the maximum number
 * of words that we should search through
 */
#define NameInfoSearchWordLimit 4


/* The following information is stored after the function name */

typedef struct	{
	unsigned int	length	:24;	/* Number of bytes used to store function name (includes 0 padding) */
	unsigned int	ff_code	: 8;	/* Should be 0xff if there is a function name before this information */
	}
	function_name_info;


#ifdef MemCheck_MEMCHECK
	#pragma -c0
	/*
	Turn off memory checking in stack-walking code - MemCheck doesn't like all the reads of the stack...
	 */
#endif

#endif
