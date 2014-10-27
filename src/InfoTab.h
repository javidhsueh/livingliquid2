//
//  InfoTab.h
//  ll2
//
//  Created by javid on 7/16/14.
//
//

#ifndef ll2_InfoTab_h
#define ll2_InfoTab_h

#include "ofxUI.h"
#include <string>


class InfoTab {
public:
    InfoTab();
    
    void toggle();
    
    void setFocus(std::string name);
    
    ofxUICanvas* getGUI(){  return this->gui;   }
    
private:
    bool isShow;
    std::string currentFocus;
    bool showInfoFlag;
    
    ofxUICanvas *gui;
    ofxUITextArea* desc_area;
    
    
};

#endif
