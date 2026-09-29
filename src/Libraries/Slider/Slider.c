/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Slider.Slider.c
    Author:  Copyright © 1994 Peter Gaunt
    Version: 1.00 (12 Mar 1994)
    Purpose: Encapsulate slider-type icons in an easy-to-use system.
*/

#include "DeskLib:Core.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Event.h"
#include "DeskLib:GFX.h"
#include "DeskLib:Sprite.h"
#include "DeskLib:Screen.h"
#include "DeskLib:ColourTran.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Slider.h"


/****************************************************************************

  static int Slider_UserToSlider( slider_info *slider, int value )

  Inputs:   slider - the slider info for this slider.
      	    value - the value (in user units) to be converted.
  Returns:  The converted value.
  Purpose:  Converts an external (user) value to an internal value in the
      	    range 0 -> SLIDER_MAX.
      	    If value is less than limits.min then returns 0.
      	    If value is greater than limits.max then returns SLIDER_MAX.
      	    If limits.min = limits.max then returns 0.
  SeeAlso:  Slider_UserValue

****************************************************************************/

static int Slider_UserToSlider( slider_info *slider, int value )
{
  int low  = slider->limits.min,
      high = slider->limits.max,
      range;

  double scale;

  /*
   * If the lower limit is higher than the upper limit,
   * then swap them over.
   */
  if (low > high)
  {
    int temp = low;
    low = high;
    high = temp;
  }

  /* Cope with out of range values, or zero ranges. */
  if ( value > high )
    return( SLIDER_MAX );
  if (value < low || low == high )
    return( 0 );

  /* Scale the value to slider units. */
  value = value - low;
  range = high - low;

  /* Work out the scaling ratio */
  scale = ((double) SLIDER_MAX) / ((double) range);

  /* Scale the value and convert back to an integer */
  return ((int) (0.5 + (((double) value) * scale)));
}




/****************************************************************************

  static int Slider_UserValue( slider_info *slider, int value )

  Inputs:   slider - the slider info for this slider.
      	    value - the value (in slider units) to be converted.
  Returns:  The converted value.
  Purpose:  Converts an internal slider value to an external (user) value
  SeeAlso:  Slider_UserToSlider

****************************************************************************/

static int Slider_UserValue( slider_info *slider, int value )
{
  int range;
  double scale;

  /* Cope with zero ranges. */
  if ( slider->limits.min == slider->limits.max )
    return 0;

  /* Scale internal units to user units. */
  range = slider->limits.max - slider->limits.min;

  /* Work out the scaling ratio */
  scale = ((double) range) / ((double) SLIDER_MAX);

  /* Scale the value and convert back to an integer */
  return (slider->limits.min + (int) (0.5 + (((double) value) * scale)));
}





/****************************************************************************

> os_error *Slider_Redraw(slider_info *slider, wimp_rect *clipwindow);


  Inputs:   slider - the slider info for this slider.
      	    clipwindow - the area of the window being redrawn (in screen
      	    coordinates).
  Returns:  An error pointer, or NULL if no errors occured.
  Purpose:  Redraws a slider icon - call this function from within your
      	    redraw loops.
            If clipwindow != NULL then does nothing if slider is outside
            clip window.
  Errors:   Unable to use the colour indicated in 'slider'.
  SeeAlso:  Slider_ReadValue; Slider_SetValue; Slider_Drag

****************************************************************************/

