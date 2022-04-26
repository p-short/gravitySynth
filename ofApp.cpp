#include "ofApp.h"

/*
 MINI-PROJECT
 
 for my mini-project I have made an interactive audio visual monophonic synthiseser.
 the code renders balls dropped from random positions at varying intervals,
 the balls then simulate gravity as they fall towards the ground. Once they hit the ground notes are triggered.
 Upon selecting a scale the windows width is divided by the number intervals in the scale,
 when a ball hits the ground it triggers the appropriate note from selected scale in relation to where the ball landed on the x axis.
 
 interact with the gui elements to;
 
 * Change the rate at which balls are produced.
 * Change the probability of balls being produced.
 * Change the amount of slew between each frequency.
 * Change scale.
 * Change octave.
 * toggle wind on/off to blow the balls across the x axis.
 
 */
//--------------------------------------------------------------
void ofApp::setup(){
    
      ofSetBackgroundColor(0);
      //set up ofx gui elements
      gui.setup();
      gui.add(spawnRateSlider.setup("Spawn Rate", 2.0f, 0.5f, 4.0f));
      gui.add(probSlider.setup("Spawn Probability", 0.5f, 0.1, 1.0));
      gui.add(slewSlider.setup("Slew Amount", 0, 0, 1.0));
      gui.add(scaleSlider.setup("scales", 0, 0, 5));
      gui.add(octaveSlider.setup("Octave",2, 0, 4));
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
    
      //set up both envelopes
      envOne.setAttack(5);
      envOne.setDecay(125);
      envOne.setSustain(1);
      envOne.setRelease(1650);
    
      envTwo.setAttack(100);
      envTwo.setDecay(100);
      envTwo.setSustain(1);
      envTwo.setRelease(650);
}

//--------------------------------------------------------------
void ofApp::update(){
    //counter keeps track of time from init
    float counter = ofGetElapsedTimef();
    //spawn balls aslong as timer is on and theres less that 6 ball instances in play (it gets a bit much with more than 6)
    if (timerOn && ballVector.size() < 6)
    {
        //spawn ball when counter exceeds value selected from spawnRateSlider
        if (counter > spawnRateSlider)
        {
            //store random value between 0-1 in foo
            float foo = ofRandom(1);
            //reset counter
            ofResetElapsedTimeCounter();
            //if foo is less that value selected from probSlider push Ball instance in to ballVector
            if (foo < probSlider)
            {
                auto b = std::make_unique<Ball>();
                ballVector.push_back(std::move(b));
            }
        }
    }
    //iterate through ballVector and call methods
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
        //else don't simulate wind
        else
        {
            ballVector[i]->applyWind(0);
        }
        
        if (ballVector[i]->triggerNote()) // when the ball bounces tigger note
        {
            //within sectionCheck it selects the scale acording the value of scaleSlider
            //checks for sections relative to scale size
            //adjusts octave of frequency according to octaveSlider
            int m = ballVector[i]->sectionCheck(scaleSlider, scales[scaleSlider].size(), octaveSlider);
            //convert midi to frequency and store it in freq variable
            freq = midiToFreq.mtof(m);
            //trigger envelope
            trig = 1;
            //lastly setNoteOff back to false
            ballVector[i]->setNoteOff();
        }
        // else statement stops stuck notes
        else
        {
            trig = 0;
        }
        
        //reset sum to 0 every loop, then sum together the x position of all the balls
        sum = 0;
        sum += ballVector[i]->getBallXpos();
        
        //within nested loop check if ball is moving
        for (int j = 0; j < ballVector.size(); j++)
        {
            if(ballVector[j]->isNotMoving())
            {
                //remove ball instance from first element of ball vector when ball is not moving
                ballVector[j]->removeBall();
                ballVector.erase(ballVector.begin());
                j--;
                //sent note off to evnvelope
                trig = 0;
                //break out of loop
                break;
            }
        }
    }
    //constantly increment t for wind force
    t += 0.0001;
    //if ballVector greater than 1 calculate average x position of balls
    if (ballVector.size() >= 1)
    {
        avarageXPosition = ofNormalize(sum / ballVector.size() ,0 ,ofGetWidth());
        
    }
    //map slewSlider values to new range
    newSlew = ofMap(slewSlider, 0, 1, 0.01, 0.0001);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw balls to screen
    for (int i = 0; i < ballVector.size(); i ++)
    {
        ballVector[i]->showBall();
    }
    //draw gui to screen
    gui.draw();
    ofSetColor(255);
    
    //draw text to screen
    ofDrawBitmapString("MONOPHONIC GRAVITY SYNTH\n\nUpon selecting a scale the windows width is divided by the number intervals in the scale\nwhen a ball hits the ground it triggers the appropriate note\nfrom selected scale in relation to where the ball landed on the x axis.\n\nBalls are dropped from random positions on the x axis\nincrease/decrease the speed at which they are produced by dragging the 'Spawn Rate' slider\nincrease/decrease the probability of spawning balls by dragging the 'Probability' slider\nincrease/decrease the amount of slew between notes by dragging the 'Slew Amount' slider\nincrease/decrease the notes octave using the 'Octave' slider\n\nSCALES\n0. Major 1 Octave\n1. Major 2 Octave\n2. Minor 1 Octave\n3. Minor 2 Octave\n4. Major 7th Arp 3 Octave\n5. Minor 7th Arp 3 Octave\n\ntoggle 'Wind' on to blow the balls across the x axis", ofGetWidth() / 4, 15);
}

//----------------------------------

void ofApp::audioOut(ofSoundBuffer &output)
{
    //loop through buffer and wrie into it
    std::size_t outChans = output.getNumChannels();
    for (int i = 0; i < output.getNumFrames(); ++i)
    {
        //saw osc with variable slew limiter on the frequency using a Maxi low pass filter
        float sawOut = mySaw.saw(mySlew.lopass(freq, newSlew));
        //saw osc output with envelope
        float envOneOut = envOne.adsr(sawOut, trig);
        //a touch of delay added to signal
        float myDelay = delay.dl(envOneOut, 10000, 0.85);
        //smoothed average values calculated in update using Maxi low pass filter
        float smoothedAvrageVals = myFilter.lopass(avarageXPosition, 0.0001);
        //a pulse wave osc, using the smoothed avragevalue to affect the pulse waves duty cycle
        float pulseOut = myPulse.pulse(freq, smoothedAvrageVals);
        //pulse osc output with envelope
        float envTwoOut = envTwo.adsr(pulseOut, trig);
        //affect the stereo image of the delayed and pulse wave signal according to smoothed average values
        double* panOut = pan(myDelay + envTwoOut, smoothedAvrageVals);
        //output signal to the speakers
        output[i * outChans] = envOneOut + panOut[0] / 1.5;
        output[i * outChans + 1] = envOneOut + panOut[1] / 1.5;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
