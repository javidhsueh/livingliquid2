//
//  RenderTrack.cpp
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#include "RenderTrack.h"
#include "ofMain.h"
#include "ofUtils.h"
#include "ConfigLoader.h"
#include "Util.h"
#include <math.h>
#include "Pen.h"


RenderTrack::RenderTrack(std::string species, std::string id, std::vector<TrackPoint*> points, std::string start_date, int track_days, int gap_days, int max_gap ){

    // copy data
    this->species = species;
    this->id = id;
    this->start_date = start_date;
    this->track_days = track_days;
    this->gap_days = gap_days;
    this->max_gap = max_gap;
    //this->reset(); // reset all flags as default
    
    this->isPlaying = false;
    this->prev_visibility_status = INVISIBLE;
    this->visibility_status = INVISIBLE;
    this->tagged_day_idx = 0;
    
    // load icon image
    if(!this->icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", species))){
        ofLog(OF_LOG_ERROR, "Fail to load icon image for RenderTrack.");
        this->icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", "Fail")); // load a default image
    }
    if(!this->tagged_icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", species+"_tagged"))){
        ofLog(OF_LOG_ERROR, "Fail to load icon image for RenderTrack.");
        this->tagged_icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", "Fail")); // load a default image
    }
    
    this->icon_width = this->icon.width;
    this->icon_height = this->icon.height;

    this->detected_opacity = ofToInt(ConfigLoader::singleton()->Value("icon", "detected_opacity"));
    this->orientationMode = FLIP;
    this->show_tagged_date = ofToBool( ConfigLoader::singleton()->Value("System", "showTaggedDate") );

    // load path style setting
    this->inner_path_opacity = ofToInt(ConfigLoader::singleton()->Value("RenderTrack", "inner_path_opacity"));
    this->inner_path_color = Util::hexToColor(ConfigLoader::singleton()->Value("RenderTrack", species));
    this->inner_path_width = ofToFloat(ConfigLoader::singleton()->Value("RenderTrack", "inner_path_width"));
    
    this->outter_path_color = Util::hexToColor(ConfigLoader::singleton()->Value("RenderTrack", "outter_path_color"));
    this->outter_path_width = ofToFloat(ConfigLoader::singleton()->Value("RenderTrack", "outter_path_width"));

    
    // tagged date color
    this->tagged_date_font_color = Util::hexToColor(ConfigLoader::singleton()->Value("RenderTrack", "tagged_date_font_color"));
    
    
    // load end circle setting
    this->track_end_circle_radius = ofToFloat(ConfigLoader::singleton()->Value("RenderTrack", "track_end_circle_radius"));
    this->track_end_circle_line_width = ofToFloat(ConfigLoader::singleton()->Value("RenderTrack", "track_end_circle_line_width"));
    
    // load parameters
    this->path_smoothness = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "trail_smoothness"));
    this->touch_detect_radius = ofToFloat(ConfigLoader::singleton()->Value("RenderTrackParameters", "touch_sensitivity_radius"));
    
    // buoyant animation setting
    this->buoyant_animation = ofToBool( ConfigLoader::singleton()->Value("System", "is_buoyant_animation") );
    this->buoyant_size_offset = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "buoyant_size_offset"));
    this->buoyant_timer = ofGetElapsedTimeMillis();
    this->buoyant_interval = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "buoyant_interval"));
    
    // fading effect setting
    this->fadeInTime = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "fadeInTime"));
    this->fadeOutTime = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "fadeOutTime"));
    
    // loop control calculation
    this->start_date_idx = Util::dateToDayIndex(this->start_date);
    this->isPlaying = false;
    for(auto p = points.begin(); p != points.end(); ++p){
        int time = (*p)->time;//+this->start_date_idx;
        this->points[time] = *p;
    }
    this->end_date_idx = this->start_date_idx + int(this->points.size());
    this->cycle = ceil(this->points.size()/365.0)*365;
    
    // build the path first
    for(auto it = this->points.begin(); it != this->points.end(); ++it){
        int t = it->first;
        TrackPoint* point = it->second;
        this->pathline.addVertex(ofVec2f(point->x, point->y));
    }
    this->pathline = this->pathline.getSmoothed(this->path_smoothness);
    
    this->faceDirection = FACE_RIGHT;
