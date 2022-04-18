//
//  Ball.hpp
//  gravityPoly
//
//  Created by Phillip Short on 01/04/2022.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>
#include "ofMain.h"

class Ball {
public:
    Ball();
    ~Ball();
    
    void updateBall();
    bool triggerNote();
//    bool getNoteOn();
    void setNoteOff();
    void showBall();
    bool isNotMoving();
    void removeBall();
    float getBallXpos();
    float sectionCheck(int scaleIndex, int n);
    void applyWind(float force);
    void edges();
    
private:
    float r;
    float x;
    float y;
    float dy;
    float friction;
    float gravity;
    float count;
    float inc { 0.1 };
    bool noteLatch { false };
    double frequency;
    int triggerCount { 0 };
    bool trigCountGreterThanTwo {false};
    bool notMoving { false };
    float t;
    float vel, acc;
    
public:
    const std::vector <std::vector<int>> scales{{0, 2, 4, 5, 7, 9, 11, 12},
                                                {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
                                                {0, 2, 3, 5, 7, 8, 10, 12},
                                                {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24},
                                                {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35},
                                                {0, 3, 7, 10, 12, 15, 19, 22, 24, 27, 31, 34}};
};

    double* pan(double input, double x);

#endif /* Ball_hpp */
