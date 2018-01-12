//
// Created by Owner on 1/8/2018.
//

#include <algorithm>
#include "Layer.h"
#include "Scene.h"

void Layer::updateSublayers(double dt) {
	for( int i = sublayers.size()-1; i >= 0; i-- ) {
		if( sublayers[i]->scene && (sublayers[i]->isUpdatable || sublayers[i]->isInteractable) )
			sublayers[i]->update(dt);
			sublayers[i]->updateSublayers(dt);
	}

	if( isInteractable )
		processInteraction();
}



void Layer::processInteraction() {
	if( scene == nullptr )
		return;

	if( scene->mouseTarget == nullptr || scene->mouseTarget == this ) {
		auto isOver = overlapsPoint(sceneToLocal(scene->mousePosition));
		auto& buttonState = scene->mouseButtons[GLFW_MOUSE_BUTTON_LEFT];

		if( isOver ) {

			if( scene->mouseTarget == nullptr || scene->mouseTarget == this ) {
				scene->mouseTarget = this;
			}

			// are we starting a click?
			if( scene->mouseDownTarget == nullptr ) {
				if( buttonState == ButtonStatePressed ) {
					scene->mouseDownTarget = this;
				}
			}
				// are we ending a click?
			else if( buttonState == ButtonStateReleased || buttonState == ButtonStateUp ) {
				if( scene->mouseDownTarget == this ) {
					if( onClick ) onClick();
					scene->mouseDownTarget = nullptr;
				}
			}
		}
		else {
			if( scene->mouseTarget == this ) {
				scene->mouseTarget = nullptr;
			}

			if( buttonState == ButtonStateReleased ) {
				if( scene->mouseDownTarget == this ) {
					scene->mouseDownTarget = nullptr;
				}
			}
		}
	}
}

void Layer::renderSublayers(NVGcontext* c) {
	for( auto& layer : sublayers ) {
		if( layer->isVisible && layer->scene ) {
			layer->updateTransform();
			auto& t = layer->transform;
			nvgSave(c);
			nvgTransform(c, t[0], t[1], t[2], t[3], t[4], t[5]);
			layer->render(c);
			layer->renderSublayers(c);
			nvgRestore(c);
		}
	}
}

bool Layer::overlapsPoint(glm::vec2 point) {
	return bounds.containsPoint(point);
}

void Layer::addLayer(Layer &layer) {
	if( layer.parent )
		layer.removeFromParent();

	if( scene != nullptr )
		layer.setScene(scene);

	layer.parent = this;
	sublayers.push_back(&layer);
}

void Layer::removeLayer(Layer &layer) {
	auto result = std::find(sublayers.begin(), sublayers.end(), &layer);
	if( result != sublayers.end() ) {
		layer.scene->layersToRemove.push_back(&layer);
		sublayers.erase(result);
	}
}

void Layer::removeAllLayers() {
	sublayers.clear();
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

void Layer::setScene(Scene *scene) {
	this->scene = scene;
	for( auto& layer : sublayers ) { layer->setScene(scene); }
	wasAddedToScene();
}




