#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"


struct TestLayer : public Layer {
	NVGcolor color{};

	TestLayer() : Layer(), color(nvgRGB(255,255,255)){
		bounds = {0,0,200,200};
		isUpdatable = true;
		anchor = { 100, 100 };
	}

	explicit TestLayer(NVGcolor _color) : color(_color) {
		bounds = {0,0,200,200};
		isUpdatable = true;
		anchor = { 100, 100 };
	}

	void render(NVGcontext* c) override {
		auto paint = nvgLinearGradient(c,0,0,0,bounds.h,nvgRGBA(0,0,0,0),color);
		nvgBeginPath(c);
		nvgFillPaint(c, paint);
		nvgRect(c, bounds.x, bounds.y, bounds.w, bounds.h);
		nvgFill(c);

		Layer::render(c);
	}

	void update(double dt) override {
		rotation += dt * 0.1f;
		Layer::update(dt);
	}
};


int main() {
	GLFWwindow* window = nullptr;
	NVGcontext* nvg = nullptr;

	Scene scene;
	scene.initGraphics(&window, &nvg);

	TestLayer test(nvgRGB(255,128,0));
	scene.addLayer(test);
	test.position = { 500, 200 };

	TestLayer test2(nvgRGB(0,128,255));
	test.addLayer(test2);
	test2.position = glm::vec2(50,50);

	constexpr int count = 10;
	TestLayer* layers = new TestLayer[count];
	for( int i = 0; i < count; i++ ) {
		auto t = static_cast<float>(i) / static_cast<float>(count) * 3.14159f * 2.0f;
		auto& l = layers[i];
		float radius = 100.0f + static_cast<float>(rand() % 1000) / 1000.0f * 150.0f;
		l.color = nvgRGBf(t/7.0f,0,1.0f-(t/7.0f));
		l.bounds = {0,0,25,25};
		l.position.x = radius * cosf(t);
		l.position.y = radius * sinf(t);
		test.addLayer(l);
	}

	TestLayer x(nvgRGB(255,255,0));
	x.bounds = {0,0,90,90};
	x.isUpdatable = false;
	scene.addLayer(x);

	TestLayer test3(nvgRGB(255,0,0));
	test3.position = {100, 300};
	test2.addLayer(test3);

	double startTime = glfwGetTime();
	double frameTime = 0.0f;
	while( !glfwWindowShouldClose(window) ) {
		frameTime = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		scene.processEvents(window);
		scene.update(frameTime);
		scene.render(window, nvg);

		x.position = test2.localToScene(glm::vec2(-45,-45));

		if( test2.bounds.containsPoint(test2.sceneToLocal(scene.mousePosition)) ) {
			test2.color = nvgRGB(255,255,0);
		}
		else {
			test2.color = nvgRGB(0,128,255);
		}

		glfwSwapBuffers(window);
	}

	return 0;
}