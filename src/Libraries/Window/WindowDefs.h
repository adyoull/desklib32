/* Window: WindowDefs.h */

extern linklist_header window_listanchor;

typedef struct
{
  linklist_header header;
  window_handle   window;
  char            templatename[wimp_MAXNAME+1];      /* Parent template name */
  window_block    *memory;                           /* memory claim         */
} windowrec;
