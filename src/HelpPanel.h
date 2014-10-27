//
//  HelpPanel.h
//  ll2
//
//  Created by javid on 7/18/14.
//
//

#ifndef ll2_HelpPanel_h
#define ll2_HelpPanel_h

#include "ofxUI.h"
#include <string>

class HelpPanel{

public:
    
    HelpPanel();
    
    void toggle();
    ofxUICanvas* getGUI(){  return this->gui;   }
    
private:

    bool isShow;
    ofxUICanvas *gui;
    
};

#endif
