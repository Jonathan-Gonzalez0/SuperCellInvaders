#pragma once
#include "Boss.h"

class SpaceStation : public Boss {
        private:
                ofPoint shootingPoint;
                ofPoint leftTriangleEdge;
                ofPoint rightTriangleEdge;

        public:
                SpaceStation(int xpos, int ypos, string name);
                void update(const ofPoint& playerPos) override;
                void draw() override;
                void shoot() override;
                float angleStep = 0;

};