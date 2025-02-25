#include "EnemyManager.h"

// Define static members here

    //Boss related:
    vector<unique_ptr<Boss>> EnemyManager::bossList;
    bool EnemyManager::bossIsSpawning = false;
    bool EnemyManager::bossIsActive = false;

    bool EnemyManager::ufoSeen = false;
    bool EnemyManager::ortSeen = false;
    bool EnemyManager::spaceSeen = false;
    bool EnemyManager::zeppellinSeen = false;

    string EnemyManager::whichBoss = "";
    int EnemyManager::bossWarningTimer = 0;

    bool EnemyManager::toggleHitBoxes = false;
    vector<unique_ptr<EnemyShip>> EnemyManager::enemyList;
    vector<unique_ptr<EnemyShip>> EnemyManager::enemiesForDeletion;
    int EnemyManager::enemySpawnTimer = 0;
    int EnemyManager::pointsPerUpdateCycle = 100 + 14 * 30;
    int EnemyManager::killSpreeTimer = 0;


// ================= Main Logic ====================
void EnemyManager::updateEnemies(Player* player){
    // Update timers and points for the current cycle
    decrementKillSpreeTimer();
    pointsPerUpdateCycle = 0;
    enemySpawnTimer++;

    if (bossWarningTimer > 0) {
       decrementBossWarningTimer(); // reduce the boss warning timer

        if(bossWarningTimer <= 0 && bossIsSpawning) {
            // Time to spawn the boss
            spawnBoss(whichBoss); // This actually spawns the boss in the world
        }
    }

    // Handle enemy spawning logic
    spawnEnemy(player);

    // Update each enemy's state and manage collisions
    for (auto& enemy : enemyList) {
        enemy->update(player->pos);
        updateEnemyBullets(enemy.get());
    }

    for(auto& Boss : bossList){
        Boss->update(player->pos);
        updateEnemyBullets(Boss.get());
    }

    // Check and handle collisions
    manageCollisions(player);

    // Remove enemies that have been marked for deletion
    removeEnemies();


}


