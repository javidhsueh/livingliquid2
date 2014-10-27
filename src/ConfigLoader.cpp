//
//  ConfigLoader.cpp
//  livingLiquid
//
//  Created by javid on 7/10/14.
//  Reading the config file, and save each entry as a key-value pair.
//

#include "ConfigLoader.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

string trim(std::string const& source, char const* delims = " \t\r\n") {
    string result(source);
    string::size_type index = result.find_last_not_of(delims);
    if(index != std::string::npos)
        result.erase(++index);
    
    index = result.find_first_not_of(delims);
    if(index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

ConfigLoader* ConfigLoader::instance=0;
ConfigLoader* ConfigLoader::singleton(){
    if(!instance){
        instance = new ConfigLoader();
    }
    return instance;
}

ConfigLoader::ConfigLoader() {
    std::ifstream file("config_debug.txt");
    
    std::string line;
    std::string name;
    std::string value;
    std::string inSection;
    int posEqual;
    while (getline(file,line)) {
        
        if (! line.length()) continue;
        
        if (line[0] == '#') continue;
        if (line[0] == ';') continue;
        
        if (line[0] == '[') {
            inSection=trim(line.substr(1,line.find(']')-1));
            continue;
        }
        
        posEqual=line.find('=');
        name  = trim(line.substr(0,posEqual));
        value = trim(line.substr(posEqual+1));
        
        for(int i = 0 ; i < value.length(); i++){
            if(i > 0 && i+1 < value.length() ){
                if(value[i] == '\\' && value[i+1] == 'n' ){
                    value[i] = '\n';
                    value[i+1] = ' ';
                    i ++;
                }
            }
        }
        
        content_[inSection+'/'+name]=value;
    }
//    ofLog(OF_LOG_ERROR, "showWave = "+ConfigLoader::singleton()->Value("System", "showWave"));
}

string const& ConfigLoader::Value(std::string const& section, std::string const& entry) const {
    
    map<std::string,string>::const_iterator ci = content_.find(section + '/' + entry);
    
    if (ci == content_.end()) throw "does not exist";
    
    return ci->second;
}


void ConfigLoader::Value(std::string const& section, std::string const& entry, std::string delim, vector<string> & tokens) const
{
    
    map<std::string,string>::const_iterator ci = content_.find(section + '/' + entry);
    
    if (ci == content_.end()) throw entry+" - does not exist";
    
    std::string str = ci->second;
    
    tokens = ofSplitString(str, delim);
}


void ConfigLoader::setValue(std::string const& section, std::string const& entry, string const& val){
    
    this->content_[section + '/' + entry ] = val;
    
}
