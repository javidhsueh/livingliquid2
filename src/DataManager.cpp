//
//  DataManager.cpp
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#include "DataManager.h"
#include "ofxJSONElement.h"
#include "ofUtils.h"
#include <algorithm>
#include <cmath> 
#include <algorithm>
#include "ConfigLoader.h"

DataManager* DataManager::instance = 0;
DataManager* DataManager::singleton(){
    if(!instance)
        instance = new DataManager();
    return instance;
}

DataManager::DataManager(){
    this->loadTrackData();
}


bool compare(const TrackPoint* a, const TrackPoint* b )
{
    return a->time < b->time;
}

void DataManager::loadTrackData(){

    //the list of the species should be read from file
    std::vector<string> species ;
    ConfigLoader::singleton()->Value("data", "all_species", ",", this->all_species);
    ConfigLoader::singleton()->Value("data", "selected_species", ",", this->selected_species);
    
    this->min_track_days = ofToInt(ConfigLoader::singleton()->Value("data", "min_track_day"));
    this->max_gap_days = ofToInt(ConfigLoader::singleton()->Value("data", "max_gap"));
    
    
    ofxJSONElement json;
    bool paringResult = json.open(ConfigLoader::singleton()->Value("System", "data_file"));
    if(paringResult){
        
        for(auto name = this->all_species.begin(); name != this->all_species.end(); name++){
            
            std::vector<std::string> id_list;
            for(int i = 0; i < json[*name].size(); i++) {
                ofxJSONElement one_animal = json[*name][i];
                
                std::string id = one_animal["id"].asString();
                std::string start_date = one_animal["start_date"].asString();
                int track_days = one_animal["time_span"].asInt();
                int gap_days = one_animal["total_gap"].asInt();
                int max_gap_days = one_animal["gap"].asInt();
                
                ofxJSONElement points = one_animal["points"];
                std::vector<TrackPoint*> t_points;
                
                for(int j = 0; j < points.size(); j++){
                    ofxJSONElement one_day = points[j];
                    float lon = one_day["lon"].asInt();
                    float lat = one_day["lat"].asInt();
                    int time = one_day["time"].asInt();
                    bool lerp = one_day["lerp"].asBool();
                    t_points.push_back(new TrackPoint(lon,lat, time, lerp) );
                }
                
                std::sort(t_points.begin(), t_points.end(), compare); //sort by time
                this->individual_tracks[id] = t_points;
                id_list.push_back(id);
                
                RenderTrack* rt = new RenderTrack(*name, id, t_points, start_date, track_days, gap_days, max_gap_days);
                this->all_tracks.push_back(rt);
                
            }
            // creating table species -> ids
            this->species2id[*name] = id_list;
        }
        
    }else{
        ofLog(OF_LOG_FATAL_ERROR, "Unable to load track data.");
    }
    
    // build selected tracks
    this->updateSelectedTracks();
}

std::vector<std::string> DataManager::getAllSpecies(){
    
    return this->all_species;
}


std::vector<std::string> DataManager::getSelectedSpecies(){
    
    return this->selected_species;
}

void DataManager::toggleSpecies(std::string name){
    auto s = std::find(this->selected_species.begin(), this->selected_species.end(), name);
    if( s != this->selected_species.end()){
        this->selected_species.erase( s );
    }else{
        this->selected_species.push_back(name);
    }
    this->updateSelectedTracks();
}


void DataManager::updateSelectedTracks(){
    for(auto rt = this->all_tracks.begin(); rt != this->all_tracks.end(); ++rt){
        std::string name = (*rt)->getSpeciesName();
        if(std::find(selected_species.begin(), selected_species.end(), name) != selected_species.end()
           && (*rt)->getTrackDays()> this->min_track_days
           && (*rt)->getMaxGap() < this->max_gap_days )
            (*rt)->setSelected(true);
        else
            (*rt)->setSelected(false);
    }
}

std::vector<RenderTrack*> DataManager::getAllTracks(){
    return all_tracks;
}

std::vector<RenderTrack*> DataManager::getSelectedTracks(){
    std::vector<RenderTrack*> selected_tracks;
    for(auto t = all_tracks.begin(); t!= all_tracks.end(); t++){
        if( (*t)->selected()){
            selected_tracks.push_back(*t);
        }
    }
    return selected_tracks;
}

RenderTrack* DataManager::getIndividualTrack(std::string id){
    for(auto t = all_tracks.begin(); t!= all_tracks.end(); t++){
        if((*t)->getID() == id){
            return *t;
        }
    }
    return 0;
}


void DataManager::setMinTrackDays(int day){
    this->min_track_days = day;
    this->updateSelectedTracks();
}

void DataManager::setMaxGap(int gap){
    this->max_gap_days = gap;
    this->updateSelectedTracks();
}


