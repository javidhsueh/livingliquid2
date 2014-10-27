//
//  AnimalPopoutPool.h
//  example-waterRipples
//
//  Created by javid on 8/27/14.
//
//

#ifndef example_waterRipples_AnimalPopoutPool_h
#define example_waterRipples_AnimalPopoutPool_h

#define MAX_POPOUT_NUM 5

#include "AnimalPopout.h"
#include <vector>

class RenderTrack;

class AnimalPopoutPool{
public:
    static AnimalPopoutPool* singleton();
    
    AnimalPopout* getAvailablePopout(RenderTrack* rt);
    void releasePopout(AnimalPopout* popout);
    
    void update();
    
private:
    static AnimalPopoutPool* instance;
    
    AnimalPopoutPool();
    
    int counter ;
    std::vector<AnimalPopout*> pool;
};

#endif
