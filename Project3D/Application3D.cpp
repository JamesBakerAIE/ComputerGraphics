#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <imgui.h>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);
	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);
	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);


	// Lighting colors
	m_light.colour = { 1, 1, 0 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (m_shader.link() == false) {
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	if (m_phongShader.link() == false) {
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/Bunny.obj") == false) {
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform = {
	0.5f,0,0,0,
	0,0.5f,0,0,
	0,0,0.5f,0,
	0,0,0,1
	};

	return true;
}

void Application3D::shutdown() {

	//Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{

	// query time since application started
	float time = getTime();
	// rotate light
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2),
		glm::sin(time * 2), 0));

	//// rotate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
	//						   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_camera.update(deltaTime);


	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f,
		1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f,
		2.0f);
	ImGui::End();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_camera.getViewMatrix();
	// bind shader
	m_shader.bind();
	// bind transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	//// bind shader
	//m_shader.bind();
	//
	//// bind transform
	//auto pvm = m_projectionMatrix * m_viewMatrix * m_bunnyTransform;
	//m_shader.bindUniform("ProjectionViewModel", pvm);

	//
	//// draw mesh
	//m_bunnyMesh.draw();


	// bind phong shader program
	m_phongShader.bind();
	
	// bind light
	m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	m_phongShader.bindUniform("LightColour", m_light.colour);
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	
	//// bind transform
	//glm::mat4 projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(),
	//	(float)getWindowHeight());
	//glm::mat4 viewMatrix = m_camera.getViewMatrix();
	pvm = m_projectionMatrix * m_viewMatrix * m_bunnyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	
	// bind transforms for lighting
	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
	
	// draw bunny
	m_bunnyMesh.draw();
	
	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	
	// draw 2D gizmos using an orthogonal projection matrix
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}