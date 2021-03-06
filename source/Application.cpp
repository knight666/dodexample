#include "Application.hpp"

#include "graphics/Quad.hpp"
#include "graphics/Sphere.hpp"
#include "text/FreeTypeLoader.hpp"
#include "text/TextBatch.hpp"
#include "voxels/dod/LogicDOD.hpp"
#include "voxels/oop/LogicOOP.hpp"
#include "voxels/Renderer.hpp"

namespace Tmpl {

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
		, m_voxelsCulled(0)
		, m_voxelsActive(0)
		, m_lightAngle(0.0f)
		, m_lightDistance(1000.0f)
		, m_lightDirty(true)
		, m_cameraAngle(45.0f)
		, m_cameraDistance(5000.0f)
	{
		memset(m_keysPressed, 0, sizeof(m_keysPressed));
		memset(m_keysReleased, 0, sizeof(m_keysReleased));
	}

	Application::~Application()
	{
	}

	int Application::run(int argc, const char** argv)
	{
		TMPL_LOG_INFO(Application) << "Starting.";

		if (!parseCommandLine(argc, argv))
		{
			return 0;
		}

		if (!initialize())
		{
			TMPL_LOG_ERROR(Application) << "Failed to initialize.";

			return -1;
		}

		// Main loop

		TMPL_LOG_INFO(Application) << "Entering main loop.";

		static const float TimeStep = 1000.0f / 30.0f;
		static const float TimeStepMaximum = TimeStep * 10.0f;

		typedef std::chrono::steady_clock clock;
		typedef std::chrono::microseconds us;

		clock::time_point time_start = clock::now();
		clock::time_point time_current;

		if (m_options.profiling)
		{
			TMPL_LOG_INFO(Application) <<
				"Starting profiling over "
				<< m_options.profilingFrames << " frames.";

			std::vector<us> frame_samples;
			frame_samples.reserve(m_options.profilingFrames);

			for (size_t i = 0; i < m_options.profilingFrames; ++i)
			{
				if (glfwWindowShouldClose(m_window))
				{
					break;
				}

				glfwPollEvents();

				time_start = clock::now();

				update(TimeStep);
				render();

				glfwSwapBuffers(m_window);

				time_current = clock::now();
				us time_delta = std::chrono::duration_cast<us>(
					 time_current - time_start);
				time_start = time_current;

				TMPL_LOG_INFO(Profiling)
					<< "Frame " << i << ": "
					<< (uint32_t)(time_delta.count() / 1000) << " ms";

				frame_samples.push_back(time_delta);
			}

			TMPL_LOG_INFO(Application) << "Finished profiling.";

			time_t time_stamp = std::chrono::system_clock::to_time_t(
				clock::now());

			struct tm* time_local = localtime(&time_stamp);

			std::string logic_text;

			switch (m_options.logic)
			{

			case Options::LogicType::DataOriented:
				logic_text = "dod";
				break;

			case Options::LogicType::ObjectOriented:
				logic_text = "ood";
				break;

			default:
				break;

			}

			std::string scene_text;

			switch (m_options.scene)
			{

			case Options::Scene::Small:
				scene_text = "1";
				break;

			case Options::Scene::Medium:
				scene_text = "2";
				break;

			case Options::Scene::Large:
				scene_text = "3";
				break;

			default:
				break;

			}

			std::stringstream csv_path;
			csv_path
				<< applicationDirectory()
				<< "profiling_"
				<< "logic_" << logic_text << "_"
				<< "scene_" << scene_text << "_"
				<< "samples_" << m_options.profilingFrames << "_"
				<< std::setw(4) << std::setfill('0')
				<< (1900 + time_local->tm_year) << "_"
				<< std::setw(2) << std::setfill('0')
				<< (time_local->tm_mon + 1) << "_"
				<< std::setw(2) << std::setfill('0')
				<< time_local->tm_mday << "_"
				<< std::setw(2) << std::setfill('0')
				<< time_local->tm_hour << "_"
				<< std::setw(2) << std::setfill('0')
				<< time_local->tm_min << "_"
				<< std::setw(2) << std::setfill('0')
				<< time_local->tm_sec
				<< ".csv";

			TMPL_LOG_INFO(Application) <<
				"Writing sample data to \"" << csv_path.str() << "\".";

			std::ofstream csv_file(
				csv_path.str().c_str(),
				std::ios::out | std::ios::trunc);

			if (csv_file.is_open())
			{
				for (us& sample : frame_samples)
				{
					std::stringstream ss;
					ss << (uint32_t)(sample.count() / 1000);
					csv_file << ss.str() << std::endl;
				}

				csv_file.close();
			}

			TMPL_LOG_INFO(Application)
				<< "Finished writing sample data.";
		}
		else
		{
			clock::time_point time_render_start;

			while (
				!glfwWindowShouldClose(m_window))
			{
				glfwPollEvents();

				time_current = clock::now();
				us time_delta = std::chrono::duration_cast<us>(
					time_current - time_start);

				float delta_time = glm::min(
					(float)time_delta.count() / 1000.0f,
					TimeStepMaximum) / TimeStep;

				updateUserInput(delta_time);
				update(delta_time);

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
		}

		TMPL_LOG_INFO(Application) << "Shutting down.";

		return 0;
	}

	bool Application::parseCommandLine(int argc, const char** argv)
	{
		if (argc <= 1)
		{
			return true;
		}

		std::string combined;
		for (int i = 1; i < argc; ++i)
		{
			if (i > 1)
			{
				combined += " ";
			}
			combined += argv[i];
		}

		TMPL_LOG_INFO(Application) << "Command line: \"" << combined << "\"";

		bool show_help = false;

		for (int i = 1; i < argc; ++i)
		{
			std::string arg = tolower(argv[i]);

			if (arg == "-help" ||
				arg == "--help" ||
				arg == "/help" ||
				arg == "-?" ||
				arg == "--?" ||
				arg == "/?")
			{
				show_help = true;

				break;
			}
			else if (
				arg == "-profile" ||
				arg == "-profiling")
			{
				TMPL_LOG_INFO(Application) <<
					"Enabling profiling mode.";

				m_options.profiling = true;

				if (i + 1 < argc)
				{
					char* found = nullptr;
					double result = strtod(argv[i + 1], &found);
					if (found != nullptr)
					{
						m_options.profilingFrames = (size_t)result;

						i++;
					}
				}
			}
			else if (
				arg == "-dod")
			{
				m_options.logic = Options::LogicType::DataOriented;
			}
			else if (
				arg == "-ood" ||
				arg == "-oop")
			{
				m_options.logic = Options::LogicType::ObjectOriented;
			}
			else if (
				arg == "-scene")
			{
				if (i + 1 < argc)
				{
					arg = tolower(argv[++i]);

					if (arg == "1" ||
						arg == "small")
					{
						m_options.scene = Options::Scene::Small;
					}
					else if (
						arg == "2" ||
						arg == "medium")
					{
						m_options.scene = Options::Scene::Medium;
					}
					else if (
						arg == "3" ||
						arg == "large")
					{
						m_options.scene = Options::Scene::Large;
					}
					else
					{
						TMPL_LOG_ERROR(Application)
							<< "Unknown scene size \"" << arg << "\".";
					}
				}
			}
			else
			{
				TMPL_LOG_ERROR(Application)
					<< "Unknown command line option \"" << arg << "\".";

				show_help = true;

				break;
			}
		}

		if (show_help)
		{
			std::stringstream help_text;

			help_text
				<< "Example of data-oriented design, by Quinten Lansu." << std::endl
				<< std::endl
				<< "   -help" << std::endl
				<< "        Display this help text and exit." << std::endl
				<< std::endl
				<< "   -profiling [FRAME_COUNT]" << std::endl
				<< "        Enable profiling mode with a default frame count of " << m_options.profilingFrames << "." << std::endl
				<< std::endl
				<< "   -dod" << std::endl
				<< "        Use data-oriented design logic." << std::endl
				<< std::endl
				<< "   -ood" << std::endl
				<< "        Use object-oriented design logic (default)." << std::endl
				<< std::endl
				<< "   -scene {SMALL|MEDIUM|LARGE}" << std::endl
				<< "        Set the scene size on start-up." << std::endl;

			TMPL_LOG_INFO(Application) << help_text.str();

		#if defined(WIN32) || defined(_WINDOWS)
			::MessageBoxW(
				NULL,
				widen(help_text.str()).c_str(),
				widen(TMPL_WINDOW_TITLE).c_str(),
				MB_OK | MB_ICONINFORMATION);
		#endif
		}

		return !show_help;
	}

	bool Application::initialize()
	{
		TMPL_LOG_INFO(Application) << "Initializing.";

		if (!initializeOpenGL())
		{
			TMPL_LOG_ERROR(Application) << "Failed to initialize OpenGL.";

			return false;
		}

		// Text

		m_loader = std::make_shared<FreeTypeLoader>();
		m_loader->loadFace("media/fonts/Roboto/Roboto-Black.ttf", 12.0f);
		m_loader->loadGlyphRange(0x00, 0xFF); // preload Basic Latin and Latin-1

		m_text = std::shared_ptr<TextBatch>(new TextBatch(m_loader, 512, 512));

		// Models

		m_lightSphere = std::make_shared<Sphere>();
		m_lightSphere->setup(20, 20);

		// Logic

		m_logicOOP = std::make_shared<LogicOOP>();
		m_logicDOD = std::make_shared<LogicDOD>();

		if (m_options.logic == Options::LogicType::ObjectOriented)
		{
			TMPL_LOG_INFO(Application) << "Using object-oriented logic.";

			m_logic = m_logicOOP;
		}
		else
		{
			TMPL_LOG_INFO(Application) << "Using data-oriented logic.";

			m_logic = m_logicDOD;
		}

		generateScene(m_options.scene);

		// Renderer

		m_renderer = std::make_shared<Renderer>();
		if (!m_renderer->initialize())
		{
			return false;
		}

		return true;
	}

	void Application::update(float deltaTime)
	{
		m_voxelsCulled = 0;

		if (m_lightDirty)
		{
			m_lightPosition = glm::vec3(
				glm::cos(glm::radians(m_lightAngle)) * m_lightDistance,
				10.0f,
				glm::sin(glm::radians(m_lightAngle)) * m_lightDistance);

			if (m_options.culling)
			{
				m_voxelsCulled = m_logic->cullVoxels(
					m_options,
					m_lightPosition);
			}

			m_lightDirty = false;
		}

		if (m_options.lightRunning)
		{
			m_lightAngle += 1.0f * deltaTime;

			m_lightDirty = true;
		}
	}

	void Application::updateUserInput(float deltaTime)
	{
		if (m_options.camera == Options::CameraType::User)
		{
			float speed = m_keysPressed[GLFW_KEY_LEFT_SHIFT] ? 5.0f : 1.0f;

			if (m_keysPressed[GLFW_KEY_A])
			{
				m_cameraAngle += 1.0f * speed * deltaTime;
			}
			if (m_keysPressed[GLFW_KEY_D])
			{
				m_cameraAngle -= 1.0f * speed * deltaTime;
			}
			if (m_keysPressed[GLFW_KEY_W])
			{
				m_cameraDistance -= 10.0f * speed * deltaTime;
			}
			if (m_keysPressed[GLFW_KEY_S])
			{
				m_cameraDistance += 10.0f * speed * deltaTime;
			}
		}

		if (!m_options.lightRunning)
		{
			if (m_keysPressed[GLFW_KEY_J])
			{
				m_lightAngle -= 1.0f * deltaTime;

				m_lightDirty = true;
			}
			if (m_keysPressed[GLFW_KEY_K])
			{
				m_lightAngle += 1.0f * deltaTime;

				m_lightDirty = true;
			}
		}

		if (m_keysReleased[GLFW_KEY_SPACE])
		{
			m_options.lightRunning = !m_options.lightRunning;
		}

		if (m_keysReleased[GLFW_KEY_1])
		{
			if (m_options.scene != Options::Scene::Small)
			{
				generateScene(Options::Scene::Small);

				m_lightDirty = true;
				m_options.scene = Options::Scene::Small;
			}
		}

		if (m_keysReleased[GLFW_KEY_2])
		{
			if (m_options.scene != Options::Scene::Medium)
			{
				generateScene(Options::Scene::Medium);

				m_lightDirty = true;
				m_options.scene = Options::Scene::Medium;
			}
		}

		if (m_keysReleased[GLFW_KEY_3])
		{
			if (m_options.scene != Options::Scene::Large)
			{
				generateScene(Options::Scene::Large);

				m_lightDirty = true;
				m_options.scene = Options::Scene::Large;
			}
		}

		if (m_keysReleased[GLFW_KEY_C])
		{
			m_options.culling = !m_options.culling;
		}

		if (m_keysReleased[GLFW_KEY_F])
		{
			if (m_options.culling)
			{
				m_options.showCulled = !m_options.showCulled;
			}
		}

		if (m_keysReleased[GLFW_KEY_R])
		{
			if (m_options.logic == Options::LogicType::ObjectOriented)
			{
				m_logic = m_logicDOD;
				m_lightDirty = true;

				m_options.logic = Options::LogicType::DataOriented;
			}
			else
			{
				m_logic = m_logicOOP;
				m_lightDirty = true;

				m_options.logic = Options::LogicType::ObjectOriented;
			}
		}

		if (m_keysReleased[GLFW_KEY_G])
		{
			if (m_options.camera == Options::CameraType::Target)
			{
				m_options.camera = Options::CameraType::User;
			}
			else
			{
				m_options.camera = Options::CameraType::Target;
			}
		}

		if (m_keysReleased[GLFW_KEY_H])
		{
			m_options.help = !m_options.help;
		}

		memset(m_keysReleased, 0, sizeof(m_keysReleased));
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
			m_eyePosition = m_lightPosition;
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
			m_lightSphere->render(
				viewProjection,
				m_lightPosition,
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
		m_keysReleased[key] = true;
		m_keysPressed[key] = false;
	}

	bool Application::initializeOpenGL()
	{
		TMPL_LOG_INFO(GLFW) << "Initializing.";

		glfwSetErrorCallback(Tmpl::glfwErrors);

		if (glfwInit() == 0)
		{
			TMPL_LOG_ERROR(GLFW) << "Failed to initialize.";

			return false;
		}

		TMPL_LOG_INFO(GLFW) << "Setting window hints.";

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		TMPL_LOG_INFO(GLFW) << "Creating window.";

		m_window = glfwCreateWindow(
			TMPL_WINDOW_WIDTH, TMPL_WINDOW_HEIGHT,
			TMPL_WINDOW_TITLE,
			nullptr,
			nullptr);

		if (m_window == nullptr)
		{
			TMPL_LOG_ERROR(GLFW) << "Failed to create window.";

			return false;
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

			return false;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		return true;
	}

	void Application::generateScene(Options::Scene scene)
	{
		float scale = 0.0f;

		switch (scene)
		{

		case Options::Scene::Small:
			scale = 1.0f;
			break;

		case Options::Scene::Medium:
			scale = 1.4f;
			break;

		case Options::Scene::Large:
			scale = 1.8f;
			break;

		default:
			break;

		}

		TMPL_LOG_INFO(Application)
			<< "Generating scene with a scale factor of " << scale << "...";

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
		spheres.push_back(SphereData(
			glm::vec3(0.0f, 0.0f, 0.0f),
			250.0f * scale));

		std::vector<Logic::VoxelData> voxels;
		glm::vec3 position;
		size_t side = 100;
		size_t planet_count = 8;
		float tilt = 25.0f;
		float distance = 550.0f * scale;
		float offset = -(float)(side / 2);
		float voxel_size = m_options.voxelHalfSize * 2.0f;

		for (size_t i = 0; i < planet_count; ++i)
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
				glm::radians((360.0f / (float)planet_count) * i),
				glm::vec3(0.0f, 1.0f, 0.0f));

			spheres.push_back(SphereData(
				glm::vec3(transform * glm::vec4(1.0f)),
				100.0f * scale));
		}

		for (size_t x = 0; x < side; ++x)
		{
			position.x = (offset + (float)x) * voxel_size;

			for (size_t y = 0; y < side; ++y)
			{
				position.y = (offset + (float)y) * voxel_size;

				for (size_t z = 0; z < side; ++z)
				{
					position.z = (offset + (float)z) * voxel_size;

					bool hit = false;

					for (auto& sphere : spheres)
					{
						glm::vec3 relative = position - sphere.position;

						if (glm::length(relative) <= sphere.radius)
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

		m_logicOOP->initialize(m_options, voxels);
		m_logicDOD->initialize(m_options, voxels);

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
			addText("Light camera: [G]");
			addText("Rotate: [A], [D]");
			addText("Zoom: [W], [S]");
			addText("Increased zoom speed: [Left Shift]");
		}
		else if (
			m_options.camera == Options::CameraType::Target)
		{
			addText("Free camera: [G]");
		}

		if (m_options.lightRunning)
		{
			addText("Pause light rotation: [Space]");
		}
		else
		{
			addText("Rotate light: [J], [K]");
			addText("Resume light rotation: [Space]");
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