//
// Created by Owner on 1/8/2018.
//

#ifndef LAYERSCENE_LAYER_H
#define LAYERSCENE_LAYER_H

#include <vector>
#include <nanovg.h>
#include <glm/mat3x3.hpp>
#include <functional>

struct Scene;

struct Rect {
	float x, y, w, h;

	bool containsPoint(glm::vec2 p) {
		return p.x > x && p.x < x + w && p.y > y && p.y < y + h;
	}
};

struct Layer {
	bool isInteractable = false;
	glm::vec2 position;
	glm::vec2 scale = glm::vec2(1,1);
	float rotation = 0.0f;
	Scene* scene = nullptr;
	Layer* parent = nullptr;
	std::vector<Layer> sublayers;
	Rect bounds = {0,0,0,0};
	void* data = nullptr;

	std::function<void(void)> onMouseOver = nullptr;
	std::function<void(void)> onMouseExit = nullptr;
	std::function<void(void)> onClick = nullptr;

	std::function<void(Layer*,double)> onUpdate = nullptr;
	std::function<void(Layer*,NVGcontext*)> onDraw = nullptr;
	std::function<bool(Layer*,glm::f64vec2)> doesOverlap = nullptr;

	Layer() = default;
	virtual ~Layer() = default;

	void update(double dt);
	void render(NVGcontext* c);
	bool overlapsPoint(glm::vec2 point);

	void addLayer(Layer& layer);
	void removeLayer(Layer& layer);
	void removeFromParent();

	glm::vec2 localToScene(glm::vec2 point);
	glm::vec2 sceneToLocal(glm::vec2 point);

	bool operator==(const Layer& other) const;
};


#endif //LAYERSCENE_LAYER_H
