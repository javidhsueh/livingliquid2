//
//  TrackBrowser.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 10/23/14.
//
//

#ifndef example_waterRipples_TrackBrowser_h
#define example_waterRipples_TrackBrowser_h


#include "ofxUI.h"
#include <vector>
#include <map>
#include <string>
#include "RenderTrack.h"

class TrackBrowser{

public:
    TrackBrowser();
    ~TrackBrowser();
    
    void toggle();
    
    void selectAllTracks();
    void unselectAllTracks();
    
    void selectSpeciesTracks(std::string species_name);
    void unselectSpeciesTracks(std::string species_name);
    void setSpeciesTrackes(std::string species_name, bool val);
    
    std::vector<std::string> getSelectedTracks();
    
    ofxUIScrollableCanvas *gui;
    
    bool isShow;
    
    ofxUILabelToggle* select_all_btn;
    ofxUILabelToggle* unselect_all_btn;
    
    std::map<std::string, ofxUIToggle*> species_toggle_map;
    std::map<std::string, ofxUIToggle*> id_toggle_map;
    
    
};

#endif