extern os_error *Slider_Redraw( slider_info *slider, wimp_rect *clipwindow )
{
  wimp_rect rect;
  int       sliderx,
      	    slidery,
      	    sliderwidth,
      	    sliderheight;
  int       barsize,
      	    backsize;
  os_error  *error;

  /* Give up if strange colours are requested. */
  if ((slider->colour.background == -1) && (slider->colour.foreground == -1))
    return( NULL );

  /* Obtain the icon's position in screen coordinates. */
  Icon_ScreenPos( slider->window, slider->icon, &rect );

  /* If slider is outside the redraw region, then don't bother to draw it. */
  if (clipwindow && !Coord_RectsOverlap(clipwindow, &rect))
      return NULL;

  /* Compute slider dimensions. */
  sliderx = rect.min.x + slider->border.x;
  slidery = rect.min.y + slider->border.y;
  sliderwidth  = rect.max.x - rect.min.x - ( slider->border.x * 2 )
                  - screen_delta.x;
  sliderheight = rect.max.y - rect.min.y - ( slider->border.y * 2 )
                 - screen_delta.y;

  /* Work out the size of the slider 'bar'. */
  if (slider->flags.vertical)
    barsize = ( sliderheight * slider->value ) / SLIDER_MAX;
  else
    barsize = ( sliderwidth * slider->value ) / SLIDER_MAX;

  /*
   * Draw the background of the slider using the colours specified in the
   * slider_info structure.
   */

  if (slider->colour.background != -1)
  {
    if (!slider->flags.rgb)
      error = Wimp_SetColour(slider->colour.background);
    else
      error = ColourTrans_SetGCOL(slider->colour.background, 0x100, 0);

    if (error != NULL)
      return error;

    if (slider->flags.vertical)
    {
      backsize = sliderheight - barsize;
      GFX_RectangleFill(sliderx, slidery + barsize, sliderwidth, backsize);
    }
    else
    {
      backsize = sliderwidth - barsize;
      GFX_RectangleFill(sliderx + barsize, slidery, backsize, sliderheight);
    }
  }

  /*
   * Draw the slider 'bar' itself, using the colours specified in the
   * slider_info structure.
   */

  if ((slider->value > 0) && (slider->colour.foreground != -1))
  {
    if (!slider->flags.rgb)
      error = Wimp_SetColour(slider->colour.foreground);
    else
      error = ColourTrans_SetGCOL(slider->colour.foreground, 0x100, 0);

    if (error != NULL)
      return error;

    if (slider->flags.vertical)
    {
      GFX_RectangleFill(sliderx, slidery, sliderwidth, barsize);
    }
    else
    {
      GFX_RectangleFill(sliderx, slidery, barsize, sliderheight);
    }
  }

  /* Everything went according to plan... */
  return NULL;
}


/****************************************************************************

  int Slider_ReadValue(slider_info *slider);

  Inputs:   slider - the slider info for this slider.
  Returns:  The current slider value, in user units.
  Purpose:  Returns current slider setting in user units.
            (i.e. between slider->limits.min and slider->limits.max)
  SeeAlso:  Slider_SetValue; Slider_Drag; Slider_Redraw

****************************************************************************/

extern int Slider_ReadValue(slider_info *slider)
{
  return(Slider_UserValue(slider, slider->value));
}


/****************************************************************************

  static os_error *Slider_Set(slider_info *slider, int value,
      	      	      	      void *ref, BOOL *stop)

  Inputs:   slider - the slider info for this slider.
      	    value  - the value (in slider units) that the slider is to be
      	      	     set to.
  Outputs:  stop - if non-NULL and the update callback function returned
      	      	   a non-NULL value, then stop is set to TRUE.
  Returns:  Standard error block, or NULL if no errors encountered.
  Purpose:  Sets slider to value (in internal units, 0-SLIDER_MAX).
      	    Value is clamped to the range 0 - SLIDER_MAX.

      	    Redraws the window rectangle containing the icon using
      	    Wimp_UpdateWindow.

      	    The icon must not be overlapped by any areas of the window which
      	    need redrawing separately since they will not be redrawn here.

      	    If the slider->update function pointer is not NULL and the slider
      	    value in *user units* changes then this calls the update
      	    callback function.
  Errors:   -
  SeeAlso:  Slider_SetValue

****************************************************************************/

static os_error *Slider_Set(slider_info *slider, int value,
      	      	    	    void *ref, BOOL *stop)
{
  icon_block         istate;
  window_redrawblock redraw;
  BOOL               more;
  int                oldvalue, stopdrag;
  os_error           *error;

  /* Limit value to the allowed range for slider units. */
  if ( value < 0 )
    value = 0;
  if ( value > SLIDER_MAX )
    value = SLIDER_MAX;

  /*
   * Remember the current slider value, and set the slider to the new
   * value.  This is so we can see if the slider value actually changes,
   * and call the update callback function if it does.
   */
  oldvalue = slider->value;
  slider->value = value;

  /*
   * Only redraw slider if value has changed.
   * Helps prevent flicker.
   */
  if (oldvalue != value)
  {
    error = Wimp_GetIconState( slider->window, slider->icon, &istate );
    if (error != NULL)
      return error;

    redraw.window = slider->window;
    redraw.rect   = istate.workarearect;

    error = Wimp_UpdateWindow( &redraw, &more );
    if (error != NULL)
      return error;

    while (more)
    {
      /* Redraw this part of the slider */
      error = Slider_Redraw( slider, &redraw.cliprect );
      if (error == NULL)
        error = Wimp_GetRectangle( &redraw, &more );
      if (error != NULL)
        return error;
    }
  }

  if (stop != NULL)
    *stop = FALSE;

  /* Call updated function (if any) in main code if user value has changed */
  value = Slider_UserValue(slider, value);
  oldvalue = Slider_UserValue(slider, oldvalue);
  if (slider->update && (value != oldvalue))
  {
    stopdrag = slider->update(slider, ref);
    if ((stop != NULL) && stopdrag && slider->flags.dragging)
      *stop = TRUE;
  }

  /* Everything went ok. */
  return NULL;
}


