//
//  WavePool.h
//  example-waterRipples
//
//  Created by Javid on 10/22/14.
//
//

#ifndef example_waterRipples_WavePool_h
#define example_waterRipples_WavePool_h

#define MAX_WAVE_NUM 8

#include <vector>
#include "Wave.h"
#include "ofPoint.h"

class WavePool{
public:
    static WavePool* singleton();
    
    Wave* getAvailableWave();
    
    void update();
    
    void draw();
    
    void toggelVisibility();
    
private:
    
    static WavePool* instance;
    
    WavePool();
    
    bool isShow;
    
    int counter ;
    std::vector<Wave*> pool;
};

#endif
