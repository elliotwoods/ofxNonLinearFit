#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "//--" << endl;
	cout << "All algorithms:" << endl;
	cout << "//--" << endl;
	cout << "//" << endl;
	const auto allAlgorithms = ofxNonLinearFit::Algorithm::ListAll();
	for(auto algorithm : allAlgorithms) {
		cout << algorithm.toString() << endl;
	}
	cout << "//" << endl;
	cout << "//--" << endl;
	cout << endl;

	cout << "//--" << endl;
	cout << "Global gradient-less algorithms:" << endl;
	cout << "//--" << endl;
	cout << "//" << endl;
	const auto globalGradientlessAlgorithms = ofxNonLinearFit::Algorithm::List(ofxNonLinearFit::Algorithm::GlobalGradientless);
	for(auto algorithm : globalGradientlessAlgorithms) {
		cout << algorithm.toString() << endl;
	}
	cout << "//" << endl;
	cout << "//--" << endl;
	cout << endl;
	
	cout << "//--" << endl;
	cout << "Global gradient algorithms:" << endl;
	cout << "//--" << endl;
	cout << "//" << endl;
	const auto globalGradientAlgorithms = ofxNonLinearFit::Algorithm::List(ofxNonLinearFit::Algorithm::GlobalGradient);
	for(auto algorithm : globalGradientAlgorithms) {
		cout << algorithm.toString() << endl;
	}
	cout << "//" << endl;
	cout << "//--" << endl;
	cout << endl;

	cout << "//--" << endl;
	cout << "Local gradient-less algorithms:" << endl;
	cout << "//--" << endl;
	cout << "//" << endl;
	const auto localGradientlessAlgorithms = ofxNonLinearFit::Algorithm::List(ofxNonLinearFit::Algorithm::LocalGradientless);
	for(auto algorithm : localGradientlessAlgorithms) {
		cout << algorithm.toString() << endl;
	}
	cout << "//" << endl;
	cout << "//--" << endl;
	cout << endl;

	cout << "//--" << endl;
	cout << "Local gradient algorithms:" << endl;
	cout << "//--" << endl;
	cout << "//" << endl;
	const auto localGradientAlgorithms = ofxNonLinearFit::Algorithm::List(ofxNonLinearFit::Algorithm::LocalGradient);
	for(auto algorithm : localGradientAlgorithms) {
		cout << algorithm.toString() << endl;
	}
	cout << "//" << endl;
	cout << "//--" << endl;
	cout << endl;

	ofSetWindowShape(300, 50);
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString("Check the console for results", 20, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
