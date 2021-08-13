#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->base_radius = 65;
	this->ico_sphere = ofIcoSpherePrimitive(this->base_radius, 3);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	float radius_start = this->base_radius;
	float radius_max = this->base_radius * 5;
	for (auto& triangle : this->ico_sphere.getMesh().getUniqueFaces()) {

		auto noise_seed_x = ofRandom(1000);
		auto noise_seed_y = ofRandom(1000);
		auto noise_seed_z = ofRandom(1000);

		auto noise_value = ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005);
		float radius_end = noise_value > 0.83 ? ofMap(noise_value, 0.83, 1, radius_start, radius_max) : radius_start;
		this->frame.addVertex(glm::vec3());

		for (float radius = radius_start; radius <= radius_end; radius += 1.5) {

			auto mesh_index = this->face.getNumVertices();
			auto frame_index = this->frame.getNumVertices();

			auto param = ofMap(radius, radius_start, radius_max, 0, PI * 0.30);

			auto angle_x = ofMap(ofNoise(noise_seed_x, radius * 0.0075 + ofGetFrameNum() * 0.005), 0, 1, -param, param);
			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto angle_y = ofMap(ofNoise(noise_seed_y, radius * 0.0075 + ofGetFrameNum() * 0.005), 0, 1, -param, param);
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto angle_z = ofMap(ofNoise(noise_seed_z, radius * 0.0075 + ofGetFrameNum() * 0.005), 0, 1, -param, param);
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
			glm::vec3 location = glm::normalize(avg) * radius;

			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec4(location + (triangle.getVertex(0) - avg), 0) * rotation_z * rotation_y * rotation_x);
			vertices.push_back(glm::vec4(location + (triangle.getVertex(1) - avg), 0) * rotation_z * rotation_y * rotation_x);
			vertices.push_back(glm::vec4(location + (triangle.getVertex(2) - avg), 0) * rotation_z * rotation_y * rotation_x);

			this->face.addVertices(vertices);
			this->frame.addVertices(vertices);

			if (radius == radius_start || radius == radius_end) {

				this->face.addIndex(mesh_index + 0); this->face.addIndex(mesh_index + 1); this->face.addIndex(mesh_index + 2);
			}

			this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index + 1);
			this->frame.addIndex(frame_index + 1); this->frame.addIndex(frame_index + 2);
			this->frame.addIndex(frame_index + 2); this->frame.addIndex(frame_index + 0);

			if (radius > radius_start) {

				this->face.addIndex(mesh_index + 0); this->face.addIndex(mesh_index + 1); this->face.addIndex(mesh_index - 2);
				this->face.addIndex(mesh_index + 0); this->face.addIndex(mesh_index - 2); this->face.addIndex(mesh_index - 3);

				this->face.addIndex(mesh_index + 1); this->face.addIndex(mesh_index + 2); this->face.addIndex(mesh_index - 1);
				this->face.addIndex(mesh_index + 1); this->face.addIndex(mesh_index - 1); this->face.addIndex(mesh_index - 2);

				this->face.addIndex(mesh_index + 2); this->face.addIndex(mesh_index + 0); this->face.addIndex(mesh_index - 3);
				this->face.addIndex(mesh_index + 2); this->face.addIndex(mesh_index - 3); this->face.addIndex(mesh_index - 1);


				this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index - 3);
				this->frame.addIndex(frame_index + 1); this->frame.addIndex(frame_index - 2);
				this->frame.addIndex(frame_index + 2); this->frame.addIndex(frame_index - 1);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.25);

	ofSetColor(239);
	this->frame.drawWireframe();

	ofSetColor(39);
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}