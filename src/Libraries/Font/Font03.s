@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Font_ReadDefn(FontHandle font, FontDefn *defn);

@       r0=font
@       r1=pointer to definition block

@       [r1,#0]  = Font name
@       [r1,#128] = x size
@       [r1,#132] = y size
@       [r1,#136] = x res
@       [r1,#140] = y res
@       [r1,#144] = age
@       [r1,#148] = usage

        
        .global Font_ReadDefn
Font_ReadDefn:
@
        STMFD   sp!, {r4,r5,r6,r7,lr}
        SWI     SWI_Font_ReadDefn + XOS_Bit
        STR     r2,[r1,#128]
        STR     r3,[r1,#132]
        STR     r4,[r1,#136]
        STR     r5,[r1,#140]
        STR     r6,[r1,#144]
        STR     r7,[r1,#148]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,r6,r7,pc}
@
