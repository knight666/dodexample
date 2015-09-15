#include "Application.hpp"

#include "graphics/Quad.hpp"
#include "graphics/Sphere.hpp"
#include "text/FreeTypeLoader.hpp"
#include "text/TextBatch.hpp"
#include "voxels/dod/LogicDOD.hpp"
#include "voxels/oop/LogicOOP.hpp"
#include "voxels/Renderer.hpp"

namespace Tmpl {

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

	static void windowKeyHandler(GLFWwindow* window, int key, int scanCode, int action, int modifierKeys)
	{
		Tmpl::Application* app = reinterpret_cast<Application*>(
			glfwGetWindowUserPointer(window));

		if (app == nullptr)
		{
			return;
		}

		if (action == GLFW_PRESS)
		{
			app->onKeyPressed(key, modifierKeys);
		}
		else if (
			action == GLFW_RELEASE)
		{
			app->onKeyReleased(key, modifierKeys);
		}
	}

	Application::Application()
		: m_window(nullptr)
		, m_loader(new FreeTypeLoader())
		, m_logicOOP(nullptr)
		, m_voxelsCulled(0)
		, m_voxelsActive(0)
		, m_voxelHalfSize(20.0f)
		, m_targetAngle(0.0f)
		, m_targetDistance(1000.0f)
		, m_targetSphere(nullptr)
		, m_cameraAngle(45.0f)
		, m_cameraDistance(5000.0f)
		, m_renderer(new Renderer())
	{
		memset(m_keysPressed, 0, sizeof(m_keysPressed));
	}

	Application::~Application()
	{
	}

	int Application::run(int argc, const char** argv)
	{
		glfwSetErrorCallback(Tmpl::glfwErrors);

		TMPL_LOG_INFO(GLFW) << "Initializing.";

		if (glfwInit() == 0)
		{
			TMPL_LOG_ERROR(GLFW) << "Failed to initialize.";

			return 1;
		}

		TMPL_LOG_INFO(GLFW) << "Setting window hints.";

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#if TMPL_FEATURE_OPENGL_DEBUG
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

		TMPL_LOG_INFO(GLFW) << "Creating window.";

		m_window = glfwCreateWindow(
			TMPL_WINDOW_WIDTH, TMPL_WINDOW_HEIGHT,
			TMPL_WINDOW_TITLE,
			nullptr,
			nullptr);

		if (m_window == nullptr)
		{
			TMPL_LOG_ERROR(GLFW) << "Failed to create window.";

			return 1;
		}

		glfwSetWindowUserPointer(m_window, this);
		glfwSetKeyCallback(m_window, windowKeyHandler);

		TMPL_LOG_INFO(GLFW) << "Enable window context.";

		glfwMakeContextCurrent(m_window);

		TMPL_LOG_INFO(GLEW) << "Initializing.";

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			TMPL_LOG_ERROR(GLEW) << "Failed to initialize.";

			return 1;
		}

	#if TMPL_FEATURE_OPENGL_DEBUG && defined(GL_ARB_debug_output)
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageCallback(&debugOutput, nullptr);
	#endif

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		TMPL_LOG_INFO(Application) << "Initializing.";

		// Text

		m_loader->loadFace("media/fonts/Roboto/Roboto-Black.ttf", 12.0f);
		m_loader->loadGlyphRange(0x00, 0xFF); // preload Basic Latin and Latin-1

		m_text = std::shared_ptr<TextBatch>(new TextBatch(m_loader, 512, 512));

		// Models

		m_targetSphere = std::make_shared<Sphere>();
		m_targetSphere->setup(20, 20);

		// Logic

		m_logicOOP = std::make_shared<LogicOOP>(m_voxelHalfSize);
		m_logicDOD = std::make_shared<LogicDOD>(m_voxelHalfSize);

		m_logic = m_logicOOP;

		generateScene(1.0f);

		// Renderer

		if (!m_renderer->initialize())
		{
			return -1;
		}

		// Main loop

		TMPL_LOG_INFO(Application) << "Entering main loop.";

		typedef std::chrono::steady_clock clock;
		typedef std::chrono::microseconds us;

		clock::time_point time_start = clock::now();
		clock::time_point time_current;
		clock::time_point time_render_start;

		while (
			!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			time_current = clock::now();
			us time_delta = std::chrono::duration_cast<us>(
				time_current - time_start);

			update((uint32_t)(time_delta.count() / 1000));

			time_render_start = clock::now();
			m_timeUpdate = std::chrono::duration_cast<us>(
				time_render_start - time_current);

			render();

			m_timeRender = std::chrono::duration_cast<us>(
				clock::now() - time_render_start);

			renderInterface();

			glfwSwapBuffers(m_window);

			time_start = time_current;
		}

