//
//  TagOffAnimation.cpp
//  example-waterRipples
//
//  Created by Javid on 10/11/14.
//
//

#include "ofApp.h"
#include "Tag.h"
#include "ConfigLoader.h"
#include "Logger.h"

float gradien_radius[5] = {0.3, 1.8, 2.5, 5.5, 7.1};
float gradien_opacity[5] = {0.9, 0.5, 0.3, 0.12, 0.07};

Tag::Tag(std::string species)
{
    
    this->off_duration = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", "duration"));
    this->anim_y_offset = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", "anim_y_offset"));
    this->anim_x_offset = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", "anim_x_offset"));
    
    this->x_offset_left = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_x_left"));
    this->y_offset_left = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_y_left"));
    this->x_offset_right = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_x_right"));
    this->y_offset_right = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_y_right"));
    
    this->tip_x_offset_left = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_tip_x_left"));
    this->tip_y_offset_left = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_tip_y_left"));
    this->tip_x_offset_right = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_tip_x_right"));
    this->tip_y_offset_right = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", species+"_tip_y_right"));
    
    this->beaconColor = Util::hexToColor(ConfigLoader::singleton()->Value("tag_beacon_color", species));
    
    this->flash_duration = ofToInt(ConfigLoader::singleton()->Value("tag_icon_offset", "flash_duration"));;

    if(!this->tag_icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", species+"_tag"))){
        Logger::singleton()->log("Fail to load icon image for RenderTrack.");
        this->tag_icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", "Fail"));
    }
    
    this->animation_flag = false;
    this->current_x_offset = this->x_offset_right;
    this->current_y_offset = this->y_offset_right;
    
    this->current_tip_x_offset = this->tip_x_offset_right;
    this->current_tip_y_offset = this->tip_y_offset_right;
    
}

void Tag::setStatus(int status){
    this->current_status = status;
    
    if(status == TAGGED_FADE_IN){
        this->flash_start_time = ofGetElapsedTimeMillis();
    }
}

void Tag::flipDirection(int direction){
    if(direction == FACE_LEFT){
        // for icon
        this->current_x_offset = this->x_offset_left;
        this->current_y_offset = this->y_offset_left;
        // for the tip
        this->current_tip_x_offset = this->tip_x_offset_left;
        this->current_tip_y_offset = this->tip_y_offset_left;
    }else{
        // for icon
        this->current_x_offset = this->x_offset_right;
        this->current_y_offset = this->y_offset_right;
        // for tip
        this->current_tip_x_offset = this->tip_x_offset_right;
        this->current_tip_y_offset = this->tip_y_offset_right;
    }
    this->tag_icon.mirror(false, true);
}

void Tag::setCurrentPosition(ofPoint p){
    if(this->current_status != TAGGED_FADE_OUT){
        this->current_pos = p;
    }
}


void Tag::buildPath(ofPoint p){

    // build path
    ofPoint start_point(p.x+this->current_x_offset, p.y+this->current_y_offset);
    ofPoint dest_point(start_point.x, start_point.y-this->current_y_offset);
    ofPoint mid_point = start_point.middle(dest_point);
    mid_point.set(mid_point.x, mid_point.y+ this->anim_y_offset);
    this->curve_path.addVertex(this->current_pos);
    this->curve_path.addVertex(mid_point);
    this->curve_path.addVertex(dest_point);
}

void Tag::startAnimation(){
    
    this->off_start_time = ofGetElapsedTimeMillis();
    this->animation_flag = true;
    
    // build path
    this->current_pos.set(this->current_pos.x+this->current_x_offset,
                          this->current_pos.y+this->current_y_offset);
    ofPoint dest_point(this->current_pos);
    ofPoint mid_point = this->current_pos.middle(dest_point);
    mid_point.set(mid_point.x, mid_point.y+ this->anim_y_offset);
    this->curve_path.addVertex(this->current_pos);
    this->curve_path.addVertex(mid_point);
    this->curve_path.addVertex(dest_point);
    
}

