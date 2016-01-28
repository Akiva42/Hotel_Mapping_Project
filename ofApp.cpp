/**
 *  ofxCsv
 *  Inspired and based on Ben Fry's [table class](http://benfry.com/writing/map/Table.pde)
 *
 *  
 *  The MIT License
 *
 *  Copyright (c) 2011-2014 Paul Vollmer, http://www.wng.cc
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *  
 *  @testet_oF          0071
 *  @testet_plattform   MacOs 10.6+
 *                      ??? Win
 *                      ??? Linux
 *  @dependencies       
 *  @modified           2012.06.28
 *  @version            0.1.3
 */

#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup() {
	//set up vars.
	_mesh.setMode(OF_PRIMITIVE_POINTS);
	_mesh.enableColors();
	_zoomLevel = 1;
	_mapCenterX = ofGetWidth() / 2;
	_mapCenterY = ofGetHeight() / 2;
	_mapOffsetX = 0;
	_mapOffsetY = 0;
	
	loadFile("hotel_Rating_Most_Small.csv");
	//countRatings();
	//writeFile("");
	//mapData();
	readSmallFile();
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight() / 2);
	ofScale(_zoomLevel, _zoomLevel, 1);
	ofTranslate(_mapCenterX, _mapCenterY);
	_mesh.drawVertices();
	ofPopMatrix();

	int line = 0;
	for (int i = countrysNumbOfRate.size() - 1; i > 0; i--) {
		float maxBarWidth = 200;
		float boxWidth = ofMap(countrysNumbOfRate[i].numberOfRaitings, 0, countrysNumbOfRate[countrysNumbOfRate.size() - 1].numberOfRaitings, 0, maxBarWidth);
		line += 1;
		float barHeight = 40;
		float centerSection = 400;
		float y = barHeight * line;
		ofSetColor(ofColor::white);
		ofDrawRectangle(ofGetWidth() / 2 - boxWidth - centerSection/2, y, boxWidth, barHeight/2);
		ofSetColor(ofColor::gray);
		ofDrawBitmapString(countrysNumbOfRate[i].name, ofGetWidth() / 2 - centerSection/2 - maxBarWidth, y+barHeight/3);

		for (int j = 0; j < countrys.size(); j++) {
			float boxWidth = ofMap(countrys[j].averageRaiting, 0, countrys[countrys.size() - 1].averageRaiting, 0, maxBarWidth);
			ofSetColor(ofColor::white);
			ofDrawRectangle(ofGetWidth() / 2 + centerSection / 2, y, boxWidth, barHeight / 2);
			ofSetColor(ofColor::gray);
			ofDrawBitmapString(countrys[i].name, ofGetWidth() / 2 + centerSection / 2 + maxBarWidth, y + barHeight / 3);
			ofSetColor(ofColor::gray);
		}

		for (int j = 0; j < countrys.size(); j++) {
			if (countrysNumbOfRate[i].name.compare(countrys[j].name) == 0) {
				if (ofGetMouseX() > ofGetWidth() / 2) {
					if (ofGetMouseY() > y && ofGetMouseY() < y + barHeight / 2) {
						ofSetColor(ofColor::red);
					}
				}
				ofDrawLine(ofGetWidth() / 2 + centerSection / 2, y + barHeight / 4, ofGetWidth() / 2 - centerSection / 2, barHeight * j + barHeight / 4);
			}
		}
	}
}
//--------------------------------------------------------------

void ofApp::loadFile(string fileName) {
	// Load a CSV File.
	cout << "Now loading the CSV file. This could take about a minute." << endl;
	_csv.loadFile(ofToDataPath(fileName));
	cout << "Finished loadeding the csv file." << endl;
}
//--------------------------------------------------------------

void ofApp::writeFile(string fileName) {
	//this creates a new CSV file with less data elements.
	int recorderLineCount = 0;
	for (int j = 0; j < _csv.data.size(); j++) {
		if (_csv.data[j].size() >= 20) {
			//lat-long
			//_csvRecorder.setString(recorderLineCount, 0, _csv.data[j][12]);
			//_csvRecorder.setString(recorderLineCount, 1, _csv.data[j][13]);

			//names and starts
			//_csvRecorder.setString(recorderLineCount, 0, _csv.data[j][7]);
			//_csvRecorder.setString(recorderLineCount, 1, _csv.data[j][2]);

			recorderLineCount++;
		}
		else {
			_csvRecorder.saveFile(ofToDataPath(fileName));
		}
	}
	_csvRecorder.saveFile(ofToDataPath(fileName));
}
//--------------------------------------------------------------

