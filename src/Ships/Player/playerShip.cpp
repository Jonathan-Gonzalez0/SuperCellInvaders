#include "playerShip.h"

//Parametrized Constructor for the playerShip
Player::Player(int Xposition, int Yposition){
    pos.x = Xposition;
    pos.y = Yposition;
    health = 100;
    shield = 0;
    velocity.set(0, 0);
    this->ship2Sprite.load("CompressedImages/secondShip.png");
    this->shipSprite.load("ShipModels/shipModel2.png");
    this->shieldSprite.load("CompressedImages/ForceShield.png");
    this->bomb.load("CompressedImages/Bomb-min.png");

    this->shipOrientation = 0;
    accelerationAmount = 5.0; // Adjust the value as needed
    score = 0;
    hitBox =  HitBox(pos, shipSprite.getWidth() * 0.25, shipSprite.getHeight() * 0.15);
            
    lastShotTime = 0;
    shotCooldown = 0.1;  // Set the cooldown duration to 0.5 seconds (adjust as needed)

}

Player::Player(int Xposition, int Yposition, string Character){
    pos.x = Xposition;
    pos.y = Yposition;
    health = 100;
    shield = 0;
    velocity.set(0, 0);
    skin = Character;
    this->shipSprite.load("ShipModels/aangAndAppa.png");
    this->shieldSprite.load("CompressedImages/ForceShield.png");
    this->bomb.load("CompressedImages/Bomb-min.png");
    windBullet.load("Bullets/airBullet.png");
    fireBullet.load("Bullets/fireBall.png");
    waterBullet.load("Bullets/waterBullet.png");
    earthBullet.load("Bullets/earthBullet.png");
    elements.load("CompressedImages/elements.png");
    bulletPic = &windBullet;
    bullet = "Wind";
    this->shipOrientation = 0;
    accelerationAmount = 5.0; // Adjust the value as needed
    score = 0;
    hitBox =  HitBox(pos, shipSprite.getWidth() * 0.090625, shipSprite.getHeight() * 0.06660156);
            
    lastShotTime = 0;
    shotCooldown = 0.5;  // Set the cooldown duration to 2.5 seconds (adjust as needed)
    this->skin = Character;

}


Player::Player(){
    Player(ofGetWidth()/2, ofGetHeight()/2);
}

int Player::getScore() { return score; }
void Player::setScore(int score) { this->score = score; }

void Player::draw() {
        if(skin == "Aang"){
       
        }else{
            if(bombs == 1){
                this->bomb.draw(ofGetWidth() - 150, 30, 50, 50); //Makes sure to draw the bomb when it is necessary
            }
        }

        // Draw the ship sprite with the calculated rotation
            ofPushMatrix();
            ofTranslate(this->pos.x, this->pos.y);
            ofRotateDeg(shipOrientation);
            if(skin == "Aang"){
                this->shipSprite.draw(-120, -90, 170, 170);
            }else{
                if(newbossdied){
                this->ship2Sprite.draw(-20, -20, 45, 45);
                }
                else{
                this->shipSprite.draw(-20, -20, 45, 45);
                }
            }
            if(shieldIsActive){
                this->shieldSprite.draw(-20, -20, 45, 45);
            }
            ofPopMatrix();
                
        // Draw the hitbox around the player ship. Uncomment this line for testing purposes
        
            if(showHitbox)  this->hitBox.draw();
            
}

void Player::update() {
    processPressedKeys();  // Process the pressed keys and calculate orientation change
if(shiftispulsed){
     velocity.limit(2*maxSpeed);
}
else{
    velocity.limit(maxSpeed); // Limit the velocity to the maximum speed
}           
    pos += velocity; // Update position based on velocity
    if(skin == "Aang"){
        this->hitBox.box.setPosition(pos.x - 113, pos.y - 80);
    }else{
        this->hitBox.box.setPosition(pos.x - 15, pos.y - 15);
    }
            
    velocity *= damping; // Apply damping to slow down the ship

    draw();  // Draw the ship

}

