
XOS_Bit                         EQU &020000

SWI_PDriver_Info                EQU &080140
SWI_PDriver_CheckFeatures       EQU &080142
SWI_PDriver_PageSize            EQU &080143
SWI_PDriver_SelectJob           EQU &080145
SWI_PDriver_CurrentJob          EQU &080146
SWI_PDriver_EndJob              EQU &080148
SWI_PDriver_AbortJob            EQU &080149
SWI_PDriver_GiveRectangle       EQU &08014B
SWI_PDriver_DrawPage            EQU &08014C
SWI_PDriver_GetRectangle        EQU &08014D
SWI_PDriver_CancelJob           EQU &08014E
SWI_PDriver_ScreenDump          EQU &08014F
SWI_PDriver_EnumerateJobs       EQU &080150
SWI_PDriver_CancelJobWithError  EQU &080152
SWI_PDriver_SelectIllustration  EQU &080153
SWI_PDriver_InsertIllustration  EQU &080154

         END
