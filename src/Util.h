//
//  Util.h
//  example-waterRipples
//
//  Created by javid on 8/8/14.
//
//

#ifndef example_waterRipples_Util_h
#define example_waterRipples_Util_h

#include "ofApp.h"
#include <string>

namespace Util{
    
    const std::string currentDateTime();
    
    ofColor hexToColor(std::string hexString);
    ofColor hexToColor(std::string hexString, int opacity);

    int dateToDayIndex(std::string dateString);
    
    std::string dayIndexToDate(int day);
    
    
    float interpolate(float current, float min, float max, float value);
    
    
}


#endif
