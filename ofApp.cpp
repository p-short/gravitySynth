#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
      ofSetBackgroundColor(0);
      timerOn = true;
    
      gui.setup();
      gui.add(spawnRateSlider.setup("Spawn Rate", 2.0f, 0.5f, 4.0f));
      gui.add(probSlider.setup("Spawn Probability", 0.5f, 0.1, 1.0));
      gui.add(slewSlider.setup("Slew Amount", 0, 0, 1.0));
      gui.add(scaleSlider.setup("scales", 0, 0, 5));
      gui.add(noiseToggle.setup("Wind", false));
    
    
      //set up of sound objects
      int sampleRate = 44100;
      int bufferSize = 512;
      
      ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
      
      ofSoundStreamSettings settings;
      settings.setOutListener(this);
      settings.sampleRate = sampleRate;
      settings.numOutputChannels = 2;
      settings.numInputChannels = 0;
      settings.bufferSize = bufferSize;
      mySoundStream.setup(settings);
    
    
      envOne.setAttack(5);
      envOne.setDecay(125);
      envOne.setSustain(1);
      envOne.setRelease(1650);
    
      envTwo.setAttack(100);
      envTwo.setDecay(100);
      envTwo.setSustain(1);
      envTwo.setRelease(650);
    
    //std::cout << scales[0].size() - 1 << "\n"; // 146 = n

    /*
    0 - 14 max
     
    switch(xpos) // x position of the balll
    {
     case xpos > n * 0 && xpos < n * 1 :
        play first note of scale;
        break;
     
     case xpos > n * 1 && xpos < n * 2 ;
        play second note of scale;
        break;
     
     and so on...
    */
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float counter = ofGetElapsedTimef();
    if (timerOn && ballVector.size() < 6)
    {
        if (counter > spawnRateSlider)
        {
            //have this only generate max 6 balls at any one time then have a poly synth with 6 voices
            float foo = ofRandom(1);
            ofResetElapsedTimeCounter();

            if (foo < probSlider)
            {
                auto b = std::make_unique<Ball>();
                ballVector.push_back(std::move(b));
            }
        }
    }
    for (int i = 0; i < ballVector.size(); i ++)
    {
        //bounce ball on y axis
        ballVector[i]->updateBall();
        //ball bounces in the opposite direct on the x axis when it hits the edges
        ballVector[i]->edges();
        //simulate wind blowing the balls then noiseToggle is true;
        if (noiseToggle)
        {
            float wind = ofMap(ofNoise(t), 0, 1, -0.125, 0.125);
            ballVector[i]->applyWind(wind);
        }
        else
        {
            //else don't simulate wind
            ballVector[i]->applyWind(0);
        }
        
        if (ballVector[i]->triggerNote()) // when the ball bounces tigger note
        {
            int foo = ballVector[i]->sectionCheck(scaleSlider, scales[scaleSlider].size()); //freq = midiToFreq.mtof(ballVector[i]->sectionCheck(scales[0].size())); scales[0].size = 8
//            std::cout << foo << "\n";
            freq = midiToFreq.mtof(foo);
            trig = 1;
            ballVector[i]->setNoteOff();
        }
      
        else
        {
            trig = 0;
        }
        
        //
        sum = 0;
        sum += ballVector[i]->getBallXpos();
        
        for (int j = 0; j < ballVector.size(); j++)
        {
            if(ballVector[j]->isNotMoving())
            {
                //remove ball instance from first element of ball vector
                ballVector[j]->removeBall();
                ballVector.erase(ballVector.begin());
                j--;
                break;
            }
        }
    }
    t += 0.0001;
    //Average = Sum / Count
    if (ballVector.size() >= 1)
    {
        avarageXPosition = ofNormalize(sum / ballVector.size() ,0 ,ofGetWidth());
        
    }
    std::cout << 1 - (slewSlider - 0.001) << "\n"; //could make 1 - 0.001 with a bit more time
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < ballVector.size(); i ++)
    {
        ballVector[i]->showBall();
    }
    gui.draw();
}

//----------------------------------

void ofApp::audioOut(ofSoundBuffer &output)
{
    //loop through buffer and wrie into it
    std::size_t outChans = output.getNumChannels();
    for (int i = 0; i < output.getNumFrames(); ++i)
    {
        float sawOut = mySaw.saw(mySlew.lopass(freq, 1 - (slewSlider - 0.001)));
        float envOneOut = envOne.adsr(sawOut, trig);
        
        float myDelay = delay.dl(envOneOut, 10000, 0.85);
        
        float smoothedAvrageVals = myFilter.lopass(avarageXPosition, 0.0001);
        
        float pulseOut = myPulse.pulse(freq, smoothedAvrageVals);
        float envTwoOut = envTwo.adsr(pulseOut, trig);
        
        double* panOut = pan(myDelay + envTwoOut, smoothedAvrageVals);
        
        output[i * outChans] = envOneOut + panOut[0] / 1.5;
        output[i * outChans + 1] = envOneOut + panOut[1] / 1.5;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if (key == ' ')
//    {
//        auto b = std::make_unique<Ball>();
//        ballVector.push_back(std::move(b));
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