void Player::shoot() { 
    // Calculate the current time
    float currentTime = ofGetElapsedTimef();

    // Check if enough time has passed since the last shot
        if (currentTime - lastShotTime >= shotCooldown) {

                if(skin == "Aang" && bullet == "Wind"){
                    double a = 30/pow(50,2);
                for(int i = -50; i <= 50; i += 10){
                    double left = i*cos(ofDegToRad(shipOrientation+90))+(30-a*pow(i,2))*sin(ofDegToRad(shipOrientation+90));
                    double right = i*sin(ofDegToRad(shipOrientation+90))+(-30+a*pow(i,2))*cos(ofDegToRad(shipOrientation+90));
                    Projectiles p = Projectiles(ofPoint(this->pos.x + 75*sin(ofDegToRad(shipOrientation))+left, this->pos.y - 75*cos(ofDegToRad(shipOrientation))+right), this->shipOrientation, "Aang", i, group, bulletPic);
                    p.setColors(ofColor::azure, ofColor::blueViolet);
                    this->bullets.push_back(p);
                }
                group++;
                if(group == 3){
                    group = 0;
                }
            }else if(skin == "Aang" && (bullet == "Fire")){
                Projectiles p = Projectiles(ofPoint(this->pos.x + 75*sin(ofDegToRad(shipOrientation)), this->pos.y - 75*cos(ofDegToRad(shipOrientation))), this->shipOrientation, "AangFire", 0, 0, bulletPic);
                this->bullets.push_back(p); 
            }else if(skin == "Aang" && bullet == "Earth"){
                Projectiles p = Projectiles(ofPoint(this->pos.x + 75*sin(ofDegToRad(shipOrientation)), this->pos.y - 75*cos(ofDegToRad(shipOrientation))), this->shipOrientation, "AangEarth", 0, 0, bulletPic);
                this->bullets.push_back(p); 
            }else if(skin == "Aang" && bullet == "Water"){
                Projectiles p = Projectiles(ofPoint(this->pos.x + 75*sin(ofDegToRad(shipOrientation)), this->pos.y - 75*cos(ofDegToRad(shipOrientation))), this->shipOrientation, "AangWater", 0, 0, bulletPic);
                this->bullets.push_back(p); 
            }else{
                if(newbossdied){
                Projectiles p = Projectiles(ofPoint(this->pos.x, this->pos.y), this->shipOrientation,20);
                p.setColors(ofColor::pink, ofColor::red); 
                 this->bullets.push_back(p);  
                }
                else{
                Projectiles p = Projectiles(ofPoint(this->pos.x, this->pos.y), this->shipOrientation);
                p.setColors(ofColor::azure, ofColor::blueViolet);
                this->bullets.push_back(p);
                }
            }

            // SoundManager::playSong("bulletSound", false);
            if(skin == "Aang"){
                if(bullet == "Wind"){
                   SoundManager::playSong("Wind", false); 
                }
                if(bullet == "Fire"){
                    SoundManager::playSong("Fire Ball", false); 
                }
                if(bullet == "Water"){
                    SoundManager::playSong("Water", false); 
                }
                if(bullet == "Earth"){
                    SoundManager::playSong("Earth", false); 
                }
            }
            else{
                SoundManager::playSong("Beam", false);
            }

            // Update the last shot time to the current time
            lastShotTime = currentTime;
        }
}

void Player::setShotCooldown(float shotCooldown) { this->shotCooldown = shotCooldown; }

void Player::removeMarkedBullets(){
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                        [](const Projectiles& p) { return p.markedForDeletion; }),
    bullets.end());
}

void Player::addPressedKey(int key) {
    key = tolower(key);

    keyMap[key] = true;
    isMoving = true; // Set the movement flag
}

void Player::processPressedKeys() {
    if(keyMap['w']) movement('w');
    if(keyMap['s']) movement('s');
    if(keyMap['d']) movement('d');
    if(keyMap['a']) movement('a');
    if(keyMap[OF_KEY_SHIFT]) shiftispulsed=true; //Makes ship move faster
    if(keyMap[' ']) shoot();
    if(keyMap['q']) actShield('q'); //Activates shield
    if(keyMap['e']) actBomb('e'); //Activates bomb
    if(keyMap['f']) actFire('f'); //Activates fire bending
    if(keyMap['g']) actWind('g'); //Activates wind bending
    if(keyMap['h']) actWater('h'); //Activates water bending
    if(keyMap['j']) actEarth('j'); //Activates earth bending

            
    if (!isMoving) {
        // Apply damping to gradually slow down the ship when no keys are being pressed
        velocity *= damping; 
    }
}      

void Player::removePressedKey(int key) {
    key = tolower(key);
    keyMap[key] = false;
    shiftispulsed=false;
}

void Player::movement(char keyPressed) {
    ofVec2f acceleration;

    if (keyPressed == 'w') {
        // Apply acceleration in the direction of the ship's orientation
        acceleration.set(cos(ofDegToRad(shipOrientation)) * accelerationAmount, sin(ofDegToRad(shipOrientation)) * accelerationAmount);
        }
    if (keyPressed == 's') {
        // Apply acceleration in the opposite direction of the ship's orientation
        acceleration.set(-cos(ofDegToRad(shipOrientation)) * accelerationAmount, -sin(ofDegToRad(shipOrientation)) * accelerationAmount);
        }

    // Apply force to velocity
    velocity += acceleration;

    if (keyPressed == 'a') {
        // Rotate the ship counterclockwise
        shipOrientation -= rotationSpeed;
        }
    if (keyPressed == 'd') {
        // Rotate the ship clockwise
        shipOrientation += rotationSpeed;
        }
    }   

void Player::actShield(char keyPressed) {

    if (keyPressed == 'q' && shield == 100) {
        
            shieldIsActive = true;
            currHealth = health; // Health at the point shield was activated
            SoundManager::playSong("Force Shield", true); //Plays shield sound
            
        }

    }  

void Player::actBomb(char keyPressed) {

    if(skin == "Aang"){

    }else{
        if (keyPressed == 'e' && bombs >= 1) {
            
                bombIsActivated = true;
                bombs--;
                
            }
    }

    }  

void Player::actFire(char keyPressed) {

    if (keyPressed == 'f') {
        
            bulletPic = &fireBullet;
            bullet = "Fire";
            
        }

    }

void Player::actWind(char keyPressed) {

    if (keyPressed == 'g') {
        
            bulletPic = &windBullet;
            bullet = "Wind";
            
        }

    }    

void Player::actWater(char keyPressed) {

    if (keyPressed == 'h') {
        
            bulletPic = &waterBullet;
            bullet = "Water";
            
        }

    } 


void Player::actEarth(char keyPressed) {

    if (keyPressed == 'j') {
        
            bulletPic = &earthBullet;
            bullet = "Earth";
            
        }

    } 