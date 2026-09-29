/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Compat.Printf.c
    Author:  Andrew Youll (DeskLib32)
    Purpose: Bounded snprintf / vsnprintf built only from C89 library calls.

    Why this file exists
    --------------------
    Upstream DeskLib calls the C99 functions snprintf() and vsnprintf() in 23
    source files.  Built with Norcroft 5.18 (build.riscos.online), those two
    functions are linked through a separate SharedCLibrary stub chunk
    (chunk 5), which was not initialised on the Raspberry Pi used for
    testing: any program linking a DeskLib member that used snprintf failed
    at start-up with "SWI &5DC34 not known".  Whether that is a quirk of the
    5.18 toolchain or a limit of the ROM's C library is not established;
    avoiding the two functions works either way.

    The DeskLib32 build compiles every other DeskLib C file with
        -Dsnprintf=DeskLib__snprintf -Dvsnprintf=DeskLib__vsnprintf
    so those calls come here instead, and the library imports only C89
    functions from the C library (stub chunks 1 and 2).  This file is compiled
    WITHOUT those definitions.

    How it stays bounded
    --------------------
    The format string is parsed one conversion at a time.  Plain text, %s and
    %c are copied by hand, so their length never matters.  Each numeric
    conversion (%d %i %o %u %x %X %e %E %f %g %G %p) is rendered by C89
    sprintf() into a scratch buffer sized from its own width and precision,
    which bounds its output, and then copied into the caller's buffer with
    truncation.  Behaviour follows C99: at most size-1 characters are stored,
    the result is always terminated when size > 0, and the return value is the
    length the complete output would have had (negative only if a scratch
    buffer cannot be allocated).
*/

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DeskLib__vsnprintf(char *buffer, size_t size, const char *format, va_list args);
int DeskLib__snprintf(char *buffer, size_t size, const char *format, ...);


typedef struct
{
  char   *buffer;
  size_t size;     /* caller's buffer size, including the terminator  */
  size_t length;   /* characters produced so far (may exceed size)    */
} printf_output;


static void output_chars(printf_output *out, const char *text, size_t count)
{
  if (out->size > 0 && out->length < out->size - 1)
  {
    size_t room = out->size - 1 - out->length;
    memcpy(out->buffer + out->length, text, (count < room) ? count : room);
  }
  out->length += count;
}


static void output_repeat(printf_output *out, char c, long count)
{
  while (count-- > 0)
    output_chars(out, &c, 1);
}


