// Includes and dependencies
#include "State.h"
#include "EnemyManager.h"
#include "enemy.h"
#include "EnemyCruiser.h"
#include "EnemyVanguard.h"
#include "ORT.h"
#include "UFO.h"
#include "SpaceStation.h"
#include "playerShip.h"

/*
    Note from author:
        This is the core of the game. Essentially everything is here, and it's where most of my time was spent.
        This game logic is split between two different classes: 
            - AvatarBattle(where the logic of what shows up and what happens)
            - EnemyManager (where all of the logic for spawning and yes, managing enemies is)

        Originally it was going to be all in this same class but your brain is not big enough to handle that (rephrase). 
        I decided to break it up into two separate classes which is a lot easier to manage.

*/


class AvatarBattle: public State { 
private:
    // Private members for internal state management
    vector<EnemyShip*> enemyList; // List of all enemy ships currently in the game
    int playerScore;              // The player's current score
    int killspreeTimer;           // Timer for kill spree feature
    string nextState;             // The next state of the game
    ofTrueTypeFont font;          // Font for text rendering
    ofTrueTypeFont indicatorFont; // Font for text rendering in regards 
    ofImage backgroundImage;       // Background image of the game
      ofImage shiplivesSprite;     //image to show how mani lives -Andrés Muñiz
public:   
    // Public members accessible outside the class
    Player* player;                   // Pointer to the player's ship
    bool displayBossWarning;          // Flag to display boss warning
    bool lostLife = false;             // Marks when a life is lost
    int blink = 0;                      // Blink timer

    // Constructor and Destructor
    AvatarBattle();                     // Constructor declaration
    ~AvatarBattle(){}                   // Destructor declaration

    // Core functionality
    void update();                    // Update game logic (main method)
    void draw();                      // Draw game elements
    void reset();                    // Reset game state

    // Input handling methods
    void keyPressed(int key);         // Handle key press events
    void keyReleased(int key);        // Handle key release events
    void mousePressed(int mouseX, int mouseY, int button); // Handle mouse press events

    // State management
    string getNextState();               // Get the next game state

    // Helper methods for game functionality
    void wrapCoords(ofPoint &currentPos);  // Ensure ship stays within screen bounds
    void updateBullets();                   // Update bullets' positions and states
    void draw_bullets();                    // Draw bullets on the screen

    // UI and feedback methods
    void healthBar(int currHealth, int maxHealth);          // Render the health bar
    void forceShield(int currHealth, int maxhealth);         // Render the force field bar
    void killSpreeTimer(int currTimer, int maxTimer);       // Render the kill spree timer
    double killSpreeMode();                                 // Logic for kill spree mode
    double scoreMultiplier();                               // Calculate score multiplier based on game state
    void removeMarkedPlayerBullets();                       // Remove bullets marked for deletion
    int lifeCounter = 0;                                    // Counts the lives the ship has
    int drawlifeCounter=3;                                  // Used for drawing the lives
    int shieldTimer = 0;                                    // Timer used for decreasing the shield                              
};
