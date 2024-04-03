#include "Zeppellin.h"


Zeppellin::Zeppellin(int xpos, int ypos, string name) : Boss(xpos, ypos, 0.5, 1375, name) {
    enemyShipSprite.load("ShipModels/zeppellin.png");
    fireBullet.load("Bullets/fireBall.png");
    enemyHitBox = new HitBox(pos.x - 10, pos.y, enemyShipSprite.getWidth() * 0.60, enemyShipSprite.getHeight() * 0.5);


    shootingPoint = ofPoint(enemyShipSprite.getWidth() / 2, enemyShipSprite.getHeight() );
    leftTriangleEdge = ofPoint(pos.x, enemyShipSprite.getHeight() * 2);
    rightTriangleEdge = ofPoint(pos.x + enemyShipSprite.getWidth(), enemyShipSprite.getHeight() * 2);

    moveRight = true;
    moveLeft = false;

    

}

void Zeppellin::update(const ofPoint& playerPos) {
    
    shootingPoint.x = (pos.x + enemyHitBox->box.getWidth() / 2) - 35;
    shootingPoint.y = pos.y + enemyHitBox->box.getHeight() /2;
    leftTriangleEdge.x = pos.x;
    leftTriangleEdge.y = pos.y + enemyShipSprite.getHeight() * 2;
    rightTriangleEdge.x = pos.x + enemyShipSprite.getWidth();
    rightTriangleEdge.y = pos.y + enemyShipSprite.getHeight() * 2;

    shotTimer++;
    if (shotTimer % 10 == 0) {
        shoot();
    }
    
// =============== Section for movement ===============

    // Move Zeppellin
        if(moveRight) pos.x += 5 * speed;

        if(moveLeft) pos.x -= 5 * speed;

    // Check if Zeppellin is at the left edge, change direction to move right
    if (pos.x <= 0) {
        // ofLogToConsole();
        // ofLogNotice("Zeppellin") << "Changed Position due to " << pos.x << " - " << enemyShipSprite.getWidth() << " = " << pos.x - enemyShipSprite.getWidth() * 3/4;
        moveRight = true;
        moveLeft = false;
    }

    // Check if Zeppellin is at the right edge, change direction to move left
    if (pos.x + enemyShipSprite.getWidth() / 2 >= ofGetWidth() && moveRight) {
        moveRight = false;
        moveLeft = true;
    }

    this->enemyHitBox->box.setPosition(pos.x - 30, pos.y - 15);

    if(enemyBullets.size() > 0) {
        // Update bullets
        for (Projectiles& bullet : enemyBullets) {
            bullet.update();
        }
    }

}



void Zeppellin::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    // ofRotateDeg(shipOrientation);   
    enemyShipSprite.draw(-32, -32, 600, 225);
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
    
    showBossHealth(); 
    
    // Draw lines from the Zeppellin to the left and right edges
        // ofSetLineWidth(2); // Set line width for visibility
        // ofSetColor(ofColor::red); // Set line color to red for visibility
        // ofDrawLine(shootingPoint, leftTriangleEdge);
        // ofDrawLine(shootingPoint, rightTriangleEdge);


}

void Zeppellin::shoot() {
    const int numBullets = 3;

    auto vectorToLeftEdge = ofVec2f(leftTriangleEdge.x - shootingPoint.x, leftTriangleEdge.y - shootingPoint.y);
    auto vectorToRightEdge = ofVec2f(rightTriangleEdge.x - shootingPoint.x, rightTriangleEdge.y - shootingPoint.y);
    float angleToLeft = atan2(vectorToLeftEdge.y, vectorToLeftEdge.x);
    float angleToRight = atan2(vectorToRightEdge.y, vectorToRightEdge.x);

    float apexAngleDegrees = (angleToRight - angleToLeft) * (180.0 / PI);

    float angleStep = apexAngleDegrees / (numBullets - 1);  // Calculate the angle between each bullet

    for (int i = 0; i < numBullets; ++i) {
        float angle = (angleToLeft * (180.0 / PI)) + angleStep * i; // Calculate each bullet's angle within the spread

        // Bullet creation code remains the same
        Projectiles bullet(ofPoint(shootingPoint.x-100, shootingPoint.y+50), angle+10,"fireNation",0,0, &fireBullet);
        bullet.setColors(ofColor::orangeRed, ofColor::orange); // Set desired colors
        enemyBullets.push_back(bullet);
        SoundManager::playSong("Fire Ball", false);
    }

}
