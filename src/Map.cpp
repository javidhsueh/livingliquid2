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
#include <cstdlib>
#include "Wave.h"
#include "Logger.h"

Map::Map(){
    
    // load background map image
    if(!this->bg_map.loadImage(ConfigLoader::singleton()->Value("System", "bg_map")) ){
        Logger::singleton()->log("Error: Fail to load background map image.");
    }
    // load layers images
    if(!this->continent_label_layer.loadImage(ConfigLoader::singleton()->Value("System", "continent_label_layer")) ){
        Logger::singleton()->log("Error: Fail to load continent label image.");
    }
    if(!this->site_label_layer.loadImage(ConfigLoader::singleton()->Value("System", "site_layer")) ){
        Logger::singleton()->log("Error: Fail to load tagging site image.");
    }
    if(!this->pin_label_layer.loadImage(ConfigLoader::singleton()->Value("System", "pin_layer")) ){
        Logger::singleton()->log("Error: Fail to load tagging site pins image.");
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

    // turn on/off layers
    this->isShowContinentLabel = ofToBool( ConfigLoader::singleton()->Value("System", "showContinentLabels") );
    this->isShowSiteLabel = ofToBool( ConfigLoader::singleton()->Value("System", "showSiteLabels") );
    this->isShowPinLabel = ofToBool( ConfigLoader::singleton()->Value("System", "showPinLabels") );
    
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
 
    this->wave_freq = ofRandom(150, 400);
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
                Logger::singleton()->log("OBJ: create boat:"+ofToString(tid) +", at:"+ofToString(p_position));
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
    
    //update wave animation
    
    
    // every XX frames, generate some random waves
    if (ofGetFrameNum() % this->wave_freq == 0){
        
        //change frequency
        this->wave_freq = ofRandom(150, 400);
        
        //select random tracks
        int wave_count = 0;
        std::vector<ofPoint> wave_pos;
        for(auto track = this->tracks.begin(); track != this->tracks.end(); track++){
            (*track)->resetDetectionStatus();
            if( (*track)->isDetectable()){
                wave_pos.push_back((*track)->getCurrentPosition());
                wave_count++;
                if(++wave_count == 10){
                    break;
                }
            }
        }
        
        int selected_wave_count = wave_pos.size() > 8? 8: wave_pos.size();
        
        std::random_shuffle ( wave_pos.begin(), wave_pos.end() );
        for(int i = 0 ; i< ofRandom(selected_wave_count);i++){
            ofPoint p = wave_pos[i];
            if(p.x< 0 || p.x> 1920 || p.y<0 || p.y > 1080 || land_mask[int(p.x)][int(p.y)])
                continue;
            Wave* w = WavePool::singleton()->getAvailableWave();
            if(w){
                w->setPosition(p);
                w->startPlay();
            }
        }
    }
    WavePool::singleton()->update();
}

void Map::draw(){
    
    ofBackground(255,255,255,255);
    ofSetColor(255,255,255,255);

    ////////////////////////////////////////////////////////////////////////////
    
//    ofBackgroundGradient(ofColor::white,ofColor(255,255,200), OF_GRADIENT_CIRCULAR);
    
    bounce.draw(0,0);
    
    if(this->isShowContinentLabel){
        ofSetColor(255,255,255,255);
        this->continent_label_layer.draw(0,0);
    }
    
    if(this->isShowSiteLabel){
        ofSetColor(255,255,255,135);
        this->site_label_layer.draw(0,0);
    }
    
    if(this->isShowPinLabel){
        ofSetColor(255,255,255,200);
        this->pin_label_layer.draw(0,0);
    }
    
    // show waves
    WavePool::singleton()->draw();
    
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
//    AnimalPopoutPool::singleton()->update();
    
    
    // draw timeline
    this->timeline->draw(this->speed);
    
    this->clock->draw(this->speed);
    
    // draw current mode
//    if( this->touchMode ){
//        textfont.drawString("Touch mode", 20, 1060);
//    }else if( this->mouseMode ){
//        textfont.drawString("Mouse mode", 20, 1060);
//    }
    
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
    
//    Logger::singleton()->log(OF_LOG_SILENT, "EVENT: create touch:"+ofToString(tid) +", at:"+ofToString(point));
    
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
                            Logger::singleton()->log("EVENT: boat:"+ofToString(tid) +", tag:"+(*rt)->getSpeciesName()+"("+(*rt)->getID()+")");
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
    
//    Logger::singleton()->log("EVENT: remove touch:"+ofToString(tid) +", at:"+ofToString(point));
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
        this->mouseboat = new Boat(0, ofPoint(780,540));
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

void Map::toggleContinentLabelLayer(){
    this->isShowContinentLabel = !this->isShowContinentLabel;
}

void Map::toggleSiteLabelLayer(){
    this->isShowSiteLabel = !this->isShowSiteLabel;
}

void Map::togglePinLabelLayer(){
    this->isShowPinLabel = !this->isShowPinLabel;
    
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
