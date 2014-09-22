//
//  Logger.h
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/9/14.
//
//

#ifndef example_waterRipples_Logger_h
#define example_waterRipples_Logger_h

#include <vector>
#include <string>

class Logger{
public:
    static Logger* singleton();
    
    void log(std::string type, std::string msg);
    
    void createNewLog();
    
private:
    static Logger* instance;
    Logger();
    
    std::vector<std::string> logs;
};

#endif
