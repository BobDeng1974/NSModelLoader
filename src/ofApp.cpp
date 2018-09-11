#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    #ifdef TARGET_OPENGLES
        shader.load("shadersES2/shader");
    #else
        if(ofIsGLProgrammableRenderer()){
            shader.load("shaders/shader"); //GL3
        }else{
            shader.load("shaders/shader"); //GL2
        }
    #endif
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0,0,0);
    ofDisableArbTex();
    

    bDrawModel = false;
    bHideGui = false;
    bLighting = false;
    bShader = false;
    

    gui.setup();
    gui.add(bMeshModeToggle.setup("Mesh Mode", false));
    gui.add(bMeshTypeToggle.setup("Mesh Type", false));
    gui.add(bShowSecondaryMesh.setup("Mesh Secondary", false));
    gui.add(transition.setup("Mesh Transition", 0.0, 0.0, 1.0f));
    gui.add(bShader.setup("CPU / GPU", true));
    gui.add(ampGPU.setup("Amplitude GPU", 0.5f, 20.0f, 0.0f));
    gui.add(speedGPU.setup("Speed GPU", 0.5f, 0.0f, 5.0f));
    gui.add(heightGPU.setup("Height GPU", 1.0f, 0.0f, 4.0f));

    
    gui.add(amp.setup("Amplitude CPU", 0.5f, -5.0f, 10.0f));
    gui.add(ampTotal.setup("Amplitude Total CPU", 0.5f, -5.0f, 10.0f));
    gui.add(liquid.setup("Liquid CPU", 0.5f, -5.0f, 500.0f));
    gui.add(dampen.setup("Dampening CPU", 0.5f, 0.0f, 5.0f));
    
    gui.add(bRotateLock.setup("Rotate Type", false));
    gui.add(speed.setup("Rotate Speed", 0.5f, 0.0f, 15.0f));
    gui.add(rotation.setup("Rotation", ofVec3f(0.5f, 0.5f, 0.5f),ofVec3f(0.0f,0.0f,0.0f), ofVec3f(5.0f,5.0f,5.0f)));
    gui.add(scaling.setup("Scaling",ofVec3f(0.5f, 0.5f, 0.5f),ofVec3f(0.0f,0.0f,0.0f), ofVec3f(5.0f,5.0f,5.0f)));
    gui.add(alpha.setup("Alpha", 0.0001f, 0.0f, 0.007f));
    gui.add(color.setup("Color", ofColor(255,255,255), ofColor(0, 0), ofColor(255, 255)));
    gui.add(colorSecondary.setup("Color Mesh Secondary", ofColor(255,0,0), ofColor(0, 0), ofColor(255, 255)));
    
    dragCounter = -1;
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
            ofVec2f modelOffset = ofVec2f(0.5f,0.5f);
            int w = ofGetWidth();
            int h = ofGetHeight();
            draggedModel.setPosition(w * modelOffset.x, h * modelOffset.y, 0.0f);
            bDrawModel = true;
            
            dragCounter++;
            dragCounter%=2;
            if(dragCounter==0){
                draggedPreviousModel.setPosition(w * modelOffset.x, h * modelOffset.y, 0.0f);
                //draggedPreviousModel.loadModel(filePath);
                draggedPreviousModel = draggedModel;
    
            }
        } else if (bTextureLoaded) {
//            vid.load(filePath);
//            vid.play();
//            tex = vid.getTexture();
            img.load(filePath);
            tex = img.getTexture();
            
        }
        //std::cout << "Total Files: " << total << std::endl;
        //std::cout << "File Path: " << filePath << std::endl;
        
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    material.setAmbientColor(color);
    material.setShininess(1.0f);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(!bHideGui) {
        gui.draw();
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10,10);
    }
    
    if(bDrawModel) {
        drawModel();
    }

    
}

void ofApp::setVerts(vector<ofVec3f> &v){
    
    float l = liquid;
    float amplitude = amp / ampTotal;
    float speedDampen = dampen;
    
    for(unsigned int i = 0; i < v.size(); i++){
        
        v[i].x += ofSignedNoise(v[i].x/l, v[i].y/l,v[i].z/l, ofGetElapsedTimef()/speedDampen)*amplitude;
        v[i].y += ofSignedNoise(v[i].z/l, v[i].x/l,v[i].y/l, ofGetElapsedTimef()/speedDampen)*amplitude;
        v[i].z += ofSignedNoise(v[i].y/l, v[i].z/l,v[i].x/l, ofGetElapsedTimef()/speedDampen)*amplitude;
    
    }
}

