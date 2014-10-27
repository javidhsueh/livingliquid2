#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGraphics.h"
#include "ConfigLoader.h"
#include "Logger.h"


//========================================================================

//extern "C"{
//    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
//    {
//        return fwrite(a, b, c, d);
//    }
//    char* strerror$UNIX2003( int errnum )
//    {
//        return strerror(errnum);
//    }
//    time_t mktime$UNIX2003(struct tm * a)
//    {
//        return mktime(a);
//    }
//    double strtod$UNIX2003(const char * a, char ** b) {
//        return strtod(a, b);
//    }
//}

int main( ){
    ofSetWorkingDirectoryToDefault();
    Logger::singleton()->log("App", "Lauched application.");
    
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgb alpha double samples depth");
    int screen_width = 1920;//1920 //= ofToInt(ConfigLoader::singleton()->Value("System", "screen_width"));
    int screen_height = 1080;//1080 //ofToInt(ConfigLoader::singleton()->Value("System", "screen_height"));
	ofSetupOpenGL(&window, screen_width, screen_height, OF_WINDOW);
    ofRunApp(new ofApp());
    
}
