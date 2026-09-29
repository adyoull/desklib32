;
;       Title                  : Font menu creation for RISC OS 3
;       System                 : Menu library
;       Version                : 1.0
;                              : 1.01 - Renamed to Menu_FontMenu
;       Copyright              : (c) Ben Summers
;       Date                   : 21 Oct 94
;       Author                 : Ben Summers
;
;       Function               : Makes a font menu
;
;
;       Modification history.
;
;       Version                :
;       Date                   :
;       Author                 :
;       Changes                :
;
;
;============================================================================
;
;  Include files.
;
;============================================================================
;
        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
;============================================================================
;
;  Code.
;
;============================================================================
;
        PREAMBLE

XFont_ListFonts EQU &40091 + XOS_Bit
XFont_DecodeMenu EQU &400A0 + XOS_Bit

        IMPORT  free
        IMPORT  malloc

          ; a1 = BOOL sysfont
          ; a2 = char *tick

        STARTCODE Menu_FontMenu

        STMFD   sp!,{v1-v6,lr}
        MOV     v5,a1
        MOV     v6,a2

        BL      free_data

        MOV     a2,#0
        CMP     v5,#0
        MOVEQ   a3,#2_101000:SHL:16
        MOVNE   a3,#2_111000:SHL:16
        MOV     v1,#0
        MOV     v3,v6
        SWI     XFont_ListFonts
        BVS     exit_err

        MOV     v4,a4           ; size of font menu buffer
        MOV     v3,v2           ; size of indirected text buffer

        MOV     a1,a4
        BL      malloc
        CMP     a1,#0           ; got a buffer?
        BEQ     exit_err
        MOV     v1,a1           ; store it...
        MOV     a1,v3           ; ind data
        BL      malloc
        CMP     a1,#0
        BEQ     no_ind_block
        STR     a1,fontmenu_ind
        STR     v1,fontmenu_def ; store the pointers

        MOV     a2,v1           ; font menu definition
        CMP     v5,#0
        MOVEQ   a3,#2_101000:SHL:16
        MOVNE   a3,#2_111000:SHL:16
        MOV     a4,v4           ; size of font menu buffer
        MOV     v1,a1           ; indirected data
        MOV     v2,v3           ; size of ind data
        MOV     v3,v6
        SWI     XFont_ListFonts
        BVS     exit_err_free

; SWI 256+7
exit_err
        LDR     a1,fontmenu_def
        LDMFD   sp!,{v1-v6,pc}

no_ind_block
        MOV     a1,v1
        BL      free            ; doesn't preseve flags
        B       exit_err

exit_err_free
        BL      free_data
        B       exit_err

free_data
        STMFD   sp!,{lr}
        LDR     a1,fontmenu_def
        CMP     a1,#0
        BLNE    free
        LDR     a1,fontmenu_ind
        CMP     a1,#0
        BLNE    free
        MOV     a1,#0
        STR     a1,fontmenu_def
        STR     a1,fontmenu_ind
        LDMFD   sp!,{pc}

        EXPORT  Menu_FontMenuDecode

          ; a1 = pointer to selection
Menu_FontMenuDecode
        STMFD   sp!,{v1-v4,lr}
        MOV     v4,a1
        MOV     a3,a1
        MOV     a1,#0
        LDR     a2,fontmenu_def
        MOV     a4,#0           ; find size of answer
        SWI     XFont_DecodeMenu
        BVS     mfmd_err

        LDR     a1,answer_size
        CMP     a1,v1
        BGE     mfmd_got_enough

        LDR     a1,answer
        BL      free
        MOV     a1,v1
        BL      malloc
        STR     a1,answer
        CMP     a1,#0
        BEQ     mfmd_err
        STR     v1,answer_size

mfmd_got_enough

        MOV     a1,#0
        LDR     a2,fontmenu_def
        MOV     a3,v4
        LDR     a4,answer
        SWI     XFont_DecodeMenu
        BVS     mfmd_err

        LDR     a1,answer
        LDMFD   sp!,{v1-v4,pc}

mfmd_err
        MOV     a1,#0
        LDMFD   sp!,{v1-v4,pc}

menu_fontmenu
fontmenu_def    ; these should really be in an area of their own... but what the hell...
        DCD     0
fontmenu_ind
        DCD     0
answer
        DCD     0
answer_size
        DCD     0

        EXPORT  menu_fontmenu

          ; two seperate variables instead of one large block to help fit the blocks into
          ; odd places in the heap

        END
