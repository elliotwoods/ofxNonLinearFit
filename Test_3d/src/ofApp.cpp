#include "ofApp.h"
#include <functional>
#include <thread>

struct DataPoint {
	DataPoint(ofVec3f x, ofVec3f xdash) {
		this->x = x;
		this->xdash = xdash;
	}

	ofVec3f x;
	ofVec3f xdash;
};

struct DataSet {
	vector<DataPoint> data;
};

vector<double> solution(6);
double error;
int step = 0;

ofMatrix4x4 getTransform(const double * params) {
	ofVec3f eulerAngles(params[3], params[4], params[5]);
	ofQuaternion quaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1));
	const auto translate = ofMatrix4x4::newTranslationMatrix(params[0], params[1], params[2]);
	const auto rotate = ofMatrix4x4::newRotationMatrix(quaternion);
	return translate * rotate;
}

double myfunc(unsigned n, const double * x, double * grad, void * my_func_data) {
	auto dataSet = * (DataSet*) my_func_data;
	if(grad) {
		throw;
	}

	auto transform = getTransform(x);

	float sumSquare = 0.0f;
	for(const auto & dataPoint : dataSet.data) {
		sumSquare += (dataPoint.xdash - dataPoint.x * transform).lengthSquared();
	}
	error = sqrt(sumSquare / (double) dataSet.data.size());
	step++;

	memcpy(&solution[0], x, 6 * sizeof(double));
	//cout << error << endl;
	return error;
}

DataSet data;

void doFitAlg(nlopt::algorithm a) {
	auto optimiser = nlopt::opt(a, 6);
	vector<double> translationRotation(6, 0.0);
	double minRms;
	optimiser.set_min_objective(myfunc, &data);

	double lowerBoundsRaw[6] = {-HUGE_VAL, -HUGE_VAL, -HUGE_VAL, -180.0, -180.0, -180.0};
	double upperBoundsRaw[6] = {+HUGE_VAL, +HUGE_VAL, +HUGE_VAL, +180.0, +180.0, +180.0};
	vector<double> lowerBounds(lowerBoundsRaw, lowerBoundsRaw + 6);
	vector<double> upperBounds(upperBoundsRaw, upperBoundsRaw + 6);
	//optimiser.set_lower_bounds(lowerBounds);
	//optimiser.set_upper_bounds(upperBounds);

	optimiser.set_stopval(0.003);
	optimiser.set_maxtime(30.0f);
	try {
		auto result = optimiser.optimize(translationRotation, minRms);
	} catch (std::exception e) {
		cout << "Exception : " << e.what() << endl;
	}

	cout << minRms << endl;
	for(auto x : translationRotation) {
		cout << x << ", ";
	}
}

void doFit() {
	nlopt::algorithm a[5] = {nlopt::LN_COBYLA, nlopt::LN_BOBYQA, nlopt::LN_PRAXIS, nlopt::LN_NELDERMEAD, nlopt::LN_SBPLX};
	vector<double> errors;
	vector<int> steps;
	for(int i=0; i<5; i++) {
		doFitAlg(a[i]);
		cout << "Algo " << i << ", error=" << error << "\tsteps=" << step << endl;
	}
}

std::thread runThread;
//--------------------------------------------------------------
void ofApp::setup(){
	const auto translate = ofMatrix4x4::newTranslationMatrix(2.5f, 0.0f, 2.0f);
	
	const auto eulerAngles = ofVec3f(90, 0, 0);
	ofQuaternion quaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1));
	const auto rotate = ofMatrix4x4::newRotationMatrix(quaternion);
	
	ofMatrix4x4 transform = translate * rotate;

	for(int i=0; i<10000; i++) {
		const auto x = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
		const auto xdash = x * transform;
		data.data.push_back(DataPoint(x, xdash));
	}

	camera.setPosition(4.0, 4.0, 4.0);
	runThread = std::thread(doFit);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(40), ofColor(0));
	stringstream msg;
	msg << "Solution = ";
	for(auto x : solution) {
		msg << x << ",\t";
	}
	msg << endl;
	msg << error << endl;
	msg << step << endl;
	ofDrawBitmapString(msg.str(), 20, 20);


	ofMesh xPoints;
	ofMesh xDashPoints;
	for(auto dataPoint : data.data) {
		xPoints.addVertex(dataPoint.x);
		xDashPoints.addVertex(dataPoint.xdash);
	}

	glPointSize(3.0f);

	camera.begin();
	ofDrawGrid();
	
	ofPushStyle();

	ofSetColor(200,100,100);
	xDashPoints.drawVertices();

	ofSetColor(100,200,100);
	ofPushMatrix();
	ofMultMatrix(getTransform(&solution[0]));
	xPoints.drawVertices();

	ofPopStyle();

	ofPopMatrix();
	camera.end();

}

void ofApp::exit() {
	runThread.join();
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
