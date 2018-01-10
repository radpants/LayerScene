#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"


struct TestLayer : public Layer {
	NVGcolor color;

	TestLayer(NVGcolor _color) : Layer(), color(_color) {
		bounds = {0,0,200,200};
	}

	void render(NVGcontext* c) override {
		nvgBeginPath(c);
		nvgFillColor(c, color);
		nvgRect(c, bounds.x, bounds.y, bounds.w, bounds.h);
		nvgFill(c);

		Layer::render(c);
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

	while( !glfwWindowShouldClose(window) ) {
		scene.processEvents(window);
		scene.update(0);
		scene.render(window, nvg);

		glfwSwapBuffers(window);
	}

	return 0;
}