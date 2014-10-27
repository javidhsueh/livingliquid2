//
//  WavePool.cpp
//  example-waterRipples
//
//  Created by Javid on 10/22/14.
//
//

#include "WavePool.h"

#include "ConfigLoader.h"


WavePool* WavePool::instance = 0;
WavePool* WavePool::singleton(){
    if(!instance)
        instance = new WavePool();
    return instance;
}

WavePool::WavePool(){
    
    this->counter = 0;
    // spawn popouts
    for(int i = 0; i < MAX_WAVE_NUM; i+=1){
        Wave* w = new Wave();
        this->pool.push_back(w);
    }
    this->isShow = true;//ofToBool( ConfigLoader::singleton()->Value("System", "showWave") );
}


Wave* WavePool::getAvailableWave(){
    
    if(!this->isShow)   return 0;
    
    int start_idx = counter ;
    int idx = (this->counter+1) % MAX_WAVE_NUM;
    
    while( idx != -1 && this->pool[idx]->isPlaying()){
        this->counter = (this->counter+1) % MAX_WAVE_NUM;
        idx = this->counter;
        if(idx == start_idx){
            idx = -1;
            break;
        }
    }
    if(idx == -1){
        return 0;
    }else{
        Wave* w = this->pool[idx];
        return w;
    }
}

void WavePool::update(){
    
    if(!this->isShow)   return;
    
    for(int i = 0; i < MAX_WAVE_NUM; i+=1){
        this->pool[i]->update();
    }
}

void WavePool::draw(){
    
    if(!this->isShow)   return;
    
    for(int i = 0; i < MAX_WAVE_NUM; i+=1){
        this->pool[i]->draw();
    }
}

void WavePool::toggelVisibility(){
    this->isShow = !this->isShow;
}