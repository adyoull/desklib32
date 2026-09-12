typedef struct pane_link
{
  window_handle window;
  pane_data panedata;
  struct pane_link *next;
} pane_link;

extern pane_link *pane_root;
extern pane_link *curpane;
