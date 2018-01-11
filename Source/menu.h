#ifndef menu
#define menu

extern FONT *fonts[];
extern BITMAP *playerSprite, *playerSpriteGrey;
extern volatile long ticks;

int openMenu();
int openControls();

void drawCredits();

void drawControls();
void drawMenu();
int drawEnd(int timer, int deaths);

#endif
