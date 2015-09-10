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
		, m_voxelsCulled(0)
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
		if (m_options.camera == Options::CameraType::User)
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
		}

		m_targetPosition = glm::vec3(
			glm::cos(glm::radians(m_targetAngle)) * m_targetDistance,
			10.0f,
			glm::sin(glm::radians(m_targetAngle)) * m_targetDistance);

		m_voxelsCulled = 0;

		if (m_options.culling)
		{
			m_voxelsCulled = m_logicOOP->cullVoxels(m_options, m_targetPosition);
		}

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

		glm::vec3 eye_position;

		if (m_options.camera == Options::CameraType::User)
		{
			eye_position = glm::vec3(
				glm::cos(glm::radians(m_cameraAngle)) * m_cameraDistance,
				0.0f,
				glm::sin(glm::radians(m_cameraAngle)) * m_cameraDistance);
		}
		else if (
			m_options.camera == Options::CameraType::Target)
		{
			eye_position = m_targetPosition;
		}

		glm::mat4x4 perspective = glm::perspectiveFov(
			120.0f,
			(float)width, (float)height,
			0.1f, 10000.0f);

		glm::mat4x4 viewCamera = glm::lookAt(
			eye_position,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		m_logicOOP->render(m_options, perspective * viewCamera);

		glm::mat4x4 interface_projection = glm::ortho(
			0.0f, (float)width,
			(float)height, 0.0f,
			-1.0f, 1.0f);

		m_textCombined.clear();

		addText("Voxels: %d (%d culled = %.2f%%)",
			m_voxelsActive,
			m_voxelsCulled,
			((float)m_voxelsCulled / (float)m_voxelsActive) * 100.0f);
		addText("Eye: (%.2f, %.2f, %.2f)",
			eye_position.x, eye_position.y, eye_position.z);

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
			m_options.camera = Options::CameraType::User;
			break;

		case GLFW_KEY_2:
			m_options.camera = Options::CameraType::Target;
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

		case GLFW_KEY_H:
			m_options.help = !m_options.help;
			break;

		default:
			break;

		}
	}

	void Application::renderHelp()
	{
		if (!m_options.help)
		{
			addText("Help: [H]");

			return;
		}

		if (m_options.camera == Options::CameraType::User)
		{
			addText("Target camera: [2]");
			addText("Rotate: [A], [D]");
			addText("Zoom: [W], [S]");
		}
		else if (m_options.camera == Options::CameraType::Target)
		{
			addText("Free camera: [1]");
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