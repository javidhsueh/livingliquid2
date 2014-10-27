//
//  Timeline.cpp
//  livingLiquid
//
//  Created by javid on 7/14/14.
//
//

#include "Timeline.h"
#include "ConfigLoader.h"
#include "ofMain.h"
#include "Util.h"
#include "Pen.h"

Timeline::Timeline(){
    
    this->isShow = ofToBool( ConfigLoader::singleton()->Value("System", "showTimeline") );
    this->tick_size = ofToInt(ConfigLoader::singleton()->Value("Timeline", "tick_height"))/2;
    this->line_color = Util::hexToColor(ConfigLoader::singleton()->Value("Timeline", "line_color"));
    this->max_font = Pen::singleton()->getFont("Timeline");
	this->monthNames[0] = "Jan";
	this->monthNames[1] = "Feb";
	this->monthNames[2] = "Mar";
	this->monthNames[3] = "Apr";
	this->monthNames[4] = "May";
	this->monthNames[5] = "Jun";
	this->monthNames[6] = "Jul";
	this->monthNames[7] = "Aug";
	this->monthNames[8] = "Sep";
	this->monthNames[9] = "Oct";
	this->monthNames[10] = "Nov";
	this->monthNames[11] = "Dec";
	
}

void Timeline::toggle(){
    this->isShow = !this->isShow;
}

void Timeline::draw(int speed){
    
    if(!this->isShow)
        return;
    
    ofSetColor(this->line_color);
    ofFill();

    int y_pos = 1030;
    
    // horizontal line
    // ofLine( 60, y_pos, 1860, y_pos);
    
    // tick
    int current_frame = ofGetFrameNum();
    
    int x = ((current_frame/speed)%365 + (1.0*(current_frame%speed)/speed)) * (1800/365.0) + 60;
    
    ofTriangle(x, y_pos, x-4, y_pos+2*this->tick_size, x+4, y_pos+2*this->tick_size);
    
    
    float scale, maxFontWidth, newFontWidth;
    float month_displacement, displacement;
    float offset;
    // draw month name
    for(int i = 0 ; i < 12; i++){
        
        maxFontWidth = max_font->stringWidth(this->monthNames[i]);
        month_displacement = i+0.5;
        
        displacement = (x-60)/150.0;
        offset = displacement-month_displacement;
        if( abs(offset) <= 0.5){
            if( offset <= 0)
                scale = 1 + offset;
            else
                scale = 1 - offset;
        }else{
            scale = 0.5;
        }
        newFontWidth = maxFontWidth * scale;
        
        ofPushMatrix();
            ofTranslate(60+month_displacement*150, y_pos-8);
            ofScale(scale, scale, 1);
            this->max_font->drawString(this->monthNames[i],-newFontWidth/2,0 );
        ofPopMatrix();
        
    }
}


