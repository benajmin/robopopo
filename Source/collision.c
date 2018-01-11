#include <allegro.h>
#include <stdio.h>
#include "robopopo.h"
#include "collision.h"


//Detect collisions between 2 objects
//return side of second object which first object collided on
int detectCollision(Object obj1, Object obj2){
    int x1 = obj1.pos.x + obj1.collisionBox.pos.x; //x coord of 1st collision box
    int y1 = obj1.pos.y + obj1.collisionBox.pos.y; //y coord of 1st collision box
    int x2 = obj2.pos.x + obj2.collisionBox.pos.x; //x coord of 2nd collision box
    int y2 = obj2.pos.y + obj2.collisionBox.pos.y; //y coord of 2nd collision box
    
    int cx1 = x1 + (obj1.collisionBox.dim.w / 2); //x coord of centre of 1st object
    int cy1 = y1 + (obj1.collisionBox.dim.h / 2); //y coord of centre of 1st object
    int cx2 = x2 + (obj2.collisionBox.dim.w / 2); //x coord of centre of 2nd object
    int cy2 = y2 + (obj2.collisionBox.dim.h / 2); //y coord of centre of 2nd object
    
    //x distance between edges of objects
    int dx = abs(cx1 - cx2) - (obj1.collisionBox.dim.w + obj2.collisionBox.dim.w) / 2 + 1;
    //y distance between edges of objects
    int dy = abs(cy1 - cy2) - (obj1.collisionBox.dim.h + obj2.collisionBox.dim.h) / 2 + 1;
    
    //see if objects have collided
    if (x1 + obj1.collisionBox.dim.w > x2 && x2 + obj2.collisionBox.dim.w > x1 &&
       y1 + obj1.collisionBox.dim.h > y2 && y2 + obj2.collisionBox.dim.h > y1){
       //see if objects have collided vertically or horizontally
       if (dx > dy){
             //Side collision
             if (cx1 < cx2){
                 //Left collision
                 return LEFT;
             }else{
                 //Right collision
                 return RIGHT;
             }
       }else{
             //Top/bottome collision
             if (cy1 < cy2){
                 //Top collision
                 return UP;
             }else{
                 //Bottom Collision
                 return DOWN;
             }
       }   
    }else{
          return 0;
    }
}

//React to collisions from top
//Used so iterations can stand on each other but dont collide side to side
Object reactUpCollision(Object obj1, Object obj2){
       if (detectCollision(obj1, obj2)==UP && obj1.vel.y >= 0){
               //move player outside of block
               obj1.pos.y = obj2.pos.y - obj1.collisionBox.dim.h - obj1.collisionBox.pos.y + obj2.collisionBox.pos.y + 1;
               //reset downwards velocity
               if (obj1.vel.y > 0)
                  obj1.vel.y = 0;
               obj1.canJump = true;
       }
       return obj1;
}

//Calls detect collision and changes velocities and moves objects accordingly
Object reactCollision(Object obj1, Object obj2, bool bounce){
       switch (detectCollision(obj1, obj2)){
              case UP:
                   //move player outside of block
                   obj1.pos.y = obj2.pos.y - obj1.collisionBox.dim.h - obj1.collisionBox.pos.y + obj2.collisionBox.pos.y + 1;
                   //reset downwards velocity
                   if (obj1.vel.y > 0)
                      obj1.vel.y = 0;
                   obj1.canJump = true;
                   break;
              case DOWN:
                   obj1.pos.y = obj2.pos.y + obj2.collisionBox.pos.y + obj2.collisionBox.dim.h - obj1.collisionBox.pos.y - 1;
                   //reset upwards velocity
                   if (obj1.vel.y < 0)
                      obj1.vel.y = 0;
                   break;
              case RIGHT:
                   //move player out of block
                   obj1.pos.x = obj2.pos.x + obj2.collisionBox.pos.x + obj2.collisionBox.dim.w - obj1.collisionBox.pos.x - 1;
                   //reset lefttwards velocity
                   if (obj1.vel.x < 0 && !bounce){
                      //printf("0");
                      obj1.vel.x = 0;
                   }else if (obj1.vel.x < 0 && bounce){
                      obj1.vel.x *= (-1);
                   }
                   break;
              case LEFT:
                   //move player out of block
                   obj1.pos.x = obj2.pos.x - obj1.collisionBox.dim.w - obj1.collisionBox.pos.x + obj2.collisionBox.pos.x + 1;
                   //reset rightwards velocity
                   if (obj1.vel.x > 0 && !bounce)
                      obj1.vel.x = 0;
                   else if (obj1.vel.x > 0 && bounce)
                      obj1.vel.x *= (-1);
                   break;
              case 0: break;
       }
       return obj1;
}
