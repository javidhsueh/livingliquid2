//
//  HelpPanel.cpp
//  ll2
//
//  Created by javid on 7/18/14.
//
//

#include "HelpPanel.h"
#include "ConfigLoader.h"



HelpPanel::HelpPanel(){
    
    
    this->gui = new ofxUICanvas();
    
    //    this->gui->setDrawBack(false);
    this->gui->setFont("font/Sansation_Light.ttf");

    this->gui->addLabel("M: enable mouse mode.", OFX_UI_FONT_SMALL);
    //this->gui->addLabel("I: open the info panel.", OFX_UI_FONT_SMALL);
    this->gui->addLabel("D: open dashboard.", OFX_UI_FONT_SMALL);
    this->gui->addLabel("F: enable/disable fullscreen mode.", OFX_UI_FONT_SMALL);
    //this->gui->addLabel("S: enable/disable SST layer.", OFX_UI_FONT_SMALL);
    this->gui->addLabel("L: show/hide labels.", OFX_UI_FONT_SMALL);
    this->gui->addLabel("C: show/hide Season clock.", OFX_UI_FONT_SMALL);
    
    this->gui->autoSizeToFitWidgets();
    
    this->gui->setPosition( ofGetWidth()/2-110, ofGetHeight()/2-110 );
    this->isShow = false;
    this->gui->setVisible(this->isShow);
}

void HelpPanel::toggle(){
    this->isShow = !this->isShow;
    this->gui->setVisible(this->isShow);
}
