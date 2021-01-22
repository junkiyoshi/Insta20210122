#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->fbo.allocate(ofGetWidth(), ofGetHeight());

	this->font.loadFont("fonts/msgothic.ttc", 200, true, true, true);
	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	int word_index = 0;
	this->word_list = { "TO", "LIVE", "IS", "TO", "THINK" };
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 90 == 0) {

		this->draw_word = this->word_list[word_index];
		word_index = (word_index + 1) % this->word_list.size();
	}

	this->location_list.clear();
	this->param_list.clear();


	this->fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0, 92);

	this->font.drawString(this->draw_word, this->font.stringWidth(this->draw_word) * -0.5, 0);

	this->fbo.end();

	this->span = 5;
	this->number_of_columns = this->fbo.getWidth() / this->span;
	ofPixels pixels;
	this->fbo.readToPixels(pixels);
	for (int y = 0; y < this->fbo.getHeight(); y += span) {

		for (int x = 0; x < this->fbo.getWidth(); x += span) {

			this->location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, ofGetHeight() - y - ofGetHeight() * 0.5, 0));

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

				this->param_list.push_back(1);
			}
			else {

				this->param_list.push_back(0);
			}
		}
	}

	this->face.clear();
	this->line.clear();

	float threshold = 0.4;
	float len_param = 0.005;
	float frame_param = -0.02;

	int z = 0;

	auto tmp_location_list = this->location_list;
	for (auto& tmp : tmp_location_list) {

		tmp += glm::vec3(0, 0, z);
	}

	for (int i = 0; i < tmp_location_list.size(); i++) {

		auto location = tmp_location_list[i];

		auto noise_value = ofNoise(location.x * len_param, location.y * len_param + ofGetFrameNum() * frame_param) * this->param_list[i];
		if (noise_value < threshold) { continue; }

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(location.x - this->span * 0.5, location.y - this->span * 0.5, location.z));
		vertices.push_back(glm::vec3(location.x + this->span * 0.5, location.y - this->span * 0.5, location.z));
		vertices.push_back(glm::vec3(location.x - this->span * 0.5, location.y + this->span * 0.5, location.z));
		vertices.push_back(glm::vec3(location.x + this->span * 0.5, location.y + this->span * 0.5, location.z));

		this->face.addVertices(vertices);

		this->face.addIndex(this->face.getNumVertices() - 4); this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 1);
		this->face.addIndex(this->face.getNumVertices() - 4); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 1);

		auto tmp_index = i - this->number_of_columns;
		if (tmp_index < 0 || ofNoise(tmp_location_list[tmp_index].x * len_param, tmp_location_list[tmp_index].y * len_param + ofGetFrameNum() * frame_param) * this->param_list[tmp_index] < threshold) {

			this->line.addVertex(vertices[2]);
			this->line.addVertex(vertices[3]);

			this->line.addIndex(this->line.getNumVertices() - 1);
			this->line.addIndex(this->line.getNumVertices() - 2);
		}

		tmp_index = i + this->number_of_columns;
		if (tmp_index > tmp_location_list.size() || ofNoise(tmp_location_list[tmp_index].x * len_param, tmp_location_list[tmp_index].y * len_param + ofGetFrameNum() * frame_param) * this->param_list[tmp_index] < threshold) {

			this->line.addVertex(vertices[0]);
			this->line.addVertex(vertices[1]);

			this->line.addIndex(this->line.getNumVertices() - 1);
			this->line.addIndex(this->line.getNumVertices() - 2);
		}

		tmp_index = i + 1;
		if (tmp_index % this->number_of_columns == 0 || ofNoise(tmp_location_list[tmp_index].x * len_param, tmp_location_list[tmp_index].y * len_param + ofGetFrameNum() * frame_param) * this->param_list[tmp_index] < threshold) {

			this->line.addVertex(vertices[1]);
			this->line.addVertex(vertices[3]);

			this->line.addIndex(this->line.getNumVertices() - 1);
			this->line.addIndex(this->line.getNumVertices() - 2);
		}

		tmp_index = i - 1;
		if (i % this->number_of_columns == 0 || ofNoise(tmp_location_list[tmp_index].x * len_param, tmp_location_list[tmp_index].y * len_param + ofGetFrameNum() * frame_param) * this->param_list[tmp_index] < threshold) {

			this->line.addVertex(vertices[0]);
			this->line.addVertex(vertices[2]);

			this->line.addIndex(this->line.getNumVertices() - 1);
			this->line.addIndex(this->line.getNumVertices() - 2);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofTranslate(0, ofGetHeight() * -0.25);

	ofSetColor(239);
	this->face.draw();

	ofSetColor(39);
	this->line.drawWireframe();

	ofRotateX(270);
	this->fbo.draw(this->fbo.getWidth() * -0.5, ofGetHeight() * -0.5);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}