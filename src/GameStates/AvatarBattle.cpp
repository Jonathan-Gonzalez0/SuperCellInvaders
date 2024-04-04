#include "AvatarBattle.h"
#include "PlayerShip.h"

// ====================================
// Constructor & Destructor Section
// ====================================

AvatarBattle::AvatarBattle() {
    this->player = new Player(ofGetWidth() / 2, ofGetHeight() / 2, "Aang");
    this->playerScore = 0;
    this->killspreeTimer = 0;
    
    font.load("Fonts/Orbitron.ttf", 20, true);
    indicatorFont.load("Fonts/Orbitron.ttf", 10, true);
    backgroundImage.load("Menu_Images/sky.jpg");
    shiplivesSprite.load("ShipModels/Appa.png");

}

// ====================================
// Core Functionality Section
// ====================================


// Update Method
void AvatarBattle::update() {
    // Boss spawn logic
    if (EnemyManager::isBossSpawning()) {
        displayBossWarning = true;
        SoundManager::stopSong("avatar");
    } 
    else {
        displayBossWarning = false;
    }

    // Update enemies and player
    EnemyManager::updateEnemies(player);
    this->player->processPressedKeys();
    this->player->update();
    wrapCoords(this->player->pos);

    //Logic for activating bomb
    if(player->bombIsActivated){
        EnemyManager::explosion(player);
        player->bombIsActivated = false;
    }
    
    // Update scoring and kill spree timer
    playerScore += EnemyManager::getPointsScored() * scoreMultiplier() * killSpreeMode();
    this->player->setScore(playerScore);
    killspreeTimer = EnemyManager::getKillSpreeTimer();

    // Update player projectiles
    if (!this->player->bullets.empty()) {
        updateBullets();
    }

    //Logic for activating shield
    if (player->shieldIsActive){
        shieldTimer++;
        player->health = player->currHealth;

        if(shieldTimer % 40 ==0 ){
            player->shield -= 5; //Removes 5 points from the shield after calling update 40 times
        }  
        if(player->shield <= 0){ 
            player->shieldIsActive = false; //Deactivates shield when it reaches cero
            SoundManager::stopSong("Force Shield");
        }
    }
    
    // State switching logic for when the player dies
    if (this->player->health <= 0) {
        lostLife = true;
        lifeCounter++;
        drawlifeCounter--;
        if(lifeCounter < 3 ){
            player->health = 100; //Sets health back to 100
        }
        if(lifeCounter == 3){
            this->setNextState("GameOverState");
            SoundManager::stopSong("avatar");
            if(EnemyManager::getSpawningBossType() != ""){
                SoundManager::stopSong(EnemyManager::getSpawningBossType());
            }
                // Write the current score to a file
                ofstream scoreFile("currentScore.txt");
                if (scoreFile.is_open()) {
                    scoreFile << playerScore;
                    scoreFile.close();
                }
                this->setFinished(true);
        }
    }
}

//====== Draw Method ====== 
void AvatarBattle::draw() {
    ofSetBackgroundColor(ofColor::black);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // Draw the score
    ofSetColor(ofColor::white);
    font.drawString("SCORE " + to_string(playerScore), ofGetWidth() / 2 - 50, 50);

    //Draw lives Indicator-Andrés Muñiz
    ofSetColor(ofColor::white);
    for(unsigned int i=drawlifeCounter; i>0; i--){
        //ofDrawCircle(105 + 28*i , 120, 9);
       shiplivesSprite.draw(80 +31*i, ofGetWindowHeight()-75, 28, 28);
    }
    font.drawString("Lives: ", 10, ofGetWindowHeight()-50);

    // Draw enemies and player
    EnemyManager::drawEnemies();
    if(lostLife){
        blink++;
        if(blink % 10 == 0){
           player->draw(); 
        }else{

        }
        if(blink == 60){
            lostLife = false;
            blink = 0;
        }
    }else{
        player->draw();
    }
    draw_bullets();
    
    // Draw boss warning if needed
    if (displayBossWarning) {
        // Semi-transparent red screen tint
        ofSetColor(255, 0, 0, 128);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

        // Warning text
        ofSetColor(255);
        font.drawString("BOSS APPROACHING!", ofGetWidth() / 2 - 100, ofGetHeight() / 2);
    }

    // Draw UI elements
    healthBar(player->health, 100);
    forceShield(player->shield, 100);
    killSpreeTimer(this->killspreeTimer, 150);
    
    //Draw a mini box for the bomb. Make sure to draw the bomb inside this box.
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 150, 30, 50, 50);
        ofFill();
    
}

