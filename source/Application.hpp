#pragma once

#include "base/Main.hpp"
#include "voxels/Options.hpp"

namespace Tmpl {

	class FreeTypeLoader;
	class Logic;
	class Quad;
	class Renderer;
	class Sphere;
	class TextBatch;

	class Application
	{

	public:

		Application();
		~Application();

		int run(int argc, const char** argv);

		bool isRunning() const { return m_running; }

		void update(uint32_t milliSeconds);
		void render();
		void renderInterface();

		void onKeyPressed(int key, int modifierKeys);
		void onKeyReleased(int key, int modifierKeys);

	private:

		void generateScene(float scale = 1.0f);

		void renderHelp();

		void addText(const char* message, ...);

	private:

		bool m_running;
		GLFWwindow* m_window;

		std::chrono::microseconds m_timeUpdate;
		std::chrono::microseconds m_timeRender;

		Options m_options;
		std::shared_ptr<Logic> m_logicOOP;
		std::shared_ptr<Logic> m_logicDOD;
		std::shared_ptr<Logic> m_logic;
		size_t m_voxelsActive;
		size_t m_voxelsCulled;

		glm::vec3 m_targetPosition;
		float m_targetAngle;
		float m_targetDistance;
		std::shared_ptr<Sphere> m_targetSphere;

		glm::vec3 m_eyePosition;

		float m_cameraAngle;
		float m_cameraDistance;

		bool m_keysPressed[GLFW_KEY_LAST];

		std::shared_ptr<FreeTypeLoader> m_loader;
		std::shared_ptr<TextBatch> m_text;
		std::string m_textCombined;

		std::shared_ptr<Renderer> m_renderer;

	};

}; // namespace Tmpl