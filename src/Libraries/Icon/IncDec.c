/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.IncDec.c
    Author:  Copyright © 1994 Lee Atkinson, David Leftley
    Version: 1.01 (21 Jun 1995)
    Purpose: High-level icon handling routines: Handler for increment and
             decrement icons
    Mods:    David Leftley - call Icon_SetCaret after each increment/decrement,
                             otherwise the caret is left inbetween digits when
                             99->100 etc.
*/

#include <stdlib.h>

#include "DeskLib:Core.h"
#include "DeskLib:Event.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Window.h"
#include "DeskLib:WimpSWIs.h"





static void Icon__ResetCaret(const window_handle window, const icon_handle icon)
{
  caret_block caret;
  Wimp_GetCaretPosition(&caret);
  if (caret.window == window && caret.icon == icon)
  {
    Icon_SetCaret(window,icon);
  }
}



static void Increment(const icon_incdecblock *b)
{
 int i;
 if ((i = Icon_GetInteger(b->window,b->texticon)) < (int)(b->max-b->step))
                               Icon_SetInteger(b->window,b->texticon,i+b->step);
      /* if value in text icon is less than the max-step increment it by step */
 else if (i<b->max) Icon_SetInteger(b->window,b->texticon,b->max); /* else if
                                    value less than max, set text icon to max */
 else if (b->loop) Icon_SetInteger(b->window,b->texticon,b->min); /* else if
                                                   loop, set text icon to min */
Icon__ResetCaret(b->window, b->texticon);
}



static void Decrement(const icon_incdecblock *b)
{
 int i;
 if ((i = Icon_GetInteger(b->window,b->texticon)) > (int)(b->min+b->step))
                               Icon_SetInteger(b->window,b->texticon,i-b->step);
      /* if value in text icon is more than the min+step decrement it by step */
 else if (i>b->min) Icon_SetInteger(b->window,b->texticon,b->min); /* else if
                                    value more than min, set text icon to min */
 else if (b->loop) Icon_SetInteger(b->window,b->texticon,b->max);/* else if
                                                   loop, set text icon to max */
Icon__ResetCaret(b->window, b->texticon);
}


static BOOL CheckIncrement(event_pollblock *event,void *ref)
{
 icon_incdecblock *b=(icon_incdecblock *)ref;
 if (event->data.mouse.button.data.select) Increment(b); /* select click */
 else if (event->data.mouse.button.data.adjust) Decrement(b); /* adjust click */

 return(FALSE);
}


static BOOL CheckDecrement(event_pollblock *event,void *ref)
{
 icon_incdecblock *b=(icon_incdecblock *)ref;
 if (event->data.mouse.button.data.select) Decrement(b); /* select click */
 else if (event->data.mouse.button.data.adjust) Increment(b); /* adjust click */

 return(FALSE);
}

icon_incdecblock *Icon_InitIncDecHandler(const window_handle window,
                                         const icon_handle   texticon,
                                         const icon_handle   incrementicon,
                                         const icon_handle   decrementicon,
                                         const BOOL          loop,
                                         const unsigned int  step,
                                         const int           min,
                                         const int           max,
                                         const int           start)
{
 icon_incdecblock *b;
 b=(icon_incdecblock *)malloc(sizeof(icon_incdecblock));
                                            /* allocate memory for data block */
 if (b==NULL) return NULL;
 b->window       =window;
 b->texticon     =texticon;
 b->incrementicon=incrementicon;
 b->decrementicon=decrementicon;
 b->loop         =loop;
 b->step         =step;
 b->min          =min;
 b->max          =max;

 Icon_SetInteger(window,texticon,start);
 if (Event_Claim(event_CLICK,window,incrementicon,CheckIncrement,(void *)b)&&
         Event_Claim(event_CLICK,window,decrementicon,CheckDecrement,(void *)b))
                                                                       return b;
 else return NULL;
}

BOOL Icon_ReleaseIncDecHandler(icon_incdecblock *incdecblock)
{
 BOOL result;
 result=(Event_Release(event_CLICK,incdecblock->window,
                incdecblock->incrementicon,CheckIncrement,(void *)incdecblock)&&
         Event_Release(event_CLICK,incdecblock->window,
                incdecblock->decrementicon,CheckDecrement,(void *)incdecblock));
 free((void *)incdecblock);
 return result;
}
