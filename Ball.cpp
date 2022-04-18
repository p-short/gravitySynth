//
//  Ball.cpp
//  gravityPoly
//
//  Created by Phillip Short on 01/04/2022.
//

#include "Ball.hpp"


Ball::Ball()
{
    r = 15;
    x = ofRandom(r, ofGetWidth() - r);
    y = ofRandom(r, ofGetHeight() / 2);
    dy = ofRandom(2);
    friction = 0.85;
    gravity = ofRandom(0.8, 1);
    count = 0.0;
    t = 0;
    vel = 0;
    acc = 0;
}

Ball::~Ball()
{
}

void Ball::updateBall()
{
  
    //code bonces ball
    if (y + r + dy >= ofGetHeight())
    {
        dy = -dy * friction;
    }
    
    else
    {
        dy += gravity;
    }
    y += dy;
    
    //moves ball on x axis then wind toggle is on
    
    vel += acc;
    x += vel;
}

bool Ball::triggerNote()
{
    if (y + r + dy >= ofGetHeight() && !trigCountGreterThanTwo)
    {
        noteLatch = true;
        triggerCount++;
        if (triggerCount >= 2)
        {
            trigCountGreterThanTwo = true;
            notMoving = true;
        }
        return noteLatch;
    }
    else
    {
        triggerCount = 0;
    }
}

void Ball::setNoteOff()
{
    noteLatch = false;
}


void Ball::showBall()
{
    ofFill();
    ofSetColor(255, 150);
    ofDrawEllipse(x, y, r * 2, r * 2);
}

bool Ball::isNotMoving()
{
    return notMoving;
}

void Ball::removeBall()
{
    notMoving = false;
}

float Ball::getBallXpos()
{
    return x;
}

float Ball::sectionCheck(int scaleIndex, int n)
{
    // s = section size
    float s = ofGetWidth() / n;
    if (x > s * 0 && x < s * 1)
    {
//        std::cout << 1 << "\n";
        frequency = scales[scaleIndex][0];
    }
    else if (x > s * 1 && x < s * 2)
    {
//        std::cout << 2 << "\n";
        frequency= scales[scaleIndex][1];
    }
    else if (x > s * 2 && x < s * 3)
    {
//        std::cout << 3 << "\n";
        frequency = scales[scaleIndex][2];
    }
    else if (x > s * 3 && x < s * 4)
    {
//        std::cout << 4 << "\n";
        frequency = scales[scaleIndex][3];
    }
    else if (x > s * 4 && x < s * 5)
    {
//        std::cout << 5 << "\n";
        frequency = scales[scaleIndex][4];
    }
    else if (x > s * 5 && x < s * 6)
    {
//        std::cout << 6 << "\n";
        frequency = scales[scaleIndex][5];
    }
    else if (x > s * 6 && x < s * 7)
    {
//        std::cout << 7 << "\n";
        frequency= scales[scaleIndex][6];
    }
    else if (x > s * 7 && x < s * 8)
    {
//        std::cout << 8 << "\n";
        frequency= scales[scaleIndex][7];
    }
    else if (x > s * 8 && x < s * 9)
    {
//        std::cout << 9 << "\n";
        frequency= scales[scaleIndex][8];
    }
    else if (x > s * 9 && x < s * 10)
    {
//        std::cout << 10 << "\n";
        frequency= scales[scaleIndex][9];
    }
    else if (x > s * 10 && x < s * 11)
    {
//        std::cout << 11 << "\n";
        frequency= scales[scaleIndex][10];
    }
    else if (x > s * 11 && x < s * 12)
    {
//        std::cout << 12 << "\n";
        frequency= scales[scaleIndex][11];
    }
    else if (x > s * 12 && x < s * 13)
    {
//        std::cout << 13 << "\n";
        frequency= scales[scaleIndex][14];
    }
    else if (x > s * 13 && x < s * 14)
    {
//        std::cout << 14 << "\n";
        frequency= scales[scaleIndex][13];
    }
    else if (x > s * 14 && x < s * 15)
    {
//        std::cout << 15 << "\n";
        frequency= scales[scaleIndex][14];
    }
    
    return frequency + 60;
}

void Ball::applyWind(float force)
{
    acc = force;
}

void Ball::edges()
{
    if (x + r >= ofGetWidth())
    {
        x = ofGetWidth() - r;
        vel *= -1;
    }
    
    else if (x - r <= 0)
    {
        x = 0 + r;
        vel *= -1;
    }
}

double* pan(double input, double x)
{
    double* chan;
    chan = new double[2];
    x *=  PI / 2;
    
    chan[0] = sin(x) * input;
    chan[1] = cos(x) * input;
    return chan;
};