void EnemyManager::manageCollisions(Player* player) {

  // Handle collisions between player bullets and enemies
  if(player->skin =="Aang"){
        for (auto& enemy : enemyList) {
            enemy->showHitboxes = toggleHitBoxes;
        
        for (auto& bullet : player->bullets) {
            if (!bullet.bulletIsOutOfBounds() && enemy->getHitBox()->isHit(bullet)) {
                player->health = min(player->health + 3.0, 100.0); // Reward the player by healing them
                player->shield = min(player->shield + 2.0, 100.0); //Adds shield after each kill
                int erase = bullet.getBatch();
                enemy->takeDamage(bullet.getDamage());            // Enemy will take damage from the bullet
                if (enemy->isDead()) {
                    SoundManager::playSong("shipDestroyed", false);
                    pointsPerUpdateCycle += enemy->getPoints();
                    resetKillSpreeTimer(150);
                }
                for (auto& bullet : player->bullets) {
                    if(erase == 0 && bullet.getBatch() == 0){
                        bullet.markForDeletion(); // Mark bullet for deletion
                    }
                    if(erase == 1 && bullet.getBatch() == 1){
                        bullet.markForDeletion(); // Mark bullet for deletion
                    }
                    if(erase == 2 && bullet.getBatch() == 2){
                        bullet.markForDeletion(); // Mark bullet for deletion
                    }     
                    
                }
            }
            if(bullet.bulletIsOutOfBounds()){
                bullet.markForDeletion();
             }
        }
    }
  }else{
    for (auto& enemy : enemyList) {
            enemy->showHitboxes = toggleHitBoxes;
        
        for (auto& bullet : player->bullets) {
            if (!bullet.bulletIsOutOfBounds() && enemy->getHitBox()->isHit(bullet)) {
                player->health = min(player->health + 3.0, 100.0); // Reward the player by healing them
                player->shield = min(player->shield + 2.0, 100.0); //Adds shield after each kill

                enemy->takeDamage(bullet.getDamage());            // Enemy will take damage from the bullet
                if (enemy->isDead()) {
                    SoundManager::playSong("shipDestroyed", false);
                    pointsPerUpdateCycle += enemy->getPoints();
                    resetKillSpreeTimer(150);
                }
                bullet.markForDeletion(); // Mark bullet for deletion
            }
            if(bullet.bulletIsOutOfBounds()){
                bullet.markForDeletion();
             }
        }
    }
}

    enemyList.erase(remove_if(enemyList.begin(), enemyList.end(), [](const unique_ptr<EnemyShip>& enemy){ return !enemy; }), enemyList.end());

    // Handle collisions between enemy bullets and the player
    for (auto& enemy : enemyList) {
        
        for (auto& bullet : enemy->getBullets()) {
            if (!bullet.bulletIsOutOfBounds() && player->hitBox.isHit(bullet)) {

                player->health = max(player->health - 10.0, 0.0);       // Player takes damage 
                
                bullet.markForDeletion(); // Mark bullet for deletion (without it you would have a memory leak (Ask Bienve what are memory leaks))
            }
        }
    }
    for (auto& enemy : enemyList) {
        enemy->showHitboxes = toggleHitBoxes;
        	if(enemy->getHitBox()->isHitPlayerWithEnemy(player->pos)){
                player->health = min(player->health - 2.0, 100.0);      //Removes 2 points after each collision
            }
    }

    for (auto& Boss : bossList) {
        Boss->showHitboxes = toggleHitBoxes;
        	if(Boss->getHitBox()->isHitPlayerWithEnemy(player->pos)){
                player->health = min(player->health - 4.0, 100.0);      //Removes 4 points after each collision
            }
        for (auto& bullet : player->bullets) {
            
            if (!bullet.bulletIsOutOfBounds() && Boss->getHitBox()->isHit(bullet)) {
                player->health = min(player->health + 3.0, 100.0); // Reward the player
                player->shield = min(player->shield + 1.0, 100.0); //Adds shield after hitting the boss
                Boss->takeDamage(bullet.getDamage());
                
                if (Boss->isDead()) {                   //If the boss has died from a bullet
                    player->bombs = 1;
                    SoundManager::stopSong(whichBoss);
                    if(player->skin == "Aang"){
                        SoundManager::playSong("avatar", true);
                    }else{
                        SoundManager::playSong("battle", false);
                    }
                    if(Boss->getBossName()=="SPACE STATION"){
                    player->newbossdied=true;
                    }
                    bossHasDied();
                    SoundManager::playSong("shipDestroyed", false);
                    pointsPerUpdateCycle += Boss->getPoints();
                    resetKillSpreeTimer(150);
                }

                bullet.markForDeletion(); // Mark bullet for deletion
            }
        }
    }

    for (auto& Boss : bossList) {
        for (auto& bullet : Boss->getBullets()) {
            if (!bullet.bulletIsOutOfBounds() && player->hitBox.isHit(bullet)) {

                player->health = max(player->health - 10.0, 0.0);       // Player takes damage 
                bullet.markForDeletion(); // Mark bullet for deletion
            }
            if(bullet.bulletIsOutOfBounds()){
                bullet.markForDeletion();
             }
        }
    }

    // Clean up bullets marked for deletion in both player and enemies
    player->removeMarkedBullets();
    
    for (auto& enemy : enemyList) {
        enemy->removeMarkedBullets();
    }
    for (auto& Boss : bossList) {
        Boss->removeMarkedBullets();
    }

}



void EnemyManager::updateEnemyBullets(EnemyShip* enemy){
    for (unsigned int i = 0; i < enemy->getBullets().size(); i++) {
       enemy->getBullets()[i].update();

    }
}

