#include "DeskLib:Debug.h"
#include "DeskLib:Error.h"

#include "DeskLib:BackTrace.h"

#include "BackDefs.h"

#ifndef __TARGET_UNIXLIB__

typedef struct	{
	char*		fnname;
	unsigned int*	save_instruction;
	unsigned int*	frame_create_instruction;
	unsigned int*	fp;
	}
	BackTrace_frameblock;


static void	BackTrace_GetFrameInfo( BackTrace_frameblock* frameblock, const _kernel_unwindblock* frame)
	{
	function_name_info	*name_info;
	unsigned int		test_words;
	unsigned int pcm = BackTrace_PCMask();

	frameblock->fnname			= NULL;
	frameblock->save_instruction	= NULL;
	frameblock->frame_create_instruction	= NULL;

	frameblock->fp = (unsigned int*) (frame->fp & pcm);

	if ( frameblock->fp)	{

		frameblock->save_instruction		= (unsigned int*) (*(frameblock->fp) & pcm);

		if ( frameblock->save_instruction)	{
			frameblock->frame_create_instruction	= frameblock->save_instruction - SaveCodePointerOffset;

			/* Search backwards from the frame creation instruction looking for a 'name info' word */
			name_info = (function_name_info *)(frameblock->frame_create_instruction-1);

			for ( test_words = NameInfoSearchWordLimit; name_info->ff_code != 0xff && test_words > 0; test_words--)
				{
				name_info--;
				}

			if ( name_info->ff_code == 0xff)
				frameblock->fnname = (char*) name_info - name_info->length;
			else	frameblock->fnname = NULL;
			}


		}
	}


const char*	BackTrace_GetFunctionName( const void* ptr)
{
  const function_name_info*	fn =  ptr;
  int			i;

/*  Debug2_Printf( "BackTrace_GetFunctionName( 0x%p)\n", ptr); */

  for ( i=0; i<5; i++, fn--)	{

#if 0
	Debug2_Printf( "fn->ff_code = %i, fn->length = %i\n",
		fn->ff_code, fn->length
		);
#endif

	if ( fn->ff_code == 0xff && fn->length<4096)	{
		const char*	c = (const char*) fn - fn->length;
#if 0
		Debug2_Printf( "BackTrace_GetFunctionName returning 0x%p '%s'\n",
			c, c
			);
#endif
		return c;
		}
	}

/*  Debug2_Printf( "BackTrace_GetFunctionName returning NULL\n"); */
  return NULL;
}



const BackTrace_functionlist*	BackTrace_GetCurrentFunctions( void)
	{
	static BackTrace_functionlist	functionlist;
	static unsigned int*			functions[ BackTrace_MAXFNS];

	_kernel_unwindblock		frame;
	BackTrace_frameblock	frameblock;
	char				*language;
	/*int			n;*/

	BackTrace_SupportCurrentFrame( &frame);

	functionlist.n = 0;
	functionlist.functions	= functions;

	while ( _kernel_unwind( &frame, &language) > 0)	{
		if ( frame.fp)	{
			BackTrace_GetFrameInfo( &frameblock, &frame);
			if ( !frameblock.save_instruction)	break;

			functions[ functionlist.n] = frameblock.frame_create_instruction;
/*			Debug2_Printf( error_PLACE "0x%p\n", functions[ functionlist.n]); */
			functionlist.n++ ;
			if ( functionlist.n >= BackTrace_MAXFNS)	break;
			}
		}

	return &functionlist;
	}

#endif
