#pragma once
#include "Enemy.h"


class airShip : public EnemyShip {
    public:
        ofImage enemyShipSprite2;            // Photo of ship to make it look like it is coming form the left of the screen
        ofImage* fireBullet;
        airShip(int xpos, int ypos, ofImage* bullet);
        void update(const ofPoint& playerPos) override;
        void draw() override;
        void shoot() override;

};