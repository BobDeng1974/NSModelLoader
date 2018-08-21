#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(ofColor::black);
    ofDisableArbTex();
    
    //light.enable();
    material.setAmbientColor(ofColor::white);
    material.setShininess(1.0f);

    bDrawModel = false;
    bHideGui = false;

    gui.setup();
    gui.add(bMeshModeToggle.setup("Mesh Mode", false));
    gui.add(bMeshTypeToggle.setup("Mesh Type", false));

    gui.add(amp.setup("Amplitude", 0.5f, 0.0f, 500.0f));
    gui.add(ampTotal.setup("Amplitude Total", 0.5f, 0.0f, 500.0f));
    gui.add(liquid.setup("Liquid", 0.5f, 0.0f, 500.0f));
    gui.add(dampen.setup("Dampening", 0.5f, 0.0f, 2.0f));
    gui.add(alpha.setup("Alpha", 0.1f, 0.0f, 0.02f));
    
    gui.add(speed.setup("Rotate Speed", 0.5f, 0.0f, 10.0f));
    gui.add(rotation.setup("Rotation", ofVec3f(0.5f, 0.5f, 0.5f),ofVec3f(0.0f,0.0f,0.0f), ofVec3f(5.0f,5.0f,5.0f)));
    gui.add(scaling.setup("Scaling",ofVec3f(0.5f, 0.5f, 0.5f),ofVec3f(0.0f,0.0f,0.0f), ofVec3f(5.0f,5.0f,5.0f)));

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(!bHideGui) {
        gui.draw();
    }

    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 50,50);
    
    if(bDrawModel) {
        ofSetColor(255, 255, 255, 255);
        drawModel();
    }


}

void ofApp::drawModel(){


    ofVec3f scale = draggedModel.getScale();
    ofVec3f position = draggedModel.getPosition();
    float normalizedScale = draggedModel.getNormalizedScale();
    ofVboMesh mesh = draggedModel.getMesh(0);
    ofTexture texture;
    ofxAssimpMeshHelper& meshHelper = draggedModel.getMeshHelper( 0 );

    if (bMeshModeToggle) {
        if(bMeshTypeToggle) {
            mesh.setMode(OF_PRIMITIVE_POINTS);
        }else{
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        }
    }

    ofTranslate(position);
    
    float t = ofGetElapsedTimef() * speed;
    float timeX = t * rotation->x;
    float timeY = t * rotation->y;
    float timeZ = t * rotation->z;
//    ofRotate(time, 1, 1, 0);
//    ofRotate(90,0,0,1);
    ofRotateX(timeX);
    ofRotateY(timeY);
    ofRotateZ(timeZ);
    ofScale(normalizedScale, normalizedScale, normalizedScale);
//    ofScale(scale.x * scaling->x,scale.y * scaling->y,scale.z * scaling->z);
    ofScale(scaling->x,scaling->y,scaling->z);
    
    //EASING//
//    float targetX = amp;
//    float dx = targetX - x;
//    x += dx * easing;

    //NOISE//
    float liquidness = liquid;
    float amplitude = amp / ampTotal;
    float speedDampen = dampen;
    vector<ofVec3f>& verts = mesh.getVertices();

    for(unsigned int i = 0; i < verts.size(); i++){

        ofFloatColor c = ofColor::white;
        c.a = i * alpha;
        mesh.addColor(c);


        verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;

    }

    ofEnableDepthTest();

    tex.bind();
    material.begin();
    ofScale(1.0f,1.0f);
    mesh.drawFaces();
    material.end();
    tex.unbind();

    ofDisableDepthTest();



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 's'){
        gui.saveToFile("settings.xml");
    }
    else if(key == 'l'){
        gui.loadFromFile("settings.xml");
    }
    
    if(key == 'h'){
        bHideGui = !bHideGui;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

    
    int total = dragInfo.files.size();
    for (int i = 0; i < total; i++) {
        string filePath = dragInfo.files.at(0);
        bool bModelLoaded = ofIsStringInString(filePath, "models");
        bool bTextureLoaded = ofIsStringInString(filePath,"textures");

        if (bModelLoaded) {
            draggedModel.loadModel(filePath);
            draggedModel.setPosition(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0.0f);
            //light.setPosition(draggedModel.getPosition() + ofPoint(0, 0, 1600));
            bDrawModel = true;
        } else if (bTextureLoaded) {
            img.load(filePath);
            tex = img.getTexture();
        }
        std::cout << "Total Files: " << total << std::endl;
        std::cout << "File Path: " << filePath << std::endl;


    }
    
}
