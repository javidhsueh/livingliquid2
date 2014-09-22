//
//  Dashboard.h
//  example-waterRipples
//
//  Created by javid on 8/1/14.
//
//

#ifndef example_waterRipples_Dashboard_h
#define example_waterRipples_Dashboard_h


#include "ofxUI.h"


class Dashboard {
    
public:
    
    Dashboard();
    ~Dashboard();
    
    void toggle();
    ofxUISuperCanvas *gui;
    
    void toggleOrientationMode(int );
    void toggleBuoyantAnimation();

    
    void toggleLegends();
    void toggleMapLabels();
    
    void toggleSeasonClock();
    void toggleTimeline();
    
    void toggleTaggedDate();
    void toggleExtraDetect();
    
    float speed, min_track_days, max_gap;
    
    float fadeInTime, fadeOutTime;
    
private:
    
    bool isShow;
    
    ofxUIToggle* icon_orientation_none;
    ofxUIToggle* icon_orientation_rotate;
    ofxUIToggle* icon_orientation_flip;
    
    bool is_buoyant_animation;
    ofxUIToggle* icon_buoyant_animation;
    
    bool showTimeline;
    ofxUIToggle* timeline_toggle;
    
    bool showLegends;
    ofxUIToggle* legends_toggle;
    
    bool showSeasonclock;
    ofxUIToggle* seasonclock_toggle;
    
    
    bool showTaggedDate;
    ofxUIToggle* tagged_date_toggle;
    
    bool showExtraDetect;
    ofxUIToggle* extra_detect_toggle;
    
    bool showMapLabels;
    ofxUIToggle* map_labels_toggle;
    
};

#endif
