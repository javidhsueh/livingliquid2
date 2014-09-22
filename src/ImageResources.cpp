//
//  ImageResources.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/3/14.
//
//

#include "ImageResources.h"
#include "ConfigLoader.h"
#include "DataManager.h"

#include <vector>
#include <map>

ImageResources* ImageResources::instance=0;
ImageResources* ImageResources::singleton(){
    if(!instance){
        instance = new ImageResources();
    }
    return instance;
}

ImageResources::ImageResources() {

    ofImage loader;
    loader.setUseTexture(false);
    
    //all_species:
    //SalmonShark, BlueWhale, ShortfinMakoShark, WhiteShark, LeatherbackSeaTurtle, LaysanAlbatross, NorthernElephantSeal
    //*, *_tagged, *_color
    std::vector<std::pair<std::string,std::string> > file_list;
    std::vector<std::string> all_species = DataManager::singleton()->getAllSpecies();
    for(auto species = all_species.begin(); species!=all_species.end(); species++){
        file_list.push_back(std::pair<std::string, std::string>("icon_file_path", *species) );
        file_list.push_back(std::pair<std::string, std::string>("icon_file_path", *species+"_tagged") );
        file_list.push_back(std::pair<std::string, std::string>("icon_file_path", *species+"_color") );
    }
    
    //Fail
    file_list.push_back(std::pair<std::string, std::string>("icon_file_path", "Fail") );

    //bg_map, label_layer
    file_list.push_back(std::pair<std::string, std::string>("System", "bg_map") );
    file_list.push_back(std::pair<std::string, std::string>("System", "label_layer") );
    
    //boat
    file_list.push_back(std::pair<std::string, std::string>("boat", "disc_image") );
    
    //season_clock, season_clock_arrow
    file_list.push_back(std::pair<std::string, std::string>("file_path", "season_clock") );
    file_list.push_back(std::pair<std::string, std::string>("file_path", "season_clock_arrow") );
    
    
    for(auto i = file_list.begin(); i != file_list.end(); i++){
        std::string section = (*i).first;
        std::string key = (*i).second;
        std::string full_file_path = ConfigLoader::singleton()->Value(section, key);
        if(!loader.loadImage(full_file_path)){
            ofLog(OF_LOG_ERROR, "Fail to load icon image for RenderTrack.");
            loader.loadImage(ConfigLoader::singleton()->Value("icon_file_path", "Fail")); // load a default image
        }
        this->images[key] = new ofTexture();
        this->images[key]->allocate(loader.getWidth(), loader.getHeight(), GL_RGBA);
        this->images[key]->loadData(loader.getPixels(), loader.getWidth(), loader.getHeight(), GL_RGBA);
    }


}


ofTexture* ImageResources::getImage(std::string name){
    std::map<std::string, ofTexture*>::const_iterator ci = this->images.find(name);
    
    if (ci == this->images.end()) throw "does not exist";
    
    return ci->second;

}
