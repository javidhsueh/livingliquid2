//
//  Timeline.h
//  livingLiquid
//
//  Created by javid on 7/14/14.
//
//

#ifndef livingLiquid_Timeline_h
#define livingLiquid_Timeline_h

#include <vector>
#include <string>
#include "ofMain.h"

class Timeline{
public:
    Timeline();
    void draw(int speed);
    
    void toggle();
    
private:
    
    bool isShow;
    
    int time;
    int tick_size;
    ofColor line_color;
    
    ofTrueTypeFont* max_font;

	std::string monthNames[12];
};

#endif