//    this->prevFaceDirection = this->faceDirection;
//
//    ofPoint first_pos = pathline.getPointAtIndexInterpolated(0);
//    ofPoint second_pos = pathline.getPointAtIndexInterpolated(1);
//    if(first_pos.x - second_pos.x > 0.0000 ){
//        this->faceDirection = FACE_LEFT;
//        this->icon.mirror(false, true);
//        this->tagged_icon.mirror(false, true);
//    }else{
//        this->faceDirection = FACE_RIGHT;
//    }
}

void RenderTrack::setVisibilityStatus(int status){
    this->prev_visibility_status = this->visibility_status;
    this->visibility_status = status;
    
    if(this->prev_visibility_status == SHOW_INFO && this->visibility_status == TAGGED_STABLE && this->isPlaying == false){
        this->visibility_status = TAGGED_FADE_OUT;
        this->fadeOut();
    }
}

void RenderTrack::setOrientationMode(int mode){
    this->orientationMode = mode;
}

void RenderTrack::toggleSelected(){
    this->isSelected = !this->isSelected;
}

void RenderTrack::toggleDebugMode(){
    if(this->visibility_status != DEBUGGING)
        this->setVisibilityStatus(DEBUGGING);
    else
        this->setVisibilityStatus(this->prev_visibility_status);
}
bool RenderTrack::isTagged(){
    return this->visibility_status == TAGGED_FADE_IN ||
           this->visibility_status == TAGGED_STABLE  ||
           this->visibility_status == TAGGED_FADE_OUT||
           this->visibility_status == SHOW_INFO;
}
void RenderTrack::setTagged(int time){
    
    this->tagged_day_idx = time-this->start_date_idx;
    while(this->tagged_day_idx< 0 || ((this->current_date_idx-this->tagged_day_idx)>= 365))
        this->tagged_day_idx += 365;
    
    this->setVisibilityStatus(TAGGED_FADE_IN);
    this->tagged_point = pathline.getPointAtIndexInterpolated(this->tagged_day_idx);
    
    // animation
    this->buoyant_timer = ofGetElapsedTimeMillis();
    this->fadeIn();
    
    // convert time to date
    this->tagged_date = Util::dayIndexToDate(time);
    ofPoint first_position = this->tagged_point;
    ofPoint unit_vector = first_position - this->pathline.getPointAtIndexInterpolated(this->tagged_day_idx+1);
    unit_vector = unit_vector.normalize();
    float text_width = Pen::singleton()->getFont("RenderTrack")->stringWidth(this->tagged_date);
    float text_height = Pen::singleton()->getFont("RenderTrack")->stringHeight(this->tagged_date);
    this->text_position = first_position + unit_vector.scale(text_height+3);
    this->text_position.set(this->tagged_point.x - text_width/2, this->text_position.y );
}

void RenderTrack::setDetected(float distance){
    this->setVisibilityStatus(DETECTED);
    this->detected_dist = distance;
}

bool RenderTrack::isDetected(){
    return this->visibility_status == DETECTED;
}

bool RenderTrack::isDetectable(){
    return this->visibility_status == DETECTABLE;
}

void RenderTrack::fadeIn(){
    this->fadeStartTime = ofGetElapsedTimeMillis();
}

void RenderTrack::fadeOut(){
    
    this->fadeStartTime = ofGetElapsedTimeMillis();
    
}

