//
//  TagOffAnimation.h
//  example-waterRipples
//
//  Created by Javid on 10/11/14.
//
//

#ifndef example_waterRipples_TagOffAnimation_h
#define example_waterRipples_TagOffAnimation_h

#include "ofImage.h"
#include "ofPolyline.h"
#include "ofPoint.h"
#include "RenderTrack.h"
#include <string>

class Tag{
public:
    Tag(std::string species);
    
    void buildPath(ofPoint p);
    void startAnimation();
    void update();
    
    void setStatus(int status);
    
    void setCurrentPosition(ofPoint p);
    void flipDirection(int direction);
    
    
private:
    ofImage tag_icon;
    
    int off_start_time;
    float off_duration;

    int flash_start_time;
    float flash_duration;
    
    ofPolyline curve_path;
    ofPoint current_pos;
    
    bool animation_flag;
    
    int current_status;
    
    
    ofColor beaconColor;
    
    // for the icon offset
    int current_x_offset, current_y_offset;
    int x_offset_left, y_offset_left, x_offset_right, y_offset_right;
    
    // for the tip offset
    int current_tip_x_offset, current_tip_y_offset;
    int tip_x_offset_left, tip_y_offset_left, tip_x_offset_right, tip_y_offset_right;
    
    // animation offset
    int anim_x_offset, anim_y_offset;
    
};


#endif
