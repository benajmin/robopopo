#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "robopopo.h"
#include "file.h"
#include "fonts.h"
#include "levels.h"
#include "sprites.h"

//Loads all sizes of lucida console from datfile to globaal array of fonts
int loadFonts(){
     DATAFILE *fontFile = NULL;
     fontFile = load_datafile("fonts.dat");
     
     if (fontFile == NULL)
        return 1;
     
     //Iterate through sizes
     for (int i = 0; i <= S20; i++){
         fonts[i] = (FONT*) fontFile[i].dat;
         
         if (fonts[i] == NULL)
            return 1;
     }
     
     return 0;
}

//Load level data into 3d array from datafile
void loadLevelData(char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], Text onScreenText[NUM_LEVELS][MAX_NUM_LINES]){
    PACKFILE *levelDataFile;
    char fileName[20];
    char buffer[100] = {' '};
    int numLines = 0;
    
    //Iterate throught levels
    for (int i = 0; i < NUM_LEVELS; i++){
        //load pack(text)file from datafile

        sprintf(fileName, "levels.dat#level%02d", i+1);
        
        levelDataFile = pack_fopen(fileName, "r");
        
        //Such Error checking
        if (levelDataFile == NULL)
           printf("AAAAAAAAAAAAAAAAAAAA");
        
        //Itterate through rows in text fiel
        for (int j = 0; j < SCREENH/BLOCKH; j++){
            //read in row of file
            pack_fgets(levelData[i][j], SCREENW/BLOCKW+2, levelDataFile);
            
            //Print level data as read in for error checking
            /*for (int k = 0; k < SCREENW/BLOCKW; k++){
                printf("%c", levelData[i][j][k]);
            }
            printf("\n");*/
        }
        //printf("\n");
        
        pack_fgets(buffer, SCREENW/BLOCKW+2, levelDataFile);
        sscanf(buffer, "%d", &numLines);
        
        for (int j = 0; j < numLines; j++){
            pack_fgets(onScreenText[i][j].text, 70, levelDataFile);
            pack_fgets(buffer, SCREENW/BLOCKW+2, levelDataFile);
            sscanf(buffer, "%d %d", &onScreenText[i][j].pos.x, &onScreenText[i][j].pos.y);
            //printf("%d, %d", i, j);
            //printf("%d %d\n", onScreenText[i][j].pos.x, onScreenText[i][j].pos.y);
        }
    }
    
    pack_fclose(levelDataFile);
    return;
}
