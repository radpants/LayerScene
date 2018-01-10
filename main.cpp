#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"


struct TestLayer : public Layer {
	NVGcolor color;

	TestLayer(NVGcolor _color) : Layer(), color(_color) {
		bounds = {0,0,200,200};
		isUpdatable = true;
		anchor = { 100, 100 };
	}

	void render(NVGcontext* c) override {
		auto paint = nvgLinearGradient(c,0,0,0,200,nvgRGB(80,50,30),nvgRGB(80,80,120));
		nvgBeginPath(c);
		nvgFillPaint(c, paint);
		nvgRect(c, bounds.x, bounds.y, bounds.w, bounds.h);
		nvgFill(c);

		Layer::render(c);
	}

	void update(double dt) override {
		Layer::update(dt);
		rotation += dt;
	}
};


int main() {
	GLFWwindow* window = nullptr;
	NVGcontext* nvg = nullptr;

	Scene scene;
	scene.initGraphics(&window, &nvg);

	TestLayer test(nvgRGB(255,128,0));
	scene.addLayer(test);
	test.position = { 200, 200 };
	test.rotation = 0.3f;

	TestLayer test2(nvgRGB(0,128,255));
	test.addLayer(test2);
	test2.position = glm::vec2(50,50);

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