#include <GLFW/glfw3.h>
#include <iostream>


#include "Scene.h"
#include "Layer.h"


struct BoxData {
	NVGcolor color = nvgRGB(255,0,0);
};

struct Box {
	Layer layer;
	NVGcolor color;

	Box() {
		color = nvgRGB(255,128,0);
		layer.onDraw = [&](Layer* l, NVGcontext* c) {
			nvgBeginPath(c);
			nvgFillColor(c, color);
			nvgRect(c, 0, 0, 200, 200);
			nvgFill(c);
		};

		layer.onUpdate = [](Layer* l, double dt) {
			l->rotation += dt;
		};
	}
};

int main() {
	GLFWwindow* window = nullptr;
	NVGcontext* nvg = nullptr;

	Scene scene;
	scene.initGraphics(&window, &nvg);

	Box box;
	scene.addLayer(box.layer);

	double frameTime = 0.0f;
	double startTime = glfwGetTime();
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