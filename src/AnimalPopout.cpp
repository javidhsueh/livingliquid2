//
//  AnimalPopout.cpp
//  example-waterRipples
//
//  Created by javid on 8/27/14.
//
//

#include "AnimalPopout.h"
#include "ConfigLoader.h"
#include "DataManager.h"
#include "Util.h"
#include "Pen.h"
#include "ImageResources.h"


AnimalPopout::AnimalPopout(){
    
    
    this->isShow = false;
    this->rt = 0;
    
    this->max_opacity = ofToInt(ConfigLoader::singleton()->Value("popout", "max_opacity"));
    this->current_opacity = 0;
    
    this->fadeInTime = ofToInt(ConfigLoader::singleton()->Value("popout", "fadeInTime"));
    this->fadeOutTime = ofToInt(ConfigLoader::singleton()->Value("popout", "fadeOutTime"));
    
    this->fadeType = "";

    this->line = new ofPolyline();

}

void AnimalPopout::update(){

    if(this->rt){
        
        this->pos = this->rt->getCurrentPosition();

        // draw the border
        float widget_width = 265, widget_height = 130, widget_radius = widget_height/2;
        float arrow_height = 30;
        ofColor panel_color = ofColor(0,0,0,current_opacity);
        ofColor border_color = ofColor(255,255,255,current_opacity);
        ofColor title_color = ofColor(255,255,255, current_opacity);
        ofColor desc_color = ofColor(255,255,255, current_opacity);
        
        ofSetLineWidth(2);
        
        ofPoint panel_origin(pos.x-(widget_width+widget_radius)/2, pos.y-widget_height-arrow_height);
        
        ofSetLineWidth(2);
        ofPolyline line;
        line.addVertex(panel_origin);
        line.addVertex(panel_origin.x+widget_width, panel_origin.y);
        line.arc(panel_origin.x+widget_width, panel_origin.y+widget_radius, widget_radius, widget_radius, -90, 90, 200);
        line.addVertex(panel_origin.x, panel_origin.y+widget_height);
        line.close();
        ofSetColor(border_color);
        ofFill();
        line.draw();
        
        // draw arrow line
        ofSetLineWidth(3);
        ofPoint arrow_pos( panel_origin.x+(widget_width+widget_radius)/2, panel_origin.y+widget_height);
        ofLine(arrow_pos.x, arrow_pos.y, arrow_pos.x, arrow_pos.y + arrow_height);
        
        // fill the polygon
        ofPath polygon;
        polygon.newSubPath();
        for(int j = 0; j < line.getVertices().size(); j++) {
            polygon.lineTo(line.getVertices()[j]);
        }
        polygon.setFillColor(panel_color);
        polygon.draw();
        
        // draw title
        ofSetColor(title_color);
        float title_height = Pen::singleton()->getFont("Popout_title")->getLineHeight();
        Pen::singleton()->getFont("Popout_title")->drawString(this->title, panel_origin.x+10, panel_origin.y+10+title_height);
        
        // draw desc
        ofSetColor(desc_color);
        std::string desc = this->desc;
        Pen::singleton()->getFont("Popout_desc")->drawString(" "+desc, panel_origin.x+10, panel_origin.y+10+title_height+20);
//        Pen::singleton()->getFont("Popout_desc")->drawString("this is first line.\nThis is second line.\nThird.", panel_origin.x+10, panel_origin.y+10+title_height+20);
        
        // draw Icon
        ofTexture* icon = ImageResources::singleton()->getImage(this->rt->getSpeciesName()+"_color");
        ofPoint icon_pos(panel_origin.x+widget_width-icon->getWidth()/2, panel_origin.y+(widget_height-icon->getHeight())/2);
        ofSetColor(ofColor(255,255,255,this->current_opacity));
        icon->draw(icon_pos);
        
    }
    
    if(this->fadeType == "in"){
        this->current_opacity = Util::interpolate(ofGetElapsedTimeMillis(), this->fadeStartTime, this->fadeStartTime+this->fadeInTime, this->max_opacity);
        
        if(this->current_opacity >= this->max_opacity){
            this->fadeType = "";
            this->current_opacity = this->max_opacity;
        }
    }else if(this->fadeType == "out"){
        this->current_opacity = this->max_opacity - Util::interpolate(ofGetElapsedTimeMillis(), this->fadeStartTime, this->fadeStartTime+this->fadeOutTime, this->max_opacity);
        if(this->current_opacity <= 0){
            this->fadeType = "";
            this->current_opacity = 0;
            
            //real detach
            if(this->rt){
                this->rt = 0;
            }
        }
    }else{
        this->current_opacity = this->max_opacity;
    }
}

void AnimalPopout::setPosition(int x, int y){
    this->pos.set(x,y);
}

void AnimalPopout::setVisibility(bool flag){
    this->isShow = flag;
}

void AnimalPopout::attach(RenderTrack* rt){

    this->rt = rt;
    // read data from rt and display
    this->species_name = rt->getSpeciesName();
    this->topp_id = rt->getID();
    this->title = ConfigLoader::singleton()->Value("description", species_name+"_title");
    this->desc = ConfigLoader::singleton()->Value("description", species_name+"_desc");

    this->setVisibility(true);
    this->fadeIn();
}

void AnimalPopout::detach(){
    
    this->setVisibility(false);
    this->fadeOut();
}

void AnimalPopout::fadeIn(){
    this->current_opacity = 0;
    this->fadeType = "in";
    fadeStartTime = ofGetElapsedTimeMillis();
}

void AnimalPopout::fadeOut(){
    this->current_opacity = this->max_opacity;
    this->fadeType = "out";
    fadeStartTime = ofGetElapsedTimeMillis();
}