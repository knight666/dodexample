#include "Application.hpp"

#include "FreeTypeLoader.hpp"
#include "Quad.hpp"
#include "TextBatch.hpp"

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
	_loader->loadFace("media/fonts/Roboto/Roboto-Black.ttf", 28.0f);

	_text = std::shared_ptr<TextBatch>(new TextBatch(_loader, 256, 256));
	_text->setText("Hello world! \xE3\x8A\x98\xE3\x85\xBB\xE2\x92\x94\xE2\x85\xA6\xE2\x84\x80\xE2\x81\xB4");

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