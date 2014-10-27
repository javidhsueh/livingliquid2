//
//  Wave.cpp
//  example-waterRipples
//
//  Created by Javid on 10/22/14.
//
//

#include "Wave.h"


Wave::Wave()
{
    this->isPlay = false;
    this->page_idx = 0;
    this->opacity = 0;
    this->animation.load("images/wave.gif");
}

void Wave::setPosition(ofPoint pos){
    this->position = pos;
}

bool Wave::isPlaying(){
    return this->isPlay;
}
void Wave::startPlay(){
    
    this->isPlay = true;
    this->page_idx = 0;
    this->opacity = 0;
}

void Wave::update(){
    
    if(!this->isPlay){
        return; // not playing
    }
    
    if (ofGetFrameNum() % 6 == 0){
		this->page_idx++;
        
        float ratio = 0;
        float half_idx = this->animation.pages.size()/2;
        if(this->page_idx < half_idx){
            ratio = this->page_idx / half_idx;
        }else{
            ratio = 1 - ( (this->page_idx-half_idx) / half_idx );
        }
        this->opacity = 35 + 200 * ratio;
        
		if (this->page_idx > this->animation.pages.size()-1){
            this->isPlay = false;
        }
	}
    
}

void Wave::draw(){
    
    if(!this->isPlay){
        return; // not playing
    }
    
    ofSetColor(28, 107, 160, this->opacity);
    ofFill();
    this->animation.pages[this->page_idx].draw(this->position);
}