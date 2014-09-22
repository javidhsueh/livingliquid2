//
//  Map.cpp
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#include "Map.h"
#include "ConfigLoader.h"
#include "ofApp.h"
#include "ConfigLoader.h"
#include "DataManager.h"
#include "ofUtils.h"
#include "FingerTouch.h"
#include <iostream>
#include "Pen.h"



Map::Map(){
    // load background map image
    if(!this->bg_map.loadImage(ConfigLoader::singleton()->Value("System", "bg_map")) ){
        ofLog(OF_LOG_FATAL_ERROR, "Fail to load background map image.");
    }
    if(!this->label_layer.loadImage(ConfigLoader::singleton()->Value("System", "label_layer")) ){
        ofLog(OF_LOG_FATAL_ERROR, "Fail to load background map image.");
    }
    
    
    // load parameters:
    this->speed = 60/ofToInt(ConfigLoader::singleton()->Value("speed", "display_speed"));
    
    // load land mask
    std::string land_mask_filename = ConfigLoader::singleton()->Value("System","land_mask");
    ofFile csv = ofFile(land_mask_filename);
	ofBuffer csvBuffer = csv.readToBuffer();
    std::string currentLine;
    vector<string> splitItems;
    int line_number = 0;
	while (! csvBuffer.isLastLine()) {
        currentLine = csvBuffer.getNextLine();
		splitItems = ofSplitString(currentLine, ",");
        
        for(int x = 0 ; x < 1920; x++){
            this->land_mask[x][line_number] = ofToBool(splitItems[x]);
        }
        line_number++;
	}
    
    // init status, flags
    
    // time information
    this->current_day = 0;
    this->start_time = ofGetElapsedTimeMillis();
    
    // information layer mode
    this->isDebug = false;

    
    this->isShowLabel = ofToBool( ConfigLoader::singleton()->Value("System", "showMapLabels") );
    
    // operating mode
    this->mouseMode = false;
    this->touchMode = false;
    this->tuio_point_num = 0;
    
    // widgets
    this->timeline = new Timeline();
    this->clock = new SeasonClock();

    // get the all tracks
    this->tracks = DataManager::singleton()->getSelectedTracks();
    
    // setup water ripple setting
    rip.damping = 0.92;
    rip.allocate(1920,1080);
    bounce.allocate(1920,1080);
    bounce.setTexture(this->bg_map.getTextureReference(), 1);
    
    // setup the debug text font
    textfont.loadFont("font/Sansation_Light.ttf", 16);
    
}

void Map::updateSelectedSpecies(){
    this->tracks = DataManager::singleton()->getSelectedTracks();
}

void Map::setSpeed(int v){
    this->speed = 60/v;
}

void Map::update(){

    rip.begin();
    ofFill();
    ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
    
    for(auto b = this->boats.begin(); b != this->boats.end(); ++b){
        Boat* boat = b->second;
        ofPoint p = boat->getPosition();
        if(!land_mask[int(p.x)][int(p.y)])
            ofEllipse(p.x, p.y,3,3);
        boat->clearNearbyAnimal();
    }
    
    rip.end();
    rip.update();
    
    bounce << rip;
    bounce.update();
    
    //update time first:
    this->current_day = int(ofGetFrameNum()/this->speed)%365;
    
    if( ofGetFrameNum() %this->speed == 0 && this->current_day == 0 ){
        this->reset();
        this->start_time = ofGetElapsedTimeMillis(); // reset start time
        this->current_day = 0;  //reset to day 0
    }
    
    // update tuio points alive time: every update increase one second
    for(auto p = this->tuio_points_alive_time.begin(); p != this->tuio_points_alive_time.end(); p++){
        
        // update alive time
        int tid = p->first;
        p->second +=1 ;
        TouchElement* te = this->tuio_points[tid];
        
        // if it's a finger touch, try to detect if it's a boat
        if( p->second > 120 && !dynamic_cast<Boat*>(te)){
            
            ofPoint p_position = te->getPosition();
            // detecting boats
            bool isCloseToOtherPoint = false;
            for(auto finger = this->tuio_points.begin(); finger != this->tuio_points.end(); finger++){
                TouchElement* ft = finger->second;
                if(finger->first != tid && ft->withinRadius(p_position, 50)){
                    isCloseToOtherPoint = true;
                    break;
                }
            }
            // alive > 5 seconds, and no nearby points
            if(!isCloseToOtherPoint){
                
                // this is a boat: remove the finger object,
                delete te;
                
                // replace it as a boat object
                Boat* b = new Boat(tid, p_position);
                this->tuio_points[tid] = b;
                this->boats[tid] = b;
                break;
            }
        }
    }
    
    // update tracks
    for(auto track = this->tracks.begin(); track != this->tracks.end(); track++){

        (*track)->resetDetectionStatus();
        if( (*track)->isDetectable()){
            for(auto b = this->boats.begin(); b != this->boats.end(); b++){
                Boat* boat = b->second;
                if( (*track)->detectExtraDistancebyBoat( boat, this->current_day)){
                    boat->addNearbyAnimal((*track));
                }
                (*track)->detectNearbyBoat( boat, this->current_day);
            }
        }
        (*track)->update(this->speed);
    }
    
    // update popouts
    AnimalPopoutPool::singleton()->update();
}

