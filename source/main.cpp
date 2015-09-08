#include "Base.hpp"

#include "Application.hpp"
#include "Buffer.hpp"
#include "Program.hpp"
#include "Quad.hpp"
#include "Texture.hpp"
#include "VertexArrays.hpp"

void traceMessage(const char* message)
{
#if defined(WIN32) || defined(_WINDOWS)
	::OutputDebugStringA(message);
#endif

	fprintf(stdout, message);
}

static void glfwErrors(int errorCode, const char* description)
{
	char buffer[1024] = { 0 };
	_snprintf_s(buffer, 1023, "%x: %s\n", errorCode, description);

	traceMessage(buffer);
}

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

	char buffer[1024] = { 0 };
	_snprintf_s(buffer, 1023, "%s: %s(%s) %d: %s\n", debSource.c_str(), debType.c_str(), debSev.c_str(), id, message);

	traceMessage(message);
}
#endif

int main(int argc, const char** argv)
{
	glfwSetErrorCallback(glfwErrors);

	if (glfwInit() == 0)
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;

		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#if TMPL_FEATURE_OPENGL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(
		TMPL_WINDOW_WIDTH, TMPL_WINDOW_HEIGHT,
		TMPL_WINDOW_TITLE,
		nullptr,
		nullptr);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GFLW window." << std::endl;

		return false;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;

		return false;
	}

#if TMPL_FEATURE_OPENGL_DEBUG && defined(GL_ARB_debug_output)
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(&debugOutput, nullptr);
#endif

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Application application(window);

	if (!application.initialize())
	{
		return 0;
	}

	std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

	static std::chrono::microseconds OneSecond = std::chrono::microseconds(1000 * 1000);
	static std::chrono::microseconds TimeStep = OneSecond / 100;
	static std::chrono::microseconds TimeStepMaximum = OneSecond / 20;

	while (
		application.isRunning() &&
		!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		std::chrono::steady_clock::time_point time_current = std::chrono::steady_clock::now();
		std::chrono::microseconds time_delta = std::chrono::duration_cast<std::chrono::microseconds>(time_current - time_start);
		if (time_delta >= TimeStep)
		{
			time_delta = std::min(time_delta, TimeStepMaximum);

			application.update((uint32_t)(time_delta.count() / 1000));

			time_start = time_current;
		}

		application.render();

		glfwSwapBuffers(window);
	}

	return 1;
}