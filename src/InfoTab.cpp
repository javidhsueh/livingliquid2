//
//  InfoTab.cpp
//  ll2
//
//  Created by javid on 7/16/14.
//
//

#include "InfoTab.h"
#include "ConfigLoader.h"
#include "DataManager.h"

// four buttons, four text area, images of each species

InfoTab::InfoTab(){
    
    this->gui = new ofxUICanvas();
    
//    this->gui->setDrawBack(false);
    this->gui->setFont("font/Sansation_Light.ttf");
    this->gui->setFontSize(OFX_UI_FONT_LARGE, 14);
    this->gui->setFontSize(OFX_UI_FONT_MEDIUM, 10);
    this->gui->setFontSize(OFX_UI_FONT_SMALL, 6);
    
    
    std::vector<string> species = DataManager::singleton()->getAllSpecies();
    for(int i = 0 ;i < species.size(); i++){
        std::string name = species[i];
        std::string title = ConfigLoader::singleton()->Value("description", name+"_title");
        std::string desc = ConfigLoader::singleton()->Value("description", name+"_desc");
        ofxUILabelButton* btn = new ofxUILabelButton(name, false, 50, 30,i*51+5,1, OFX_UI_FONT_SMALL, true);
        gui->addWidget(btn);
        
    }
    
    this->desc_area = new ofxUITextArea("desc",ConfigLoader::singleton()->Value("description", species[0]+"_desc"), 220, 70, 5, 31,OFX_UI_FONT_SMALL);
    
    this->gui->addWidget(this->desc_area);
    this->gui->autoSizeToFitWidgets();
    
    this->isShow = false;
    this->gui->setVisible(this->isShow);
}

void InfoTab::toggle(){
    this->isShow = !this->isShow;
    this->gui->setVisible(this->isShow);
}


void InfoTab::setFocus(std::string name){
    this->currentFocus = name;
    
    this->desc_area->setTextString(ConfigLoader::singleton()->Value("description", name+"_desc"));
    
}


