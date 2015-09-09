#include "Application.hpp"

#include "graphics/Quad.hpp"
#include "text/FreeTypeLoader.hpp"
#include "text/TextBatch.hpp"
#include "voxels/oop/LogicOOP.hpp"

namespace Tmpl {

	Application::Application(GLFWwindow* window)
		: m_window(window)
		, m_loader(new FreeTypeLoader())
		, m_logicOOP(new LogicOOP())
		, m_voxelsActive(1000)
		, m_voxelHalfSize(20.0f)
		, m_targetAngle(0.0f)
		, m_targetDistance(1000.0f)
		, m_cameraAngle(45.0f)
		, m_cameraDistance(5000.0f)
	{
		memset(m_keysPressed, 0, sizeof(m_keysPressed));
	}

	Application::~Application()
	{
	}

	bool Application::initialize()
	{
		if (!m_logicOOP->initialize())
		{
			return false;
		}

		m_logicOOP->generateVoxels(m_voxelsActive, m_voxelHalfSize);

		m_loader->loadFace("media/fonts/Roboto/Roboto-Black.ttf", 12.0f);
		m_loader->loadGlyphRange(0x00, 0xFF); // preload Basic Latin and Latin-1

		m_text = std::shared_ptr<TextBatch>(new TextBatch(m_loader, 256, 256));

		return true;
	}

	void Application::update(uint32_t milliSeconds)
	{
		if (m_keysPressed[GLFW_KEY_A])
		{
			m_cameraAngle += 1.0f;
		}
		if (m_keysPressed[GLFW_KEY_D])
		{
			m_cameraAngle -= 1.0f;
		}
		if (m_keysPressed[GLFW_KEY_W])
		{
			m_cameraDistance -= 10.0f;
		}
		if (m_keysPressed[GLFW_KEY_S])
		{
			m_cameraDistance += 10.0f;
		}

		m_targetPosition = glm::vec3(
			glm::cos(glm::radians(m_targetAngle)) * m_targetDistance,
			10.0f,
			glm::sin(glm::radians(m_targetAngle)) * m_targetDistance);

		m_logicOOP->cullVoxels(m_targetPosition);

		m_targetAngle += 1.0f;
	}

	void Application::render()
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		glViewport(
			0, 0,
			(GLsizei)width, (GLsizei)height);

		float depth = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &depth);

		glm::vec4 clear_color(0.0f, 0.0f, 0.0f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clear_color));

		/*glm::vec3 eyePosition(
			glm::cos(glm::radians(m_cameraAngle)) * m_cameraDistance,
			0.0f,
			glm::sin(glm::radians(m_cameraAngle)) * m_cameraDistance);*/

		glm::vec3 eyePosition = m_targetPosition;

		glm::mat4x4 perspective = glm::perspectiveFov(
			120.0f,
			(float)width, (float)height,
			0.1f, 10000.0f);

		glm::mat4x4 viewCamera = glm::lookAt(
			eyePosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		m_logicOOP->render(perspective *  viewCamera);

		glm::mat4x4 projection = glm::ortho(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f);

		char cameraText[256] = { 0 };
		sprintf_s(cameraText, "Eye: (%.2f, %.2f, %.2f)", eyePosition.x, eyePosition.y, eyePosition.z);
		m_text->setText(cameraText);

		m_text->render(projection, glm::vec2(5.0f, 5.0f));
	}

	void Application::onKeyPressed(int key, int modifierKeys)
	{
		m_keysPressed[key] = true;
	}

	void Application::onKeyReleased(int key, int modifierKeys)
	{
		m_keysPressed[key] = false;
	}

}; // namespace Tmpl