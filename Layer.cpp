//
// Created by Owner on 1/8/2018.
//

#include <algorithm>
#include "Layer.h"
#include "Scene.h"


void Layer::update(double dt) {
	for( auto& layer : sublayers ) {
		if( layer.onUpdate != nullptr )
			layer.onUpdate(&layer,dt);
	}
}

void Layer::render(NVGcontext *c) {
	for( auto& layer : sublayers ) {
		if( layer.onDraw != nullptr ) {
			nvgSave(c);
			nvgTranslate(c, position.x, position.y);
			nvgRotate(c, rotation);
			nvgScale(c, scale.x, scale.y);
			layer.onDraw(&layer,c);
			layer.render(c);
			nvgRestore(c);
		}
	}
}

// TODO: This should use the lambda provided by doesOverlap
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
	sublayers.push_back(layer);
}

void Layer::removeLayer(Layer &layer) {
	auto result = std::find(sublayers.begin(), sublayers.end(), layer);
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

// TODO: This should translate a local point to the global scene (window) coordinates
glm::vec2 Layer::localToScene(glm::vec2 point) {
	return point;
}

// TODO: This should translate from global (window) coordinates to local coordinates
glm::vec2 Layer::sceneToLocal(glm::vec2 point) {
	return point;
}

