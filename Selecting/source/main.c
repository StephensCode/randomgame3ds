#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    gfxInitDefault();
    //Initializes the console to use the top screen and tells the console libary to use the internal data structure as the current.
    consoleInit(GFX_TOP, NULL);
    amInit();

    u32 titleCount = 0;
    AM_GetTitleCount(MEDIATYPE_SD, &titleCount);

    u64 *titleList = (u64 *)malloc(titleCount * sizeof(u64));

    u32 titlesRead;
    Result r = AM_GetTitleList(&titlesRead, MEDIATYPE_SD, titleCount, titleList);

    srand(time(NULL));
    int n = rand() % titlesRead;

    u64 titleId = (u64)titleList[n];

    int selector = 0;

    //While loop for while application is running.
    while(aptMainLoop())
    {
        //Scanning for input each frame.
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_UP){if(selector > 0){selector--;}}
        if (kDown & KEY_DOWN){if(selector < 1){selector++;}}

        printf("\x1b[16;10HLaunch Random Game from SD Card");
        printf("\x1b[18;10HNumber of Games: %ld", titleCount);

        if(selector == 0){printf("\x1b[16;10H");printf("\x1b[31mLaunch Random Game from SD Card\x1b[0m");}
        if(selector == 1){printf("\x1b[18;10H");printf("\x1b[31mNumber of Games: %ld\x1b[0m", titleCount);}

        if (kDown & KEY_START){free(titleList); break;}

        if (kDown & KEY_A)
        {
            if(selector == 0){aptSetChainloader(titleId, 1); free(titleList); break;}
        }
    
        // Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
    }
    gfxExit();
    return 0;
}