void RenderTrack::update(int speed){
    
    // Javid: dirty code
    if(!this->isPlaying && this->visibility_status == SHOW_INFO){
        this->current_opacity = this->inner_path_opacity;
        return;
    }
    
    // update time
    this->current_frame = ofGetFrameNum();
    int temp_current_date_idx = ((this->current_frame/speed) - this->start_date_idx) % this->cycle;
    
    // update loop control
    if(temp_current_date_idx > 0 && temp_current_date_idx < this->points.size() ){
        this->isPlaying = true;
    }
    else{
        this->isPlaying = false;
    }
    
    // Javid: dirty code
    if(this->isPlaying && this->visibility_status == TAGGED_FADE_OUT){
        this->current_opacity = this->inner_path_opacity - Util::interpolate(ofGetElapsedTimeMillis(), this->fadeStartTime, this->fadeStartTime+this->fadeOutTime, this->inner_path_opacity);
        if(this->current_opacity <= 0){
            this->current_opacity = 0;
            this->setVisibilityStatus(INVISIBLE);
            this->reset();
        }
        this->isPlaying = false;
        return;
    }
    
    if(!this->isPlaying && (this->visibility_status == DETECTED || this->visibility_status == DETECTABLE)){
        this->setVisibilityStatus(INVISIBLE);
        this->reset();
        return;
    }
    
    
    if(!this->isPlaying && this->visibility_status == TAGGED_STABLE){
        this->setVisibilityStatus(TAGGED_FADE_OUT);
        this->fadeOut();
    }
    
    // Javid: dirty code
    this->current_date_idx = temp_current_date_idx;
    
    
    // update current position
    if(this->isPlaying){
        
        this->lastPoint = this->current_point;
        
        int offset = this->current_frame % speed;
        
        if(this->current_date_idx+1 <= this->end_date_idx-1){
            float interpolated_idx = (current_date_idx)+(1.0*offset/speed);
            this->current_point = pathline.getPointAtIndexInterpolated(interpolated_idx);
        }else{
            this->current_point = lastPoint;
        }
        

        if(this->current_date_idx+1 <= this->end_date_idx-1){
            ofPoint next_point = pathline.getPointAtIndexInterpolated(this->current_date_idx+1);
            
            float angleRad = atan2(current_point.y - next_point.y, current_point.x - next_point.x);
            if(current_point.x - next_point.x > 0.0000 ){
                this->nextFaceDirection = FACE_LEFT;
            }else{
                this->nextFaceDirection = FACE_RIGHT;
            }
            lastAngle = (angleRad * 180 / PI) + 180;
            
        }
        
//        this->prevFaceDirection = this->faceDirection;
//        float angleRad = atan2(lastPoint.y - current_point.y, lastPoint.x - current_point.x);
//        if(lastPoint.x - current_point.x > 0.0000 ){
//            this->faceDirection = FACE_LEFT;
//        }else{
//            this->faceDirection = FACE_RIGHT;
//        }
//        lastAngle = (angleRad * 180 / PI) + 180;
    }
    
    // update color , fade in/out or still
    if(this->visibility_status == TAGGED_STABLE){
        this->current_opacity = this->inner_path_opacity;
    }else if(this->visibility_status == TAGGED_FADE_IN){
        this->current_opacity = Util::interpolate(ofGetElapsedTimeMillis(), this->fadeStartTime, this->fadeStartTime+this->fadeInTime, this->inner_path_opacity);
        if(this->current_opacity >= this->inner_path_opacity){
            this->current_opacity = this->inner_path_opacity;
            this->setVisibilityStatus(TAGGED_STABLE);
        }
    }
    else if(this->visibility_status == TAGGED_FADE_OUT){
        this->current_opacity = this->inner_path_opacity - Util::interpolate(ofGetElapsedTimeMillis(), this->fadeStartTime, this->fadeStartTime+this->fadeOutTime, this->inner_path_opacity);
        if(this->current_opacity <= 0){
            this->current_opacity = 0;
            this->setVisibilityStatus(INVISIBLE);
            this->reset();
        }
    }else if(this->visibility_status == DETECTED){
        this->current_opacity = this->detected_opacity*(this->detected_dist < 0.5 ? 1.0: 1 -(this->detected_dist-0.5)/0.5);
        // ofLog(OF_LOG_NOTICE, ofToString(this->current_opacity));
    }else if(this->visibility_status == SHOW_INFO){
        this->current_opacity = this->inner_path_opacity;
    }
}

