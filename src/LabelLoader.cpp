//
//  LabelLoader.cpp
//  livingLiquid
//
//  Created by javid on 7/11/14.
//
//

//#include "LabelLoader.h"
//#include <fstream>
//
//std::string trim(std::string const& source, char const* delims = " \t\r\n") {
//    std::string result(source);
//    std::string::size_type index = result.find_last_not_of(delims);
//    if(index != std::string::npos)
//        result.erase(++index);
//    
//    index = result.find_first_not_of(delims);
//    if(index != std::string::npos)
//        result.erase(0, index);
//    else
//        result.erase();
//    return result;
//}
//
//LabelLoader* LabelLoader::instance=0;
//LabelLoader* LabelLoader::singleton()
//{
//    if(!instance){
//        instance = new LabelLoader();
//    }
//    return instance;
//}
//
//LabelLoader::LabelLoader() {
//    std::ifstream file("label.txt");
//    
//    std::string line;
//    std::string name;
//    std::string value;
//    std::string inSection;
//    int posEqual;
//    while (getline(file,line)) {
//        if (! line.length()) continue;
//        
//        if (line[0] == '#') continue;
//        if (line[0] == ';') continue;
//        
//        if (line[0] == '[') {
//            inSection=trim(line.substr(1,line.find(']')-1));
//            continue;
//        }
//        
//        posEqual=line.find('=');
//        name  = trim(line.substr(0,posEqual));
//        value = trim(line.substr(posEqual+1));
//        
//        content_[inSection+'/'+name]=value;
//    }
//}
//
//std::string const& LabelLoader::Value(std::string const& section, std::string const& entry) const {
//    
//    map<std::string,string>::const_iterator ci = content_.find(section + '/' + entry);
//    
//    if (ci == content_.end()) throw "does not exist";
//    
//    return ci->second;
//}
