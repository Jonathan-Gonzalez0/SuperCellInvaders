#pragma once

#include "ofMain.h"
#include "Projectile.h"
#include "HitBox.h"

using namespace std;

class Player{ 

    private:
        ofImage shipSprite;                 // Sprite for the ship   
        ofImage ship2Sprite;               //Sprite for the second ship - Andrés Muñiz
        ofImage shieldSprite;                // Shield for the ship
        ofImage bomb;                       // Bomb that will be displayed

        int score;                          // Score of the player

    // === Attributes related to movement ===
        float maxSpeed = 5;         // Maximum speed of the player
        float speed;                // Current speed of the player
        ofVec2f velocity;           
        float accelerationAmount;   // Amount of acceleration
        bool isMoving = false;      // Flag to track if a movement key is being held
        float damping = 0.95;       // Damping factor for slowing down

       // === Attributes related to shooting ===
        float lastShotTime;                // Last time a shot was fired
        float shotCooldown;                // Cooldown between shots

    // === Attributes related to rotation ===
        float rotationSpeed = 2.5;         // Speed of rotation


    public: 

       // === Public attributes ===
        int health;                         // Needed in public for ease of use and direct access
        int shield;                         // Shield of ship
        int shipOrientation;                // Orientation of the ship
        unordered_map<int, bool> keyMap;    // A Map is used to handle the keys pressed
        vector<Projectiles> bullets;        // List of bullets
        ofPoint pos;                        // Position of the player
        HitBox hitBox;                      // Hitbox for the player
        bool showHitbox = false;
        bool shieldIsActive = false;        //Sets shield to be deactivated as default
        int currHealth;                     // Health that the player has at the instant
        int bombs = 0;                      // Bomb counter
        bool bombIsActivated = false;       // Verifies if bomb was activated
        string skin;
        int group = 0;                          // Group of projectiles
        ofImage windBullet;                 // Bullet of Character
        ofImage fireBullet;
        ofImage waterBullet;
        ofImage earthBullet;
        ofImage* bulletPic;
        string bullet;
        ofImage elements;

        bool newbossdied=false;             //Determine if the boss created in spec 2 have died-Andrés Muñiz
    // === Constructors ===
        Player();                                      // Default Constructor
        Player(int Xposition, int Yposition);         // Parametrized Constructor for the playerShip
        Player(int Xposition, int Yposition, string Character);         // Parametrized Constructor for the playerShip
    
        int getScore(); 
        void setScore(int score); 
  
    // Main method to draw the playerShip
        void draw(); 
        ofImage getSprite(){return shipSprite;}
    /*
        Main method to update the playerShip. It handles the movement indirectly by calling processPressedKeys(), and updates the position.
        This is the brain of the class.
    */
        void update();

    /*
        Method to generate the projectiles. It creates a projectile object 
        and places it into the bullets vector.  
    */
        void shoot(); 

    /*
        Useful method in case you want to modify the shot cooldown.
    */
        void setShotCooldown(float shotCooldown);

    /*
        method used for bullet removal.
    */
        void removeMarkedBullets();
 
    // === Input handling ===
        void addPressedKey(int key);         // Function to add a pressed key to the keyMap
        void processPressedKeys();          //Function that will process if the value of the keys inside the Map are being pressed 
        void removePressedKey(int key);      // Function to remove a pressed key from the keyMap 
        void movement(char keyPressed);     // Function that will handle the movement for the ship


        bool shiftispulsed=false;

    //Method for verifying if shield is activated
        void actShield(char keyPressed);

    //Method for activating bomb
        void actBomb(char keyPressed);

        //Activates fire bending
        void actFire(char keyPressed);

        //Activate wind bending
        void actWind(char keyPressed);

        //Activate water bending
        void actWater(char keyPressed);

        //Activate earth bending
        void actEarth(char keyPressed);
};