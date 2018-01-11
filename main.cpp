#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"

struct Box {
	NVGcolor color;
	glm::ivec2 position;
};

struct Data {
	static Data* instance;

	std::vector<Box> boxes;
	void generateBoxes() {
		for( int y = 0; y < 10; y++ ) {
			for( int x = 0; x < 10; x++ ) {
				Box box = { .color = nvgRGB(0,128,255), .position = glm::ivec2(x,y) };
				boxes.push_back(box);
			}
		}
	}
};

Data* Data::instance = new Data();

struct BoxLayer : public Layer {
	Box* box = nullptr;

	BoxLayer() {
		int size = 20 + rand() % 20;
		bounds = {0,0,size,size};
		isUpdatable = true;
		isInteractable = true;

		onClick = [&]() {
			bounds = {0,0,100,100};
		};
	}

	void update(double dt) override {
		Layer::update(dt);
		position.x = box->position.x * 30.0f;
		position.y = box->position.y * 30.0f;

		auto isHovered = (this == scene->mouseTarget);
		auto isActive = (this == scene->mouseDownTarget);

		box->color = isHovered ? nvgRGB(0,128,255) : nvgRGB(255,128,0);
		if( isActive )
			box->color = nvgRGB(255,255,0);
	}

	void render(NVGcontext* c) override {
		nvgBeginPath(c);
		nvgStrokeColor(c, nvgRGBA(0,0,0,128));
		nvgFillColor(c, box->color);
		nvgRect(c,0,0,bounds.w,bounds.h);
		nvgStroke(c);
		nvgFill(c);
		Layer::render(c);
	}
};


struct TestLayer : public Layer {
	TestLayer() {
		bounds = {0,0,200,200};
		isInteractable = true;
	}

	void render(NVGcontext* c) override {
		auto isHovered = (this == scene->mouseTarget);
		auto color = isHovered ? nvgRGB(0,128,255) : nvgRGB(255,128,0);
		auto paint = nvgLinearGradient(c, 0, 0, 0, bounds.h, nvgRGBA(0,0,0,0), color);
		nvgBeginPath(c);
		nvgFillPaint(c, paint);
		nvgRect(c,0,0,bounds.w,bounds.h);
		nvgFill(c);
		Layer::render(c);
	}
};


int main() {
	GLFWwindow* window = nullptr;
	NVGcontext* nvg = nullptr;

	Scene scene;
	scene.initGraphics(&window, &nvg);

	auto& data = Data::instance;

	data->generateBoxes();

	const uint32_t count = data->boxes.size();
	std::vector<BoxLayer> boxLayers(count);
	for( int i = 0; i < data->boxes.size(); i++ ) {
		boxLayers[i].box = &data->boxes[i];
		scene.addLayer(boxLayers[i]);
	}

	TestLayer testLayer;
	testLayer.position = {100,100};
	scene.addLayer(testLayer);

	TestLayer test2;
//	test2.bounds = {50,50};
	test2.position = {45,20};
	testLayer.addLayer(test2);


	double startTime = glfwGetTime();
	double frameTime = 0.0f;
	while( !glfwWindowShouldClose(window) ) {
		frameTime = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		scene.processEvents(window);
		scene.update(frameTime);
		scene.render(window, nvg);

		glfwSwapBuffers(window);
	}

	return 0;
}