//  =================== Utility Method ====================
 ofVec2f EnemyManager::getRandomEdgePoint() {
    int edge = ofRandom(4);  // Select a random edge (0: top, 1: right, 2: bottom, 3: left)
    float x, y;

    if (edge == 0) {  // Top edge
        x = ofRandomWidth();
        y = 0;
    } else if (edge == 1) {  // Right edge
        x = ofGetWidth();
        y = ofRandomHeight();
    } else if (edge == 2) {  // Bottom edge
        x = ofRandomWidth();
        y = ofGetHeight();
    } else {  // Left edge
        x = 0;
        y = ofRandomHeight();
    }

    return ofVec2f(x, y);

}



// ==================== Timers and Reset ====================
void EnemyManager::decrementKillSpreeTimer() {
    if (killSpreeTimer > 0) {
        --killSpreeTimer;
    
    }
}

void EnemyManager::decrementBossWarningTimer() {
    if (bossWarningTimer > 0) {
        --bossWarningTimer;
    
    }
}

int EnemyManager::getKillSpreeTimer() {
    return killSpreeTimer;
}

int EnemyManager::getBossWarningTimer() {
    return bossWarningTimer;
}

void EnemyManager::resetKillSpreeTimer(int value) {
    killSpreeTimer = value;
}

void EnemyManager::resetBossWarningTimer(int value) {
    bossWarningTimer = value;
}


// ==================== Drawing ====================
void EnemyManager::drawEnemies() {
    for (const auto& enemy : enemyList) {
        enemy->draw();
        drawEnemyBullets(*enemy);
    }
    for (const auto& boss : bossList) {
        boss->draw();
        drawEnemyBullets(*boss);
    }
}

void EnemyManager::drawEnemyBullets(EnemyShip& enemy) {
    for (Projectiles& bullet : enemy.getBullets()) {
        bullet.draw();
    }
}

// ==================== Spawning Logic ====================
void EnemyManager::spawnEnemy(Player* player){
    int currentScore = player->getScore();
    int spawnInterval = whichSpawnInterval(currentScore);
    
    if (enemySpawnTimer >= spawnInterval) {
        ofPoint spawnLocation = getRandomEdgePoint();
        if(player->skin== "Aang"){
            // Check if it's time to spawn a boss
            if (!bossIsActive) { // Ensure no boss is currently active before spawning another
                if (currentScore >= 10000 && !zeppellinSeen) {
                    // Spawn ORT Xibalba
                    initiateBossSpawn("Zeppellin");
                    zeppellinSeen = true; // Prevent multiple spawns
                }
            }
        }else{
            // Check if it's time to spawn a boss
            if (!bossIsActive) { // Ensure no boss is currently active before spawning another
                if (currentScore > 75000 && !ortSeen) {
                    // Spawn ORT Xibalba
                    initiateBossSpawn("ORT Xibalba");
                    ortSeen = true; // Prevent multiple spawns
                }
                else if (currentScore > 50000 && !ufoSeen) {
                    // Spawn UFO ORT
                    initiateBossSpawn("Galactica Supercell ORT");
                    ufoSeen = true; // Prevent multiple spawns
                }
                else if(currentScore > 10000 && !spaceSeen){
                    initiateBossSpawn("SPACE STATION");
                    spaceSeen = true;
                }
            }
        }
        

        // Spawn regular enemies if no boss is being spawned
        if(player->skin == "Aang"){
            enemyList.push_back(make_unique<airShip>(spawnLocation.x, spawnLocation.y, &player->fireBullet));
            
            enemySpawnTimer = 0; // Reset timer after spawning
        }else{
            if (currentScore > 1500) {
                enemyList.push_back(make_unique<EnemyVanguard>(spawnLocation.x, spawnLocation.y));
            }
            if (currentScore >= 5000) {
                enemyList.push_back(make_unique<NewEnemy>(spawnLocation.x, spawnLocation.y));
            }  
            else {
                enemyList.push_back(make_unique<EnemyCruiser>(spawnLocation.x, spawnLocation.y));
            }

            enemySpawnTimer = 0; // Reset timer after spawning
        }
    }
    
    }

