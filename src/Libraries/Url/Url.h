
/* ANT URL broadcast wimp message number */
#define message_ANTOPENURL ((message_action) 0x4AF80)

/* Acorn URI messages */
#define message_URI_MPROCESS ((message_action) 0x4E382)
#define message_URI_MRETURNRESULT ((message_action) 0x4E383)
#define message_URI_MPROCESSACK ((message_action) 0x4E384)

/* Format of an Acorn URI_MProcess message */
typedef struct
{
  int flags;
  const char *uri;
  int uri_handle;
} message_uriprocess;

/* Used in the ANT URLOpen message */
typedef union
{
  char *ptr;
  int offset;
} string_value;


/* Format of an ANT URLOpen message */
typedef struct
{
  union
  {
    char url[236];
    struct
    {
      int tag;
      string_value url;
      int flags;
      string_value body_file;
      string_value target;
      string_value body_mimetype;
    } indirect;
  } data;
} message_anturl;

