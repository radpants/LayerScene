//
// Created by Owner on 1/8/2018.
//

#include <algorithm>
#include "Layer.h"
#include "Scene.h"

void Layer::update(double dt) {
	for( auto& layer : sublayers ) {
		if( layer->isUpdatable )
			layer->update(dt);
	}
}

void Layer::render(NVGcontext *c) {
	for( auto& layer : sublayers ) {
		if( layer->isVisible ) {
			layer->updateTransform();
			auto& t = layer->transform;
			nvgSave(c);
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

void Layer::updateTransform() {
	float mat[6];
	auto& t = transform;

	nvgTransformIdentity(t);

	nvgTransformTranslate(mat, position.x, position.y);
	nvgTransformPremultiply(t, mat);

	nvgTransformTranslate(mat, anchor.x, anchor.y);
	nvgTransformPremultiply(t, mat);

	nvgTransformRotate(mat, rotation);
	nvgTransformPremultiply(t, mat);

	nvgTransformTranslate(mat, -anchor.x, -anchor.y);
	nvgTransformPremultiply(t, mat);

	nvgTransformScale(mat, scale.x, scale.y);
	nvgTransformPremultiply(t, mat);
 }

// TODO: This should translate a local point to the global scene (window) coordinates
glm::vec2 Layer::localToScene(glm::vec2 point) {
	glm::vec2 p = point;

	// TODO: There has GOT to be a better way to do this
	std::vector<Layer*> chain;
	Layer* l = this;
	while(l) {
		chain.push_back(l);
		l = l->parent;
	}

	// Build up a transformation matrix from parent -> this sublayer
	float t[6];
	nvgTransformIdentity(t);
	for( int i = chain.size() - 1; i >= 0; i-- ) {
		nvgTransformPremultiply(t, chain[i]->transform);
	}

	nvgTransformPoint(&p[0], &p[1], t, p.x, p.y);

	return p;
}

glm::vec2 Layer::sceneToLocal(glm::vec2 point) {
	glm::vec2 p = point;

	// TODO: There has GOT to be a better way to do this
	std::vector<Layer*> chain;
	Layer* l = this;
	while(l) {
		chain.push_back(l);
		l = l->parent;
	}

	// Build up a transformation matrix from parent -> this sublayer
	float t[6];
	nvgTransformIdentity(t);
	for (auto &i : chain) {
		float inv[6];
		nvgTransformInverse(inv, i->transform);
		nvgTransformPremultiply(t, inv);
	}

	nvgTransformPoint(&p[0], &p[1], t, p.x, p.y);

	return p;
}


