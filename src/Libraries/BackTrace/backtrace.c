/*** backtrace.c ***/
/* Generate a stack backtrace
 * (c) Paul Field 1995
 * Based closely on code by Tom Hughes
 */

#include <assert.h>
#include <stdio.h>

#include "DeskLib:BackTrace.h"

#include "BackDefs.h"

#ifndef __TARGET_UNIXLIB__

void BackTrace_OutputToStdErr(void)
{
  BackTrace_OutputToStreamWithPrefix(stderr, "");
}

void BackTrace_OutputToStreamWithPrefix(FILE *stream, const char *prefix)
{
  BackTrace_OutputToFFunctionWithPrefix((backtrace_printf_fn)fprintf, stream, prefix);
}

#endif
