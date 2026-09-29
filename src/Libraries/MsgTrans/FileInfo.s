


.equ	XOS_Bit, 0x020000

.equ	SWI_MessageTrans_FileInfo, 0x041500
.equ	SWI_MessageTrans_OpenFile, 0x041501
.equ	SWI_MessageTrans_Lookup, 0x041502
.equ	SWI_MessageTrans_CloseFile, 0x041504

        .global MessageTrans_FileInfo

MessageTrans_FileInfo:

        STMFD   sp!,{v1,v2,lr}

        MOV     v1,a2
        MOV     v2,a3
        MOV     a2,a1
        SWI     SWI_MessageTrans_FileInfo + XOS_Bit
        STR     a1,[v1]
        STR     a3,[v2]

        MOVVC   a1,#0
        LDMFD   sp!,{v1,v2,pc}



