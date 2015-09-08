#include "base/Main.hpp"

#include "Application.hpp"
#include "Buffer.hpp"
#include "Program.hpp"
#include "Quad.hpp"
#include "Texture.hpp"
#include "VertexArrays.hpp"

#if TMPL_FEATURE_OPENGL_DEBUG
static void APIENTRY debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		// don't care

		return;
	}

	std::string debSource;

	switch (source)
	{

	case GL_DEBUG_SOURCE_API:
		debSource = "OpenGL";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		debSource = "Window";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		debSource = "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		debSource = "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		debSource = "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		debSource = "Other";
		break;
	default:
		break;

	}

	std::string debType;

	switch (type)
	{

	case GL_DEBUG_TYPE_ERROR:
		debType = "error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debType = "deprecated behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debType = "undefined behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		debType = "portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		debType = "performance";
		break;
	case GL_DEBUG_TYPE_OTHER:
		debType = "message";
		break;
	case GL_DEBUG_TYPE_MARKER:
		debType = "marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		debType = "push group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		debType = "pop group";
		break;
	default:
		break;

	}

	std::string debSev;

	switch (severity)
	{

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		debSev = "notification";
		break;
	case GL_DEBUG_SEVERITY_HIGH_ARB:
		debSev = "high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:
		debSev = "medium";
		break;
	case GL_DEBUG_SEVERITY_LOW_ARB:
		debSev = "low";
		break;
	default:
		break;

	}

	TMPL_LOG_TRACE(OpenGL) << debSource << ": " << debType << "(" << debSev << ") " << id << ": " << message;
}
#endif

int main(int argc, const char** argv)
{
	Tmpl::Logger::initialize();

	TMPL_LOG_INFO(Main) << "Starting...";

	glfwSetErrorCallback(Tmpl::glfwErrors);

	TMPL_LOG_INFO(GLFW) << "Initializing.";

	if (glfwInit() == 0)
	{
		TMPL_LOG_ERROR(GLFW) << "Failed to initialize.";

		return 0;
	}

	TMPL_LOG_INFO(GLFW) << "Setting window hints.";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#if TMPL_FEATURE_OPENGL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	TMPL_LOG_INFO(GLFW) << "Creating window.";

	GLFWwindow* window = glfwCreateWindow(
		TMPL_WINDOW_WIDTH, TMPL_WINDOW_HEIGHT,
		TMPL_WINDOW_TITLE,
		nullptr,
		nullptr);

	if (window == nullptr)
	{
		TMPL_LOG_ERROR(GLFW) << "Failed to create window.";

		return 0;
	}

	TMPL_LOG_INFO(GLFW) << "Enable window context.";

	glfwMakeContextCurrent(window);

	TMPL_LOG_INFO(GLEW) << "Initializing.";

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		TMPL_LOG_ERROR(GLEW) << "Failed to initialize.";

		return 0;
	}

#if TMPL_FEATURE_OPENGL_DEBUG && defined(GL_ARB_debug_output)
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(&debugOutput, nullptr);
#endif

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	TMPL_LOG_INFO(Application) << "Initializing.";

	Application* application = new Application(window);

	if (!application->initialize())
	{
		TMPL_LOG_ERROR(Application) << "Failed to initialize.";

		return 0;
	}

	std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

	static std::chrono::microseconds OneSecond = std::chrono::microseconds(1000 * 1000);
	static std::chrono::microseconds TimeStep = OneSecond / 100;
	static std::chrono::microseconds TimeStepMaximum = OneSecond / 20;

	TMPL_LOG_INFO(Application) << "Starting main loop.";

	while (
		application->isRunning() &&
		!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		std::chrono::steady_clock::time_point time_current = std::chrono::steady_clock::now();
		std::chrono::microseconds time_delta = std::chrono::duration_cast<std::chrono::microseconds>(time_current - time_start);
		if (time_delta >= TimeStep)
		{
			time_delta = std::min(time_delta, TimeStepMaximum);

			application->update((uint32_t)(time_delta.count() / 1000));

			time_start = time_current;
		}

		application->render();

		glfwSwapBuffers(window);
	}

	TMPL_LOG_INFO(Application) << "Shutting down.";

	delete application;
	application = nullptr;

	TMPL_LOG_INFO(Main) << "Done.";

	Tmpl::Logger::destroy();

	return 1;
}