#include "SpaceStation.h"


SpaceStation::SpaceStation(int xpos, int ypos, string name) : Boss(ofGetWidth()/2-143, ofGetWindowHeight()/2-54, 0.5, 400, name) {
    enemyShipSprite.load("ShipModels/Space_Station.png"); 
    enemyHitBox = new HitBox(pos.x - 10, pos.y, enemyShipSprite.getWidth() * 0.60, enemyShipSprite.getHeight() * 0.28);


    shootingPoint = ofPoint(ofGetWindowWidth()/2+25, ofGetWindowHeight()/2); //Creates point where bullets will come from.

}

void SpaceStation::update(const ofPoint& playerPos) {

    shotTimer++;
    if (shotTimer % 10 == 0) {
        shoot(); //Every 10 calls of Update it will shoot.
    }
    if (shotTimer % 2 == 0){
        angleStep++; //Every two calls of update it will add two degrees to the bullts being shot.
    }
    

    this->enemyHitBox->box.setPosition(pos.x+8, pos.y - 15);

    if(enemyBullets.size() > 0) {
        // Update bullets
        for (Projectiles& bullet : enemyBullets) {
            bullet.update();
        }
    }

}



void SpaceStation::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    // ofRotateDeg(shipOrientation);   
    enemyShipSprite.draw(-32, -32, 400, 150);
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
    
    showBossHealth(); 
    



}

void SpaceStation::shoot() {

        // Bullet creation code remains the same
    Projectiles bullet(shootingPoint, angleStep);
    bullet.setColors(ofColor::red, ofColor::orange); // Set desired colors
    enemyBullets.push_back(bullet);
    SoundManager::playSong("bulletSound", false);

}
