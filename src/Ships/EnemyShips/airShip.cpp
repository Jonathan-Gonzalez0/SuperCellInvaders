#include "airShip.h"

airShip::airShip(int xpos, int ypos, ofImage* bullet) : EnemyShip(xpos, ypos, bullet, 1.0, 10, 50) {
    amIBoss = false;
    enemyShipSprite.load("ShipModels/airBalloon.png");
    enemyShipSprite2.load("ShipModels/airBalloonL.png");
    enemyHitBox = new HitBox(pos, enemyShipSprite.getWidth() * 0.4125, enemyShipSprite.getHeight() * 0.4875);
    fireBullet = bullet;
}


void airShip::update(const ofPoint& playerPos) {
    // Calculate the angle to the player
        ofVec2f toPlayer = playerPos - pos;
        float angleToPlayer = ofRadToDeg(-atan2(toPlayer.x, toPlayer.y));

    // Update ship's orientation
        shipOrientation = angleToPlayer;

    // Move towards the player
        pos += toPlayer.getNormalized() * speed;
        if((shipOrientation+90 >= 0 && shipOrientation +90 <= 90) || (shipOrientation + 90 <= 0 && shipOrientation + 90 >= -90 )){
            enemyHitBox->box.setPosition(pos.x - 30, pos.y-30);
         }else{
            enemyHitBox->box.setPosition(pos.x-82.5, pos.y-82.5);
         }
        

    this->shotTimer++;
    // Check if the ship should shoot
        if (shotTimer % 200 == 0) {
            shoot();
            shotTimer = 0; // resets the timer
        }
}


void airShip::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation + 90);
    if((shipOrientation+90 >= 0 && shipOrientation + 90 <= 90) || (shipOrientation + 90 <= 0 && shipOrientation + 90 >= -90 )){
        enemyShipSprite.draw(-32, -32, 120, 120);
    }else{
        enemyShipSprite2.draw(-32, -32, 120, 120);
    }
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
}


void airShip::shoot() {
    // Create a new projectile and add it to the list of bullets
        Projectiles p = Projectiles(pos, shipOrientation + 90, "fireNation",0,0, fireBullet);
        p.setColors(ofColor::red, ofColor::orange);
        enemyBullets.push_back(p);
        SoundManager::playSong("Fire Ball", false);
}