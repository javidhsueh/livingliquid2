//
//  Map.h
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#ifndef livingLiquid_Map_h
#define livingLiquid_Map_h

#include "RenderTrack.h"
#include "ofImage.h"
#include "Boat.h"
#include <vector>
#include <map>
#include "ofPoint.h"
#include "TouchElement.h"
#include "Timeline.h"
#include "SeasonClock.h"

#include "ofxRipples.h"
#include "ofxBounce.h"
#include <map>
#include "WavePool.h"

//#include "ofxProgressBar.h"


class Map {
public:
    Map();
    
    void update();
    void draw();
    void mouseMove(int x, int y);
    void mousePressed(int x, int y);
    void mouseDragged(int x, int y);
    
    // data operations
    void updateSelectedSpecies();
    void setSpeed(int v);
    
    // status operations
    void reset();
    void toggleDebugMode();
    void toggleMouseMode();
    void toggleClock();
    void setIconOrientationMode(int mode);
    void toggleBuoyantAnimation();
    void toggleTimeline();
    
    // toggle layers
    void toggleContinentLabelLayer();
    void toggleSiteLabelLayer();
    void togglePinLabelLayer();
    
    void toggleShowTaggedDate();
    void toggleShowExtraDetect();
    
    void updateFadeInTime(int t);
    void updateFadeOutTime(int t);
    
    // touch events
    void tuioFingerAdded(int tid, ofPoint point);
    void tuioFingerUpdated(int tid, ofPoint point);
    void tuioFingerRemoved(int tid, ofPoint point);
    
    void tuioObjAdded(int tid, ofPoint point);
    void tuioObjUpdated(int tid, ofPoint point);
    void tuioObjRemoved(int tid, ofPoint point);
    
    
private:
    Timeline* timeline;
    SeasonClock* clock;
    
    // background image
    ofImage bg_map;

    // lable layers
    ofImage continent_label_layer;
    ofImage site_label_layer;
    ofImage pin_label_layer;
    
    bool isShowContinentLabel;
    bool isShowSiteLabel;
    bool isShowPinLabel;
    
    
    
    
    bool land_mask[1920][1080];
    
    std::map<int, ofImage> sst_maps;
    
    // container
    int tuio_point_num;
    std::map<int , TouchElement* > tuio_points;
    std::map<int , int > tuio_points_alive_time;
    std::map<int , Boat* > boats;
    
    // data
    std::vector<RenderTrack*> tracks;
    

    // parameters
    int speed; //days per second
        
    // status, flags
    bool isDebug;
    int current_day;
    


    // mouse mode
    Boat* mouseboat;
    bool mouseMode;
    int mouseX, mouseY;
    
    
    // touch mode
    bool touchMode;
    
    
    // animation control
    float start_time;
    
    int current_progress, max_progress;
//    ofxProgressBar sst_progress_bar;
    
    ofxRipples  rip;
    ofxBounce   bounce;
    ofTrueTypeFont textfont;
    
    //random wave frequency
    int wave_freq;
    
};

#endif