bool EnemyManager::isBossSpawning() {
    return bossIsSpawning && bossWarningTimer > 0;
}

void EnemyManager::updateBossWarningTimer(float deltaTime) {
    if (bossWarningTimer > 0) {
        bossWarningTimer -= deltaTime;
    }
}

string EnemyManager::getSpawningBossType() {
    return EnemyManager::whichBoss;
}

void EnemyManager::initiateBossSpawn(string bossType) {
    bossIsActive = true;
    bossIsSpawning = true;
    whichBoss = bossType;
    SoundManager::playSong(bossType, true);
    resetBossWarningTimer(200); // Set to desired warning duration in frames per second (60fps = 1sec)
}


void EnemyManager::spawnBoss(const string& bossType) {
    // Based on bossType, spawn the actual boss
    if (bossType == "ORT Xibalba") {
        ortSeen = true;   
        auto boss = make_unique<ORT>(0, ofGetHeight()/2 -50, "ORT Xibalba");
        bossList.push_back(move(boss));
    } 
    else if (bossType == "Galactica Supercell ORT") {
        ufoSeen = true;
        auto boss = make_unique<UFO>(ofGetWidth()/2, 20, "Galactica Supercell ORT");
        bossList.push_back(move(boss));
    }
    else if(bossType == "SPACE STATION"){
        spaceSeen = true;
        auto boss = make_unique<SpaceStation>(ofGetWidth()/2, 20, "SPACE STATION");
        bossList.push_back(move(boss));
    }
    else if(bossType == "Zeppellin"){
        spaceSeen = true;
        auto boss = make_unique<Zeppellin>(ofGetWidth()/2, 20, "Zeppellin");
        bossList.push_back(move(boss));
    }
    // Reset the spawn timer and clear boss spawning flags
    enemySpawnTimer = 0;
    bossIsSpawning = false;
}


void EnemyManager::bossHasDied() {
    bossIsActive = false;
    bossIsSpawning = false;
    whichBoss = "";
}

int EnemyManager::whichSpawnInterval(int playerScore) {
    // Simplified example, adjust intervals as needed
    if (!bossIsActive && spaceSeen) return 55;
    if (!bossIsActive && ortSeen) return 31;
    if (!bossIsActive && ufoSeen) return 49;
    if (!bossIsActive && zeppellinSeen) return 20;
    if (bossIsActive) return 150; // Slower spawn rate if a boss is active
    if (playerScore < 1000) return 60; // Fast spawn rate for low scores
    if (playerScore < 5000) return 80; // Slower spawn as difficulty increases


    return 100; // Default slowest spawn rate for very high scores or when a boss is likely
}

void EnemyManager::cleanUp() {
    enemyList.clear();
    bossList.clear();
    ufoSeen = false;
    ortSeen = false;
    spaceSeen = false;
    zeppellinSeen = false;
    bossHasDied();
}


void EnemyManager::removeEnemies() {
    enemyList.erase(remove_if(enemyList.begin(), enemyList.end(),
        [](const unique_ptr<EnemyShip>& enemy) { return enemy->isDead(); }),
        enemyList.end());

    bossList.erase(remove_if(bossList.begin(), bossList.end(),
        [](const unique_ptr<Boss>& boss) { return boss->isDead(); }),
        bossList.end());
}

void EnemyManager::explosion(Player* player){
    for (auto& enemy : enemyList) {
        player->health = min(player->health + 3.0, 100.0); // Reward the player by healing them
        player->shield = min(player->shield + 2.0, 100.0); //Adds shield after each kill          
        pointsPerUpdateCycle += enemy->getPoints();
        enemy->takeDamage(20);  // Kills each ship
        SoundManager::playSong("shipDestroyed", false); 
        resetKillSpreeTimer(150);
    }
    
    for (auto& Boss : bossList) {
        Boss->takeDamage(50);
    }
}
