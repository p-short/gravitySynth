#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "Ball.hpp"

class ofApp : public ofBaseApp{
    
public:
    void setup() override;
    void update() override;
    void draw() override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    //ofx gui components
    ofxPanel gui;
    ofxFloatSlider spawnRateSlider;
    ofxFloatSlider probSlider;
    ofxFloatSlider slewSlider;
    ofxIntSlider octaveSlider;
    ofxIntSlider scaleSlider;
    ofxToggle noiseToggle;
    
    
    //setting up sound objects
     ofSoundStream mySoundStream;
     void audioOut(ofSoundBuffer& output) override;
        
    //maxi objects
    maxiOsc mySaw, myPulse;
    maxiEnv envOne, envTwo;
    maxiDelayline delay;
    maxiFilter myFilter, mySlew;
    convert midiToFreq;
    
    //nested vector containing scale intervals
    const std::vector <std::vector<int>> scales{{0, 2, 4, 5, 7, 9, 11, 12},
                                                {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
                                                {0, 2, 3, 5, 7, 8, 10, 12},
                                                {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24},
                                                {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35},
                                                {0, 3, 7, 10, 12, 15, 19, 22, 24, 27, 31, 34}};
    //vector to store Ball instances
    std::vector <unique_ptr <Ball>> ballVector;
    
    //global variables
    bool timerOn { true };
    float freq;
    int trig { 0 };
    float t { 0 };
    float avarageXPosition {0.5};
    float sum { 0 };
    float newSlew;
};
