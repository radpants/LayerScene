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
	nvgTransformRotate(test.transform, 0.1f);

	TestLayer test2(nvgRGB(0,128,255));
	test.addLayer(test2);
	nvgTransformTranslate(test2.transform, 20, 20);


	while( !glfwWindowShouldClose(window) ) {
		scene.processEvents(window);
		scene.update(0);
		scene.render(window, nvg);

		glfwSwapBuffers(window);
	}

	return 0;
}