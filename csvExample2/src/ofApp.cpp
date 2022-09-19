#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    ofSetLogLevel("ofxCsv", OF_LOG_VERBOSE); // See what's going on inside.
    
    // Load a CSV File.
    if(csv.load("OSC Calls.csv")) {
        /*
         csv.trim(); // Trim leading/trailing whitespace from non-quoted fields.
         
         //         Like with C++ vectors, the index operator is a quick way to grab row
         //         & col data, however this will cause a crash if the row or col doesn't
         //         exist, ie. the file didn't load.
         ofLog() << "Print out a specific CSV value";
         ofLog() << csv[0][1];
         // also you can write...
         ofLog() << csv[0].at(1);
         // or you can get the row itself...
         ofxCsvRow row = csv[0];
         ofLog() << row.getString(1);
         */
    }
    
    
    
    // A safer method is to use the getters which will do a check on the
    // given row & col indices but will be slightly slower.
    /*
     ofLog() << "Print out the first value";
     ofLog() << csv.getRow(0).getString(0);
     
     // Print the table to the console.
     ofLog() << "Print the table";
     //csv.print(); // Uses default separator ",".
     // ... or do it manually
     for(auto row : csv) {
     ofLog() << ofJoinString(row, "|");
     }
     */
    
    
    
    maxCues = csv.getNumRows();
    currentCue_index = -1;
    currentCue_str = "";
    currentOSCmsg_str = "";
    
    bEnableAuto = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

void ofApp::pickRandomCue(){
    
    //for testing pick random OSCmsg
    int random_index = ofRandom(1,csv.getNumRows()); //0 is column header
    currentOSCmsg_str = csv[random_index][OSCmsg];
    
    //find OSCmsg in csv and get it's index
    currentCue_index = ofFind(csv.getColumn(OSCmsg),currentOSCmsg_str);
   
    //use the index to find the related action
    if(currentCue_index >= 0 && currentCue_index<csv.getNumRows()){
        
        currentCue_str = csv[currentCue_index][Cue];
        
        currentActions.clear();
        string temp_actions = csv[currentCue_index][VoiceTank];
        currentActions = ofSplitString(temp_actions, "/");
        
        
        ofLog()<<"random_index "<<random_index<<" currentOSCmsg_str "<<currentOSCmsg_str<<" currentCue_index "<<currentCue_index<<" currentActions[0] "<<currentActions[0]; 
    }else{
        ofLog()<<"currentOSCmsg_str not found in csv file";
    }
}

void ofApp::getNewCue(){
    currentOSCmsg_str = csv[currentCue_index][OSCmsg];
    currentCue_str = csv[currentCue_index][Cue];
    
    currentActions.clear();
    
    string temp_actions = csv[currentCue_index][VoiceTank];
    currentActions = ofSplitString(temp_actions, "/");
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    if(bEnableAuto && ofGetElapsedTimef() - cueTimer > 1){
        cueTimer = ofGetElapsedTimef();
        currentCue_index++;
        getNewCue();
    }
    
    
    int tempX = 10;
    for(int i = 0; i < csv.getNumRows(); i++) {
        if(i == currentCue_index){
            ofFill();
            ofSetColor(255, 0, 0);
            ofDrawRectangle(tempX, 150+i*20, 1200, -20);
        }
        ofSetColor(0);
        ofDrawBitmapString(csv[i][SCENE], tempX, 150+i*20);
    }
    tempX+=200;
    for(int i = 0; i < csv.getNumRows(); i++) {
        ofDrawBitmapString(csv[i][Cue], tempX, 150+i*20);
    }
    tempX+=200;
    for(int i = 0; i < csv.getNumRows(); i++) {
        ofDrawBitmapString(csv[i][OSCmsg], tempX, 150+i*20);
    }
    tempX+=200;
    for(int i = 0; i < csv.getNumRows(); i++) {
        ofDrawBitmapString(csv[i][Delay_sec], tempX, 150+i*20);
    }
    tempX+=200;
    for(int i = 0; i < csv.getNumRows(); i++) {
        ofDrawBitmapString(csv[i][VoiceTank], tempX, 150+i*20);
    }
    
    
    //-------draw info
    ofSetColor(0);
    ofFill();
    int tempY = 10;
    
    ofDrawBitmapString("csv rows: " + ofToString(csv.getNumRows()), 20, tempY+=20);
    ofDrawBitmapString("csv cols: " + ofToString(csv.getNumCols()), 20, tempY+=20);
    ofDrawBitmapString("current OSCmsg: " + currentOSCmsg_str, 20, tempY+=20);
    ofDrawBitmapString("current Cue: " + currentCue_str, 20, tempY+=20);
    
    stringstream ss;
    ss<<"current actions: ";
    for(auto & oneAction : currentActions ){
        ss<<oneAction<<" , ";
    }
    ss<<endl;
    ofDrawBitmapString(ss.str(), 20, tempY+=20);
    
    tempY = 10;
    ofDrawBitmapStringHighlight("key Up/Down == cue index change", 500, tempY+=20);
    ofDrawBitmapStringHighlight("key a == auto change", 500, tempY+=20);
    ofDrawBitmapStringHighlight("key r == pick random OSCmsg", 500, tempY+=20);

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_UP){
        currentCue_index--;
        currentCue_index = ofClamp(currentCue_index, 0, maxCues);
        getNewCue();
    }
    if(key == OF_KEY_DOWN){
        currentCue_index++;
        currentCue_index = ofClamp(currentCue_index, 0, maxCues);
        getNewCue();
    }
    if(key == 'a'){
        bEnableAuto = !bEnableAuto;
    }
    
    if(key == 'r'){
        bEnableAuto = false;
        pickRandomCue();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){	
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
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
