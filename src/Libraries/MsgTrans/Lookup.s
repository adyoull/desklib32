



.equ	XOS_Bit, 0x020000

.equ	SWI_MessageTrans_FileInfo, 0x041500
.equ	SWI_MessageTrans_OpenFile, 0x041501
.equ	SWI_MessageTrans_Lookup, 0x041502
.equ	SWI_MessageTrans_CloseFile, 0x041504

        .global MessageTrans_Lookup

MessageTrans_Lookup:

        MOV     ip,sp
        STMFD   sp!,{v1,v2,v3,v4,v5,v6,lr}

        MOV     r8,r2
        MOV     r9,r3
        LDR     r2,[r8]
        LDR     r3,[r9]
        LDMIA   ip,{r4,r5,r6,r7}
        SWI     SWI_MessageTrans_Lookup + XOS_Bit
        STR     r2,[r8]
        STR     r3,[r9]

        MOVVC   a1,#0
        LDMFD   sp!,{v1,v2,v3,v4,v5,v6,pc}

