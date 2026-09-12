#ifndef __dl_Core_h
#include "DeskLib:Core.h"
#endif


typedef struct msg_def *msgdefptr;

typedef struct msg_def
{
  msgdefptr   next;
  union
  {
    char        *text;
    msgdefptr   taglist;
  } data;
  char        tag[10];
} msg_def;

extern msgdefptr msgs_grouplist;
extern BOOL Msgs__MatchToken(const char *tag1, const char *tag2, BOOL wcallowed);

extern msgdefptr Msgs__Find(msgdefptr *liststart, const char *tag,
                            BOOL create, BOOL wcallowed);

#define MatchTokenI(t1, t2) (((const int*)(const void*)t1)[0]==((const int*)(const void*)t2)[0] && ((const int*)(const void*)t1)[1]==((const int*)(const void*)t2)[1])