int DeskLib__vsnprintf(char *buffer, size_t size, const char *format, va_list args)
{
  printf_output out;
  const char    *p = format;
  char          local[512];

  out.buffer = buffer;
  out.size   = (buffer == NULL) ? 0 : size;
  out.length = 0;

  while (*p != '\0')
  {
    int  left = 0, plus = 0, space = 0, alt = 0, zero = 0;
    long width = 0, precision = -1;
    char lengthmod = '\0', conversion;
    char spec[64];
    int  s;

    if (*p != '%')
    {
      const char *q = p;
      while (*q != '\0' && *q != '%')
        q++;
      output_chars(&out, p, (size_t) (q - p));
      p = q;
      continue;
    }
    p++;                                             /* skip '%' */

    /* flags */
    for (;; p++)
    {
      if      (*p == '-') left  = 1;
      else if (*p == '+') plus  = 1;
      else if (*p == ' ') space = 1;
      else if (*p == '#') alt   = 1;
      else if (*p == '0') zero  = 1;
      else break;
    }

    /* width */
    if (*p == '*')
    {
      width = va_arg(args, int);
      if (width < 0) { left = 1; width = -width; }
      p++;
    }
    else
      while (*p >= '0' && *p <= '9')
        width = width * 10 + (*p++ - '0');

    /* precision */
    if (*p == '.')
    {
      p++;
      if (*p == '*')
      {
        precision = va_arg(args, int);               /* negative = omitted */
        p++;
      }
      else
      {
        precision = 0;
        while (*p >= '0' && *p <= '9')
          precision = precision * 10 + (*p++ - '0');
      }
    }

    /* length modifier */
    if (*p == 'h' || *p == 'l' || *p == 'L')
      lengthmod = *p++;

    conversion = *p;
    if (conversion == '\0')
      break;                                         /* incomplete spec */
    p++;

    switch (conversion)
    {
      case '%':
        output_chars(&out, "%", 1);
        break;

      case 'c':
      {
        char c = (char) va_arg(args, int);
        if (!left) output_repeat(&out, ' ', width - 1);
        output_chars(&out, &c, 1);
        if (left)  output_repeat(&out, ' ', width - 1);
        break;
      }

      case 's':
      {
        const char *text = va_arg(args, const char *);
        size_t      len  = 0;
        if (text == NULL)
          text = "(null)";
        if (precision >= 0)
          while (len < (size_t) precision && text[len] != '\0')
            len++;
        else
          len = strlen(text);
        if (!left) output_repeat(&out, ' ', width - (long) len);
        output_chars(&out, text, len);
        if (left)  output_repeat(&out, ' ', width - (long) len);
        break;
      }

      case 'n':
        if      (lengthmod == 'h') *va_arg(args, short *) = (short) out.length;
        else if (lengthmod == 'l') *va_arg(args, long *)  = (long)  out.length;
        else                       *va_arg(args, int *)   = (int)   out.length;
        break;

      case 'd': case 'i': case 'o': case 'u': case 'x': case 'X': case 'p':
      case 'e': case 'E': case 'f': case 'g': case 'G':
      {
        int    floating = (conversion == 'e' || conversion == 'E' ||
                           conversion == 'f' || conversion == 'g' ||
                           conversion == 'G');
        size_t need;
        char   *scratch;
        int    n;

        /* Rebuild the conversion with '*' resolved to numbers. */
        s = 0;
        spec[s++] = '%';
        if (left)  spec[s++] = '-';
        if (plus)  spec[s++] = '+';
        if (space) spec[s++] = ' ';
        if (alt)   spec[s++] = '#';
        if (zero)  spec[s++] = '0';
        if (width > 0)
          s += sprintf(spec + s, "%ld", width);
        if (precision >= 0)
          s += sprintf(spec + s, ".%ld", precision);
        if (lengthmod != '\0')
          spec[s++] = lengthmod;
        spec[s++] = conversion;
        spec[s]   = '\0';

        /* Largest possible output: the width, the precision, and the digits
           of the value itself (a double can need over 300 digits in %f). */
        need = (size_t) width + (size_t) (precision > 0 ? precision : 0) +
               (floating ? 400 : 80);
        scratch = (need <= sizeof(local)) ? local : (char *) malloc(need);
        if (scratch == NULL)
        {
          if (out.size > 0)
            buffer[(out.length < out.size) ? out.length : out.size - 1] = '\0';
          return -1;
        }

        if (conversion == 'p')
          n = sprintf(scratch, spec, va_arg(args, void *));
        else if (floating)
        {
          if (lengthmod == 'L')
            n = sprintf(scratch, spec, va_arg(args, long double));
          else
            n = sprintf(scratch, spec, va_arg(args, double));
        }
        else if (conversion == 'd' || conversion == 'i')
        {
          if (lengthmod == 'l')
            n = sprintf(scratch, spec, va_arg(args, long));
          else
            n = sprintf(scratch, spec, va_arg(args, int));
        }
        else
        {
          if (lengthmod == 'l')
            n = sprintf(scratch, spec, va_arg(args, unsigned long));
          else
            n = sprintf(scratch, spec, va_arg(args, unsigned int));
        }

        if (n > 0)
          output_chars(&out, scratch, (size_t) n);
        if (scratch != local)
          free(scratch);
        break;
      }

      default:
        /* Unknown conversion: copy the '%' sequence through unchanged. */
        {
          const char *start = p - 1;
          while (start > format && *start != '%')
            start--;
          output_chars(&out, start, (size_t) (p - start));
        }
        break;
    }
  }

  if (out.size > 0)
    buffer[(out.length < out.size) ? out.length : out.size - 1] = '\0';

  return (int) out.length;
}


int DeskLib__snprintf(char *buffer, size_t size, const char *format, ...)
{
  va_list args;
  int     result;

  va_start(args, format);
  result = DeskLib__vsnprintf(buffer, size, format, args);
  va_end(args);
  return result;
}
