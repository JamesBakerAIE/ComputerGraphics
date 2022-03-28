#include "SpearModelScene.h"
#include "Camera.h"

SpearModelScene::SpearModelScene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight) 
	: Scene(camera, windowSize, light, ambientLight)
{

}

SpearModelScene::~SpearModelScene()
{

}

void SpearModelScene::setupScene()
{
}
