#include "jank.h"
#include "maximilian.h"

//--------------------------------------------------------------
void jank::setup(){
	ofBackground(34, 34, 34);

	// 2 output channels,
	// 0 input channels
	// 22050 samples per second
	// 512 samples per buffer
	// 4 num buffers (latency)
	
	int bufferSize		= 512;
	sampleRate 			= 44100;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	
	soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 

	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void jank::update(){

}

//--------------------------------------------------------------
void jank::draw(){
	ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 150, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofRect(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (int i = 0; i < lAudio.size(); i++){
				float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
				ofVertex(x, 100 -lAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 350, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofRect(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (int i = 0; i < rAudio.size(); i++){
				float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
				ofVertex(x, 100 -rAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
}

void jank::exit(){
	soundStream.close();
}

//--------------------------------------------------------------
void jank::keyPressed(int key){

}

//--------------------------------------------------------------
void jank::keyReleased(int key){

}

//--------------------------------------------------------------
void jank::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void jank::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void jank::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void jank::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void jank::windowResized(int w, int h){

}

//--------------------------------------------------------------
void jank::audioOut(float * output, int bufferSize, int nChannels){
		for (int i = 0; i < bufferSize; i++){
	
			sample=test1.sinewave(440);

			lAudio[i] = output[i*nChannels    ] = sample;
			rAudio[i] = output[i*nChannels + 1] = sample;

		}
}

//--------------------------------------------------------------
void jank::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void jank::dragEvent(ofDragInfo dragInfo){ 

}