void ofApp::interpolateVerts(vector<ofVec3f>& currentMesh, vector<ofVec3f>& previousMesh){
    
    ///float offsetScale = 1.0;
    
    for(unsigned int i = 0; i < currentMesh.size(); i++){
        ofVec3f from = currentMesh[i];
        ofVec3f to = previousMesh[i];
        ofVec3f result = ofVec3f(ofLerp(from.x, to.x, transition),ofLerp(from.y, to.y, transition),ofLerp(from.z, to.z, transition));
        currentMesh[i] = result;
        
//        currentMesh[i].x += currentMesh[i].x * offsetScale + previousMesh[i].x * transition;
    }
}


void ofApp::drawModel(){

    ofVec3f scale = draggedModel.getScale();
    ofVec3f position = draggedModel.getPosition();
    float normalizedScale = draggedModel.getNormalizedScale();
    
    ofVboMesh mesh = draggedModel.getMesh(0);
    ofVboMesh meshSecondary = draggedModel.getMesh(0);
    ofVboMesh meshPrevious = draggedPreviousModel.getMesh(0);
    
    ofTexture texture;
    ofxAssimpMeshHelper& meshHelper = draggedModel.getMeshHelper( 0 );

    ofTranslate(position);
    
    float t = ofGetElapsedTimef() * speed;
    float rx;
    float ry;
    float rz;
    
    if (bRotateLock){
        rx = ofRadToDeg(rotation->x);
        ry = ofRadToDeg(rotation->y);
        rz = ofRadToDeg(rotation->z);

    } else {
        rx = t * rotation->x;
        ry = t * rotation->y;
        rz = t * rotation->z;

    }
    
    ofRotateX(rx);
    ofRotateY(ry);
    ofRotateZ(rz);
    ofScale(normalizedScale, normalizedScale, normalizedScale);
    ofScale(scaling->x,scaling->y,scaling->z);

    
    if (bMeshModeToggle) {
        if(bMeshTypeToggle) {
            mesh.setMode(OF_PRIMITIVE_POINTS);
        }else{
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        }
    }
    

    vector<ofVec3f>& verts = mesh.getVertices(); //Main Mesh Displace
    vector<ofVec3f>& vertsPrevious = meshPrevious.getVertices(); //Mix Mesh Transition

    if (!bShader) { ///SET VERTS CPU

    setVerts(verts);
    
        if(bShowSecondaryMesh){
            vector<ofVec3f>& vertsSecondary = meshSecondary.getVertices(); //Secondary Mesh Displace
            setVerts(vertsSecondary);
            interpolateVerts(vertsSecondary, vertsPrevious);
        }
    
    interpolateVerts(verts,vertsPrevious);
        
    }
    
    for(int i = 0; i < verts.size(); i++){
        ofFloatColor c = color;
        c.a = i * alpha;
        mesh.addColor(c);
        
        if(bShowSecondaryMesh){
            ofFloatColor c2 = colorSecondary;
            c2.a = i * alpha;
            meshSecondary.addColor(c2);
            }
        }
    
    
    ofEnableDepthTest();
    
    if (bShader) {
        
        shader.begin();
        float t = ofGetElapsedTimef() * speedGPU;
        shader.setUniform1f("time", t);
        shader.setUniform1f("amount", ampGPU);
        shader.setUniform1f("height", heightGPU);

    }
    
    
    tex.bind();
    material.begin();
    float s = 1.0;
    ofScale(s,s);
    mesh.drawFaces();
    material.end();
    tex.unbind();
    
    if(bShowSecondaryMesh) {
        //ofPushStyle();
            ofPushMatrix();
               // ofVec3f offsetSize = ofVec3f( 2.2f, 2.4f, 2.3f );
               // ofScale(scaling->x * offsetSize.x,scaling->y * offsetSize.y, scaling->z * offsetSize.z);
                meshSecondary.setMode(OF_PRIMITIVE_POINTS);
                meshSecondary.drawWireframe();
        
            ofPopMatrix();
        //ofPopStyle();
    }
    
    if(bShader) {
        shader.end();
    }
    
    
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
    
    if(key == 'l'){
        bLighting = !bLighting;
        if(bLighting) {
            light.enable();
            light.setSpotlightCutOff(100.0f);
        } else {
            light.disable();
            
        }
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

