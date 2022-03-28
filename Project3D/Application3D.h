#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Scene.h"

class Instance;



class Application3D : public aie::Application 
{
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_normalMapShader;


	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	aie::OBJMesh m_spearMesh;
	//glm::mat4 m_spearTransform;

	Instance* m_spearInstance;


	Light m_light;
	glm::vec3 m_ambientLight;

	Camera m_camera;

	Scene* m_scene;
};