void ofApp::mapData() {
	//map the data
	for (int j = 0; j < _csv.data.size(); j++) {
		float myX = ofMap(ofToFloat(_csv.data[j][0]), -180, 180, 2000, -2000);
		float myY = ofMap(ofToFloat(_csv.data[j][1]), -90, 90, -1000, 1000);
		ofVec3f thisPoint(myY, myX, 0.0f);
		_mesh.addVertex(thisPoint);
		_mesh.addColor(ofFloatColor(1, 1));
	}
}
//--------------------------------------------------------------

void ofApp::countRatings() {
	//find the average raiting for each country
	for (int i = 0; i < _csv.data.size(); i++) {
		if (ofToString(_csv.data[i][0]).compare("")!=0 && ofToString(_csv.data[i][1]).compare("")!=0) {
			//if this is the first country on the list.
			if (countrys.size() == 0) {
				Country tempContry;
				tempContry.name = ofToString(_csv.data[i][0]);
				tempContry.numberOfRaitings = 1;
				tempContry.averageRaiting = ofToFloat(_csv.data[i][1]);
				countrys.push_back(tempContry);
			}
			bool foundMatch = false;
			int matchNum = -1;
			for (int j = countrys.size()-1; j > 0; j--) {
				if (ofToString(_csv.data[i][0]).compare(countrys[j].name) == 0) {
					foundMatch = true;
					matchNum = j;
					break;
				}
			}
			if (foundMatch) {
				countrys[matchNum].numberOfRaitings += 1;
				countrys[matchNum].averageRaiting = (countrys[matchNum].averageRaiting*float(countrys[matchNum].numberOfRaitings-1) + ofToFloat(_csv.data[i][1])) / countrys[matchNum].numberOfRaitings;
				matchNum = false;
			}
			else {
				Country tempContry;
				tempContry.name = ofToString(_csv.data[i][0]);
				tempContry.numberOfRaitings = 1;
				tempContry.averageRaiting = ofToFloat(_csv.data[i][1]);
				countrys.push_back(tempContry);
			}
		}
	}
	//print out the country data
	//for (int i = 0; i < countrys.size(); i++) {
	//	cout << countrys[i].name + " : " + ofToString(countrys[i].numberOfRaitings) + " : " + ofToString(countrys[i].averageRaiting) << endl;
	//}
	cout << "done making countrys" << endl;
	sortByAvgRaitings();
}
//--------------------------------------------------------------

bool sortByAvg(Country a, Country b) {
	return (a.averageRaiting < b.averageRaiting);
}
bool sortByNumOfRate(Country a, Country b) {
	return (a.numberOfRaitings < b.numberOfRaitings);
}

void ofApp::sortByAvgRaitings() {
	countrys = countrysNumbOfRate;
	sort(countrysNumbOfRate.begin(), countrysNumbOfRate.begin() + countrysNumbOfRate.size(), sortByNumOfRate);
	sort(countrys.begin(), countrys.begin() + countrys.size(), sortByAvg);

	//print out the country data
	////for (int i = 0; i < countrysNumbOfRate.size(); i++) {
		//cout << countrys[i].name + " : " + ofToString(countrys[i].numberOfRaitings) + " : " + ofToString(countrys[i].averageRaiting) << endl;
	//}
}

//--------------------------------------------------------------

void ofApp::readSmallFile() {
	for (int i = 0; i < _csv.data.size(); i++) {
		Country temp;
		temp.name = _csv.data[i][0];
		temp.numberOfRaitings = ofToFloat(_csv.data[i][1]);
		temp.averageRaiting = ofToFloat(_csv.data[i][2]);
		countrysNumbOfRate.push_back(temp);
	}
	countrys = countrysNumbOfRate;
	sort(countrys.begin(), countrys.begin() + countrys.size(), sortByAvg);
}
//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
	if (key == '-') {
		_zoomLevel -= 0.2;
	}
	if (key == '=') {
		_zoomLevel += 0.2;
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
	_mapCenterX = ofGetMouseX() - _mapOffsetX;
	_mapCenterY = ofGetMouseY() - _mapOffsetY;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	_mapOffsetX = ofGetMouseX() - _mapCenterX;
	_mapOffsetY = ofGetMouseY() - _mapCenterY;
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
