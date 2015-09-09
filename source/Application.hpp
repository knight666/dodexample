#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class FreeTypeLoader;
	class Logic;
	class Quad;
	class TextBatch;

	class Application
	{

	public:

		Application(GLFWwindow* window);
		~Application();

		bool isRunning() const { return m_running; }

		bool initialize();
		void update(uint32_t milliSeconds);
		void render();

		void onKeyPressed(int key, int modifierKeys);
		void onKeyReleased(int key, int modifierKeys);

	private:

		bool m_running;
		GLFWwindow* m_window;
		std::shared_ptr<FreeTypeLoader> m_loader;
		std::shared_ptr<TextBatch> m_text;

		std::shared_ptr<Logic> m_logicOOP;
		std::shared_ptr<Logic> m_logicDOD;
		size_t m_voxelsActive;
		float m_voxelHalfSize;

		glm::vec3 m_targetPosition;
		float m_targetAngle;
		float m_targetDistance;

		float m_cameraAngle;
		float m_cameraDistance;

		bool m_keysPressed[GLFW_KEY_LAST];

	};

}; // namespace Tmpl