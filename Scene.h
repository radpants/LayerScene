//
// Created by Owner on 1/8/2018.
//

#ifndef LAYERSCENE_SCENE_H
#define LAYERSCENE_SCENE_H

#include "Layer.h"

#include <vector>
#include <nanovg.h>
#include <glm/mat3x3.hpp>
#include <GLFW/glfw3.h>

enum ButtonState : uint8_t {
	ButtonStateUp = 0,
	ButtonStateDown,
	ButtonStatePressed,
	ButtonStateReleased
};

struct Scene : public Layer {
	Layer* mouseTarget;
	Layer* mouseDownTarget;
	glm::f64vec2 mousePosition;
	glm::f64vec2 previousMousePosition;
	glm::f64vec2 mouseScroll;
	std::vector<ButtonState> mouseButtons;
	std::vector<ButtonState> keys;
	glm::ivec2 windowSize;
	glm::ivec2 framebufferSize;

	static Scene* instance;

	Scene();

	void initGraphics(GLFWwindow** window, NVGcontext** c);
	void processEvents(GLFWwindow* window);
	void update(double dt);
	void render(GLFWwindow* window, NVGcontext* c);

	void addLayer(Layer& layer);
};


#endif //LAYERSCENE_SCENE_H
