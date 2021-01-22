#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	ofEasyCam cam;
	ofFbo fbo;
	ofTrueTypeFont font;
	vector<string> word_list;
	int word_index;
	string draw_word;

	vector<glm::vec3> location_list;
	vector<int> param_list;

	int span, number_of_columns;
	ofMesh face, line;
};