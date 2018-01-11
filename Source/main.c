/*Benjamin Wilkins      ICS3U       May 22 2015
RoboPopo - Cool platformer game, see proposal for more info
*/
#include <stdio.h>
#include <math.h>
#include <allegro.h>
#include "robopopo.h"
#include "menu.h"
#include "collision.h"
#include "file.h"
#include "fonts.h"

BITMAP *playerSprite, *playerSpriteGrey, *blockSprite, *buttonSprite;
BITMAP *enemySprite[3];
SAMPLE *music;
FONT *fonts[20];

//Regulating FPS
volatile long ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(increment_speed_counter)

int main(int argc, char *argv[]) {
    Object player;
    Object blocks[NUM_BLOCKS], enemies[3][NUM_ENEMIES], players[MAX_PLAYERS], button, buttonBlocks[NUM_BUTTON_BLOCKS];
    int currentIteration = 0, frameCount = 0, level = 0, timer = 0, deaths = 0;
    Input pastInputs[MAX_PLAYERS][FPS*MAX_LEVEL_TIME*60] = {0};
    char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW];
    Text onScreenText[NUM_LEVELS][MAX_NUM_LINES] = {0};

    init(blocks, enemies, players, button, buttonBlocks, levelData, onScreenText);

    //play_sample(music, 255, 127, 1000, true);

    resetLevel(blocks, enemies, players, frameCount, currentIteration, pastInputs, button, buttonBlocks);

    loadLevel(levelData[0], blocks, enemies, players, button, buttonBlocks);

    if (openMenu())
       return 0;

    ticks = 0;

    while (!key[KEY_ESC]){
          while (ticks == 0)
                rest(1);
          while (ticks > 0){
                if (update(players, blocks, enemies, pastInputs, button, buttonBlocks, currentIteration, frameCount, levelData, level, timer, deaths))
                   return 0;

                draw(players, blocks, enemies, button, buttonBlocks, frameCount, onScreenText[level], timer, deaths);
                timer++;
                frameCount++;
                ticks --;
          }
    }

   	deinit();
	return 0;
}
END_OF_MAIN()

