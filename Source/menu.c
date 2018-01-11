#include <allegro.h>
#include <stdio.h>
#include "menu.h"
#include "robopopo.h"
#include "fonts.h"


int openMenu(){
     show_mouse(screen);
     bool pastMouse = false;

     while (true){        
          drawMenu();

          //Detect mouse press
          if (!(mouse_b & 1) && pastMouse){
             //Play button
             if (mouse_x < 540 && mouse_x > 100 && mouse_y < 280 && mouse_y > 180){
                  return 0;
             }
             //controls button
             if (mouse_x < 540 && mouse_x > 100 && mouse_y < 400 && mouse_y > 300){
                  if (openControls())
                     return 1;
             }
          }
          
          
          if (key[KEY_ESC])//close program
             return -1;
          if (key[KEY_ENTER]){ //play
             return 0;
          }
          pastMouse = mouse_b & 1;
     }
     
}

int openControls(){
    bool pastMouse = false;
    
     while (true){
           drawControls();
           
           if (!(mouse_b & 1) && pastMouse){
              //return to menu button
              if(mouse_x < 620 && mouse_x > 460 && mouse_y < 460 && mouse_y > 400){
                  return 0;
              }
           }
           
           if (key[KEY_ESC])//close program
              return 1;
              
           pastMouse = mouse_b & 1;
     }
}

void drawControls(){
     BITMAP *buffer = create_bitmap(SCREENW, SCREENH);
     
     clear_to_color(buffer, BACKCOL);
     
     //DRAW TITLE
     textout_centre_ex(buffer, fonts[LC48], "CONTROLS", 320, 10, BLACK, -1);
     //DRAW RETURN TO MENU BUITTON
     rectfill(buffer, 460, 400, 620, 460, BLACK);
     textout_centre_ex(buffer, fonts[LC18], "Return to", 540, 410, WHITE, -1);
     textout_centre_ex(buffer, fonts[LC18], "Menu", 540, 430, WHITE, -1);
     
     //Such error checking
     if (playerSprite == NULL)
        printf("hvbfdsbhjgdsa");
     
     //Draw images for each control
     for (int i = 1; i <= 3; i++){
         masked_blit(playerSprite, buffer, 0, PLAYERH*i, 35, (120*i)-40, PLAYERW, PLAYERH);
     }
     
     //draw more images for controls
     masked_blit(playerSprite, buffer, 0, PLAYERH*4, 350, 120-40, PLAYERW, PLAYERH);
     masked_blit(playerSpriteGrey, buffer, 0, 0, 350, 120*2-40, PLAYERW, PLAYERH);
     
     //TEXT describing each control
     textout_centre_ex(buffer, fonts[LC18], "Run Left", 73, 155, BLACK, -1);
     textout_centre_ex(buffer, fonts[LC18], "Run Right", 73, 275, BLACK, -1);
     textout_centre_ex(buffer, fonts[LC18], "Jump", 73, 395, BLACK, -1);
     textout_centre_ex(buffer, fonts[LC18], "Quick Fall", 388, 155, BLACK, -1);
     textout_centre_ex(buffer, fonts[LC18], "Suicide", 388, 275, BLACK, -1);
     textout_centre_ex(buffer, fonts[LC18], "Reset Level", 388, 365, BLACK, -1);
     
     //KEY CONTROLS
     textout_ex(buffer, fonts[LC24], "A/<", 160, 110, BLACK, -1);
     textout_ex(buffer, fonts[LC24], "D/>", 160, 230, BLACK, -1);
     textout_ex(buffer, fonts[LC24], "W/^", 160, 350, BLACK, -1);
     textout_ex(buffer, fonts[LC24], "S/v", 475, 110, BLACK, -1);
     textout_ex(buffer, fonts[LC24], "R", 475, 230, BLACK, -1);
     textout_ex(buffer, fonts[LC24], "X", 475, 320, BLACK, -1);
       
     blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
     
     destroy_bitmap(buffer);
}

void drawMenu(){
     BITMAP *buffer = create_bitmap(SCREENW, SCREENH);

     clear_to_color(buffer, BACKCOL);
     //BUTTONS
     rectfill(buffer, 100, 180, 540, 280, BLACK);
     rectfill(buffer, 100, 300, 540, 400, BLACK);
     //TITLE
     textout_centre_ex(buffer, fonts[LC72], "RoboPopo", 320, 50, BLACK, -1);
     //BUTTON TEXT
     textout_centre_ex(buffer, fonts[LC48], "Play", 320, 200, WHITE, -1);
     textout_centre_ex(buffer, fonts[LC48], "Controls", 320, 320, WHITE, -1);
      
     blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
     
     destroy_bitmap(buffer);
}

