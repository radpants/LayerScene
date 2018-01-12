//
// Created by Owner on 1/8/2018.
//

#include <glad/glad.h>
#include "Scene.h"


#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

Scene* Scene::instance = nullptr;


Scene::Scene() : Layer() {
	instance = this;

	updateTransform();

	mouseTarget = nullptr;
	mouseDownTarget = nullptr;

	mouseButtons.resize(GLFW_MOUSE_BUTTON_LAST+1);
	for( auto i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++ ) {
		mouseButtons[i] = ButtonStateUp;
	}

	keys.resize(GLFW_KEY_LAST+1);
	for( auto i = 0; i <= GLFW_KEY_LAST; i++ ) {
		keys[i] = ButtonStateUp;
	}
}

static void glfwScrollCallback(GLFWwindow* window, double x, double y) {
	auto& scene = Scene::instance;
	scene->mouseScroll.x = x;
	scene->mouseScroll.y = y;
}

void Scene::initGraphics(GLFWwindow** window, NVGcontext** c) {
	glfwSetErrorCallback([](int code, const char* description) {
		fprintf(stderr, "[GLFW Error %d]: %s\n", code, description);
	});

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(1280, 720, "LayerScene", nullptr, nullptr);
	glfwMakeContextCurrent(*window);

	glfwSetScrollCallback(*window, glfwScrollCallback);

	glfwSetInputMode(*window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	glfwSwapInterval(1);
	glfwSetWindowAspectRatio(*window, 16, 9);

	if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		fprintf(stderr, "[GLAD Error]: Could not initialize\n");
		exit(EXIT_FAILURE);
	}

	glfwGetWindowSize(*window, &windowSize[0], &windowSize[1]);
	glfwGetFramebufferSize(*window, &framebufferSize[0], &framebufferSize[1]);

	*c = nvgCreateGL3(NVG_ANTIALIAS);
}

void Scene::processEvents(GLFWwindow *window) {
	glfwPollEvents();
	previousMousePosition = mousePosition;
	glfwGetCursorPos(window, &mousePosition[0], &mousePosition[1]);

	for( int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_LAST; i++ ) {
		auto state = glfwGetMouseButton(window, i);
		if( mouseButtons[i] == ButtonStateUp && state == GLFW_PRESS ) {
			mouseButtons[i] = ButtonStatePressed;
		}
		else if( mouseButtons[i] == ButtonStateDown && state == GLFW_RELEASE ) {
			mouseButtons[i] = ButtonStateReleased;
		}
		else
			mouseButtons[i] = (state == GLFW_PRESS) ? ButtonStateDown : ButtonStateUp;
	}

	// NOTE: Keyboard key-codes are not contigous, so this will assign a lot of non-existent keys to 'UP'
	for( int i = 0; i <= GLFW_KEY_LAST; i++ ) {
		switch(keys[i]) {
			case static_cast<ButtonState>(GLFW_PRESS): { keys[i] = ButtonStatePressed; break; }
			case static_cast<ButtonState>(GLFW_RELEASE): { keys[i] = ButtonStateReleased; break; }
			case ButtonStatePressed: { keys[i] = ButtonStateDown; break; }
			case ButtonStateReleased: { keys[i] = ButtonStateUp; break; }
			default: {}
		}
	}

}

void Scene::update(double dt) {
	mouseTarget = nullptr;
	Layer::updateSublayers(dt);

	// Because we don't get an update if there is no scroll but we need to stop scrolling
	mouseScroll.x = 0;
	mouseScroll.y = 0;

	for( auto& layer : layersToRemove ) {
		layer->willExitScene();
		layer->parent = nullptr;
		layer->scene = nullptr;
	}
	layersToRemove.clear();
}

void Scene::render(GLFWwindow* window, NVGcontext *c) {
	glfwGetWindowSize(window, &windowSize[0], &windowSize[1]);
	glfwGetFramebufferSize(window, &framebufferSize[0], &framebufferSize[1]);

	auto pixelRatio = framebufferSize[0] / windowSize[0];

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, framebufferSize[0], framebufferSize[1]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	nvgBeginFrame(c, windowSize[0], windowSize[1], pixelRatio);
	Layer::renderSublayers(c);
	nvgEndFrame(c);
}

void Scene::addLayer(Layer &layer) {
	Layer::addLayer(layer);
	layer.setScene(this);
}




