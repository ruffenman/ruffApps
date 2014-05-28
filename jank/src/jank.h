#pragma once

#include "ofMain.h"
#include "lua.hpp"

class jank : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	private:
		static void * luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize);
		static int luaPanic (lua_State *L);

		void audioOut(float * input, int bufferSize, int nChannels);
		double luaGetSample(bool isLeft);
		
		ofSoundStream soundStream;

		int		sampleRate;

		vector <float> lAudio;
		vector <float> rAudio;
		
		//------------------- for the simple sine wave synthesis

		lua_State* L;
};
