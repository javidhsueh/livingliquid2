//
//  Legend.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 8/21/14.
//
//

#ifndef example_waterRipples_Legend_h
#define example_waterRipples_Legend_h

#include "ofImage.h"
#include "ofColor.h"
#include "ofTrueTypeFont.h"
#include "Util.h"
#include <map>
#include <vector>
#include <string>


class Legend {
public:
    Legend();
    
    void toggle();
    void update();
    void draw();
    
private:
    bool isShow;
    
    // data: selected species
    std::vector<std::string> selected_species;
    
    // map : species to icon
    std::map<std::string, ofImage> species2icon;
    
    // map: species: to color
    std::map<std::string, ofColor> species2color;
    
};

#endif
