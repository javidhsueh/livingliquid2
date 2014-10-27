#pragma once

#include "ofMain.h"
#include "Map.h"
#include "Boat.h"
#include <vector>
#include "DataManager.h"
#include "ofxUI.h"
#include "InfoTab.h"
#include "HelpPanel.h"
#include "Dashboard.h"
#include "Legend.h"
#include "ofxTuio.h"
#include "TrackBrowser.h"

class Legend;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void guiEvent(ofxUIEventArgs &e);
    
    
    void tuioAdded(ofxTuioCursor & tuioCursor);
	void tuioRemoved(ofxTuioCursor & tuioCursor);
	void tuioUpdated(ofxTuioCursor & tuioCursor);
    
    
    void tuioObjAdded(ofxTuioObject & tuioObj);
    void tuioObjUpdated(ofxTuioObject & tuioObj);
    void tuioObjRemoved(ofxTuioObject & tuioObj);
    
    ofxTuioClient   tuioClient;

    
private:
    Map* map;

    bool isFullscreen;
    
    TrackBrowser* track_browser;
    InfoTab* info;
    HelpPanel* help;
    Dashboard* dashboard;
    Legend* legend;
    
};


