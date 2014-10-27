//
//  Legend.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 8/21/14.
//
//

#include "Legend.h"
#include "DataManager.h"
#include "ConfigLoader.h"
#include "Pen.h"

Legend::Legend(){
    
    this->isShow = ofToBool( ConfigLoader::singleton()->Value("System", "showSpeciesLegends") );
    
    // create icon map
    std::vector<string> all_species = this->selected_species = DataManager::singleton()->getAllSpecies();
    for(auto s = all_species.begin(); s != all_species.end(); s++){
        std::string name = (*s);
        ofImage icon;
        icon.loadImage(ConfigLoader::singleton()->Value("icon_file_path", name+"_color"));
        this->species2icon[name] = icon;
        this->species2color[name] = Util::hexToColor(ConfigLoader::singleton()->Value("RenderTrack", name));
    }
}

void Legend::toggle(){
    this->isShow = !this->isShow;
}

void Legend::update(){
    this->selected_species = DataManager::singleton()->getSelectedSpecies();
}

void Legend::draw(){
    
    if(!this->isShow)
        return;
    
    // translate
    ofPushMatrix();
    ofTranslate(5, 5);
    
    
    // draw background
    ofSetColor(0,0,0,95);
    ofFill();
    ofRect(0,0, 690, 90);
    
    
    // draw keys
    ofSetColor(50,50,50,190);
    ofFill();
    int gap = 10;
    int temp_x = 0;
    for(auto s = this->selected_species.begin(); s != this->selected_species.end(); s++){
        std::string name = (*s);
        ofSetColor(255,255,255,255);
        ofNoFill();
        ofImage icon = this->species2icon[name];
        
        int icon_width = icon.width;
        int icon_height = icon.height;
        int text_width = Pen::singleton()->getFont("Legends")->stringWidth(ConfigLoader::singleton()->Value("description", name+"_title"));
        
        int grid_width = icon_width > text_width? icon_width : text_width;
        icon.draw( temp_x + (grid_width-icon_width)/2.0, (70-icon_height)/2);

        
        ofSetColor(255,255,255,255);
        ofFill();
        
        Pen::singleton()->getFont("Legends")->drawString(ConfigLoader::singleton()->Value("description", name+"_title"), temp_x + (grid_width-text_width)/2, 85);
        temp_x += grid_width + gap;
    }
    ofPopMatrix();
    
}