/****************************************************************************

  os_error *Slider_SetValue(slider_info *slider,
                            int value,
                            int *valueset,
                            void *ref );

  Inputs:   slider - the slider info for this slider.
      	    value  - the value (in user units) that the slider should be
      	      	     set to.
      	    ref    - a reference to pass to the update callback funtion.
  Outputs:  valueset - if not NULL, this is updated to hold the value
                       actually the slider is actually set to (this can be
                       different to 'value', e.g. if value is outside the
                       slider limits).
  Returns:  Standard error block or NULL if no error occurs.
  Purpose:  Sets slider to value in value (user units).
      	    If the slider is being dragged (i.e. if slider->status.dragging
      	    is set) then the function does nothing.

            The value is clamped to numbers between slider->limits.min and
            slider->limits.max.

            The slider->update function (if any) will be called if the value
            has changed.

            Can also be used to alter other settings (e.g. colour) by
            directly changing the slider structure before calling.
  Errors:   An error is returned if there is a problem accessing or
      	    redrawing the icon.
  SeeAlso:  Slider_ReadValue; Slider_Drag; Slider_Redraw

****************************************************************************/

extern os_error *Slider_SetValue(slider_info *slider, int value,
      	      	      	      	 int *valueset, void *ref )
{
  os_error *error;

  /* Ignore if slider is being dragged */
  if (slider->flags.dragging)
  {
    if (valueset != NULL)
      *valueset = Slider_ReadValue(slider);
    return NULL;
  }

  error = Slider_Set(slider, Slider_UserToSlider(slider, value), ref, NULL);
  if (valueset != NULL)
    *valueset = Slider_ReadValue(slider);

  return error;
}





/****************************************************************************

  static os_error *Slider_Update(slider_info *slider,
      	      	      	      	 void *ref, BOOL *stop)

  Inputs:   slider - the slider info for this slider.
      	    ref    - the reference to be passed to the update callback.
  Outputs:  stop - if not NULL, then if the update callback returns a value
      	    indicating that dragging should stop, then stop is set to TRUE.
  Returns:  Standard error block, or NULL if no errors encountered.
  Purpose:  Updates slider from current mouse pointer position.
      	    If value differs from that in slider->value then calls Slider_Set
      	    (which in turn calls the slider's slider->update function, if
      	    any).
  Errors:   Unable to get the mouse position, or unable to redraw or access
      	    the slider icon in some way.

****************************************************************************/

static os_error *Slider_Update( slider_info *slider, void *ref, BOOL *stop )
{
  wimp_rect   rect;
  mouse_block mouse;
  int         sliderx,
      	      slidery,
      	      sliderwidth,
      	      sliderheight,
      	      sliderpos;
  os_error    *error;

  /* Find out where the mouse pointer is. */
  error = Wimp_GetPointerInfo(&mouse);
  if (error != NULL)
    return error;

  /* Obtain the icon's position in screen coordinates. */
  Icon_ScreenPos(slider->window, slider->icon, &rect);

  /* Calculate the slider dimensions. */
  if (slider->flags.vertical)
  {
    sliderheight = rect.max.y - rect.min.y - (slider->border.y * 2);
    slidery      = rect.min.y + slider->border.y;
    sliderpos = (mouse.pos.y - slidery) * SLIDER_MAX;
    sliderpos = (int) (0.5 + (((double) sliderpos) / ((double) sliderheight)));
  }
  else
  {
    sliderwidth = rect.max.x - rect.min.x - ( slider->border.x * 2 );
    sliderx     = rect.min.x + slider->border.x;
    sliderpos = (mouse.pos.x - sliderx) * SLIDER_MAX;
    sliderpos = (int) (0.5 + (((double) sliderpos) / ((double) sliderwidth)));
  }

  /* Limit the slider's position to the legal range of slider values. */
  if ( sliderpos < 0 )
    sliderpos = 0;
  if ( sliderpos > SLIDER_MAX )
    sliderpos = SLIDER_MAX;

  /* If the slider position has changed, update the slider. */
  if (sliderpos != slider->value)
  {
    if (!slider->flags.clickstop)
      error = Slider_Set(slider, sliderpos, ref, stop);
    else
    /* If click stops then only redraw slider if *user* value has altered */
    {
      int newvalue = Slider_UserValue(slider, sliderpos);

      if (newvalue != Slider_UserValue(slider, slider->value))
      {
        /*
         * This oddity simply ensures that slider is set to an internal value
         * corresponding to an integral user value. Does this by converting
         * internal value to user value and then back again.
         */
        error = Slider_Set(slider, Slider_UserToSlider(slider, newvalue),
                           ref, stop );
      }
    }
  }

  /* Inform caller of any problems. */
  return error;
}

