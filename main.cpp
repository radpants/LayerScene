#include <GLFW/glfw3.h>
#include <iostream>
#include <tweeny.h>

#include "Scene.h"
#include "Layer.h"


struct Screen : public Layer {

	NVGcolor color1 = nvgRGB(0,0,0);
	NVGcolor color2 = nvgRGB(80,80,80);
	tweeny::tween<float> animIn;

	Screen() {
		isInteractable = true;
		isUpdatable = true;
		animIn = tweeny::from(0.0f).to(1.0f).during(500).via(tweeny::easing::quadraticIn);
	}

	void wasAddedToScene() override {
		animIn.forward();
		animIn.seek(0.0f);
	}

	void update(double dt) override {
		auto ms = static_cast<uint32_t>(dt*1000.0);
		animIn.step(ms);
		bounds = {0,0,(float)scene->windowSize.x,(float)scene->windowSize.y};
	}

	void render(NVGcontext* c) override {
		nvgGlobalAlpha(c, animIn.peek());

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
	}
};

struct RootScreen : public Layer {
	Screen a;
	Screen b;

	void wasAddedToScene() override {
		b.color1 = nvgRGB(20,40,80);
		b.color2 = nvgRGB(40,80,160);

		a.onClick = [&]() {
			addLayer(b);
			removeLayer(a);
		};

		b.onClick = [&]() {
			addLayer(a);
			removeLayer(b);
		};

		addLayer(a);
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