void Tag::update(){
    
    
    
    
    if(this->current_status == TAGGED_FADE_IN ||
       this->current_status == TAGGED_STABLE){
        
        
        ofSetColor(0, 0, 0);
        ofFill();
        ofPoint tag_pos(this->current_pos.x+this->current_x_offset, this->current_pos.y+this->current_y_offset);
        this->tag_icon.draw(tag_pos);
        
        
        // draw beacon
        int elapsed_time = ofGetElapsedTimeMillis() ;
        elapsed_time -= this->flash_start_time;
        if(elapsed_time > this->flash_duration){
            this->flash_start_time = ofGetElapsedTimeMillis();
            elapsed_time = this->flash_duration;
        }
        
        float ratio = 0;
        float half_flash_duration = this->flash_duration /2.0;
        if(elapsed_time <= half_flash_duration){
            ratio = elapsed_time / half_flash_duration;
        }else{
            ratio = 1 - ((elapsed_time - half_flash_duration) / half_flash_duration);
        }
        
        float offset = 110+145* ratio;

        ofPoint tip_pos(this->current_pos.x+this->current_tip_x_offset, this->current_pos.y+this->current_tip_y_offset);
        
        /*
        myGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
        myFbo.begin();
            //ofClear(0, 0, 0,255);
            ofSetColor(255);
            ofCircle(tip_pos, 3);
        myFbo.end();
        
        //myGlitch.generateFx();
        ofSetColor(255);
        myFbo.draw(100,100);
        */

        ofPushStyle();
        ofEnableSmoothing();
        //ofSetSmoothLighting(true);
        ofEnableBlendMode(OF_BLENDMODE_ADD );
        //glBlendFunc(GL_ONE, GL_ONE);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        for(int i = 4 ; i >=0; i--){
            ofSetColor(this->beaconColor, offset*gradien_opacity[i]);
            ofFill();
            ofCircle(tip_pos, gradien_radius[i]);
        }
        ofPopStyle();
        
        
    }else if( this->current_status == TAGGED_FADE_OUT){
        
        if(this->animation_flag == true){
            int elapsed_time = ofGetElapsedTimeMillis() - this->off_start_time;
            if(elapsed_time > this->off_duration){
                this->animation_flag = false;
                this->current_pos = this->curve_path.getPointAtIndexInterpolated(2);
            }else{
                float index = (elapsed_time/this->off_duration)*2.0 ;
                this->current_pos = this->curve_path.getPointAtIndexInterpolated(index);
            }
            ofSetColor(255, 0, 0);
            ofFill();
        }
        
        // ofCircle(this->current_pos, 14);
        // this->curve_path.draw();
        // ofLog(OF_LOG_NOTICE, "tag current_pos: "+ofToString(this->current_pos));
        
        this->tag_icon.draw(this->current_pos);
        
        // draw beacon
        int elapsed_time = ofGetElapsedTimeMillis() ;
        elapsed_time -= this->flash_start_time;
        if(elapsed_time > this->flash_duration){
            this->flash_start_time = ofGetElapsedTimeMillis();
            elapsed_time = this->flash_duration;
        }
        
        float ratio = 0;
        float half_flash_duration = this->flash_duration /2.0;
        if(elapsed_time <= half_flash_duration){
            ratio = elapsed_time / half_flash_duration;
        }else{
            ratio = 1 - ((elapsed_time - half_flash_duration) / half_flash_duration);
        }
        
        float offset = 80+175* ratio;
        
        ofSetColor(this->beaconColor, offset);
        ofFill();
        ofPoint tip_pos(this->current_pos.x+this->current_tip_x_offset - this->current_x_offset, this->current_pos.y+this->current_tip_y_offset-current_y_offset);
        ofCircle(tip_pos, 3);

    }

}
