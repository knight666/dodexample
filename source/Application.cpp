#include "Application.hpp"

#include "graphics/Quad.hpp"
#include "text/FreeTypeLoader.hpp"
#include "text/TextBatch.hpp"

namespace Tmpl {

	Application::Application(GLFWwindow* window)
		: _window(window)
		, _loader(new FreeTypeLoader())
	{
		m_camera.z = 1.0f;
	}

	Application::~Application()
	{
	}

	bool Application::initialize()
	{
		_loader->loadFace("media/fonts/Roboto/Roboto-Black.ttf", 12.0f);

		_text = std::shared_ptr<TextBatch>(new TextBatch(_loader, 256, 256));
		_text->setText("Update: 00 ms\nRender: 00 ms");

		return true;
	}

	void Application::update(uint32_t milliSeconds)
	{

	}

	void Application::render()
	{
		int width, height;
		glfwGetWindowSize(_window, &width, &height);

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);

		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glm::mat4x4 projection = glm::ortho(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), m_camera);

		_text->render(projection * view);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void Application::onKeyPressed(int key, int modifierKeys)
	{

	}

	void Application::onKeyReleased(int key, int modifierKeys)
	{
		switch (key)
		{

		case GLFW_KEY_A:
			m_camera.x -= 1.0f;
			break;

		case GLFW_KEY_D:
			m_camera.x += 1.0f;
			break;

		case GLFW_KEY_W:
			m_camera.y += 1.0f;
			break;

		case GLFW_KEY_S:
			m_camera.y -= 1.0f;
			break;

		case GLFW_KEY_Q:
			m_camera.z -= 1.0f;
			break;

		case GLFW_KEY_E:
			m_camera.z += 1.0f;
			break;

		}
	}

}; // namespace Tmpl