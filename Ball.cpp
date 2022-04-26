//
//  Ball.cpp
//  gravityPoly
//
//  Created by Phillip Short on 01/04/2022.
//

#include "Ball.hpp"

//Ball constructor initialises all the member variables
Ball::Ball()
{
    //r = radius of the ball
    r = 15;
    //balls x & y positions a generated randomly when the constructor is called
    x = ofRandom(r, ofGetWidth() - r);
    y = ofRandom(r, ofGetHeight() / 2);
    //dy, gravity, and frictions are use in the simulation of gravity
    dy = ofRandom(2);
    friction = 0.85;
    gravity = ofRandom(0.8, 1);
    //vel, & acc are used to apply the force of wind.
    vel = 0;
    acc = 0;
}

Ball::~Ball()
{
}

//update method holds the code that simulates gravity and bounces the ball.
//this code was inspired and adapted from a video by youtuber Chris Courses found here. https://www.youtube.com/watch?v=3b7FyIxWW94&t=1515s
void Ball::updateBall()
{
    //reverse the polarity of dy when the ball hits the ground to make it bounce upward, each time it bounces its multiplied by friction to dull the bounce.
    if (y + r + dy >= ofGetHeight())
    {
        dy = -dy * friction;
    }
    //when ball starts to fall add gravity to it till it bounces again.
    else
    {
        dy += gravity;
    }
    y += dy;
    //this code moves ball on x axis then wind toggle is on
    vel += acc;
    x += vel;
}

bool Ball::triggerNote()
{
    //when ball bouces trigger note.
    //as long as trigCountGreterThanTwo is false, this is to stop repeating retiggering of the note as the gets closer and closer to the being still
    if (y + r + dy >= ofGetHeight() && !trigCountGreterThanTwo)
    {
        //trigger note and increment triggerCount by 1
        noteLatch = true;
        triggerCount++;
        if (triggerCount >= 2)
        {
            //if ball is still triggerCount exceeds two and trigCountGreterThanTwo & notMoving are set to true
            trigCountGreterThanTwo = true;
            notMoving = true;
        }
        //return true when ball bounces
        return noteLatch;
    }
    //if ball is still moving reset triggerCount to 0
    else
    {
        triggerCount = 0;
    }
}

//call method to set noteLatch bool to false
void Ball::setNoteOff()
{
    noteLatch = false;
}

//draws ball the the screen
void Ball::showBall()
{
    ofFill();
    ofSetColor(255, 150);
    ofDrawEllipse(x, y, r * 2, r * 2);
}

//returns bool
bool Ball::isNotMoving()
{
    return notMoving;
}

//sets noteMoving bool to false
void Ball::removeBall()
{
    notMoving = false;
}

//returns balls position on x axis
float Ball::getBallXpos()
{
    return x;
}

//this method checks when the ball is on the x axis and adjusts the frequency accordingly
float Ball::sectionCheck(int scaleIndex, int n, int oct)
{
    // s = section size
    //divide the width of the screen by the number of intervals in chosen scale
    float s = ofGetWidth() / n;
    for (int i = 0; i < n; i ++)
    {
        //ball in first section? set frequency to first interval in selected scale.
        //ball in second section? set frequency to second interval in selected scale ect..
        if (x > s * i && x < s * (i + 1))
        {
            frequency = scales[scaleIndex][i];
        }
    }
    //return frequency with an offset of 36 so the frequency isnt REALLY low.
    //the int value from the octave slider is multiplied by 12 to increase/decrease the octave of the note
    return frequency + 36 + (oct * 12);
}

//blow wind when the 'wind' toggle is on
//method inspired by Daniel Shiffman video found here https://www.youtube.com/watch?v=MkXoQVWRDJs
void Ball::applyWind(float force)
{
    acc = force;
}

//check if ball has hit the edges of the screen and reposition them accordingly
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

//panning method by Benjamin Wheatly
double* pan(double input, double x)
{
    double* chan;
    chan = new double[2];
    x *=  PI / 2;
    
    chan[0] = sin(x) * input;
    chan[1] = cos(x) * input;
    return chan;
};
