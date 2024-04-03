#pragma once
#include "Boss.h"

class Zeppellin : public Boss {
        private:
                ofPoint shootingPoint;
                ofPoint leftTriangleEdge;
                ofPoint rightTriangleEdge;

        public:
                Zeppellin(int xpos, int ypos, string name);
                void update(const ofPoint& playerPos) override;
                void draw() override;
                void shoot() override;
                ofImage fireBullet;                  // Bullet of Character
};