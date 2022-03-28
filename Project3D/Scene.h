#pragma once
#include <list>
#include <glm/glm.hpp>
#include <vector>

class Camera;
class Instance;

#define MAX_LIGHTS 4

struct Light
{
	glm::vec3 direction;
	glm::vec3 colour;
	Light();
	Light(glm::vec3 pos, glm::vec3 col, float intensity);
};

enum class Scenes
{
	SpearModelScene,
	BunnyModelScene,
	MultipleLightsScene,
};


class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3
		ambientLight);
	~Scene();

	void addInstance(Instance* instance);

	void draw();

	void loadScene(Scenes scenes);
	
	glm::vec3 getAmbientLight() { return m_ambientLight; }
	Camera* getCamera() { return m_camera; }
	glm::vec2 getWindowSize() { return m_windowSize; }
	Light& getLight() { return m_sunLight; }
	int getNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* getPointlightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointlightColours() { return &m_pointLightColours[0]; }
	std::vector<Light>& getPointLights() { return m_pointLights; }

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_sunLight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];


};