void init(Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[], Object &button, Object buttonBlocks[], char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], Text onScreenText[NUM_LEVELS][MAX_NUM_LINES]) {
	printf("Initializing Allegro...\n");
    int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREENW, SCREENH, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	set_window_title("RoboPopo");


	install_int_ex(ticker, BPS_TO_TIMER(FPS));

    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0 );
	install_timer();
	install_keyboard();
	install_mouse();

	/* add other initializations here */
	//Load Bitmaps
	printf("Loading Bitmaps...\n");

	playerSprite = load_bitmap("bitmaps/playerSprites.bmp", NULL);
	playerSpriteGrey = load_bitmap("bitmaps/playerSpritesGrey.bmp", NULL);

	blockSprite = load_bitmap("bitmaps/block.bmp", NULL);

	buttonSprite = load_bitmap("bitmaps/button.bmp", NULL);

	enemySprite[GREEN] = load_bitmap("bitmaps/enemySpritesGreen.bmp", NULL);
	enemySprite[BLUE] = load_bitmap("bitmaps/enemySpritesBlue.bmp", NULL);
	enemySprite[PURPLE] = load_bitmap("bitmaps/enemySpritesPurple.bmp", NULL);

	//Loading Fonts
	printf("Loading Fonts...\n");

	if (loadFonts()){
	   printf("\n\n\n********Error loading fonts********\n\n\n");
    }

    //Check for errors
	if (playerSprite == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Player Sprites********\n\n\n\n");
    if (playerSpriteGrey == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Player Sprites Grey********\n\n\n\n");
    if (enemySprite[GREEN] == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Enemy Sprite Green********\n\n\n\n");
    if (enemySprite[BLUE] == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Enemy Sprite Blue********\n\n\n\n");
    if (enemySprite[PURPLE] == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Enemy Sprite Purple********\n\n\n\n");
	if (blockSprite == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Block Sprites********\n\n\n\n");
	if (buttonSprite == NULL)
        printf("\n\n\n********Error Loading Bitmaps - Button Sprites********\n\n\n\n");

	printf("Loading Music...\n");
	//music = load_sample("We_Fly_How_We_Fly.wav");

	if (music == NULL)
	   printf("\n\n\n********Error Loading Music********\n\n\n\n");

	printf("Loading Level Data...\n");
    loadLevelData(levelData, onScreenText);

    //Initialize objects
	printf("Initializing Object Data...\n");

    //Initialize blocks
    for (int i = 0; i < NUM_BLOCKS; i++){
        blocks[i] = initObject(blocks[i], BLOCKW, BLOCKH, BLOCKCX, BLOCKCY, BLOCKCW, BLOCKCH, blockSprite);
    }

    for (int i = 0; i < NUM_BUTTON_BLOCKS; i++){
        buttonBlocks[i] = initObject(blocks[i], BLOCKW, BLOCKH, BLOCKCX, BLOCKCY, BLOCKCW, BLOCKCH, blockSprite);
    }

    button = initObject(button, BUTTONW, BUTTONH, 0, 0, BUTTONW, BUTTONH, buttonSprite);

    //Initialize past players of hero
    for (int i = 0; i < MAX_PLAYERS; i++){
        players[i] = initObject(players[i], PLAYERW, PLAYERH, PLAYERCX, PLAYERCY, PLAYERCW, PLAYERCH, playerSpriteGrey);
    }

    for (int i = 0; i < NUM_ENEMIES; i++){
        enemies[0][i] = initObject(enemies[0][i], ENEMYW, ENEMYH, ENEMYCX, ENEMYCY, ENEMYCW, ENEMYCH, enemySprite[GREEN]);
        enemies[1][i] = initObject(enemies[1][i], ENEMYW, ENEMYH, ENEMYCX, ENEMYCY, ENEMYCW, ENEMYCH, enemySprite[BLUE]);
        enemies[2][i] = initObject(enemies[2][i], ENEMYW, ENEMYH, ENEMYCX, ENEMYCY, ENEMYCW, ENEMYCH, enemySprite[PURPLE]);
    }

    return;
}

void deinit() {
	clear_keybuf();

	destroy_bitmap(blockSprite);
	destroy_bitmap(playerSprite);
	destroy_bitmap(playerSpriteGrey);

	destroy_sample(music);
	/* add other deinitializations here */
}

Object initObject(Object object, int w, int h, int cx, int cy, int cw, int ch, BITMAP *sprite){
     object.pos.x = -100;
     object.pos.y = 0;
     object.vel.x = 0;
     object.vel.y = 0;
     object.dim.w = w;
     object.dim.h = h;
     object.collisionBox.pos.x = cx;
     object.collisionBox.pos.y = cy;
     object.collisionBox.dim.w = cw;
     object.collisionBox.dim.h = ch;
     object.visible = false;
     object.sprite = sprite;
     object.canJump = false;

     return object;
}

void draw(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Object button, Object buttonBlocks[], int frameCount, Text onScreenText[MAX_NUM_LINES], int timer, int deaths){
     BITMAP *buffer = create_bitmap(SCREENW, SCREENH);

     clear_to_color(buffer, BACKCOL);

     //Draw on screen Text
     for (int i = 0; i < MAX_NUM_LINES; i++){
         if (onScreenText[i].pos.x != 0){
            textprintf_ex(buffer, fonts[LC16], onScreenText[i].pos.x, onScreenText[i].pos.y, BLACK, -1, "%s", onScreenText[i].text);
         }
     }

     //Draw button
     if (button.visible){
        masked_blit(button.sprite, buffer, 0, 0, button.pos.x, button.pos.y, button.dim.w, button.dim.h);
     }

     //draw player to buffer
     for (int i = 0; i < MAX_PLAYERS; i++){
         if (players[i].visible) {
            animateSprite(players[i], buffer, frameCount);
         }
     }

     //Draw blocks appear and disappear with button press
     for (int i = 0; i < NUM_BUTTON_BLOCKS; i++){
         if (buttonBlocks[i].visible){
            masked_blit(buttonBlocks[i].sprite, buffer, 0, 0, buttonBlocks[i].pos.x, buttonBlocks[i].pos.y, buttonBlocks[i].dim.w, buttonBlocks[i].dim.h);
         }
     }

     //draw blocks
     for (int i = 0; i < NUM_BLOCKS; i++){
         if (blocks[i].visible){
            masked_blit(blocks[i].sprite, buffer, 0, 0, blocks[i].pos.x, blocks[i].pos.y, blocks[i].dim.w, blocks[i].dim.h);
         }
     }

     //draw enemies
     for (int i = 0; i < NUM_ENEMIES; i++){
         for (int j = 0; j < 3; j++){
             if (enemies[j][i].visible){
                //printf("%d  %d\n", j, i);
                animateSprite(enemies[j][i], buffer, frameCount);
             }
         }
     }

     //display time and health
     textprintf_right_ex(buffer, fonts[LC12], SCREENW - 10, 10, BLACK, BACKCOL, "%d:%2d", timer/(30*60), (timer/30)%60);
     textprintf_right_ex(buffer, fonts[LC12], SCREENW - 100, 10, BLACK, BACKCOL, "%d Deaths", deaths);

     blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);

     destroy_bitmap(buffer);

     return;
}

int update(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Input pastInputs[][FPS*MAX_LEVEL_TIME*60],  Object &button, Object buttonBlocks[],
     int &currentIteration, int &frameCount, char levelData[NUM_LEVELS][SCREENH/BLOCKH][SCREENW/BLOCKW], int &level, int &timer, int &deaths){

     //INPUT
     //record input to array of past inputs
     pastInputs[currentIteration][frameCount] = readInput();

     //Suicide Button and falling off screen
     if ((key[KEY_R] || players[currentIteration].pos.y > SCREENH) && frameCount > 30 ){
        killPlayer(players, blocks, enemies, pastInputs, button, buttonBlocks, currentIteration, frameCount, levelData[level]);
        deaths++;
     }

     if (key[KEY_X]){ // kill player
           resetLevel(blocks, enemies, players, frameCount, currentIteration, pastInputs, button, buttonBlocks);
           loadLevel(levelData[level], blocks, enemies, players, button, buttonBlocks);
        }

     //NEXT LEVEL
     if (players[currentIteration].pos.x >= SCREENW - 10 && players[currentIteration].pos.y > 0){
        level ++;
        resetLevel(blocks, enemies, players, frameCount, currentIteration, pastInputs, button, buttonBlocks);
        loadLevel(levelData[level], blocks, enemies, players, button, buttonBlocks);
     }

     //END GAME
     if (level >= NUM_LEVELS){
        drawCredits();
        if (drawEnd(timer, deaths)){
           deaths = 0;
           timer = 0;
           level = 0;
           resetLevel(blocks, enemies, players, frameCount, currentIteration, pastInputs, button, buttonBlocks);
           loadLevel(levelData[level], blocks, enemies, players, button, buttonBlocks);
           frameCount = 0;
           ticks = 0;
        }
        else{
            return 1;
        }
     }

     //Assume button is not pressed
     if (button.visible){
         for (int i = 0; i < NUM_BUTTON_BLOCKS; i++){
             buttonBlocks[i].visible = true;
         }
     }

     for (int i = 0; i < MAX_PLAYERS; i++){
         if (players[i].visible){
             //collide with button
             if (detectCollision(players[i], button) && button.visible){
                for (int j = 0; j < NUM_BUTTON_BLOCKS; j++){
                    buttonBlocks[j].visible = false;
                }
             }
         }
     }

     //Cycle through enemies
     for (int i = 0; i < NUM_ENEMIES; i++){
         for (int k = 0; k < 3; k++){
             if (enemies[k][i].visible){

                //COLLISION
                //collide enemy with button
                if (detectCollision(enemies[k][i], button)){
                    for (int j = 0; j < NUM_BUTTON_BLOCKS; j++){
                        buttonBlocks[j].visible = false;
                    }
                }

                //enemy AI for green and blue pantsed enemies
                if (k == GREEN || k == BLUE){
                    //Collide with blocks
                    for (int j = 0; j < NUM_BLOCKS; j++){
                        if (blocks[j].visible){
                           //bounce off blocks
                           enemies[k][i] = reactCollision(enemies[k][i], blocks[j], true);
                        }
                    }
                    //collide with blocks controlled by button
                    for (int j = 0; j < NUM_BUTTON_BLOCKS; j++){
                         if (buttonBlocks[j].visible){
                            //bounce of button controlled blocks
                             enemies[k][i] = reactCollision(enemies[k][i], buttonBlocks[j], true);
                         }
                     }

                    //loop enemies back to top of screen
                    if (enemies[k][i].pos.y > SCREENH ){
                       enemies[k][i].pos.y = (-1) * enemies[k][i].dim.h;
                    }
                }else if(k == PURPLE){//PURPLE PANTSED ENEMY AI
                    //Track player
                    if (players[currentIteration].pos.x > enemies[PURPLE][i].pos.x + enemies[PURPLE][i].dim.w/2){
                       enemies[PURPLE][i].vel.x = ENEMY_SPEED;
                    }else if (players[currentIteration].pos.x < enemies[PURPLE][i].pos.x - enemies[PURPLE][i].dim.w/2){
                       enemies[PURPLE][i].vel.x = ENEMY_SPEED * (-1);
                    }else{
                       enemies[PURPLE][i].vel.x = 0;
                    }
                    //collide purple enmies with blocks
                    for (int j = 0; j < NUM_BLOCKS; j++){
                        if (blocks[j].visible){
                           enemies[k][i] = reactCollision(enemies[k][i], blocks[j], false);

                        }
                    }
                    //collide purple enemies with button blocks
                    for (int j = 0; j < NUM_BUTTON_BLOCKS; j++){
                         if (buttonBlocks[j].visible){
                             enemies[k][i] = reactCollision(enemies[k][i], buttonBlocks[j], false);
                         }
                     }
                }

                //TERMINAL VELOCITY
                // stop clipping through blocks
                if (enemies[k][i].vel.y > 24){
                    enemies[k][i].vel.y = 24;
                }

                //Apply Gravity
                enemies[k][i].vel.y += 1;

                enemies[k][i] = move(enemies[k][i]);
             }
         }
     }

     //Loop through all past players
     for (int i = 0; i < MAX_PLAYERS; i++){
         //Collide iteration with stuff if its visible
         if(players[i].visible){

             //update iteration with inputs for current frame
             players[i] = updatePlayer(players[i], pastInputs[i][frameCount]);

             //Collide with left edge of screen
             if (players[i].pos.x + players[i].collisionBox.pos.x <= 0 && players[i].vel.x < 0){
                players[i].vel.x = 0;
                players[i].pos.x = - players[i].collisionBox.pos.x;
             }

             //Collide with right side of screen if player is above screen
             if (players[i].pos.x + players[i].collisionBox.pos.x >= SCREENW && players[i].vel.x >= 0 && players[i].pos.y < 0){
                players[i].vel.x = 0;
                players[i].pos.x = SCREENW - players[i].collisionBox.pos.x - 30;
             }

             players[i].canJump = false;

             //Collision detection
             //collide with all blocks
             for (int j = 0; j < NUM_BLOCKS; j++){
                 if (blocks[j].visible){
                     players[i] = reactCollision(players[i], blocks[j], false);
                 }
             }

             //collide with blocks controlled by button
             for (int j = 0; j < NUM_BUTTON_BLOCKS; j++){
                 if (buttonBlocks[j].visible){
                     players[i] = reactCollision(players[i], buttonBlocks[j], false);
                 }
             }

             //collide with all other players
             for (int j = 0; j < MAX_PLAYERS; j++){
                 if (players[j].visible){
                     players[i] = reactUpCollision(players[i], players[j]);
                 }
             }

             //COLLIDE WITH ENEMIES
             //collide with green enemies
             for (int j = 0; j < NUM_ENEMIES; j++){
                 if (enemies[GREEN][j].visible){
                     switch (detectCollision(players[i], enemies[GREEN][j])){
                            //Squash green pantsed enemies
                            case UP: enemies[GREEN][j].visible = false;
                                 players[i].vel.y = -2;
                                 break;
                            case 0: break; // No collision
                            default: //Player dies
                                 players[i].visible = false;
                                 if (i == currentIteration){
                                     killPlayer(players, blocks, enemies, pastInputs, button, buttonBlocks, currentIteration, frameCount, levelData[level]);
                                     deaths++;
                                 }
                                 break;
                     }
                 }

                 //Collide with blue pantsed enemies
                 if (enemies[BLUE][j].visible){
                     switch (detectCollision(players[i], enemies[BLUE][j])){
                            case 0: break; //No collsion
                            default: //Kill both player and enemy
                                 enemies[BLUE][j].visible = false;
                                 players[i].visible = false;
                                 if (i == currentIteration){
                                    killPlayer(players, blocks, enemies, pastInputs, button, buttonBlocks, currentIteration, frameCount, levelData[level]);
                                    deaths++;
                                 }
                                 break;
                     }
                 }

                 //Collide with purple pantsed enemies
                 if (enemies[PURPLE][j].visible){
                     switch (detectCollision(players[i], enemies[PURPLE][j])){
                            case 0: break;//No colision
                            default: //Player dies
                                 players[i].visible = false;
                                 if (i == currentIteration){
                                    deaths++;
                                    killPlayer(players, blocks, enemies, pastInputs, button, buttonBlocks, currentIteration, frameCount, levelData[level]);
                                 }
                                 break;
                     }
                 }
             }
         }
         //move players
         players[i] = move(players[i]);
     }

     return 0;
}

//move object according to velocities
Object move(Object object){
     object.pos.x += object.vel.x;
     object.pos.y += object.vel.y;

     return object;
}

//set players velocities according to input, apply gravity/friction
Object updatePlayer(Object player, Input input){
     //apply friction
     if (player.vel.x > 0)
        player.vel.x-= 1;
     else if (player.vel.x < 0)
        player.vel.x+=1;

     //change velocities according to input
     if (input.right)
        player.vel.x = PLAYER_SPEED;
     else if (input.left)
        player.vel.x = - PLAYER_SPEED;

     if (input.up && player.canJump){
        player.vel.y = - JUMP_SPEED;
        player.canJump = false;
     }else if (input.down){
        player.vel.y += 3;
     }

     //TERMINAL VELOCITY
     // stop clipping through blocks
     if (player.vel.y > 24){
        player.vel.y = 24;
     }

     //Apply Gravity
     player.vel.y += 1;

     return player;
}

//Returns current key presses as structure
Input readInput(){
      Input input = {false};

      if (key[KEY_RIGHT] || key[KEY_D]){
         input.right = true;
      }
      else if (key[KEY_LEFT] || key[KEY_A]){
         input.left = true;
      }

      if (key[KEY_UP] || key[KEY_W]){
         input.up  = true;
      }else if (key[KEY_DOWN] || key[KEY_S]){
         input.down = true;
      }

      return input;
}

//Clear past inputs and make everything invisible
void resetLevel(Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[], int &frameCount, int &currentIteration, Input pastInputs[][FPS*MAX_LEVEL_TIME*60],  Object &button, Object buttonBlocks[]){
    players[0].visible = true;

    //Clear past inputs
    for (int i = 0; i < MAX_PLAYERS; i++){
        for (int j = 0; j < FPS*MAX_LEVEL_TIME*60; j++){
            pastInputs[i][j].up = false;
            pastInputs[i][j].down = false;
            pastInputs[i][j].right = false;
            pastInputs[i][j].left = false;
        }
    }

    //Make all players but first invisible
    for (int i = 1; i < MAX_PLAYERS; i++)
         players[i].visible = false;

    //Giva all iterations of players living sprite
    for (int i = 0; i < MAX_PLAYERS; i++){
        players[i].sprite = playerSprite;
    }

    //make blocks invisible
    for (int i = 0; i < NUM_BLOCKS; i++){
         blocks[i].visible = false;
    }

    //make blocks invisible
    for (int i = 0; i < NUM_BUTTON_BLOCKS; i++){
         buttonBlocks[i].pos.x = -100;
    }

    //make button invisible
    button.visible = false;

    //make enemies invisible
    for (int i = 0; i < NUM_ENEMIES; i++){
         enemies[GREEN][i].visible = false;
         enemies[BLUE][i].visible = false;
         enemies[PURPLE][i].visible = false;
    }

    frameCount = 0;
    currentIteration = 0;
}

//places blocks and players in correct place according to array of objects in level
void loadLevel(char levelData[SCREENH/BLOCKH][SCREENW/BLOCKW], Object blocks[], Object enemies[3][NUM_ENEMIES], Object players[],  Object &button, Object buttonBlocks[]){
     int block = 0, enemy[3] = {0}, buttonBlock = 0;

     //iterate through rows
     for (int i = 0; i < SCREENH/BLOCKH; i++){
         //iterate through collumns
         for (int j = 0; j < SCREENW/BLOCKW; j++){
             //check what character is there/what object should go there
             switch (levelData[i][j]){
                    case '0':
                         //place block and make visible
                         blocks[block].visible = true;
                         blocks[block].pos.y = i * BLOCKH;
                         blocks[block].pos.x = j * BLOCKW;
                         block++;
                         break;
                    case 'X':
                         //Place all iterations of player their
                         for (int k = 0; k < MAX_PLAYERS; k++){
                             players[k].pos.y = i * BLOCKH;
                             players[k].pos.x = j * BLOCKW;
                         }
                         break;
                    case '9':
                         //place enemy and make visible
                         enemies[GREEN][enemy[GREEN]].visible = true;
                         enemies[GREEN][enemy[GREEN]].pos.y = i * BLOCKH;
                         enemies[GREEN][enemy[GREEN]].pos.x = j * BLOCKW;
                         enemies[GREEN][enemy[GREEN]].vel.x = ENEMY_SPEED;

                         enemy[GREEN]++;
                         break;
                    case '8':
                         //place enemy and make visible
                         enemies[BLUE][enemy[BLUE]].visible = true;
                         enemies[BLUE][enemy[BLUE]].pos.y = i * BLOCKH;
                         enemies[BLUE][enemy[BLUE]].pos.x = j * BLOCKW;
                         enemies[BLUE][enemy[BLUE]].vel.x = ENEMY_SPEED;

                         enemy[BLUE]++;
                         break;
                    case '7':
                         //place enemy and make visible
                         enemies[PURPLE][enemy[PURPLE]].visible = true;
                         enemies[PURPLE][enemy[PURPLE]].pos.y = i * BLOCKH;
                         enemies[PURPLE][enemy[PURPLE]].pos.x = j * BLOCKW;
                         enemies[PURPLE][enemy[PURPLE]].vel.x = ENEMY_SPEED;

                         enemy[PURPLE]++;
                         break;
                    case 'B':
                         //Place button
                         button.visible = true;
                         button.pos.y = (i + 1) * BLOCKH - BUTTONH;
                         button.pos.x = (j + 1) * BLOCKW - BUTTONW;
                         break;
                    case '#':
                         //place button block
                         buttonBlocks[buttonBlock].pos.y = i * BLOCKH;
                         buttonBlocks[buttonBlock].pos.x = j * BLOCKW;
                         buttonBlock++;
                         break;
                    default: break;
             }
         }
     }
}

//Increase iteration, reset framecount and change old player bitmap to greyed out version
void killPlayer(Object players[], Object blocks[], Object enemies[3][NUM_ENEMIES], Input pastInputs[][FPS*MAX_LEVEL_TIME*60],
     Object &button, Object buttonBlocks[],  int &currentIteration, int &frameCount, char levelData[SCREENH/BLOCKH][SCREENW/BLOCKW]){

    loadLevel(levelData, blocks, enemies, players, button, buttonBlocks);

    if (currentIteration == MAX_PLAYERS - 1){ // kill player
       //Actual Error checking
       allegro_message("You have spawned too many players");
       resetLevel(blocks, enemies, players, frameCount, currentIteration, pastInputs, button, buttonBlocks);
    }

    frameCount = 0;
    currentIteration++;

    for (int i = 0; i <= currentIteration; i++){
        players[i].visible = true;
    }

    players[currentIteration - 1].sprite = playerSpriteGrey;
    ticks = 0;
}

//draws animated sprite to buffer
void animateSprite(Object obj, BITMAP * &buffer, int frameCount){
     int frame = (frameCount/4)%4; //current frame of animation

     if (obj.canJump){//Check if player on ground
        if (obj.vel.x == 0){
           //draw player standing still
           masked_blit(obj.sprite, buffer, 0, 0, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
        }else if (obj.vel.x < 0){
           //draw player running left
           masked_blit(obj.sprite, buffer, obj.dim.w*frame, obj.dim.h, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
        }else if (obj.vel.x > 0){
           //draw player running right
           masked_blit(obj.sprite, buffer, obj.dim.w*frame, obj.dim.h*2, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
        }
     }else if (obj.vel.y < 0){
           if (obj.vel.x < 0){
              //draw player jumping left
              masked_blit(obj.sprite, buffer, 0, obj.dim.h*3, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
           }else{
               //draw player jumping right
               masked_blit(obj.sprite, buffer, obj.dim.w, obj.dim.h*3, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
           }
     }else{
           if (obj.vel.x < 0){
              //FALL RIGHT
              masked_blit(obj.sprite, buffer, 0, obj.dim.h*4, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
           }else{
               //FALL LEFT
               masked_blit(obj.sprite, buffer, obj.dim.w, obj.dim.h*4, obj.pos.x, obj.pos.y, obj.dim.w, obj.dim.h);
           }
     }
}
