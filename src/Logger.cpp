//
//  Logger.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/9/14.
//
//

#include "Logger.h"
#include "ofApp.h"
#include "Util.h"

Logger* Logger::instance=0;
Logger* Logger::singleton(){
    if(!instance){
        instance = new Logger();
    }
    return instance;
}

Logger::Logger() {
    
	ofLogToFile("log_"+Util::currentDateTime()+".txt", true);
//	ofLogToFile("log.txt", true);
}

void Logger::log(std::string type, std::string msg){
    // encode message
    std::string encoded_message =  Util::currentDateTime()+"@"+msg;
    ofLogVerbose(type, encoded_message);
}

void Logger::log(std::string msg){
    // encode message
    std::string encoded_message =  Util::currentDateTime()+"@"+msg;
//    ofLogVerbose("Record", encoded_message);
    ofLog(OF_LOG_SILENT, encoded_message);
}


void Logger::createNewLog(){
    ofLogToFile("./log_"+Util::currentDateTime()+".txt", true);
}