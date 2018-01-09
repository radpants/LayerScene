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
	bool isVisible = true;
	bool isUpdatable = false;
	bool isInteractable = false;
	float transform[6];
	Scene* scene = nullptr;
	Layer* parent = nullptr;
	std::vector<Layer*> sublayers;
	Rect bounds = {0,0,0,0};

	std::function<void(void)> onMouseOver = nullptr;
	std::function<void(void)> onMouseExit = nullptr;
	std::function<void(void)> onClick = nullptr;

	Layer();

	virtual void update(double dt);
	virtual void render(NVGcontext* c);
	virtual bool overlapsPoint(glm::vec2 point);

	void addLayer(Layer& layer);
	void removeLayer(Layer& layer);
	void removeFromParent();

	glm::vec2 localToScene(glm::vec2 point);
	glm::vec2 sceneToLocal(glm::vec2 point);

	bool operator==(const Layer& other) const;
};


#endif //LAYERSCENE_LAYER_H