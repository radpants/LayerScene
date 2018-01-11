#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"


struct Screen : public Layer {

	NVGcolor color1 = nvgRGB(0,0,0);
	NVGcolor color2 = nvgRGB(80,80,80);

	Screen() {
		isInteractable = true;
	}

	void render(NVGcontext* c) override {
		if( scene ) {
			bounds = {0,0,(float)scene->windowSize.x,(float)scene->windowSize.y};
		}
		auto paint = nvgLinearGradient(c, 0, 0, bounds.w, bounds.h, color1, color2);
		nvgBeginPath(c);
		nvgFillPaint(c, paint);
		nvgRect(c,0,0,bounds.w,bounds.h);
		nvgFill(c);

		auto color = (scene->mouseDownTarget == this) ? nvgRGB(0,255,0) : nvgRGB(255,0,0);
		nvgBeginPath(c);
		nvgFillColor(c, color);
		nvgRect(c,20,20,20,20);
		nvgFill(c);
		Layer::render(c);
	}
};

struct RootScreen : public Screen {
	Screen a;
	Screen b;

	void wasAddedToScene() override {
		b.color1 = nvgRGB(20,40,80);
		b.color2 = nvgRGB(40,80,160);

		a.onClick = [&]() {
			removeLayer(a);
			addLayer(b);
		};

		b.onClick = [&]() {
			removeLayer(b);
			addLayer(a);
		};

		addLayer(a);
	}

	void render(NVGcontext* c) override {
		Layer::render(c);
	}

};

int main() {
	GLFWwindow* window = nullptr;
	NVGcontext* nvg = nullptr;

	Scene scene;
	scene.initGraphics(&window, &nvg);

	RootScreen rootScreen;
	scene.addLayer(rootScreen);

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