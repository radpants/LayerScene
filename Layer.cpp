//
// Created by Owner on 1/8/2018.
//

#include <algorithm>
#include "Layer.h"
#include "Scene.h"

Layer::Layer() {
	nvgTransformIdentity(transform);
}

void Layer::update(double dt) {
	for( auto& layer : sublayers ) {
		if( layer->isUpdatable )
			layer->update(dt);
	}
}

void Layer::render(NVGcontext *c) {
	for( auto& layer : sublayers ) {
		if( layer->isVisible ) {
			nvgSave(c);
			auto& t = layer->transform;
			nvgTransform(c, t[0], t[1], t[2], t[3], t[4], t[5]);
			layer->render(c);
			nvgRestore(c);
		}
	}
}

bool Layer::overlapsPoint(glm::vec2 point) {
	auto p = sceneToLocal(point);
	return bounds.containsPoint(p);
}

void Layer::addLayer(Layer &layer) {
	if( layer.parent )
		layer.removeFromParent();

	if( layer.scene == nullptr )
		layer.scene = scene;

	layer.parent = this;
	sublayers.push_back(&layer);
}

void Layer::removeLayer(Layer &layer) {
	auto result = std::find(sublayers.begin(), sublayers.end(), &layer);
	if( result != sublayers.end() ) {
		sublayers.erase(result);
	}
}

void Layer::removeFromParent() {
	if( parent )
		parent->removeLayer(*this);
}

bool Layer::operator==(const Layer &other) const {
	return this == &other;
}

glm::vec2 Layer::localToScene(glm::vec2 point) {
	return point;
}

glm::vec2 Layer::sceneToLocal(glm::vec2 point) {
	return point;
}

