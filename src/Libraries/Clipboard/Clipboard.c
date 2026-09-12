/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Clipboard.h
    Author:  Copyright © 2005 Peter Naulls
    Version: 1.00 (5th July 2005)
    Purpose: RISC OS Clipboard manipulation
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:Clipboard.h"
#include "DeskLib:WimpMsg.h"
#include "DeskLib:WimpSWIs.h"

#include <string.h>

static BOOL clipboard_init;

static BOOL clipboard_held = FALSE;

static paste_handler *phandler = NULL;

static void Clipboard_Init(void) {


}



void Clipboard_Claim(BOOL Selection, BOOL Clipboard) {
  message_block message;

  message.header.size      = 24;
  message.header.yourref   = 0;
  message.header.action    = message_CLAIMENTITY;
  message.data.claimentity.selectionclaimed = (Selection ? 3 : 0);
  message.data.claimentity.clipboardclaimed = Clipboard;

  Wimp_SendMessage(event_SEND, &message, 0, 0);
}


void Clipboard_Request(window_handle window, unsigned int internal, BOOL send,
                       paste_handler *paste, int *filetypes) {
  message_block message;
  int *filetype = message.data.datarequest.filetypes;
  int num = 0;

  phandler = paste;

  memset(&message, 0, sizeof(message));

  message.header.yourref   = 0;
  message.header.action    = message_DATAREQUEST;

  message.data.datarequest.window          = window;
  message.data.datarequest.internal_handle = internal;
  message.data.datarequest.senddata        = send;

  if (filetypes) {
    while (*filetypes != -1 && num < 50) {
      *filetype = *filetypes;
      filetype++;
      filetypes++;
      num++;
    }
  }
  *filetype = -1;

  message.header.size = 44 + num;

  Wimp_SendMessage(event_SEND, &message, 0, 0);
}


static BOOL Clipboard_RamTransmit(event_pollblock *event, void *reference) {



  return FALSE;
}

