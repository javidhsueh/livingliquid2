//
//  SeasonClock.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 8/22/14.
//
//

#ifndef example_waterRipples_SeasonClock_h
#define example_waterRipples_SeasonClock_h

#include "ofImage.h"


class SeasonClock{
public:
    
    SeasonClock();
    void update();
    void draw(int speed);
    void toggle();
    
    
private:
    
    bool isShow;
    int current_day;
        
    ofImage clock, arrow;
    
};


#endif
