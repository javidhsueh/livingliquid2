//
//  Pen.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/2/14.
//
//

#ifndef example_waterRipples_Pen_h
#define example_waterRipples_Pen_h

#include <map>
#include <string>
#include "ofTrueTypeFont.h"

class Pen{
public:
    static Pen* singleton();
    
    ofTrueTypeFont* getFont(std::string font_name);
    
private:
    static Pen* instance;
    Pen();
    
    std::map<std::string, ofTrueTypeFont*> fonts;
    
};

#endif
