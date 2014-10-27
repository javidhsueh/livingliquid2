//
//  Boat.cpp
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#include "Boat.h"
#include "ConfigLoader.h"
#include "ofUtils.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <math.h>
#include "Logger.h"
#include "ImageResources.h"


Boat::Boat(int tid, ofPoint p)
    :TouchElement(tid, p)
{
    
    // load size parameters
    this->puck_radius = ofToInt(ConfigLoader::singleton()->Value("boat", "boat_puck_radius"));
    this->detect_radius = ofToInt(ConfigLoader::singleton()->Value("boat", "boat_detect_radius"));
    this->squaredDetectRadius = (this->detect_radius) * (this->detect_radius);
    this->puck_img_size = this->puck_radius * 2;
    
    this->isExtraDetect = ofToBool( ConfigLoader::singleton()->Value("System", "showExtraDetect") );
    this->extra_detect_radius = ofToInt(ConfigLoader::singleton()->Value("boat", "extra_detect_radius"));
    this->squaredExtracDetectRadius = this->extra_detect_radius * this->extra_detect_radius;
    
    this->showDetectRange = false;
    
    // load images
    if(!this->img.loadImage(ConfigLoader::singleton()->Value("boat", "disc_image")) ){
        Logger::singleton()->log("Error: Fail to load boat image.");
        this->img.loadImage(ConfigLoader::singleton()->Value("icon_file_path", "Fail")); // load a default image
    }
    
    //expanding animation
    this->isExpand = false;
    this->expect_expand_time = ofToInt(ConfigLoader::singleton()->Value("boat", "boat_expanding_time"));
    this->expand_timer = ofGetElapsedTimeMillis();
    
    // initialize the sonar circles
    for(int i = 0 ; i < 3; i++){
        this->sonar_circles[i] = this->puck_radius + (this->detect_radius-this->puck_radius)* (i/3.0);
    }
    this->boat_sonar_freq = ofToFloat(ConfigLoader::singleton()->Value("boat", "boat_sonar_freq"));
}

Boat::~Boat(){

}

void Boat::draw(){
    
    if(!isExpand){
        
        int elapsed_time = ofGetElapsedTimeMillis() - this->expand_timer;
        
        if(elapsed_time < this->expect_expand_time){
            
            double ratio = 1.0*elapsed_time/this->expect_expand_time;
            
            ofSetColor(255,255,255, 120*ratio);
            ofNoFill();
            this->img.draw(this->pos.x-this->puck_radius*ratio,this->pos.y-this->puck_radius*ratio, this->puck_img_size*ratio, this->puck_img_size*ratio);
            
        }else{
            ofSetColor(255,255,255, 120);
            ofNoFill();
            this->img.draw(this->pos.x-this->puck_radius,this->pos.y-this->puck_radius, this->puck_img_size, this->puck_img_size);
            this->isExpand = true;
            
        }
        
        ofNoFill();
        ofSetLineWidth(2);
        for(int i =0 ; i< 3; i++){
            this->sonar_circles[i] += this->boat_sonar_freq;
            
            if(this->sonar_circles[i] < this->detect_radius){
                int alpha = ofMap(this->sonar_circles[i], this->puck_radius, this->detect_radius, 240, 30);
                ofSetColor(255,130,0, alpha);
                ofCircle(this->pos.x, this->pos.y, this->sonar_circles[i]);
            }else{
                this->sonar_circles[i] = this->puck_radius;
            }
        }
        
        return;
    }

    ofSetColor(255,255,255, 120);
    ofNoFill();
    
    this->img.draw(this->pos.x-this->puck_radius,this->pos.y-this->puck_radius, this->puck_img_size, this->puck_img_size);
    
    ofNoFill();
    ofSetLineWidth(2);
    for(int i =0 ; i< 3; i++){
        this->sonar_circles[i] += this->boat_sonar_freq;
        
        if(this->sonar_circles[i] < this->detect_radius){
            int alpha = ofMap(this->sonar_circles[i], this->puck_radius, this->detect_radius, 240, 30);
            ofSetColor(255,130,0, alpha);
            ofCircle(this->pos.x, this->pos.y, this->sonar_circles[i]);
        }else{
            this->sonar_circles[i] = this->puck_radius;
        }
    }
    
    // debug only
//    ofSetColor(255,130,0);
//    ofCircle(this->pos.x, this->pos.y, this->detect_radius);
    
    if(this->showDetectRange){
        ofSetColor(255,0,0, 120);
        ofNoFill();
        ofCircle(this->pos.x, this->pos.y, this->detect_radius);
    }
    
    if(this->isExtraDetect){
        ofSetColor(50,50,50, 50);
        ofNoFill();
        ofCircle(this->pos.x, this->pos.y, this->detect_radius);
        float small_circle_radisu = 8;
        
        for(auto rt = this->nearby_animals.begin(); rt!=this->nearby_animals.end(); rt++){
            RenderTrack* r = (*rt);
            
            ofSetColor(r->getColor(),100);
            ofFill();
            
            ofPoint position = r->getCurrentPosition();
            ofPoint direction = position - this->pos;
            direction = direction.normalize();
            ofPoint circle_position = this->pos + direction.getScaled(this->detect_radius);
            ofCircle(circle_position.x, circle_position.y, small_circle_radisu);
            
            
            ofTexture* icon = ImageResources::singleton()->getImage(r->getSpeciesName());
            ofPoint icon_pos(circle_position.x-small_circle_radisu, circle_position.y-small_circle_radisu);
            ofSetColor(ofColor(255,255,255));
            icon->draw(icon_pos, small_circle_radisu*2, small_circle_radisu*2);
            
        }
    }
    
}

