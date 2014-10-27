//
//  FingerTouch.h
//  ll2
//
//  Created by javid on 7/18/14.
//
//

#ifndef ll2_FingerTouch_h
#define ll2_FingerTouch_h

#include "ofMain.h"
#include "TouchElement.h"

class FingerTouch : public TouchElement{
public:
    
    FingerTouch(int tid, ofPoint p);
    ~FingerTouch();
    
    void draw();
    
    bool isWithinRange(int pointX, int pointY);
    
private:
    
    int radius, squaredRadius;
};


#endif
