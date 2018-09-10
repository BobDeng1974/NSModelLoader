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
    void setVerts(vector<ofVec3f>& v);
    void interpolateVerts(vector<ofVec3f>& currentMesh, vector<ofVec3f>&previousMesh);

    ofLight light;
    ofShader shader;
    ofMaterial material;
    ofTexture tex;
    ofImage img;
    //ofVideoPlayer vid; 
    ofxAssimpModelLoader draggedModel, draggedPreviousModel;
    int dragCounter;
    
    bool bDrawModel, bHideGui, bLighting;
    
    //GUI//
    ofxPanel gui;
    
    ofxFloatSlider speed, amp, ampTotal, liquid, dampen, alpha, transition, ampGPU;
    ofxVec3Slider rotation, scaling;
    ofxFloatColorSlider color, colorSecondary;
    ofxToggle bMeshModeToggle, bMeshTypeToggle, bShowSecondaryMesh, bRotateLock, bShader;
    
    
};
