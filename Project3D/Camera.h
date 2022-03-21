#pragma once
#include "glm/glm.hpp"
class Camera
{
public:

	Camera();
	~Camera();
	void update(float deltaTime);

	glm::mat4 getViewMatrix();


	glm::vec3 getPosition()
	{
		return m_position;
	}

	glm::mat4 getProjectionMatrix(float w, float h);

protected:

	float m_lastMouseX;
	float m_lastMouseY;

	float m_theta;
	float m_phi;
	glm::vec3 m_position;

};

