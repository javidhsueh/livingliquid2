#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGraphics.h"
#include "ConfigLoader.h"
#include "Logger.h"


//========================================================================
int main( ){
    
    Logger::singleton()->log("App", "Lauched application.");
    
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgb alpha double samples depth");
    int screen_width = 1920;//= ofToInt(ConfigLoader::singleton()->Value("System", "screen_width"));
    int screen_height = 1080;//ofToInt(ConfigLoader::singleton()->Value("System", "screen_height"));
	ofSetupOpenGL(&window, screen_width, screen_height, OF_WINDOW);
    ofRunApp(new ofApp());
    
}