//DRAW END SCREEN
int drawEnd(int timer, int deaths){
     BITMAP *buffer = create_bitmap(SCREENW, SCREENH);

     clear_to_color(buffer, BACKCOL);
     textout_centre_ex(buffer, fonts[LC72], "You Won", 320, 140, BLACK, -1);
     textprintf_centre_ex(buffer, fonts[LC22], SCREENW/2, 240, BLACK, -1, "You beat the game in %d:%2d", timer/(30*60), (timer/30)%60);
     textprintf_centre_ex(buffer, fonts[LC22], SCREENW/2, 280, BLACK, -1, "You died %d times", deaths);
     textout_centre_ex(buffer, fonts[LC36], "Press R to replay", 320, 340, BLACK, -1);
      
     blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
     
     destroy_bitmap(buffer);
     
     while(true){
         if (key[KEY_ESC])
            return 0;
         if (key[KEY_R])
            return 1;
     }
}

void drawCredits(){
     BITMAP *buffer = create_bitmap(SCREENW, SCREENH);
     int frameCount = 0, x = 460;

     while (frameCount < 900){
          while (ticks == 0){
                rest(1);
          }
          while (ticks > 0){
                clear_to_color(buffer, BACKCOL);

                textout_centre_ex(buffer, fonts[LC72], "Credits", SCREENW/2, 50 + x, BLACK, -1);
                
                textout_centre_ex(buffer, fonts[LC36], "Sprites", SCREENW/2, 200 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Player Sprite - hfbn2's Bondman", SCREENW/2, 260 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Enemy Sprite - Redrums's Haggar", SCREENW/2, 300 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Block Sprite - Pelle's block", SCREENW/2, 340 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC18], "Button Sprite - JESGRAD07's RED ? BUTTON", SCREENW/2, 380 + x, BLACK, -1);
                
                textout_centre_ex(buffer, fonts[LC36], "Music", SCREENW/2, 470 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "We Fly How We Fly - DJ  tter", SCREENW/2, 540 + x, BLACK, -1);
                textout_centre_ex(buffer, fonts[S20], "W", 530, 535 + x, BLACK, -1);
                
                textout_centre_ex(buffer, fonts[LC20], "Creative Consultant - Ryan T-S", SCREENW/2, 0 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Creative Consultant - Hannah Delion", SCREENW/2, 40 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Creative Consultant - Nathan Kelly", SCREENW/2, 80 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Alpha Testing - Hannah Delion", SCREENW/2, 120 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Alpha Testing - Nathan Kelly", SCREENW/2, 160 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Alpha Testing - Ruth Wilkins", SCREENW/2, 200 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Audio Specialist - Ryan T-S", SCREENW/2, 240 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Audio Specialist - Willie Lau", SCREENW/2, 280 + x + 620, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Executive Producer - Caelan Rebus", SCREENW/2, 320 + x + 620, BLACK, -1);
                
                textout_centre_ex(buffer, fonts[LC20], "Head Programmer - Benjamin Wilkins", SCREENW/2, 0 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Level Design - Benjamin Wilkins", SCREENW/2, 40 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Creative Design  - Benjamin Wilkins", SCREENW/2, 80 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Character Design  - Benjamin Wilkins", SCREENW/2, 120 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Executive Producer - Benjamin Wilkins", SCREENW/2, 160 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Foley Artist - Benjamin Wilkins", SCREENW/2, 200 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Costume Designer - Benjamin Wilkins", SCREENW/2, 240 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Makeup Artist - Benjamin Wilkins", SCREENW/2, 280 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Marketing - Benjamin Wilkins", SCREENW/2, 320 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Public Relations - Benjamin Wilkins", SCREENW/2, 360 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Stunt Coordinator - Benjamin Wilkins", SCREENW/2, 400 + x + 1050, BLACK, -1);
                textout_centre_ex(buffer, fonts[LC20], "Explosives Specialist - Benjamin Wilkins", SCREENW/2, 440 + x + 1050, BLACK, -1);
                
                blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);

                frameCount++;
                x -= 2;
                ticks --;
          }
    }
    destroy_bitmap(buffer);
}
