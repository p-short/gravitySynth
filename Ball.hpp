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

//simple Ball class. see Ball.cpp for method definitions.
class Ball {
public:
    Ball();
    ~Ball();
    //custom methods
    void updateBall();
    bool triggerNote();
    void setNoteOff();
    void showBall();
    bool isNotMoving();
    void removeBall();
    float getBallXpos();
    float sectionCheck(int scaleIndex, int n, int oct);
    void applyWind(float force);
    void edges();
    
    //member variables
private:
    float r, x, y, dy, friction, gravity, vel, acc;
    float inc { 0.1 };
    double frequency;
    int triggerCount { 0 };
    bool noteLatch { false };
    bool trigCountGreterThanTwo {false};
    bool notMoving { false };
    
public:
    /*
        nested vector containing integers the represent intervals in a musical scale.
        [0] = Major scale 1 octave
        [1] = Major scale 2 octave
        [2] = Minor scale 1 octave
        [3] = Minor scale 2 octave
        [4] = Major 7th arp 3 octave
        [5] = Minor 7th arp 3 octave
    */
    const std::vector <std::vector<int>> scales{{0, 2, 4, 5, 7, 9, 11, 12},
                                                {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
                                                {0, 2, 3, 5, 7, 8, 10, 12},
                                                {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24},
                                                {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35},
                                                {0, 3, 7, 10, 12, 15, 19, 22, 24, 27, 31, 34}};
};

    double* pan(double input, double x);

#endif /* Ball_hpp */
