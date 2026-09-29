
.equ	XOS_Bit, 0x020000

.equ	SWI_Font_CacheAddr, 0x040080
.equ	SWI_Font_FindFont, 0x040081
.equ	SWI_Font_LoseFont, 0x040082
.equ	SWI_Font_ReadDefn, 0x040083
.equ	SWI_Font_ReadInfo, 0x040084
.equ	SWI_Font_StringWidth, 0x040085
.equ	SWI_Font_Paint, 0x040086
.equ	SWI_Font_Caret, 0x040087
.equ	SWI_Font_ConverttoOS, 0x040088
.equ	SWI_Font_Converttopoints, 0x040089
.equ	SWI_Font_SetFont, 0x04008a
.equ	SWI_Font_CurrentFont, 0x04008b
.equ	SWI_Font_FutureFont, 0x04008c
.equ	SWI_Font_FindCaret, 0x04008d
.equ	SWI_Font_CharBBox, 0x04008e
.equ	SWI_Font_ReadScaleFactor, 0x04008f
.equ	SWI_Font_SetScaleFactor, 0x040090
.equ	SWI_Font_ListFonts, 0x040091
.equ	SWI_Font_SetFontColours, 0x040092
.equ	SWI_Font_SetPalette, 0x040093
.equ	SWI_Font_ReadThresholds, 0x040094
.equ	SWI_Font_SetThresholds, 0x040095
.equ	SWI_Font_FindCaretJ, 0x040096
.equ	SWI_Font_StringBBox, 0x040097
.equ	SWI_Font_ReadColourTable, 0x040098
.equ	SWI_Font_MakeBitmap, 0x040099
.equ	SWI_Font_UnCacheFile, 0x04009a
.equ	SWI_Font_SetFontMax, 0x04009b
.equ	SWI_Font_ReadFontMax, 0x04009c
.equ	SWI_Font_ReadFontPrefix, 0x04009d
.equ	SWI_Font_SwitchOutputToBuffer, 0x04009e
.equ	SWI_Font_ReadFontMetrics, 0x04009f
.equ	SWI_Font_DecodeMenu, 0x0400a0
.equ	SWI_Font_ScanString, 0x0400a1
.equ	SWI_Font_SetColourTable, 0x0400a2
.equ	SWI_Font_CurrentRGB, 0x0400a3
.equ	SWI_Font_FutureRGB, 0x0400a4
.equ	SWI_Font_ReadEncodingFilename, 0x0400a5
.equ	SWI_Font_FindField, 0x0400a6
.equ	SWI_Font_ApplyFields, 0x0400a7

.equ	SWI_Wimp_ReadSysInfo, 0x0400f2


