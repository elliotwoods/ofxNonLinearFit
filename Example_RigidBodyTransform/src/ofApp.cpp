#include "ofApp.h"

using namespace ofxNonLinearFit;

//--------------------------------------------------------------
void ofApp::setup(){
	//in this example, we will try many different fitting algorithms against one data set
	//to keep track of which algorithm we are currently using, we have a pointer called:
	this->currentFitter = NULL;

	//pull this type out for convenience
	typedef ofxNonLinearFit::Models::RigidBody Model;

	//--
	//synthesise training data
	//
	//
	//
	//setup a target model with our target parameters
	//the target model is generally unknown (that's why we're doing the fit!)
	//but we use one here to synthesise a training dataSet
	Model targetModel;
	targetModel.setTransform(ofVec3f(2.0f, 1.0f, 0.0f), ofVec3f(45, 10, 0));
	//
	//synthesise 10,000 data points
	for(int i=0; i<10000; i++) {
		Model::DataPoint dataPoint;
		dataPoint.x = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
		targetModel.evaluate(dataPoint); //calculate dataPoint.xdash using target model

		//add this dataPoint to the dataSet
		this->dataSet.push_back(dataPoint);
	}
	//
	//--


	//get a list of all algorithms we could use for the test
	this->listOfAlgorithms = Algorithm::List(Algorithm::LocalGradientless);

	//define the function that will run the rests in the background thread
	auto runTests = [=] () {
		//we run forever (p.s. this will cause crash on quit)
		while(true) {
			int index = 0;
			for(auto & algorithm : this->listOfAlgorithms) {
				auto fitter = ofxNonLinearFit::Fit<Model>(algorithm);
				this->currentFitter = & fitter;

				//reset model parameters
				this->model.initialiseParameters();
				double residual;

				auto startTime = ofGetElapsedTimeMicros();
				bool success = fitter.optimise(this->model, &dataSet, &residual);
				auto endTime = ofGetElapsedTimeMicros();
				
				this->currentFitter = NULL;

				Result result = {
					algorithm.toString(),
					residual,
					(double) (endTime - startTime) / 1e6,
					success
				};

				this->lockResults.lock();
				if (index >= this->results.size()) {
					this->results.push_back(Result());
				}
				this->results[index] = result;
				this->lockResults.unlock();

				index++;
			}
		}
	};

	//we run the fits in a background thread so that we can visualise the results as the fit is being performed
	this->thread = std::thread(runTests);
	
	glPointSize(2.0f);
}
	
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(40, 0);

	//make a copy of the training set to get the x values
	auto evaluatedDataSet = this->dataSet;
	
	//evaluate the xdash values using the current model
	this->model.evaluateSet(evaluatedDataSet);
	
	//draw the 3d preview scene of target points and current fit results
	camera.begin();
	ofDrawGrid();
	ofMesh targetPoints;
	ofMesh evalatedPoints;
	int i=0;
	for(auto & evaluatedDataPoint : evaluatedDataSet) {
		targetPoints.addVertex(this->dataSet[i].xdash);
		evalatedPoints.addVertex(evaluatedDataPoint.xdash);
		i++;
	}
	ofPushStyle();
	ofSetColor(200,100,100);
	targetPoints.drawVertices();
	ofSetColor(100,200,100);
	evalatedPoints.drawVertices();
	ofPopStyle();
	camera.end();

	//print the status to the screen
	stringstream status;
	string algoName;
	if (this->currentFitter) {
		algoName = this->currentFitter->getAlgorithm().toString();
	}
	status << "Current algorithm = " << algoName << endl;
	status << endl;
	status << "Success\t Residual\t\t Calc Time\t \tAlgorithm" << endl;
	this->lockResults.lock();
	for(auto result : this->results) {
		status << result.success << "\t " << std::setprecision(10) << result.minResidual << "\t" << std::setprecision(8) << result.calcTime << "s\t" << result.algorithmName << endl;
	}
	this->lockResults.unlock();
	ofDrawBitmapString(status.str(), 20, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		if (this->currentFitter) {
			this->currentFitter->forceStop();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
