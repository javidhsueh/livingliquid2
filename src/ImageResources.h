//
//  ImageResources.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/3/14.
//
//

#ifndef example_waterRipples_ImageResources_h
#define example_waterRipples_ImageResources_h

#include <map>
#include <string>
#include "ofImage.h"

class ImageResources{
public:
    static ImageResources* singleton();
    
    ofTexture* getImage(std::string name);

private:
    static ImageResources* instance;
    ImageResources();
    
    std::map<std::string, ofTexture*> images;
    
};

#endif
