#pragma once
#include <glm/glm.hpp>
#include "Scene.h"

class Camera;
struct Light;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}



class Instance
{
public:
	Instance(glm::mat4 transform, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	Instance(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	~Instance();
	void draw(Scene* scene);
	glm::mat4 Instance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;

	glm::vec3 m_position;
	glm::vec3 m_eulerAngles;
	glm::vec3 m_scale;
};

