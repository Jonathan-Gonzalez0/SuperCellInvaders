#include "Projectile.h"

Projectiles::Projectiles(ofPoint p, float angle) {
    this->position = p;
    this->angle = angle;
    width = 7;
    height = 12;
    speed = 10;
    damage = 10;
    markedForDeletion = false;
}

Projectiles::Projectiles(ofPoint p, float angle, int dmg) {
    this->position = p;
    this->angle = angle;
    width = 7;
    height = 12;
    speed = 10;
    damage = dmg;
    markedForDeletion = false;
}

Projectiles::Projectiles(ofPoint p, float angle, string character, int index, int batch, ofImage* bullet) {
    this->position = p;
    this->angle = angle;
    width = 7;
    height = 12;
    speed = 10;
    damage = 10;
    markedForDeletion = false;
    this->character = character;
    this->index = index;
    this-> batch = batch;
    this-> bullet = bullet;

}



void Projectiles::update() {    
    // Calculate the new position based on angle and speed
    float vx = cos(ofDegToRad(angle)) * speed;
    float vy = sin(ofDegToRad(angle)) * speed;
    position.x += vx;
    position.y += vy;

}

void Projectiles::draw(){
    if(character == "Aang"){
        ofPushMatrix();
        ofTranslate(position);
        ofRotateDeg(angle + 90); // Rotate the projectile based on its angle + 90 degrees to rotate in the appropriate orientation

        //Uncomment for debugging:
        // Draw the two ellipses with distinct colors
            // ofSetColor(color1); //This color is for the tail of the projectile
            // ofDrawEllipse(0, 0, width, height);
            // ofSetColor(color2); //This color is the main color of the projectile
            // ofDrawEllipse(0, -height / 2, width, height);
            // ofSetColor(ofColor::white);
            if( index == 0){
                bullet->draw(-90, -70, 170, 170); //Draws a fake a skin on top of the bullet
            }
        ofPopMatrix();

    }
    if(character == "fireNation"){
        ofPushMatrix();
        ofTranslate(position);
        ofRotateDeg(angle+90); // Rotate the projectile based on its angle + 90 degrees to rotate in the appropriate orientation
        bullet->draw(-22.5, -17.5, 42.5, 42.5); // Draws a fake a skin on top of the bullet
        ofPopMatrix();
    }else if(character != "Aang"){
        ofPushMatrix();
        ofTranslate(position);
        ofRotateDeg(angle + 90); // Rotate the projectile based on its angle + 90 degrees to rotate in the appropriate orientation

        // Draw the two ellipses with distinct colors
            ofSetColor(color1); //This color is for the tail of the projectile
            ofDrawEllipse(0, 0, width, height);
            ofSetColor(color2); //This color is the main color of the projectile
            ofDrawEllipse(0, -height / 2, width, height);
            ofSetColor(ofColor::white);
        ofPopMatrix();
    }

}

bool Projectiles::bulletIsOutOfBounds() {
    if (position.x < 0 || position.x > ofGetWidth() || position.y < 0 || position.y > ofGetHeight()) {
        return true; // Bullet is out of bounds
    }
    return false; // Bullet is within bounds
}

void Projectiles::markForDeletion() {
    markedForDeletion = true;
}

void Projectiles::setColors(ofColor c1, ofColor c2) {
    color1 = c1;
    color2 = c2;
}


int Projectiles::getBatch() {
    return batch;
}