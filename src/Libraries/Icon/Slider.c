#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Icon.h"


/* ==========================================================================
   Icon_Slider:
   This is a draggable "volume bar" as in the task window memory displays.
   It consists of two icons, a "base" icon, which provides
     a) a pretty border for the slider (indented plinth) if you want it.
     b) a simple, tidy, and effective means of setting the bounds within
        which the slider can be dragged. Note that this also means that the
        slider can easily be resized just by changing the template
        definition of the icons.

   ...and a "slider" icon, which defines:
     a) The type of icon used. Currently slider only provides a
        "left-end-fixed, right-end draggable" style slider, in which cse you
        specify the size, y-position, and colours of the icon. Later I might
        get around to writing code to handle a fixed-size moving icon (so
        you can have a "volume knob" that moves rather than a "bar graph")
     b) The y-position of the icon (X positions are read from the base icon)
        and the left hand end position of the icon.

     As the slider is dragged (or indeed at any time) a value is read from
     the slider in the form of an integer. When the slider is at a
     minimum size, 0 is returned; at maximum, 1000 is returned.

     The base icon should have a LOWER icon handle than the slider (so the
     slider appears on top)
     When either icon is clicked, initiate the drag with DragSlider, then
     continuously update by repeatedly calling UpdateSlider on NULL (or
     perhaps button if buttontype == always) events.
     At any time, the current slider value can be read using ReadSlider.

     All these calls return the current slider position, so that you may
     update other displays (for example, alter a colour display as an rgb
     slider is dragged, etc.)
*/


        /* Gap at each end of slider in OS coordinates */
#define SLIDE_GAP 9
        /* Minimum size slider can shrink to in OS coordinates
           (should be equal to slider height) */
#define SLIDE_MIN 12



extern int Icon_SetSlider(window_handle window,
                          icon_handle baseicon, icon_handle slidericon,
                          int sliderpos)
/* Sets a slider icon-pair to the specified position (specify as a
 * percentage). Values < 0 and > 1000 are clipped, and the value to which the
 * slider has been set is returned.
 */
{
  icon_handle      icon;
  icon_createblock icreate;
  register int     sliderwidth;

  if (sliderpos < 0)    sliderpos = 0;
  if (sliderpos > 1000) sliderpos = 1000;

  Wimp_GetIconState(window, baseicon, &icreate.icondata);

  sliderwidth = (icreate.icondata.workarearect.max.x -
                 icreate.icondata.workarearect.min.x) -
                 (SLIDE_GAP * 2 + SLIDE_MIN);

  Wimp_GetIconState(window, slidericon, &icreate.icondata);
  icreate.icondata.workarearect.max.x = icreate.icondata.workarearect.min.x +
                                   (sliderpos * sliderwidth)/1001 + SLIDE_MIN;
  icreate.window = window;
  Wimp_DeleteIcon(window, slidericon);                 /* And resize icon... */
  Wimp_CreateIcon(&icreate, &icon);

  {
    window_redrawblock r;  /* Force window redraw to remove: getting smaller */

    r.window = window;
    r.rect.min.x = icreate.icondata.workarearect.max.x + 4;
    r.rect.max.x = icreate.icondata.workarearect.min.x + sliderwidth+SLIDE_MIN;
    r.rect.min.y = icreate.icondata.workarearect.min.y;
    r.rect.max.y = icreate.icondata.workarearect.max.y;
    Wimp_ForceRedraw(&r);
  }
  Wimp_SetIconState(window, slidericon, 0, 0);         /* forceredraw slider */
  return(sliderpos);
}



extern int Icon_UpdateSlider(window_handle window,
                             icon_handle baseicon, icon_handle slidericon,
                             int lastpos)
/* (call on null events while slider being dragged)
 * Calculates a new slider percentage from the mouse pointer position.
 * If this differs from lastpos, the slider is updated (by recreating the
 * slidericon to the new length.
 * returns the new slider position percentage value.
 * NOTE: Slider update is achieved by DELETING and re-creating the slider
 * icon. This relies upon no icons of a lower icon handle having been deleted!
 */
{
  window_state     wstate;
  icon_createblock icreate;
  register int     sliderwidth, sliderpos;
  mouse_block      ptr;
  convert_block    convert;

  UNUSED( lastpos);

  Wimp_GetPointerInfo(&ptr);
  Wimp_GetWindowState(window, &wstate);

  convert.screenrect = wstate.openblock.screenrect;
  convert.scroll = wstate.openblock.scroll;

  Wimp_GetIconState(window, baseicon, &icreate.icondata);

  sliderwidth = (icreate.icondata.workarearect.max.x -
                 icreate.icondata.workarearect.min.x) -
                 (SLIDE_GAP * 2 + SLIDE_MIN);
  sliderpos = Coord_XToWorkArea(ptr.pos.x, &convert) -
              (icreate.icondata.workarearect.min.x + SLIDE_GAP + SLIDE_MIN);

  sliderpos = ((sliderpos * 1001) / sliderwidth);          /* get as 1..1000 */
  return(Icon_SetSlider(window, baseicon, slidericon, sliderpos));
}



extern int Icon_DragSlider(window_handle window,
                           icon_handle baseicon, icon_handle slidericon)
/* Initiates the drag operation on a slider. Call this when you get a click
 * in either baseicon or slidericon.
 * Returns the new percentage represented by the slider (remember this so
 * you can pass it in to Icon_UpdateSlider to save unnecessary (flickery)
 * update.)
 * NOTE: It is up to you to turn on NULL events and remember that
 * Icon_UpdateSlider must be called on each poll...
 * An alternative is to make the 2 slider icons use button type ALWAYS, and
 * check the mouse button state yourself to see whether a drag needs to be
 * started/continued...
 */
{
  convert_block convert;
  window_state  state;
  drag_block    dragdata;
  icon_block    bicon, sicon;

  Wimp_GetIconState(window, baseicon, &bicon);
  Wimp_GetIconState(window, slidericon, &sicon);

  Wimp_GetWindowState(window, &state);
  convert.screenrect = state.openblock.screenrect;
  convert.scroll     = state.openblock.scroll;

  dragdata.window    = window;
  dragdata.type      = drag_INVISIBLE;
  dragdata.screenrect= state.openblock.screenrect;   /* dragged box not used */
  dragdata.parent.min.x = bicon.workarearect.min.x + SLIDE_GAP;
  dragdata.parent.max.x = bicon.workarearect.max.x - SLIDE_GAP;
  dragdata.parent.min.y = sicon.workarearect.min.y;
  dragdata.parent.max.y = sicon.workarearect.max.y;

  Coord_RectToScreen(&dragdata.parent, &convert);

  Wimp_DragBox(&dragdata);
  return(Icon_UpdateSlider(window, baseicon, slidericon, -1));
}



extern int Icon_ReadSlider(window_handle window,
                           icon_handle baseicon, icon_handle slidericon)
/* Given a slider icon-pair, returns a percentage representing the state */
{
  icon_block   istate;
  register int sliderwidth, sliderpos;

  Wimp_GetIconState(window, baseicon, &istate);
  sliderwidth = (istate.workarearect.max.x - istate.workarearect.min.x) -
                (SLIDE_GAP * 2 + SLIDE_MIN);
  Wimp_GetIconState(window, slidericon, &istate);
  sliderpos = ((istate.workarearect.max.x - istate.workarearect.min.x)* 1001)
                / sliderwidth;

  if (sliderpos < 0)  sliderpos = 0;
  if (sliderpos > 1000) sliderpos = 1000;

  return(sliderpos);
}
