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
#include <map>

class Dashboard {
    
public:
    
    Dashboard();
    ~Dashboard();
    
    void toggle();
    ofxUISuperCanvas *gui;
    
    void toggleOrientationMode(int );
    void toggleBuoyantAnimation();

    
    void toggleLegends();
    
    // map layers
    void toggleContinentLabels();
    void toggleSiteLabels();
    void togglePinLabels();
    
    
    void toggleSeasonClock();
    void toggleTimeline();
    
    void toggleTaggedDate();
    void toggleExtraDetect();
    
    void toggleWave();
    
    float speed, min_track_days, max_gap;
    
    float fadeInTime, fadeOutTime;
    
    
    bool isShow;
    
    ofxUIToggle* icon_orientation_none;
    ofxUIToggle* icon_orientation_rotate;
    ofxUIToggle* icon_orientation_flip;
    
    std::map<std::string, ofxUIToggle*> species_toggle_map;
    
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
    
    bool showWave;
    ofxUIToggle* wave_toggle;
    
    // map layers:
    
    bool showContinentLabels;
    ofxUIToggle* continent_labels_toggle;
    
    bool showSiteLabels;
    ofxUIToggle* site_labels_toggle;
    
    bool showPinLabels;
    ofxUIToggle* pins_labels_toggle;
    
    
};

#endif
