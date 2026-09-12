/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    File.AllocLoad.c
    Author:  Copyright © 1995 Julian Smith
    Version: 1.01 (22 Jul 1995)
    Mods:    Changed name from File_AllocLoad to File_AllocLoad0
*/

#include <stdlib.h>

#include "DeskLib:File.h"


char	*File_AllocLoad0(const char *filename)
{
char	*buffer;
int	length;

if ( !File_Exists( filename))	return NULL;

File_GetLength( filename, &length);

buffer = malloc( 1+length);
if ( !buffer)	return NULL;

if ( File_LoadTo( filename, buffer, NULL))	{
	free( buffer);
	return NULL;
	}

buffer[ length] = 0;

return buffer;
}