void RenderTrack::draw(int speed){

    if(!isSelected)
        return;
    
    if(this->visibility_status == DEBUGGING){ // show the whole trajectory
        ofSetLineWidth(3);
        ofSetColor(70,70,70, 70);
        pathline.draw();
        return;
    }
    
    if(this->visibility_status == TAGGED_STABLE   ||
       this->visibility_status == TAGGED_FADE_IN  ||
       this->visibility_status == TAGGED_FADE_OUT ||
       this->visibility_status == SHOW_INFO){

        // draw the whole line in a batch mode, much faster
        ofPolyline line;
        int offset = this->current_frame % speed;
        int next_day_idx = this->current_date_idx + 1;
        
        std::vector<ofPoint> points = pathline.getVertices();
        int point_idx = this->tagged_day_idx;
        int last_idx = this->current_date_idx <= this->end_date_idx ? this->current_date_idx: this->end_date_idx;
        
        while(point_idx <= last_idx){
            line.addVertex(this->pathline.getPointAtIndexInterpolated(point_idx));
            point_idx++;
        }
        line.addVertex(this->current_point);
        line = line.getSmoothed(this->path_smoothness);
        line.insertVertex(this->tagged_point, 0);
        line.addVertex(this->current_point);
        
        // draw outter line
        if(this->visibility_status == SHOW_INFO){
            ofSetColor(this->outter_path_color, this->current_opacity);
            ofFill();
            ofSetLineWidth(this->outter_path_width);
            line.draw();
        }
        
        // draw inner line
        ofSetColor(this->inner_path_color, this->current_opacity);
        ofSetLineWidth(this->inner_path_width);
        line.draw();
        
        // draw tagging point : inner circle
        ofPoint first_tag_point = line.getPointAtLength(0);
        ofCircle(first_tag_point, this->track_end_circle_radius);
        
        // draw tagging point: outter cicrcle
        ofSetLineWidth(this->track_end_circle_line_width);
        ofSetColor(this->outter_path_color, this->current_opacity);
        ofNoFill();
        ofCircle(first_tag_point, this->track_end_circle_radius);
        
        // draw the tagged time
        if(this->show_tagged_date){
            ofSetColor(tagged_date_font_color, this->current_opacity);
            Pen::singleton()->getFont("RenderTrack")->drawString(tagged_date, this->text_position.x, this->text_position.y);
        }
        
    }
}

