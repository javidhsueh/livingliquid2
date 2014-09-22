//
//  Dashboard.cpp
//  example-waterRipples
//
//  Created by javid on 8/1/14.
//
//

#include "Dashboard.h"
#include "ofxUIButton.h"
#include "DataManager.h"
#include "ConfigLoader.h"

Dashboard::Dashboard(){
    
    this->isShow = ofToBool(ConfigLoader::singleton()->Value("System", "show_dashboard"));
    
    gui = new ofxUISuperCanvas("Control panel");
	gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    
    speed = ofToInt(ConfigLoader::singleton()->Value("speed", "display_speed"));
    gui->addMinimalSlider("Display Speed", 0, 50, &this->speed);
    
    gui->addSpacer();
    gui->addToggle( "Show all tracks", false);
    //gui->addToggle( "Show SST", false);
    
    
    this->showTimeline = ofToBool(ConfigLoader::singleton()->Value("System", "showTimeline"));
    this->timeline_toggle = gui->addToggle("Show Timeline", this->showTimeline);
    
    this->showSeasonclock = ofToBool(ConfigLoader::singleton()->Value("System", "showSeasonclock"));
    this->seasonclock_toggle = gui->addToggle("Show Season Clock", this->showSeasonclock);
    
    
    this->showLegends = ofToBool(ConfigLoader::singleton()->Value("System", "showLegends"));
    this->legends_toggle = gui->addToggle("Show Legends", this->showLegends);

    
    this->showMapLabels = ofToBool(ConfigLoader::singleton()->Value("System", "showMapLabels"));
    this->map_labels_toggle = gui->addToggle("Show Map Labels", this->showMapLabels);
    
    
    this->showExtraDetect = ofToBool(ConfigLoader::singleton()->Value("System", "showExtraDetect"));
    this->extra_detect_toggle = gui->addToggle("Nearby animal hint", this->showExtraDetect);
    
    
    /////////////////////////////////////////////////////////////////////
    
    gui->addSpacer();
    gui->addLabel("Animal Icon: ");
    
    
    this->showTaggedDate = ofToBool(ConfigLoader::singleton()->Value("System", "showTaggedDate"));
    this->tagged_date_toggle = gui->addToggle("Show Tagged Date", this->showTaggedDate);
    
    this->is_buoyant_animation = ofToBool(ConfigLoader::singleton()->Value("System", "is_buoyant_animation"));
    this->icon_buoyant_animation = gui->addToggle( "Buoyant animation", this->is_buoyant_animation);
    
    
    this->fadeInTime = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "fadeInTime"));
    // gui->addBiLabelSlider("Fade-In Time", "Slow", "Fast", 1000, 15000, &this->fadeInTime);
    gui->addMinimalSlider("Fade-In Time", 1000, 15000, &this->fadeInTime);
    
    this->fadeOutTime = ofToInt(ConfigLoader::singleton()->Value("RenderTrackParameters", "fadeOutTime"));
    //gui->addBiLabelSlider("Fade-Out Time", "Slow", "Fast", 1000, 30000, &this->fadeOutTime);
    gui->addMinimalSlider("Fade-Out Time", 1000, 30000, &this->fadeOutTime);
    
    // gui->addLabelToggle("LABEL TGL", false)->getLabelWidget()->setColorFill(ofColor(255, 0, 0));
    
    gui->addLabel("Icon Orientation");
    this->icon_orientation_none = gui->addToggle( "None", false);
    this->icon_orientation_rotate = gui->addToggle( "Rotate", false);
    this->icon_orientation_flip = gui->addToggle( "Flip", true);
    
    // display data setting
    gui->addSpacer();
    gui->addLabel("Data Filtering");
    
    std::vector<string> all_species = DataManager::singleton()->getAllSpecies();
    std::vector<string> selected_species = DataManager::singleton()->getSelectedSpecies();
    
    for(auto name = all_species.begin(); name != all_species.end(); name++){
        
        if( std::find(selected_species.begin(), selected_species.end(), *name) != selected_species.end())
            gui->addToggle( *name, true);
        else
            gui->addToggle( *name, false);
    }
    
    
    min_track_days = ofToInt(ConfigLoader::singleton()->Value("data", "min_track_day"));
    gui->addMinimalSlider("Min Total Track Days", 0, 600, &min_track_days)->getLabelWidget()->setColorFill(ofColor(255, 255, 255));
    max_gap = ofToInt(ConfigLoader::singleton()->Value("data", "max_gap"));
    gui->addMinimalSlider("Max Gap", 0, 100, &max_gap)->getLabelWidget()->setColorFill(ofColor(255, 255, 255));

    
    gui->setPosition(20, 400);
    gui->autoSizeToFitWidgets();
    
    //ofxUILabelButton* btn = new ofxUILabelButton("Settings", true, 50, 30,500,1, OFX_UI_FONT_SMALL, true);
    
    this->gui->setVisible(this->isShow);
}

