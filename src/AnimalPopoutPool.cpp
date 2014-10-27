//
//  AnimalPopoutPool.cpp
//  example-waterRipples
//
//  Created by javid on 8/27/14.
//
//

#include "AnimalPopoutPool.h"


AnimalPopoutPool* AnimalPopoutPool::instance = 0;
AnimalPopoutPool* AnimalPopoutPool::singleton(){
    if(!instance)
        instance = new AnimalPopoutPool();
    return instance;
}

AnimalPopoutPool::AnimalPopoutPool(){
    
    counter = 0;
    // spawn popouts
    for(int i = 0; i < MAX_POPOUT_NUM; i+=1){
        AnimalPopout* popout = new AnimalPopout();
        this->pool.push_back(popout);
    }
}


AnimalPopout* AnimalPopoutPool::getAvailablePopout(RenderTrack *rt){
    int idx = ++counter % MAX_POPOUT_NUM;
    AnimalPopout* popout = this->pool[idx];
    popout->detach();
    popout->attach(rt);
    return popout;
}

void AnimalPopoutPool::releasePopout(AnimalPopout *popout){
    popout->detach();
}

void AnimalPopoutPool::update(){
    for(int i = 0; i < MAX_POPOUT_NUM; i+=1){
        this->pool[i]->update();
    }
}