// ====================================
// Input Handling Section
// ====================================

void AvatarBattle::keyPressed(int key) {
    player->addPressedKey(key);
    
    //DEBUG KEYS - Press these keys to ease the debugging with the game
    if(key == 'k') {
        EnemyManager::toggleHitBoxes = !EnemyManager::toggleHitBoxes; 
        player->showHitbox = !player->showHitbox;
    }
    if(key == 'o')  player->health = 100;
    if(key == 'p')  playerScore += 10000; 
    if(key == 'z') player->shield = 100;
    if(key == 'x') player->shield = 0;
}

void AvatarBattle::keyReleased(int key) {
    key = tolower(key);
    this->player->removePressedKey(key);
}

void AvatarBattle::mousePressed(int x, int y, int button) {
    // Implementation for mousePressed event in case you want to do something with this
}

// ====================================
// Helper Methods Section
// ====================================

void AvatarBattle::wrapCoords(ofPoint &currentPos) {
    // Screen wrapping logic for player ship
    if (currentPos.x < 0.0) currentPos.x = ofGetWidth() - 10;
    if (currentPos.x >= ofGetWidth()) currentPos.x = 10;
    if (currentPos.y < 0.0) currentPos.y = ofGetHeight() - 10;
    if (currentPos.y >= ofGetHeight()) currentPos.y = 10;
}

void AvatarBattle::draw_bullets() {
    // Draw all player bullets
    for (auto &bullet : this->player->bullets) {
        bullet.draw();
    }
}

void AvatarBattle::updateBullets() {
    // Update logic for player bullets
    for (auto &bullet : this->player->bullets) {
        bullet.update();
    }
}

// ====================================
// UI and Feedback Methods Section
// ====================================
void AvatarBattle::healthBar(int currHealth, int maxHealth) {
    indicatorFont.drawString("HEALTH", 10, 30);
    ofNoFill();
    ofDrawRectangle(10, 40, maxHealth *2, 20);
    ofFill();
    ofSetColor(ofColor::green);
    ofDrawRectangle(10, 40, currHealth *2, 20);
    ofSetColor(ofColor::white);
}

void AvatarBattle::forceShield(int currShield, int maxShield) {
    indicatorFont.drawString("Force Shield", 10, 120);
    ofNoFill();
    ofDrawRectangle(10, 130, maxShield *2, 20);
    ofFill();
    ofSetColor(ofColor::blue);
    ofDrawRectangle(10, 130, currShield *2, 20);
    ofSetColor(ofColor::white);
}

void AvatarBattle::killSpreeTimer(int currTimer, int maxTimer) {
    indicatorFont.drawString("KILL SPREE", 10, 80);
    ofNoFill();
    ofDrawRectangle(10, 90, maxTimer, 10);
    ofFill();
    ofSetColor(ofColor::red);
    ofDrawRectangle(10, 90, currTimer, 10);
    ofSetColor(ofColor::white);
}

// ====================================
// Game Mechanics Methods Section
// ====================================

double AvatarBattle::killSpreeMode() {
    // Logic for kill spree mode bonus
    return this->killspreeTimer > 0 ? 1.5 : 1.0;
}

double AvatarBattle::scoreMultiplier() {
    // Logic for score multiplier based on current score
    if (playerScore >= 350) return 3.5;
    else if (playerScore >= 200) return 2.5;
    else if (playerScore >= 100) return 1.5;
    else return 1.0; // Default multiplier
}

void AvatarBattle::reset(){
    setFinished(false);
    setNextState("");
}


                                        