void Dashboard::toggleOrientationMode(int mode){
    if(mode == ROTATE){
        
        this->icon_orientation_none->setValue(false);
        this->icon_orientation_rotate->setValue(true);
        this->icon_orientation_flip->setValue(false);
        
    }else if (mode == FLIP){
        
        this->icon_orientation_none->setValue(false);
        this->icon_orientation_rotate->setValue(false);
        this->icon_orientation_flip->setValue(true);
        
    }else{
        
        this->icon_orientation_none->setValue(true);
        this->icon_orientation_rotate->setValue(false);
        this->icon_orientation_flip->setValue(false);
        
    }
    
}

void Dashboard::toggleBuoyantAnimation(){
    this->is_buoyant_animation = !this->is_buoyant_animation;
    this->icon_buoyant_animation->setValue(this->is_buoyant_animation);
    ConfigLoader::singleton()->setValue("System", "is_buoyant_animation", ofToString(this->is_buoyant_animation));
}

void Dashboard::toggleTimeline(){
    this->showTimeline = !this->showTimeline;
    this->timeline_toggle->setValue(this->showTimeline);
    ConfigLoader::singleton()->setValue("System", "showTimeline", ofToString(this->showTimeline));
}

void Dashboard::toggleLegends(){
    this->showLegends = !this->showLegends;
    this->legends_toggle->setValue(this->showLegends);
    ConfigLoader::singleton()->setValue("System", "showLegends", ofToString(this->showLegends));
}

void Dashboard::toggleMapLabels(){
    this->showMapLabels = !this->showMapLabels;
    this->map_labels_toggle->setValue(this->showMapLabels);
    ConfigLoader::singleton()->setValue("System", "showMapLabels", ofToString(this->showMapLabels));
}

void Dashboard::toggleSeasonClock(){
    this->showSeasonclock = !this->showSeasonclock;
    this->seasonclock_toggle->setValue(this->showSeasonclock);
    ConfigLoader::singleton()->setValue("System", "showSeasonclock", ofToString(this->showSeasonclock));
}

void Dashboard::toggleTaggedDate(){
    this->showTaggedDate = !this->showTaggedDate;
    this->tagged_date_toggle->setValue(this->showTaggedDate);
    ConfigLoader::singleton()->setValue("System", "showTaggedDate", ofToString(this->showTaggedDate));
}

void Dashboard::toggleExtraDetect(){
    this->showExtraDetect = !this->showExtraDetect;
    this->extra_detect_toggle->setValue(this->showExtraDetect);
    ConfigLoader::singleton()->setValue("System", "showExtraDetect", ofToString(this->showExtraDetect));
}

Dashboard::~Dashboard(){
    delete gui;
}

void Dashboard::toggle(){
    
    this->isShow = !this->isShow;
    this->gui->setVisible(this->isShow);
    
}