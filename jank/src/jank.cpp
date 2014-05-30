#include "jank.h"

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

	// Create a new independent Lua global state and set our own allocator function
	L = lua_newstate(&luaAlloc, NULL);
	if (L) {
		// Set our own panic function
		lua_atpanic(L, &luaPanic);

		// Load standard Lua libraries
		luaL_openlibs(L);

#ifdef _DEBUG
		lua_pushboolean(L, true);
		lua_setglobal(L, "DEBUG_BUILD");
#endif

		luaL_dofile(L,"lua\\jank.lua");
		
		// Place init func at top of stack
		lua_getglobal(L, "init");
		// Call the function
		lua_call(L, 0, 0);
	}
}

//--------------------------------------------------------------
void jank::update(){

}

//--------------------------------------------------------------
void jank::draw(){
	ofSetColor(225);
	ofDrawBitmapString("GAMEPAD DO SOUND THINGS", 31, 92);
	
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
	lua_close(L);
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
		double sampleL = luaGetSample(true);
		double sampleR = luaGetSample(false);

		lAudio[i] = output[i*nChannels    ] = sampleL;
		rAudio[i] = output[i*nChannels + 1] = sampleR;
	}
}

double jank::luaGetSample(bool isLeft) {
	double sample;

	// Place sample func and arg at top of stack
	lua_getglobal(L, "getSample");
	lua_pushboolean(L, isLeft);
	// Call the function
	lua_call(L, 1, 1);

	// Get returned sample value from stack
	sample = static_cast<double>(lua_tonumber(L, -1));
	// Pop value from stack
	lua_pop(L, 1);

	return sample;
}

//--------------------------------------------------------------
void jank::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void jank::dragEvent(ofDragInfo dragInfo){ 

}

void * jank::luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize){
	if(NULL != ptr){
		size_t origBlockSize = osize;
	} else {
		size_t luaType = osize;
	}
	if(nsize == 0){
		free(ptr);
		return NULL;
	} else {
		return realloc(ptr, nsize);
	}
}

int jank::luaPanic (lua_State *L) {
  luai_writestringerror("PANIC: unprotected error in call to Lua API (%s)\n",
                   lua_tostring(L, -1));
  return 0;  /* return to Lua to abort */
}