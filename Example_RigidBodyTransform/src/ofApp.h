#pragma once

#include "ofxNonLinearFit.h"
#include "ofxGrabCam.h"
#include "ofMain.h"
#include <thread>

struct Result {
	string algorithmName;
	double minResidual;
	double calcTime;
	bool success;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		vector<ofxNonLinearFit::Algorithm> listOfAlgorithms;
		ofxNonLinearFit::Models::RigidBody::DataSet dataSet;

		ofxNonLinearFit::Fit<ofxNonLinearFit::Models::RigidBody> * currentFitter;
		ofxNonLinearFit::Models::RigidBody model;

		std::thread thread;
		ofMatrix4x4 transform;

		ofMutex lockResults;
		vector<Result> results;

		ofxGrabCam camera;
};
