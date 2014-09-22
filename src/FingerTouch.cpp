//
//  FingerTouch.cpp
//  ll2
//
//  Created by javid on 7/18/14.
//
//

#include "FingerTouch.h"
#include "ConfigLoader.h"

FingerTouch::FingerTouch(int tid, ofPoint p)
    :TouchElement(tid,p)
{
    this->radius = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "touch_sensitivity_radius"));
    this->squaredRadius = (this->radius) * (this->radius);
}

FingerTouch::~FingerTouch(){}

void FingerTouch::draw(){
    
//    ofSetColor(255,0,0, 100);
//    ofFill();
//    ofCircle(this->pos.x, this->pos.y, 5);
    
}


bool FingerTouch::isWithinRange(int pointX, int pointY){
    
    float dX = (this->pos.x - pointX) * (this->pos.x - pointX);
    float dY = (this->pos.y - pointY) * (this->pos.y - pointY);
    if ( dX + dY <= this->squaredRadius ){
        return true;
    }
    return false;
    
}