void Map::draw(){
    
    ofBackground(255,255,255);
    ofSetColor(255,255,255,250);

    ////////////////////////////////////////////////////////////////////////////
    
    // this->bg_map.draw(0,0, ofGetWidth(), ofGetHeight()); // bg map image
    bounce.draw(0,0);
    
    if(this->isShowLabel){
        ofSetColor(255,255,255,80);
        label_layer.draw(0,0);
    }
    
    // draw tracks
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->draw(this->speed); // draw by current time
    }
    
    // draw icons
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->drawIcon(this->speed); // draw by current time
        
    }
    // draw tuio elements
    
    for(auto el = this->tuio_points.begin(); el != this->tuio_points.end(); ++el){
        TouchElement* e = el->second;
        e->draw();
    }
    
    // update popouts
    AnimalPopoutPool::singleton()->update();
    
    
    // draw timeline
    this->timeline->draw(this->speed);
    
    this->clock->draw(this->speed);
    
    // draw current mode
    if( this->touchMode ){
        textfont.drawString("Touch mode", 20, 1060);
    }else if( this->mouseMode ){
        textfont.drawString("Mouse mode", 20, 1060);
    }
    
//    textfont.drawString(ofToString(ofGetFrameRate()), 1890, 20);
//    textfont.drawString(ofToString(ofGetFrameNum()), 20, 20);

}

void Map::mouseMove(int x, int y){
//    if(!this->mouseMode)
//        return;

    //this->mouseboat->mouseMove(x, y);

    //rip.damping = ofMap(y, 0, ofGetHeight(), 0.9, 1.0, true);
}

void Map::mousePressed(int x, int y){
    
    if(!this->mouseMode)
        return;
    
    // detect click on ocean
    if(!land_mask[x][y]){
        // visual effect
        rip.begin();
        ofFill();
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        ofEllipse(x, y,2,2);
        rip.end();
        rip.update();
        
        for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
            if((*rt)->testTouch(x, y)){
                // if it's not tagged, see if there's a boat nearby
                if(!(*rt)->isTagged()){
                    for(auto b = this->boats.begin(); b != this->boats.end(); ++b){
                        Boat* boat = b->second;
                        if(boat->isWithinRange(x, y) > 0){
                            (*rt)->setTagged(this->current_day);
                            boat->addAnimal((*rt));
                            break;
                        }
                    }
                }else{//it's tagged
                    (*rt)->toggleShowInfo();
                    
                }
            }
        }
        
    }
    
    
}

void Map::mouseDragged(int x, int y){
    
    for(auto b = this->boats.begin(); b != this->boats.end(); ++b){
        Boat* boat = b->second;
        if(boat->isInside(x, y)){
            boat->setPosition(ofPoint(x,y));
            return;
        }
    }

    
}

//----------------- TUIO Object ---------------------//

void Map::tuioObjAdded(int tid, ofPoint point){
    
    return; //debug only
    
    if(this->mouseMode)
        return;
    
    Boat* b = new Boat(tid, point);
    this->tuio_points[tid] = b;
    this->boats[tid] = b;
    
}

void Map::tuioObjUpdated(int tid, ofPoint point){
    
    return; //debug only
    
    if(this->mouseMode)
        return;
    
    TouchElement* el = (TouchElement*)(this->tuio_points[tid]);
    el->setPosition(point);
    
}

