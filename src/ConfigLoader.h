//
//  ConfigLoader.h
//  livingLiquid
//
//  Created by javid on 7/10/14.
//  Reading the config file, and save each entry as a key-value pair.
//

#ifndef livingLiquid_ConfigLoader_h
#define livingLiquid_ConfigLoader_h


#include <string>
#include <map>
#include <vector>
using namespace std;

class ConfigLoader {
    
public:
    static ConfigLoader* singleton();
    string const& Value(std::string const& section, std::string const& entry) const;
    void Value(std::string const& section, std::string const& entry, std::string delimiter, vector<string> & tokens) const;
    
    
    void setValue(std::string const& section, std::string const& entry, string const& val);
    
private:
    static ConfigLoader* instance;
    
    ConfigLoader();
    
    map<string,string> content_;
};

#endif
