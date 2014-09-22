//
//  Pen.cpp
//  example-waterRipples
//
//  Created by Javid Hsueh on 9/2/14.
//
//

#include "Pen.h"
#include "ConfigLoader.h"

Pen* Pen::instance=0;
Pen* Pen::singleton(){
    if(!instance){
        instance = new Pen();
    }
    return instance;
}

Pen::Pen() {
   
    // load fonts
    ofTrueTypeFont* font1 = new ofTrueTypeFont();
    font1->loadFont(ConfigLoader::singleton()->Value("RenderTrack", "tagged_date_font"),
                   ofToInt(ConfigLoader::singleton()->Value("RenderTrack", "tagged_date_font_size")), true, true);
    this->fonts["RenderTrack"] = font1;
    
    
    ofTrueTypeFont* font2 = new ofTrueTypeFont();
    font2->loadFont(ConfigLoader::singleton()->Value("Timeline", "font"),
                   ofToInt(ConfigLoader::singleton()->Value("Timeline", "max_font_size")), true, true);
    this->fonts["Timeline"] = font2;
    
    
    ofTrueTypeFont* font3 = new ofTrueTypeFont();
    font3->loadFont(ConfigLoader::singleton()->Value("popout", "title_font"),
                    ofToInt(ConfigLoader::singleton()->Value("popout", "title_font_size")), true, true);
    this->fonts["Popout_title"] = font3;
    
    
    ofTrueTypeFont* font4 = new ofTrueTypeFont();
    font4->loadFont(ConfigLoader::singleton()->Value("popout", "desc_font"),
                    ofToInt(ConfigLoader::singleton()->Value("popout", "desc_font_size")), true, true);
    this->fonts["Popout_desc"] = font4;
    
    
    // legends
    
    ofTrueTypeFont* font5 = new ofTrueTypeFont();
    font5->loadFont(ConfigLoader::singleton()->Value("Legend", "font"),
                    ofToInt(ConfigLoader::singleton()->Value("Legend", "font_size")), true, true);
    this->fonts["Legends"] = font5;
    
}

ofTrueTypeFont* Pen::getFont(std::string font_name){
    return this->fonts[font_name];
}
