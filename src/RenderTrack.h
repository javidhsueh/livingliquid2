//
//  RenderTrack.h
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#ifndef livingLiquid_RenderTrack_h
#define livingLiquid_RenderTrack_h

#include <vector>
#include <map>
#include <string>
#include "TrackPoint.h"
#include "ofImage.h"
#include "FingerTouch.h"
#include "AnimalPopout.h"
#include "AnimalPopoutPool.h"


enum AnimalStatus { DEBUGGING, INVISIBLE, DETECTABLE, DETECTED, TAGGED_FADE_IN, TAGGED_STABLE, TAGGED_FADE_OUT, SHOW_INFO };

enum FaceDirection { FACE_LEFT, FACE_RIGHT };

enum OrientaionMode {NONE, ROTATE, FLIP};


class Boat;


class RenderTrack{
public:
    RenderTrack(std::string species, std::string id, std::vector<TrackPoint*> point, std::string start_date, int trackdays, int gap_days, int max_gap);
    
    std::string getSpeciesName(){   return species; }
    std::string getID(){    return id;  }
    int getTrackDays(){     return track_days;  }
    int getMaxGap(){    return max_gap;     }
    
    ofPoint getCurrentPosition();
    ofColor getColor();
    int getOpacity();

    void update(int speed);
    void draw(int speed);
    void drawIcon(int speed);
    
    
    void resetDetectionStatus();
    bool detectNearbyBoat(Boat* b, int day);
    
    bool detectExtraDistancebyBoat(Boat* b, int day);
    
    void toggleSelected();
    void setOrientationMode(int mode);
    
    void setSelected(bool flag){    this->isSelected = flag;    }
    bool selected(){                return this->isSelected;    }
    
//    void setShowInfo(bool flag){    this->isShowInfo = flag;    }
    void toggleShowInfo();
    
    
    void setBuoyantAnimation(bool flag){    this->buoyant_animation = flag;                     }
    void toggleBuoyantAnimation(){          this->buoyant_animation = !this->buoyant_animation; }
    void toggleShowTaggedDate(){            this->show_tagged_date = !this->show_tagged_date;   }
    
    
    bool testTouch(int x, int y);
    
    void setTagged(int day);
    bool isTagged();
    
    void setDetected(float distance);
    bool isDetected();
    bool isDetectable();
    

    void fadeIn();
    void fadeOut();
    void setFadeInTime(int t){  this->fadeInTime = t;   }
    void setFadeOutTime(int t){ this->fadeOutTime = t;  }
    
    
    void reset();
    
//    void setDebug(bool flag){   this->debugFlag = flag;     }
    void toggleDebugMode();
    
    void print(){ofLog(OF_LOG_NOTICE, this->species+", "+this->id);};
    
private:
    
    //private member functions
    void setVisibilityStatus(int status);
    
    
    // basic attributes:
    std::string start_date;
    std::string species;
    std::string id ;
    std::map<int, TrackPoint*> points; // time to point

    // statistics attributes
    int track_days;
    int gap_days;
    int max_gap;
    

    // track setting
    
    ofPolyline pathline;
    ofColor inner_path_color;   // the color of the inner path (hex, opacity)
    int inner_path_opacity;     // the opacity of the inner path
    float inner_path_width;     // the width of inner path

    ofColor outter_path_color;  // the color of the inner path (hex, opacity)
    float outter_path_width;    // the width of outter path
    
    float track_end_circle_radius;  // the radius of the circle at the tagged location
    float track_end_circle_line_width; // the border of the end circle
    
    // icon image
    ofImage icon;
    ofImage tagged_icon;
    int icon_width, icon_height;
    int detected_opacity;
    
    // orientation
    int orientationMode;
    int faceDirection, nextFaceDirection;
//    int prevFaceDirection;
    
    // animated effect
    bool buoyant_animation;
    int buoyant_size_offset;
    int buoyant_timer;
    int buoyant_interval;
    
    // parameters
    float detected_dist;
    float touch_detect_radius;
    int path_smoothness;
 
    // loop control
    bool isPlaying;         // this track is playing its cycle
    int start_date_idx;     // the start date of the timeline
    int end_date_idx;       // the end date of the timeline
    int current_date_idx;   // current date idx
    int cycle;              // the loop calender year cycle
    
    // status
    bool isSelected; // true: this track is selected to display
    
    int prev_visibility_status;
    int visibility_status;
    
 
    // fade in/out control
    //std::string fadeType;
    int fadeStartTime;
    int fadeInTime;
    int fadeOutTime;
    int current_opacity;
    
    
    // display tagged date
    bool show_tagged_date;
    ofPoint text_position;
    std::string tagged_date;
    ofColor tagged_date_font_color;
    
    // time
    int tagged_day_idx;     // the day it got tagged
    int current_frame;      // current frame

    // drawing data and positions
    ofPoint tagged_point;
    ofPoint current_point;
    ofPoint lastPoint;
    
    float lastAngle;
    
    
    
    // widget
    AnimalPopout* popout;

};

#endif
