//
//  Point.h
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#ifndef livingLiquid_TrackPoint_h
#define livingLiquid_TrackPoint_h

#include <string>
#include <iostream>

class TrackPoint {
public:
    
    TrackPoint(float x, float y, int time, bool lerp)
    :x(x), y(y), time(time), lerp(lerp)
    {}
    
    void print(){
        std::cout << time << " " << x << "," << y << std::endl;
    }
    
    float x, y;
    int time;
    bool lerp;

};

#endif
