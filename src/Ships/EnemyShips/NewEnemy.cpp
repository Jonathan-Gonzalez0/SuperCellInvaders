#include "NewEnemy.h"

NewEnemy::NewEnemy(int xpos, int ypos) : EnemyShip(xpos, ypos, 0.5, 20, 50) {
    amIBoss = false;
    enemyShipSprite.load("CompressedImages/newEnemy-min.png");
    enemyHitBox = new HitBox(pos, enemyShipSprite.getWidth() * 0.1, enemyShipSprite.getHeight()*0.09 );
}


void NewEnemy::update(const ofPoint& playerPos) {
    // Calculate the angle to the player
        ofVec2f toPlayer = playerPos - pos;
        float angleToPlayer = ofRadToDeg(-atan2(toPlayer.x, toPlayer.y));

    // Update ship's orientation
        shipOrientation = angleToPlayer;

    // Move towards the player
        pos += toPlayer.getNormalized() * speed;
        enemyHitBox->box.setPosition(pos.x - 25, pos.y-20);

    this->shotTimer++;
    // Check if the ship should shoot
        if (shotTimer % 200 == 0) {
            shoot();
             // resets the timer
        }
        else if(shotTimer%205 == 0){
            shoot();
            shotTimer = 0;
        }
}


void NewEnemy::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation);
    enemyShipSprite.draw(-32, -32, 60, 60);
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
}


void NewEnemy::shoot() {
    // Create a new projectile and add it to the list of bullets
        Projectiles p = Projectiles(pos, shipOrientation + 90);
        p.setColors(ofColor::red, ofColor::orange);
        enemyBullets.push_back(p);
        SoundManager::playSong("bulletSound", false);
}