void Map::tuioObjRemoved(int tid, ofPoint point){
    
    return; //debug only
    
    if(this->mouseMode)
        return;
    
    auto el = this->tuio_points.find(tid);
    if(el != this->tuio_points.end()){
        delete el->second;
        this->tuio_points.erase(tid);
    }
    auto b = this->boats.find(tid);
    if(b != this->boats.end()){
        this->boats.erase(tid);
    }
    
}

//----------------- TUIO Finger ---------------------//

void Map::tuioFingerAdded(int tid, ofPoint point){
    
    if(this->mouseMode)
        return;
    
    
    //touch to tag animals
    FingerTouch* p = new FingerTouch(tid, point);
    this->tuio_points[tid] = p;
    this->tuio_points_alive_time[tid] = 0;
    
    // detect click on ocean
    if(!land_mask[int(point.x)][int(point.y)]){
        
        // visual effect
        rip.begin();
        ofFill();
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        ofEllipse(point.x, point.y,2,2);
        rip.end();
        rip.update();
        
        for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
            if((*rt)->testTouch(point.x, point.y)){
                // if it's not tagged, see if there's a boat nearby
                if(!(*rt)->isTagged()){
                    for(auto b = this->boats.begin(); b != this->boats.end(); ++b){
                        Boat* boat = b->second;
                        if(boat->isWithinRange(point.x, point.y) > 0){
                            (*rt)->setTagged(this->current_day);
                            boat->addAnimal((*rt));
                            break;
                        }
                    }
                }else{//it's tagged
                    (*rt)->toggleShowInfo();
                    
                }
            }
        }
        
    }
    
}

void Map::tuioFingerUpdated(int tid, ofPoint point){
    
    if(this->mouseMode)
        return;
    
    TouchElement* el = (TouchElement*)(this->tuio_points[tid]);
    el->setPosition(point);
}

void Map::tuioFingerRemoved(int tid, ofPoint point){
    
    if(this->mouseMode)
        return;
    
    auto el = this->tuio_points.find(tid);
    if(el != this->tuio_points.end()){
        delete el->second;
        this->tuio_points.erase(tid);
    }
    
    auto b = this->boats.find(tid);
    if(b != this->boats.end()){
        this->boats.erase(tid);
    }
    
    auto at = this->tuio_points_alive_time.find(tid);
    if(at != this->tuio_points_alive_time.end()){
        this->tuio_points_alive_time.erase(tid);
    }
    

}

//----------------- Status operations ---------------------//

void Map::reset(){

}



void Map::toggleDebugMode(){
    this->isDebug = ! this->isDebug;
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->toggleDebugMode();
    }
}

void Map::toggleClock(){
    this->clock->toggle();
}

void Map::toggleMouseMode(){
    this->mouseMode = !this->mouseMode;

    // if open, create new boat
    if(this->mouseMode){
        this->mouseboat = new Boat(0, ofPoint(960,540));
        this->tuio_points[0] = this->mouseboat;
        this->boats[0] = this->mouseboat;
    }else{
        auto el = this->tuio_points.find(0);
        if(el != this->tuio_points.end()){
            delete el->second;
            this->tuio_points.erase(0);
        }
        auto b = this->boats.find(0);
        if(b != this->boats.end()){
            this->boats.erase(0);
        }
    }
}


void Map::setIconOrientationMode(int mode){
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->setOrientationMode(mode);
    }
}

void Map::toggleBuoyantAnimation(){
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->toggleBuoyantAnimation();
    }
}

void Map::toggleTimeline(){
    this->timeline->toggle();
}

void Map::toggleShowTaggedDate(){
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->toggleShowTaggedDate();
    }
}

void Map::toggleShowExtraDetect(){
    for(auto b = this->boats.begin(); b != this->boats.end(); ++b){
        b->second->toggleDetectExtraNearby();
    }
}

void Map::toggleLabelLayer(){
    this->isShowLabel = !this->isShowLabel;
}

void Map::updateFadeInTime(int t){
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->setFadeInTime(t);
    }
    
}

void Map::updateFadeOutTime(int t){
    for(auto rt = this->tracks.begin(); rt != this->tracks.end(); ++rt){
        (*rt)->setFadeOutTime(t);
    }
}
