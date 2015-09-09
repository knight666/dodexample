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
		, m_cameraAngle(0.0f)
	{
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
		m_logicOOP->update(milliSeconds);
	}

	void Application::render()
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		glViewport(
			0, 0,
			(GLsizei)width, (GLsizei)height);

		glClearBufferfv(
			GL_COLOR,
			0,
			glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));

		glm::vec3 eyePosition(
			glm::cos(glm::radians(m_cameraAngle)) * 1000.0f,
			0.0f,
			glm::sin(glm::radians(m_cameraAngle)) * 1000.0f);

		glm::mat4x4 perspective = glm::perspectiveFov(
			90.0f,
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
		switch (key)
		{

		case GLFW_KEY_A:
			m_cameraAngle -= 1.0f;
			break;

		case GLFW_KEY_D:
			m_cameraAngle += 1.0f;
			break;

		default:
			break;

		}
	}

	void Application::onKeyReleased(int key, int modifierKeys)
	{
		switch (key)
		{

		case GLFW_KEY_LEFT:
			m_camera.x -= 1.0f;
			break;

		case GLFW_KEY_RIGHT:
			m_camera.x += 1.0f;
			break;

		case GLFW_KEY_UP:
			m_camera.y -= 1.0f;
			break;

		case GLFW_KEY_DOWN:
			m_camera.y += 1.0f;
			break;

		}
	}

}; // namespace Tmpl