#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void drawModel();
    
    ofLight light;
    ofMaterial material;
    ofTexture tex;
    ofImage img;
    ofxAssimpModelLoader draggedModel;
    
    bool bDrawModel, bHideGui;
    float x,y;
    float easing;
    
    //GUI//
    ofxPanel gui;
    ofxFloatSlider speed, amp, ampTotal, liquid, dampen, alpha;
    ofxVec3Slider rotation, scaling;

    ofxToggle bMeshModeToggle, bMeshTypeToggle;
    
    
};