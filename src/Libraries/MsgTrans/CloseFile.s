


.equ	XOS_Bit, 0x020000

.equ	SWI_MessageTrans_FileInfo, 0x041500
.equ	SWI_MessageTrans_OpenFile, 0x041501
.equ	SWI_MessageTrans_Lookup, 0x041502
.equ	SWI_MessageTrans_CloseFile, 0x041504

        .global MessageTrans_CloseFile

MessageTrans_CloseFile:

        STMFD   sp!,{lr}

        SWI     SWI_MessageTrans_CloseFile + XOS_Bit

        MOVVC   a1,#0
        LDMFD   sp!,{pc}