		TMPL_LOG_INFO(Application) << "Shutting down.";

		return 0;
	}

	void Application::update(uint32_t milliSeconds)
	{
		static const float TimeStep = 1000.0f / 30.0f;
		static const float TimeStepMaximum = TimeStep * 10.0f;

		float delta = glm::min((float)milliSeconds, TimeStepMaximum) / TimeStep;

		if (m_options.camera == Options::CameraType::User)
		{
			if (m_keysPressed[GLFW_KEY_A])
			{
				m_cameraAngle += 1.0f * delta;
			}
			if (m_keysPressed[GLFW_KEY_D])
			{
				m_cameraAngle -= 1.0f * delta;
			}
			if (m_keysPressed[GLFW_KEY_W])
			{
				m_cameraDistance -= 10.0f * delta;
			}
			if (m_keysPressed[GLFW_KEY_S])
			{
				m_cameraDistance += 10.0f * delta;
			}
		}

		m_targetPosition = glm::vec3(
			glm::cos(glm::radians(m_targetAngle)) * m_targetDistance,
			10.0f,
			glm::sin(glm::radians(m_targetAngle)) * m_targetDistance);

		m_voxelsCulled = 0;

		if (m_options.culling)
		{
			m_voxelsCulled = m_logic->cullVoxels(m_options, m_targetPosition);
		}

		m_targetAngle += 1.0f * delta;
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

		if (m_options.camera == Options::CameraType::User)
		{
			m_eyePosition = glm::vec3(
				glm::cos(glm::radians(m_cameraAngle)) * m_cameraDistance,
				0.0f,
				glm::sin(glm::radians(m_cameraAngle)) * m_cameraDistance);
		}
		else if (
			m_options.camera == Options::CameraType::Target)
		{
			m_eyePosition = m_targetPosition;
		}

		glm::mat4x4 perspective = glm::perspectiveFov(
			120.0f,
			(float)width, (float)height,
			0.1f, 10000.0f);

		glm::mat4x4 viewCamera = glm::lookAt(
			m_eyePosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4x4 viewProjection = perspective * viewCamera;

		m_renderer->render(
			m_logic,
			m_options,
			viewProjection);

		if (m_options.camera != Options::CameraType::Target)
		{
			m_targetSphere->render(
				viewProjection,
				m_targetPosition,
				15.0f,
				glm::vec3(1.0f, 1.0f, 0.0f));
		}
	}

	void Application::renderInterface()
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		glViewport(
			0, 0,
			(GLsizei)width, (GLsizei)height);

		glm::mat4x4 interface_projection = glm::ortho(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f);

		m_textCombined.clear();

		if (m_options.logic == Options::LogicType::ObjectOriented)
		{
			addText("Object-oriented logic");
		}
		else if (
			m_options.logic == Options::LogicType::DataOriented)
		{
			addText("Data-oriented logic");
		}

		addText("Update: %d ms",
			(uint32_t)(m_timeUpdate.count() / 1000));
		addText("Render: %d ms",
			(uint32_t)(m_timeRender.count() / 1000));

		addText("Voxels: %d",
			m_voxelsActive);
		addText("Culled: %d (%.2f%%)",
			m_voxelsCulled,
			((float)m_voxelsCulled / (float)m_voxelsActive) * 100.0f);
		addText("Eye: (%.2f, %.2f, %.2f)",
			m_eyePosition.x, m_eyePosition.y, m_eyePosition.z);

		renderHelp();

		m_text->setText(m_textCombined.c_str());
		m_text->render(interface_projection, glm::vec2(5.0f, 5.0f));
	}

	void Application::onKeyPressed(int key, int modifierKeys)
	{
		m_keysPressed[key] = true;
	}

	void Application::onKeyReleased(int key, int modifierKeys)
	{
		m_keysPressed[key] = false;

		switch (key)
		{

		case GLFW_KEY_1:
			generateScene(1.0f);
			break;

		case GLFW_KEY_2:
			generateScene(1.4f);
			break;

		case GLFW_KEY_3:
			generateScene(1.8f);
			break;

		case GLFW_KEY_C:
			m_options.culling = !m_options.culling;
			break;

		case GLFW_KEY_F:
			if (m_options.culling)
			{
				m_options.showCulled = !m_options.showCulled;
			}
			break;

		case GLFW_KEY_R:
			if (m_options.logic == Options::LogicType::ObjectOriented)
			{
				m_logic = m_logicDOD;

				m_options.logic = Options::LogicType::DataOriented;
			}
			else
			{
				m_logic = m_logicOOP;

				m_options.logic = Options::LogicType::ObjectOriented;
			}
			break;

		case GLFW_KEY_G:
			if (m_options.camera == Options::CameraType::Target)
			{
				m_options.camera = Options::CameraType::User;
			}
			else
			{
				m_options.camera = Options::CameraType::Target;
			}
			break;

		case GLFW_KEY_H:
			m_options.help = !m_options.help;
			break;

		default:
			break;

		}
	}

	void Application::generateScene(float scale /*= 1.0f*/)
	{
		TMPL_LOG_INFO(Application) << "Generating scene with a scale factor of " << scale << "...";

		std::vector<Logic::VoxelData> voxels;
		glm::vec3 position;
		size_t side = 100;

		struct SphereData
		{
			SphereData(const glm::vec3& position, float radius)
				: position(position)
				, radius(radius)
			{
			}

			glm::vec3 position;
			float radius;
		};

		std::vector<SphereData> spheres;
		spheres.push_back(SphereData(glm::vec3(0.0f, 0.0f, 0.0f), 250.0f * scale));

		float tilt = 25.0f;
		float distance = 550.0f * scale;

		for (size_t i = 0; i < 8; ++i)
		{
			glm::mat4x4 transform(1.0f);
			transform = glm::translate(
				transform,
				glm::vec3(0.0f, -distance, 0.0f));
			transform = glm::scale(
				transform,
				glm::vec3(distance));
			transform = glm::rotate(
				transform,
				glm::radians(tilt),
				glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(
				transform,
				glm::radians((360.0f / 8.0f) * i),
				glm::vec3(0.0f, 1.0f, 0.0f));

			spheres.push_back(SphereData(glm::vec3(transform * glm::vec4(1.0f)), 100.0f * scale));
		}

		for (size_t x = 0; x < side; ++x)
		{
			position.x = (-(float)(side / 2) + (float)x) * (m_voxelHalfSize * 2.0f);

			for (size_t y = 0; y < side; ++y)
			{
				position.y = (-(float)(side / 2) + (float)y) * (m_voxelHalfSize * 2.0f);

				for (size_t z = 0; z < side; ++z)
				{
					position.z = (-(float)(side / 2) + (float)z) * (m_voxelHalfSize * 2.0f);

					bool hit = false;

					for (auto& sphere : spheres)
					{
						float distance = glm::length(position - sphere.position);
						if (distance <= sphere.radius)
						{
							hit = true;

							break;
						}
					}

					if (hit)
					{
						Logic::VoxelData voxel;
						voxel.position = position;
						voxel.color = glm::linearRand(
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(1.0f, 1.0f, 1.0f));

						voxels.push_back(voxel);
					}
				}
			}
		}

		m_logicOOP->initialize(voxels);
		m_logicDOD->initialize(voxels);

		m_voxelsActive = voxels.size();
	}

	void Application::renderHelp()
	{
		if (!m_options.help)
		{
			addText("Help: [H]");

			return;
		}

		if (m_options.logic == Options::LogicType::ObjectOriented)
		{
			addText("Data-oriented logic: [R]");
		}
		else if (
			m_options.logic == Options::LogicType::DataOriented)
		{
			addText("Object-oriented logic: [R]");
		}

		addText("Scene size: [1], [2], [3]");

		if (m_options.camera == Options::CameraType::User)
		{
			addText("Target camera: [G]");
			addText("Rotate: [A], [D]");
			addText("Zoom: [W], [S]");
		}
		else if (
			m_options.camera == Options::CameraType::Target)
		{
			addText("Free camera: [G]");
		}

		if (m_options.culling)
		{
			addText("Disable culling: [C]");

			if (m_options.showCulled)
			{
				addText("Hide culled voxels: [F]");
			}
			else
			{
				addText("Show culled state: [F]");
			}
		}
		else
		{
			addText("Enable culling: [C]");
		}

		addText("Hide help: [H]");
	}

	void Application::addText(const char* message, ...)
	{
		char buffer[1024] = { 0 };
		va_list args;
		va_start(args, message);
		vsnprintf(buffer, 1023, message, args);
		va_end(args);

		if (!m_textCombined.empty())
		{
			m_textCombined += "\n";
		}
		m_textCombined += buffer;
	}

}; // namespace Tmpl