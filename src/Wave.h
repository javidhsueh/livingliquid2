//
//  Wave.h
//  example-waterRipples
//
//  Created by Javid on 10/22/14.
//
//

#ifndef example_waterRipples_Wave_h
#define example_waterRipples_Wave_h

#include "ofPoint.h"
#include "ofxGif.h"

class Wave {
public:
    
    Wave();
   
    void setPosition(ofPoint pos);
        
    void update();
    
    void draw();
    
    void startPlay();
    
    bool isPlaying();
   
private:
    
    bool isPlay;
    
    int page_idx;
    ofPoint position;
    int opacity;
    ofxGIF::fiGifLoader animation;
    
};

#endif
