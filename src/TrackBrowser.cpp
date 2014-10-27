//
//  TrackBrowser.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 10/23/14.
//
//

#include "TrackBrowser.h"
#include "DataManager.h"

TrackBrowser::TrackBrowser(){
    
    this->isShow = false;
    this->gui = new ofxUIScrollableCanvas(0,0, 210,1060);
    this->gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    this->gui->setScrollAreaToScreen();
    this->gui->setScrollableDirections(false, true);
    
    this->select_all_btn = this->gui->addLabelToggle("Select all", true);
    this->unselect_all_btn = this->gui->addLabelToggle("Unselect all", false);
    
    // create empty drop down list for each species:
    
    std::vector<std::string> species = DataManager::singleton()->getAllSpecies();
    for(int i = 0 ; i <species.size(); i+=1){
        std::string species_name = species[i];
        
        this->gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
        
        ofxUIToggle* species_toggle = this->gui->addToggle(species_name, true);
        species_toggle_map[species_name] = species_toggle;

        
        this->gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
        
        std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
        for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
            std::string id = (*rt)->getID();
            ofxUIToggle* toggle = this->gui->addToggle(id, true);
            this->id_toggle_map[id] = toggle;
            bool val = this->id_toggle_map[id]->getValue();
            val =  val;
            
        }
        this->gui->addSpacer();

    }
    gui->setPosition(270, 10);
    gui->autoSizeToFitWidgets();
    this->gui->setVisible(this->isShow);
}


void TrackBrowser::toggle(){
    
    this->isShow = !this->isShow;
    this->gui->setVisible(this->isShow);
    
}

void TrackBrowser::selectAllTracks(){
    this->unselect_all_btn->toggleValue();
    std::vector<std::string> species = DataManager::singleton()->getAllSpecies();
    for(int i = 0 ; i <species.size(); i+=1){
        std::string species_name = species[i];
        this->species_toggle_map[species_name]->toggleValue();
        std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
        for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
            std::string id = (*rt)->getID();
            this->id_toggle_map[id]->toggleValue();
        }
    }
    
}

void TrackBrowser::unselectAllTracks(){
    
    this->select_all_btn->toggleValue();
    
    std::vector<std::string> species = DataManager::singleton()->getAllSpecies();
    for(int i = 0 ; i <species.size(); i+=1){
        std::string species_name = species[i];
        this->species_toggle_map[species_name]->toggleValue();
        std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
        for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
            std::string id = (*rt)->getID();
            this->id_toggle_map[id]->toggleValue();
        }
    }
    
}

void TrackBrowser::setSpeciesTrackes(std::string species_name, bool val){
    this->species_toggle_map[species_name]->setValue(val);
    std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
    for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
        std::string id = (*rt)->getID();
        this->id_toggle_map[id]->setValue(val);
    }

}

void TrackBrowser::selectSpeciesTracks(std::string species_name){
    this->species_toggle_map[species_name]->setValue(true);
    std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
    for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
        std::string id = (*rt)->getID();
        this->id_toggle_map[id]->setValue(true);
    }
}

void TrackBrowser::unselectSpeciesTracks(std::string species_name){
    this->species_toggle_map[species_name]->setValue(false);
    std::vector<RenderTrack*> tracks = DataManager::singleton()->getTracksBySpecies(species_name);
    for(auto rt = tracks.begin(); rt != tracks.end(); ++rt){
        std::string id = (*rt)->getID();
        this->id_toggle_map[id]->setValue(false);
    }
}


std::vector<std::string> TrackBrowser::getSelectedTracks(){
    std::vector<std::string> selected_tracks;
    for(auto rt = id_toggle_map.begin(); rt != id_toggle_map.end(); ++rt){
        std::string id = rt->first;
        ofxUIToggle* toggle = rt->second;
        bool val = toggle->getValue();
        if(val == true){
            selected_tracks.push_back(id);
        }
    }
    return selected_tracks;
}
