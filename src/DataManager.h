//
//  DataManager.h
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

#ifndef livingLiquid_DataManager_h
#define livingLiquid_DataManager_h

#include "TrackPoint.h"
#include <string>
#include <vector>
#include <map>
#include "RenderTrack.h"

// Data store
class DataManager{
public:
    
    static DataManager* singleton();
    
    // getters, setter
    
    std::vector<std::string> getAllSpecies();
    std::vector<std::string> getSelectedSpecies();
    
    
    std::vector<RenderTrack*> getTracksBySpecies(std::string species_name);
    
    std::vector<RenderTrack*> getAllTracks();
    std::vector<std::string> getSelectedTracksID();
    std::vector<std::string> getAllTracksID();
    
    std::vector<RenderTrack*> getSelectedTracks();
    
    RenderTrack* getIndividualTrack(std::string id);
    
    
    void setSelectedTracks(std::vector<std::string>);
    
    void updateSelectedTracks();
    
    
    void toggleSpecies(std::string);
    
    void setMinTrackDays(int day);
    void setMaxGap(int gap);
    
    
private:
    
    void filterTrack();
    
    static DataManager* instance;
    DataManager();
    void loadTrackData();
    
    

    // data containers
    std::map<std::string, std::vector<TrackPoint*> > individual_tracks;
    
    std::map<std::string, std::vector<std::string> > species2id;

    std::vector<std::string> all_species;
    
    std::vector<std::string> selected_species;
    
    std::vector<RenderTrack*> all_tracks;
    
    
    // filtering parameters
    int min_track_days;
    int max_gap_days;
    
    
};

#endif
