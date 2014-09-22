#include "ofApp.h"
#include "ConfigLoader.h"
#include <string>
#include "ofxMouseAutoHider.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    // init window settings
	ofSetWindowTitle("Living liquid Prototyping"); // nvm
    this->isFullscreen = true;
    
    // create objects
    DataManager::singleton();
    this->map = new Map();
    this->info = new InfoTab();
    this->help = new HelpPanel();
    this->dashboard = new Dashboard();
    this->legend = new Legend();
    
    ofAddListener(this->dashboard->gui->newGUIEvent,this,&ofApp::guiEvent);
    
    
    // GUI listener
    ofAddListener(this->info->getGUI()->newGUIEvent,this,&ofApp::guiEvent);
    ofxMouseAutoHider::enable();
	
	// enable TUIO
	tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,&ofApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&ofApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&ofApp::tuioUpdated);
    
    
    ofAddListener(tuioClient.objectAdded, this, &ofApp::tuioObjAdded);
    ofAddListener(tuioClient.objectUpdated, this, &ofApp::tuioObjUpdated);
    ofAddListener(tuioClient.objectRemoved, this, &ofApp::tuioObjRemoved);
    
    
    ////////////////////////////////////////////////////////////
    //ofSetFullscreen(this->isFullscreen);
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    ofEnableAntiAliasing();
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofSetCurveResolution(100);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    tuioClient.getMessage();
    this->map->update();
    this->legend->update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    this->map->draw();
    
    this->legend->draw();
    
    //tuioClient.drawCursors();
    //tuioClient.drawObjects();
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
//    ofLog(OF_LOG_NOTICE, name);
    
	int kind = e.widget->getKind();
    
    std::vector<std::string> all_species = DataManager::singleton()->getAllSpecies();
    
    if(std::find(all_species.begin(), all_species.end(), name) != all_species.end()){
        DataManager::singleton()->toggleSpecies(name);
        this->map->updateSelectedSpecies();
    }

    if(name == "Show all tracks"){
        this->map->toggleDebugMode();
    }
    if(name == "Show Timeline"){
        this->dashboard->toggleTimeline();
        this->map->toggleTimeline();
    }
    
    if(name == "Show Season Clock"){
        this->dashboard->toggleSeasonClock();
        this->map->toggleClock();
    }
    
    if(name == "Show Legends"){
        this->dashboard->toggleLegends();
        this->legend->toggle();
    }
    
    if(name == "Show Map Labels"){
        this->dashboard->toggleMapLabels();
        this->map->toggleLabelLayer();
    }
    
    if(name == "Show Tagged Date"){
        this->dashboard->toggleTaggedDate();
        this->map->toggleShowTaggedDate();
    }
    
    if(name == "Nearby animal hint"){
        this->dashboard->toggleExtraDetect();
        this->map->toggleShowExtraDetect();
    }
    
    
    ///////////////////// icon orientation //////////////////
    if(name == "None"){
        this->dashboard->toggleOrientationMode(NONE);
        this->map->setIconOrientationMode(NONE);
    }
    if(name == "Rotate") {
        this->dashboard->toggleOrientationMode(ROTATE);
        this->map->setIconOrientationMode(ROTATE);
    }
    if(name == "Flip"){
        this->dashboard->toggleOrientationMode(FLIP);
        this->map->setIconOrientationMode(FLIP);
    }
    
    if(name == "Buoyant animation"){
        this->dashboard->toggleBuoyantAnimation();
        this->map->toggleBuoyantAnimation();
    }
    
    if(name == "Fade-In Time"){
        int fadeInTime = int(dashboard->fadeInTime);
        this->map->updateFadeInTime(fadeInTime);
    }
    
    if(name == "Fade-Out Time"){
        int fadeOutTime = int(dashboard->fadeOutTime);
        this->map->updateFadeOutTime(fadeOutTime);
    }
        
    /////////////////// filter //////////////////////////////
    if(name == "Min Total Track Days"){
        float new_min_track_days = int(dashboard->min_track_days);
        DataManager::singleton()->setMinTrackDays(new_min_track_days);
        this->map->updateSelectedSpecies();
        
    }
    
    if(name == "Max Gap"){
        float max_gap = int(dashboard->max_gap);
        DataManager::singleton()->setMaxGap(max_gap);
        this->map->updateSelectedSpecies();
    }
    
    if(name == "Display Speed"){
        float speed = int(dashboard->speed);
        this->map->setSpeed(speed);
        
    }
    
}


void ofApp::exit()
{
    delete info;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
//    ofLog(OF_LOG_NOTICE, ofToString(key));
    
    if(key == 'd'){

        this->dashboard->toggle();
        
    }else if(key == 'i'){
        
        this->info->toggle();
        
    }else if(key == 'm'){
        
        this->map->toggleMouseMode();
        
    }else if(key == 'h'){
        
        this->help->toggle();
        
    }else if(key == 'f'){
        
        this->isFullscreen = !this->isFullscreen;
        ofSetFullscreen(this->isFullscreen);
        
    }else if(key == 's'){
        
        //this->map->toggleSST();
        
    }else if(key == 'c'){
        this->dashboard->toggleSeasonClock();
        this->map->toggleClock();
        
    }else if(key == 'l'){
        this->dashboard->toggleLegends();
        this->legend->toggle();
        
    }else if(key =='t'){
        this->dashboard->toggleTimeline();
        this->map->toggleTimeline();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    this->map->mouseMove(x, y);
    
    this->info->getGUI()->setPosition(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

   this->map->mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    this->map->mousePressed(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
//	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    this->map->tuioFingerAdded(tuioCursor.getSessionId(), loc);
    
}

void ofApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
//	cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    this->map->tuioFingerUpdated(tuioCursor.getSessionId(), loc);
}

void ofApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
//	cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
    this->map->tuioFingerRemoved(tuioCursor.getSessionId(), loc);
}

//--------------------------------------------------------------
void ofApp::tuioObjAdded(ofxTuioObject &obj){
	ofPoint loc = ofPoint(obj.getX()*ofGetWidth(),obj.getY()*ofGetHeight());
//	cout << "Point n" << obj.getSessionId() << " add at " << loc << endl;
    this->map->tuioObjAdded(obj.getSessionId(), loc);
    
}

void ofApp::tuioObjUpdated(ofxTuioObject &obj){
	ofPoint loc = ofPoint(obj.getX()*ofGetWidth(),obj.getY()*ofGetHeight());
//	cout << "Point n" << obj.getSessionId() << " updated at " << loc << endl;
    this->map->tuioObjUpdated(obj.getSessionId(), loc);
}

void ofApp::tuioObjRemoved(ofxTuioObject &obj){
	ofPoint loc = ofPoint(obj.getX()*ofGetWidth(),obj.getY()*ofGetHeight());
//	cout << "Point n" << obj.getSessionId() << " remove at " << loc << endl;
    this->map->tuioObjRemoved(obj.getSessionId(), loc);
}

