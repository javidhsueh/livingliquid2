//
//  Boat.h
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#ifndef livingLiquid_Boat_h
#define livingLiquid_Boat_h

//#include "RenderTrack.h"
#include <vector>


#include "ofMain.h"
#include "ofImage.h"
#include "TouchElement.h"
#include "RenderTrack.h"

//class RenderTrack; // forward declaration


class Boat : public TouchElement {

public:
    Boat(int tid, ofPoint p);
    ~Boat();
    
    void draw();
    
    void mouseMove(int x, int y);
    void mousePressed(int x, int y);
    
    bool isInside(int x, int y);
    float isWithinRange(int pointX, int pointY);
    
    void addAnimal(RenderTrack*);
    
    void setPosition(ofPoint p);
    
    void toggleDetectExtraNearby();
    bool isWithinNearbyRange(int pointX, int pointY);
    void clearNearbyAnimal();
    void addNearbyAnimal(RenderTrack*);

private:
    
    ofImage img;
    
    int puck_img_size;
    int puck_radius, detect_radius, squaredDetectRadius;

    bool isExtraDetect;
    float squaredExtracDetectRadius;
    float extra_detect_radius;
    
    float sonar_circles[3];
    float boat_sonar_freq;
    
    bool showDetectRange;
    bool isExpand;
    int expand_timer;
    int expect_expand_time;
    
    std::vector<RenderTrack*> collected_animals;
    
    
    std::vector<RenderTrack*> nearby_animals;
    
};

#endif