/****************************************************************************

  os_error *Slider_Drag(slider_info *slider,
                        BOOL *closed,
                        int *value,
                        void *ref )

  Inputs:   slider - the slider info for this slider.
      	    ref    - a reference to pass to the update callback funtion.
  Outputs:  closed - if not NULL, and the window is closed during the drag,
      	      	     then closed is set to TRUE.
      	    value  - if not NULL, then it is set to the slider value (in
      	      	     user units) on exit.
  Returns:  Standard error block, or none if no errors encountered.
  Purpose:  Drag a slider. Call when slider's base icon is clicked on.
      	    Polls the Wimp, grabbing NULL events but passing the rest on to
      	    Event_Process.
      	    Exits when dragging stops or the slider->update function (if any)
      	    returns a non-NULL value.
      	    Also exits if window is closed while dragging (see Outputs).
  Errors:   An error is returned if there is a problem accessing or
      	    redrawing the icon.
  SeeAlso:  Slider_SetValue; Slider_ReadValue; Slider_Redraw

****************************************************************************/

extern os_error *Slider_Drag(slider_info *slider, int *closed,
      	      	      	     int *value, void *ref)
{
  convert_block   convert;
  window_state    state;
  drag_block      dragdata;
  icon_block      bicon;
  event_pollblock *event;
  event_pollmask  mask;
  window_state    wstate;
  BOOL            finished = FALSE;
  os_error        *error;

  if (closed != NULL)
    *closed = FALSE;

  error = Wimp_GetIconState(slider->window, slider->icon, &bicon);
  if (error !=NULL)
    return error;

  error= Wimp_GetWindowState(slider->window, &state);
  if (error != NULL)
    return error;

  /* Start the Wimp drag process */
  convert.screenrect = state.openblock.screenrect;
  convert.scroll     = state.openblock.scroll;

  dragdata.window       = slider->window;
  dragdata.type         = drag_INVISIBLE;
  dragdata.screenrect   = state.openblock.screenrect;
  dragdata.parent.min.x = bicon.workarearect.min.x;
  dragdata.parent.max.x = bicon.workarearect.max.x - screen_delta.x;
  dragdata.parent.min.y = bicon.workarearect.min.y;
  dragdata.parent.max.y = bicon.workarearect.max.y - screen_delta.y;

  Coord_RectToScreen( &dragdata.parent, &convert );

  error = Wimp_DragBox( &dragdata );
  if (error != NULL)
    return error;

  /*
   * Inform the other functions in this module that the slider is
   * currently being dragged.
   */
  slider->flags.dragging = -1;

  /* Try to update the slider once. */
  error = Slider_Update(slider, ref, &finished);
  if (error != NULL)
    return error;

  /* Did the update callback ask for the drag to be terminated? */
  if (finished)
  {
    /* Yes - tell the Wimp to cancel the drag. */
    error = Wimp_DragBox((drag_block *) -1);
    if (error != NULL)
      return error;

    /*
     * Inform the rest of the module that the slider is no longer
     * being dragged.
     */
    slider->flags.dragging = 0;

    /* Return the current slider value to caller if required. */
    if (value != NULL)
      *value = Slider_ReadValue(slider);

    return NULL;
  }

  /*
   * Poll the Wimp for events.
   * Null events are used to update/redraw the slider position.
   * DragBox events are used to detect the end of the drag.
   * All other events are passed on for normal processing by Event_Process.
   */
  event = &event_lastevent;
  mask = event_mask;
  mask.data.null = 0;

  while (!finished)
  {
    error = Wimp_Poll(mask, event);
    if (error != NULL)
      return error;

    switch (event->type)
    {
      case event_NULL:
      	error = Wimp_GetWindowState(slider->window, &wstate);
        if (error != NULL)
          return error;

        if (wstate.flags.data.open)
        {
          /* Window is still open - update the slider display. */
          error = Slider_Update(slider, ref, &finished);
      	  if (error != NULL)
            return error;
        }
        else
        {
          /* Window has been closed during drag - terminate the drag. */
      	  finished = TRUE;
          if (closed != NULL)
            *closed = TRUE;
        }
        break;

      case event_USERDRAG:
      	/* User has let go of the mouse button - the drag has finished. */
        finished = TRUE;
        break;

      default:
      	/* Pass on for usual event processing. */
        Event_Process(event);
        break;
    }
  }

  /* We're not dragging the slider any more. */
  slider->flags.dragging = 0;

  /* Ensure the Wimp's drag operation has finished. */
  error = Wimp_DragBox((drag_block *) -1);

  /* Return the new slider value to the caller, if required. */
  if (value != NULL)
    *value = Slider_ReadValue(slider);

  /* Inform caller of any errors that occured. */
  return error;
}
