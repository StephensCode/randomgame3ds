#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fileCreation(void);
u64* fileLoad(void);

int main(int argc, char **argv)
{
    gfxInitDefault();
    //Initializes the console to use the top screen and tells the console libary to use the internal data structure as the current.
    consoleInit(GFX_TOP, NULL);
    amInit();

    u32 titleCount = 0;
    AM_GetTitleCount(MEDIATYPE_SD, &titleCount);

    int selector = 0;

    FILE* title = fopen("randomizetitles.txt", "r");
    if (title == NULL){fileCreation();}
    fclose(title);

    u64* titlelist;
    titlelist = fileLoad();

    consoleClear();

    srand(time(NULL));
    int n = rand() % titleCount;

    u64 titleId = (u64)titlelist[n];

    //While loop for while application is running.
    while(aptMainLoop())
    {
        //Scanning for input each frame.
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_UP){if(selector > 0){selector--;}}
        if (kDown & KEY_DOWN){if(selector < 2){selector++;}}

        printf("\x1b[16;10HLaunch Random Game from SD Card");
        printf("\x1b[18;10HNumber of Games: %ld", titleCount);
        printf("\x1b[20;10HReload Games List File");

        if(selector == 0){printf("\x1b[16;10H");printf("\x1b[31mLaunch Random Game from SD Card\x1b[0m");}
        if(selector == 1){printf("\x1b[18;10H");printf("\x1b[31mNumber of Games: %ld\x1b[0m", titleCount);}
        if(selector == 2){printf("\x1b[20;10H");printf("\x1b[31mReload Games List File\x1b[0m");}

        printf("\x1b[1;1H");
        for (int i = 0; i < titleCount; i++)
        {
            printf("%llu\n", titlelist[i]);
        }

        if (kDown & KEY_START){break;}

        if (kDown & KEY_A)
        {
            if(selector == 0){aptSetChainloader(titleId, 1); free(titlelist); break;}
            if(selector == 2){consoleClear(); fileCreation(); consoleClear();}
        }
    
        // Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
    }
    gfxExit();
    amExit();
    return 0;
}

void fileCreation(void)
{
    //Here we find all the titles, and then write to the randomizetitles text file.
    printf("\x1b[16;10HReading Titles from the SD Card, please wait.");

    u32 titleCount = 0;
    AM_GetTitleCount(MEDIATYPE_SD, &titleCount);

    u64 *titleList = (u64 *)malloc(titleCount * sizeof(u64));

    u32 titlesRead;
    Result r = AM_GetTitleList(&titlesRead, MEDIATYPE_SD, titleCount, titleList);

    int counter = 0;

    FILE *titles = fopen("randomizetitles.txt", "w");

    while (counter < titlesRead)
    {
        fprintf(titles, "%llu\n", titleList[counter]);
        counter++;
    }
    fclose(titles);
    free(titleList);
    return;
}

u64* fileLoad(void)
{
    //Grabbing the title list from the text file
    u32 titleCount = 0;
    AM_GetTitleCount(MEDIATYPE_SD, &titleCount);

    FILE *titles = fopen("randomizetitles.txt", "r");
    u64 *titlelist = (u64 *)malloc(titleCount * sizeof(u64));
    int counter = 0;
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), titles) != NULL)
    {
        sscanf(buffer, "%llu", &titlelist[counter]);
        counter++;
    }
    fclose(titles);
    return titlelist;
}