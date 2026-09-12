
XOS_Bit                        EQU &020000

SWI_Font_CacheAddr                    EQU &040080
SWI_Font_FindFont                     EQU &040081
SWI_Font_LoseFont                     EQU &040082
SWI_Font_ReadDefn                     EQU &040083
SWI_Font_ReadInfo                     EQU &040084
SWI_Font_StringWidth                  EQU &040085
SWI_Font_Paint                        EQU &040086
SWI_Font_Caret                        EQU &040087
SWI_Font_ConverttoOS                  EQU &040088
SWI_Font_Converttopoints              EQU &040089
SWI_Font_SetFont                      EQU &04008a
SWI_Font_CurrentFont                  EQU &04008b
SWI_Font_FutureFont                   EQU &04008c
SWI_Font_FindCaret                    EQU &04008d
SWI_Font_CharBBox                     EQU &04008e
SWI_Font_ReadScaleFactor              EQU &04008f
SWI_Font_SetScaleFactor               EQU &040090
SWI_Font_ListFonts                    EQU &040091
SWI_Font_SetFontColours               EQU &040092
SWI_Font_SetPalette                   EQU &040093
SWI_Font_ReadThresholds               EQU &040094
SWI_Font_SetThresholds                EQU &040095
SWI_Font_FindCaretJ                   EQU &040096
SWI_Font_StringBBox                   EQU &040097
SWI_Font_ReadColourTable              EQU &040098
SWI_Font_MakeBitmap                   EQU &040099
SWI_Font_UnCacheFile                  EQU &04009a
SWI_Font_SetFontMax                   EQU &04009b
SWI_Font_ReadFontMax                  EQU &04009c
SWI_Font_ReadFontPrefix               EQU &04009d
SWI_Font_SwitchOutputToBuffer         EQU &04009e
SWI_Font_ReadFontMetrics              EQU &04009f
SWI_Font_DecodeMenu                   EQU &0400a0
SWI_Font_ScanString                   EQU &0400a1
SWI_Font_SetColourTable               EQU &0400a2
SWI_Font_CurrentRGB                   EQU &0400a3
SWI_Font_FutureRGB                    EQU &0400a4
SWI_Font_ReadEncodingFilename         EQU &0400a5
SWI_Font_FindField                    EQU &0400a6
SWI_Font_ApplyFields                  EQU &0400a7

SWI_Wimp_ReadSysInfo                  EQU &0400f2


        END
