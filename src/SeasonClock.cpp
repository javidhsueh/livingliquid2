//
//  SeasonClock.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 8/22/14.
//
//
#include "ofApp.h"
#include "SeasonClock.h"
#include "ConfigLoader.h"

SeasonClock::SeasonClock(){
    
    this->clock.loadImage(ConfigLoader::singleton()->Value("file_path", "season_clock"));
    this->arrow.loadImage(ConfigLoader::singleton()->Value("file_path", "season_clock_arrow"));
    this->isShow = ofToBool( ConfigLoader::singleton()->Value("System", "showSeasonclock") );
}

void SeasonClock::update(){
    
}

void SeasonClock::draw(int speed){
    if(!isShow)
        return;
    
    ofSetColor(0,0,0);
    ofFill();
    
    int x = 1500, y = 50;
    this->clock.draw(x, y);

    int current_frame = ofGetFrameNum();
    int degree = ((current_frame/speed)%365 + (1.0*(current_frame%speed)/speed)) * (360/365.0)-45;
    int clock_width = this->clock.width;
    
    ofPushMatrix();
        ofTranslate(x+clock_width/2.0, y+clock_width/2.0, 0);//move pivot to centre
        ofRotate(degree, 0, 0, 1);//rotate from centre
        ofPushMatrix();
            ofTranslate(-this->arrow.width+4,-this->arrow.height+4,0);//move back by the centre offset
            ofSetColor(255,255,255,255);
            ofNoFill();
            this->arrow.draw(0,0);
        ofPopMatrix();
    ofPopMatrix();
    
}

void SeasonClock::toggle(){
    this->isShow = !this->isShow;
}