void Boat::mouseMove(int x, int y){
    this->pos.x = x , this->pos.y = y;
}

void Boat::mousePressed(int x, int y){

    this->pos.x = x, this->pos.y = y;
    // give some visual feedback
}


float Boat::isWithinRange(int pointX, int pointY){
    
    float dX = (this->pos.x - pointX) * (this->pos.x - pointX);
    float dY = (this->pos.y - pointY) * (this->pos.y - pointY);
    if ( dX + dY <= this->squaredDetectRadius ){
        return sqrt(dX+dY)/this->detect_radius;
    }
    return -1;
}

bool Boat::isInside(int x, int y){
    float dX = (this->pos.x - x) * (this->pos.x - x);
    float dY = (this->pos.y - y) * (this->pos.y - y);
    if ( dX + dY <= this->puck_radius*this->puck_radius ){
        return true;
    }
    return false;
}

void Boat::addAnimal(RenderTrack *rt){
    
    this->collected_animals.push_back(rt);
    
}

void Boat::toggleDetectExtraNearby(){
    this->isExtraDetect = !this->isExtraDetect;
}

bool Boat::isWithinNearbyRange(int pointX, int pointY){
    if(!this->isExtraDetect)
        return false;
    
    float dX = (this->pos.x - pointX) * (this->pos.x - pointX);
    float dY = (this->pos.y - pointY) * (this->pos.y - pointY);
    if ( dX + dY <= this->squaredExtracDetectRadius && dX + dY > this->squaredDetectRadius ){
        return true;
    }
    return false;
}

void Boat::addNearbyAnimal(RenderTrack* rt){
    this->nearby_animals.push_back(rt);
}

void Boat::clearNearbyAnimal(){
    this->nearby_animals.clear();
}

void Boat::setPosition(ofPoint p){
    
    if( abs(p.x - this->getPosition().x) > 1 || abs(p.y - this->getPosition().y) > 1)
        Logger::singleton()->log("EVENT: boat:"+ofToString(tid) +", moves to:"+ofToString(p));

    TouchElement::setPosition(p);
}

