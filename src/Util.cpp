//
//  Util.cpp
//  example-waterRipples
//
//  Created by javid on 8/8/14.
//
//

#include "Util.h"



namespace Util {
    
    #include <sstream>
    #include <fstream>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <vector>
    #include <iostream>
    #include <string>
    #include <iomanip>
    
    
    #include <stdio.h>
    #include <time.h>
    
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        return buf;
    }
    
    
    ofColor hexToColor(std::string hexString){
        int hex = ofHexToInt( hexString.replace(0, 1, "0x"));
        return ofColor::fromHex(hex);
    }
    
    ofColor hexToColor(std::string hexString, int opacity){
        int hex = ofHexToInt( hexString.replace(0, 1, "0x"));
        return ofColor::fromHex(hex, opacity);
    }
    
    
    int dateToDayIndex(std::string dateString){
        
        std::string mystring(dateString);
        std::vector<std::string> tokens;
        string token;
        for(int i = 0; i < 3; i++){
            token = mystring.substr(0,mystring.find_first_of("-"));
            mystring = mystring.substr(mystring.find_first_of("-") + 1);
            tokens.push_back(token);
        }
        
        if(tokens.size() < 3){
            ofLog(OF_LOG_ERROR, "date string error: "+ dateString);
            return -1;
        }
        
        int year = atoi(tokens[0].c_str());
        int month = atoi(tokens[1].c_str());
        int day = atoi(tokens[2].c_str());
        
        int month_day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

        int day_idx = 0;
        if(month > 1){
            for(int i = 0; i < month-1; i++){
                day_idx += month_day[i];
            }
        }
        day_idx += day;
        
        return day_idx;
    }
    
    std::string dayIndexToDate(int day){
        
        int month_day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        
        int months = 0, days = 0;
        while((day-month_day[months]) > 0){
            day -= month_day[months];
            months = (months+1) % 12;
        }
        if(day == 0){
            if(months == 0)
                days = month_day[11];
            else
                days = month_day[months-1];
        }else{
            days = day;
        }
        std::string months_text = ofToString(months+1);
        std::string days_text = ofToString(days);
        if(months < 9)
            months_text = "0"+ofToString(months+1);
        if(days < 9)
            days_text = "0"+ofToString(days);
        
        return months_text+"/"+days_text;
    }
    
    float interpolate(float current, float min, float max, float value){
        float lerp = value * (current-min)/(max-min);
        return lerp;
    }

}