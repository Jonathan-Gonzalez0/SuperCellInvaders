#pragma once
#include "Projectile.h"

class HitBox {
    
   // private:
    //ofPoint pos;
    //float width;
   // float height;
    public:
        ofRectangle box; //will serve as the skeleton for the hitbox 

        HitBox(){} //Useless constructor, just use the other constructors
        
        HitBox(ofPoint pos, float width, float height){
    //        this->pos=pos;
     //       this->width=width;
     //       this->height=height;
            this->box = ofRectangle(pos, width, height);
        }

        HitBox (float px, float py, float width, float height){
            this->box = ofRectangle(px, py, width, height);
        }

    //Method to draw the hitboxes in case you may want to test with them
        void draw(){
            ofNoFill();
            ofDrawRectangle(box);
            ofFill();
        }

    /*
        Method to check if a collision occurs with a projectile.
        Phase 2; perhaps we should have another one that checks
        if we're colliding with another hitbox aka another enemy/boss no?
     */ 
        bool isHit(Projectiles& bullet){
            return box.inside(bullet.position.x, bullet.position.y);          
        }
        //no podemos chekear si hitbox esta dentro de otro hitbox debido
        //a que se debe tomar enuenta cualquier punto que toca hitbox con otro hitbox - Andrés E. Muñiz Ríos
   /////////////////////////////////////////////////////////////////////////    
    // bool isHitPlayerWithEnemy(HitBox a){
    //           return this->box.inside(a.pos.x, a.pos.y);    
    //    }
        //Getters -Andrés Muñiz
    //     ofPoint getPos() { return pos; }
    //    float getWidth() { return width; }
    //    float getHeight() { return height; }
    //////////////////////////////////////////////////////////////
        bool isHitPlayerWithEnemy(ofPoint pos){
            return box.inside(pos.x, pos.y); //Method used for verifying if player is inside enemy hitbox
        }
};