void RenderTrack::drawIcon(int speed){
    
    if(!isSelected)
        return;
    
    if(this->visibility_status == DEBUGGING){
        this->icon.draw(this->current_point.x-this->icon_width/2, this->current_point.y-this->icon_height/2, this->icon_width, this->icon_height);
        return;
    }
    
    if(this->visibility_status == DETECTED        ||
       this->visibility_status == TAGGED_FADE_IN  ||
       this->visibility_status == TAGGED_STABLE   ||
       this->visibility_status == TAGGED_FADE_OUT ||
       this->visibility_status == SHOW_INFO){
    
        float new_icon_width, new_icon_height;
        
        // apply buoyant animation effect
        if(this->buoyant_animation){
            int elapsed_time = ofGetElapsedTimeMillis() - this->buoyant_timer;
            if(elapsed_time > this->buoyant_interval){
                this->buoyant_timer = ofGetElapsedTimeMillis();
            }
            float ratio =( 1.0 - abs(elapsed_time-this->buoyant_interval/2.0)/(this->buoyant_interval/4.0) );
            float offset = this->buoyant_size_offset* ratio;
            
            if(abs(ratio) > 1 ){
                ratio = ratio > 0 ? 1 : -1;
            }
            new_icon_width = this->icon_width + offset;
            new_icon_height = this->icon_height + offset;
            
        }else{
            new_icon_width = this->icon_width;
            new_icon_height = this->icon_height;
        }
        
        
        // start to determine the orientation of icon
        
        ofSetColor(255, 255, 255, this->current_opacity);
        ofFill();
        
        if(this->orientationMode == ROTATE){
            ofPushMatrix();
                ofTranslate(this->current_point.x, this->current_point.y, 0);//move pivot to centre
                ofRotate(lastAngle, 0, 0, 1);//rotate from centre
                ofPushMatrix();
                    ofTranslate(-new_icon_width/2.0,-new_icon_height/2.0,0);//move back by the centre offset
                    if(this->visibility_status == DETECTED){
                        this->icon.draw(0,0, new_icon_width, new_icon_height);
                    }else{
                        this->tagged_icon.draw(0,0, new_icon_width, new_icon_height);
                    }
                ofPopMatrix();
            ofPopMatrix();
            
        }else if(this->orientationMode == FLIP){
            if(nextFaceDirection != faceDirection){
                this->icon.mirror(false, true);
                this->tagged_icon.mirror(false, true);
                this->faceDirection = this->nextFaceDirection;
            }
            if(this->visibility_status == DETECTED){
                this->icon.draw(this->current_point.x-new_icon_width/2.0, this->current_point.y-new_icon_height/2.0, new_icon_width, new_icon_height);
            }else{
                this->tagged_icon.draw(this->current_point.x-new_icon_width/2.0, this->current_point.y-new_icon_height/2.0, new_icon_width, new_icon_height);
            }
            
        }else{
            
            if(this->visibility_status == DETECTED){
                this->icon.draw(this->current_point.x-new_icon_width/2.0, this->current_point.y-new_icon_height/2.0, new_icon_width, new_icon_height);
            }else{
                this->tagged_icon.draw(this->current_point.x-new_icon_width/2.0, this->current_point.y-new_icon_height/2.0, new_icon_width, new_icon_height);
            }

        }

    }
    
}

ofPoint RenderTrack::getCurrentPosition(){
    return this->current_point;
}

ofColor RenderTrack::getColor(){
    return this->inner_path_color;
}

int RenderTrack::getOpacity(){
    return this->current_opacity;
}

bool RenderTrack::detectExtraDistancebyBoat(Boat* b, int day){
    if(!isSelected || !this->isPlaying)
        return false;
    
    if(this->visibility_status == DETECTABLE){
        if(b->isWithinNearbyRange(this->current_point.x, this->current_point.y)){
            return true;
        }
    }
    return false;
}

void RenderTrack::resetDetectionStatus(){
    if(this->isPlaying && (this->visibility_status == INVISIBLE || this->visibility_status == DETECTED) ){
        this->setVisibilityStatus(DETECTABLE);
    }
}

bool RenderTrack::detectNearbyBoat(Boat* b, int time){
    
    if(!isSelected || !this->isPlaying)
        return false;
    
    if(this->visibility_status == DETECTABLE){
        float detected_dist = b->isWithinRange(this->current_point.x, this->current_point.y);
        if(detected_dist > 0){
            this->setDetected(detected_dist);
            return true;
        }
    }
    return false;
    
}

bool RenderTrack::testTouch(int x, int y){
    
    if(!isSelected)
        return false;
    
    if(this->visibility_status == DETECTED        ||
       this->visibility_status == TAGGED_STABLE   ||
       this->visibility_status == TAGGED_FADE_OUT ||
       this->visibility_status == SHOW_INFO){
        
        ofPoint p(x,y);
        float dist = p.distance(this->current_point);
        if(dist < touch_detect_radius){
            return true;
        }
    }
    return false;
}

void RenderTrack::toggleShowInfo(){
    
    if(this->visibility_status == SHOW_INFO){
        AnimalPopoutPool::singleton()->releasePopout(this->popout);
        this->setVisibilityStatus(TAGGED_STABLE);
    }else{
        this->popout = AnimalPopoutPool::singleton()->getAvailablePopout(this);
        this->setVisibilityStatus(SHOW_INFO);
    }
}


void RenderTrack::reset(){
    
    this->isPlaying = false;
    this->tagged_day_idx = -1;
    this->visibility_status == INVISIBLE;
        
}
