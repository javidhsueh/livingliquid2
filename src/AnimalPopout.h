//
//  AnimalPopout.h
//  example-waterRipples
//
//  Created by javid on 8/27/14.
//
//

#ifndef example_waterRipples_AnimalPopout_h
#define example_waterRipples_AnimalPopout_h


#include <string>

#include "ofPolyline.h"

class RenderTrack;

class AnimalPopout{
public:
    AnimalPopout();
    
    void setVisibility(bool flag);
    
    void update();
    void setPosition(int x, int y);
    
    void attach(RenderTrack* rt);
    void detach();
    
    
    void fadeIn();
    void fadeOut();
    
private:
    
    //status
    bool isShow;
    
    // fading control
    std::string fadeType;
    int max_opacity;
    float current_opacity;
    
    int fadeStartTime;
    int fadeInTime;
    int fadeOutTime;
    
    
     ofPolyline* line;
    
    // data
    RenderTrack* rt;
    ofPoint pos;
    std::string title;
    std::string species_name;
    std::string topp_id;
    std::string time;
    std::string desc;
    
};

#endif
