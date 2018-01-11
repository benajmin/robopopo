#ifndef roboPopo
#define roboPopo

#define FPS 30

#define SCREENW 648
#define SCREENH 480

#define MAX_NUM_LINES 5

#define NUM_LEVELS 14

#define NUM_BUTTON_BLOCKS 100
#define NUM_BLOCKS 200
#define NUM_ENEMIES 200

#define MAX_PLAYERS 20
#define MAX_LEVEL_TIME 5

#define PLAYERW 70
#define PLAYERH 70
#define PLAYERCW 54
#define PLAYERCH 62
#define PLAYERCX 8
#define PLAYERCY 8
#define PLAYER_SPEED 5
#define JUMP_SPEED 17

#define BLOCKW 24
#define BLOCKH 24
#define BLOCKCW 24
#define BLOCKCH 24
#define BLOCKCX 0
#define BLOCKCY 0

#define BUTTONW 24
#define BUTTONH 19

#define ENEMYW 56
#define ENEMYH 70
#define ENEMYCW 56
#define ENEMYCH 70
#define ENEMYCX 0
#define ENEMYCY 0

#define ENEMY_SPEED 4

#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
#define BACKCOL makecol(220, 220, 255)

struct Input{
       bool up, right, left, down;
};

struct OrdPair{
       int x, y;
};

struct Dimension{
       int w, h;
};

struct CollisionBox{
       OrdPair pos;
       Dimension dim;
};

struct Object{
       BITMAP *sprite;
       OrdPair pos;
       OrdPair vel;
       Dimension dim;
       CollisionBox collisionBox;
       bool visible;
       bool canJump;
};

struct Text{
       char text[70];
       OrdPair pos;
};

enum Direction {UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4};
enum PantColour {GREEN = 0, BLUE = 1, PURPLE = 2};

void init(Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[], Object &button, Object buttonBlocks[], 
     char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], Text onScreenText[NUM_LEVELS][MAX_NUM_LINES]);
     
void deinit();

Object initObject(Object object, int w, int h, int cx, int cy, int cw, int ch, BITMAP *sprite);

void draw(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Object button, Object buttonBlocks[], int frameCount, 
     Text onScreenText[MAX_NUM_LINES], int timer, int deaths);
int update(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Input pastInputs[][FPS*MAX_LEVEL_TIME*60],  Object &button, 
    Object buttonBlocks[], int &currentIteration, int &frameCount, char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], int &level, 
    int &timer, int &deaths);

Object move(Object object);
Object updatePlayer(Object player, Input input);

void resetLevel(Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[], int &frameCount, int &currentIteration, 
     Input pastInputs[][FPS*MAX_LEVEL_TIME*60], Object &button, Object buttonBlocks[]);
void loadLevel(char levelData[SCREENH/BLOCKH][SCREENW/BLOCKW], Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[], Object &button, 
     Object buttonBlocks[]);
void killPlayer(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Input pastInputs[][FPS*MAX_LEVEL_TIME*60], Object &button, 
     Object buttonBlocks[],  int &currentIteration, int &frameCount, char levelData[SCREENH/BLOCKH][SCREENW/BLOCKW]);

void animateSprite(Object obj, BITMAP * &buffer, int frameCount);

Input readInput();

#endif
