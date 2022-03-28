#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Scene.h"

class Scene;
class Camera;

class SpearModelScene : public Scene
{
	SpearModelScene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight);
	~SpearModelScene();

	void setupScene();

};

