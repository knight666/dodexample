#include "Application.hpp"

#include "graphics/Quad.hpp"
#include "text/FreeTypeLoader.hpp"
#include "text/TextBatch.hpp"

namespace Tmpl {

	Application::Application(GLFWwindow* window)
		: _window(window)
		, _loader(new FreeTypeLoader())
	{
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

		glm::mat4x4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4x4 camera = projection * view;

		_text->render(camera);
	}

}; // namespace Tmpl