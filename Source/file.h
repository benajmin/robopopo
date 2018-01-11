#ifndef file
#define file

extern FONT *fonts[];

int loadFonts();
void loadLevelData(char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], Text onScreenText[NUM_LEVELS][MAX_NUM_LINES]);

#endif
