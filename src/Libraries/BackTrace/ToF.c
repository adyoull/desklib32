/*** backtrace.c ***/
/* Generate a stack backtrace
 * (c) Paul Field 1995
 * Based closely on code by Tom Hughes
 */

/*
  23 Oct 1996 JS. Added check that embedded fn name is < 4096 characters
  (there are 24 bits reserved for the name length, but anything bigger
  than this is unlikely to be a fn name.
 */


#include <assert.h>
#include <stdio.h>

#include "DeskLib:BackTrace.h"
#include "DeskLib:Debug.h"
#include "DeskLib:Error.h"

#include "BackDefs.h"

#ifndef __TARGET_UNIXLIB__

void	BackTrace_OutputToFFunctionWithPrefix( backtrace_printf_fn fn, void* reference, const char* prefix)
	{
	_kernel_unwindblock frame;
	char	*language;
	unsigned int	pcm = BackTrace_PCMask();

	fn( reference, "%sBacktrace:\n", prefix);

	BackTrace_SupportCurrentFrame( &frame);

	while ( _kernel_unwind( &frame, &language) > 0)
		{

		function_name_info*	name_info;
		unsigned int*			save_code_pointer;
		unsigned int*			frame_create_instruction;
		unsigned int*			return_pc;
		unsigned int*			fp;
		unsigned int			test_words;

		/*
		'frame' contains info on a particular stackframe. However, we only use 'frame' to identify
		the /previous/ stackframe, partly because 'frame' (annoyingly) only describes the stack
		frame incompletely.

		The top 4 words of each stackframe are:
			[fp, #-0]=savecodepointer
			[fp, #-4]=return_pc
			[fp, #-8]=return_sp
			[fp,#-12]=return_fp
			(see 'Desktop Tools' 253)

		'frame' doesn't contain info on the savecodepointer (which points to the instruction that
		created the stack frame). This is needed to identify the function which created the
		stack frame, as it is usually at the start of the function.

		frame.pc contains the address to return to, which is usually the next instruction after
		the instruction which called the function which created stack frame 'frame'.

		Thus the data we are interested in is:
			fp = frame.fp		prev frame to the frame incompletely described by 'frame'
			pc = frame.return_pc	Address+4 that called function which created 'frame'
			[fp,#-0]		Address of instruction that created frame at fp (the frame previous to that described by 'frame').

		Also, frame.sl doesn't seem to be updated when _kernel_unwind changes into a different stack chunk.
		 */

		/*fp = (unsigned int*) (frame.fp & PCMask);*/
		fp		= (unsigned int*) frame.fp;
		return_pc	= (unsigned int*) (frame.pc-4);	/* frame.pc points to instruction to return to. Usually, prev instruct is the BL. */

/*		Debug5_Printf( error_PLACE "BackTrace: fp=0x%p\n", fp); */

		if (fp != NULL)	{
			save_code_pointer = (unsigned*) (*fp & pcm);
/*			Debug5_Printf( error_PLACE "BackTrace: save_code_pointer=0x%p\n", save_code_pointer); */

			if ( save_code_pointer)	{

				char*	fnname = NULL;

				frame_create_instruction	= save_code_pointer - SaveCodePointerOffset;

/*				Debug5_Printf( error_PLACE "BackTrace: frame_create_instruction=0x%p\n", frame_create_instruction); */

				/* Search backwards from the frame creation instruction looking for a 'name info' word */
				name_info = (function_name_info*) (frame_create_instruction-1);

/*				Debug5_Printf( error_PLACE "BackTrace: name_info=0x%p\n", name_info); */

				for	(
					test_words = NameInfoSearchWordLimit;
					name_info->ff_code != 0xff && test_words > 0;
					test_words--
					)
					{
/*					Debug5_Printf( error_PLACE "BackTrace: name_info=0x%p\n", name_info); */
					name_info--;
					}

				/* If we found the name info word we can print the name, otherwise the function is anonymous */

/*				Debug5_Printf( error_PLACE "name_info->ff_code=0x%x\n", name_info->ff_code); */
				if ( name_info->ff_code == 0xff && name_info->length<4096)	{
					fnname = (char*) name_info - name_info->length;
/*					Debug5_Printf( error_PLACE "fn name = '%s'\n", fnname); */
					}

				fn( reference, "%s pc=0x%p from function at 0x%p (%s), (language is %s)\n",
					prefix,
					return_pc,
					save_code_pointer,
					(fnname) ? fnname : "<unknown name>",
					(language) ? language : "<unknown>"
					);

				/*
				fn( reference, "_kernel_unwindblock has fp=0x%08x sp=0x%08x pc=0x%08x sl=0x%08x\n", frame.fp, frame.sp, frame.pc, frame.sl);
				*/
/*				Debug5_Printf( error_PLACE "\n"); */
				}

			else	{
				fn( reference, "%s pc=0x%p. No function information (stack frame's framecreate word is 0)\n", prefix, return_pc);
				}
			}
		}

	fn( reference, "%s\n", prefix);
	}

#endif
