#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include "glm/vec3.hpp"
#include <imgui.h>
#include "Instance.h"

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
	m_light.direction = { -1.0, -1.0, -1.0 };

	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalmap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalmap.frag");
	if (m_normalMapShader.link() == false) {
		printf("Shader Error: %s\n", m_normalMapShader.getLastError());
		return false;
	}

	if (m_spearMesh.load("./soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	// Create scene
	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(),
		getWindowHeight()), m_light, m_ambientLight);

	// red light on the left
	m_scene->getPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 1, 0), 50));
	// green light on the right
	m_scene->getPointLights().push_back(Light(vec3(-5, 3, 0), vec3(1, 1, 0), 50));

	// Create 10 spears
	for (int i = 0; i < 10; i++)
	{
		m_scene->addInstance(new Instance(glm::vec3(i, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &m_spearMesh,
			&m_normalMapShader));
	}

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void Application3D::update(float deltaTime) 
{
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
	ImGui::DragFloat3("Sunlight Direction", &m_scene->getLight().direction[0], 0.1f, -1.0f,
		1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->getLight().colour[0], 0.1f, 0.0f,
		2.0f);
	ImGui::End();

	ImGui::Begin("Scene Settings");
	if (ImGui::Button("Spear Model - Normal Maps & Specular Highlights", ImVec2(400, 50)))
	{

	}
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

	m_scene->draw();
	
	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	
	// draw 2D gizmos using an orthogonal projection matrix
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}