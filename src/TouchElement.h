//
//  TouchElement.h
//  ll2
//
//  Created by javid on 7/18/14.
//
//

#ifndef ll2_TouchElement_h
#define ll2_TouchElement_h

#include "ofMain.h"

class TouchElement{
public:
    TouchElement(int tid, ofPoint p): tid(tid), pos(p){};
    
    virtual ~TouchElement(){};

    //abstracted member functions
    virtual void draw() = 0;
    
    bool withinRadius(ofPoint p, float radius){
        float dX = (this->pos.x - p.x) * (this->pos.x - p.x);
        float dY = (this->pos.y - p.y) * (this->pos.y - p.y);
        if ( dX + dY <= radius){
            return true;
        }
        return false;
    }
    
    // getter, setter
    ofPoint getPosition()       {   return this->pos;   }
    virtual void setPosition(ofPoint p) {   this->pos = p;      }
    int getTID(){   return this->tid;   }
    
protected:
    int tid;
    ofPoint pos;
    
};

#endif
