#include "kernel.h"

#include "DeskLib:BackTrace.h"

#include "BackDefs.h"

#ifndef __TARGET_UNIXLIB__

int	BackTrace_GetNestingDepth( void)
	{
	_kernel_unwindblock	frame;
	char			*language;
	int			depth;
	unsigned int		pcm = BackTrace_PCMask();

	for	(
		BackTrace_SupportCurrentFrame( &frame), depth=0;
		_kernel_unwind( &frame, &language) > 0;
		depth++
		)
		{
		/*
		Have to calculate save_code_pointer - if NULL, we have reached end of
		stack. _kernel_unwind doesn't return <=0 sometimes when using CMHG.
		 */
		unsigned int		*save_code_pointer;
		unsigned int		*fp;

		fp			= (unsigned int*)	(frame.fp & pcm);	if ( fp==NULL)				break;
		save_code_pointer	= (unsigned int*)	(*fp & pcm);		if ( save_code_pointer==NULL)	break;
		}

	return depth-1;	/* don't include this function	*/
	}

#endif
