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

		bool parseCommandLine(int argc, const char** argv);

		bool initialize();

		void update(float deltaTime);
		void updateUserInput(float deltaTime);

		void render();
		void renderInterface();

		void onKeyPressed(int key, int modifierKeys);
		void onKeyReleased(int key, int modifierKeys);

	private:

		bool initializeOpenGL();

		void generateScene(Options::Scene scene);

		void renderHelp();

		void addText(const char* message, ...);

	private:

		GLFWwindow* m_window;

		std::chrono::microseconds m_timeUpdate;
		std::chrono::microseconds m_timeRender;
		std::chrono::microseconds m_timeTotal;

		Options m_options;
		std::shared_ptr<Logic> m_logicOOP;
		std::shared_ptr<Logic> m_logicDOD;
		std::shared_ptr<Logic> m_logic;
		size_t m_voxelsActive;
		size_t m_voxelsCulled;

		glm::vec3 m_lightPosition;
		float m_lightAngle;
		float m_lightDistance;
		bool m_lightDirty;
		std::shared_ptr<Sphere> m_lightSphere;

		glm::vec3 m_eyePosition;

		float m_cameraAngle;
		float m_cameraDistance;

		bool m_keysPressed[GLFW_KEY_LAST];
		bool m_keysReleased[GLFW_KEY_LAST];

		std::shared_ptr<FreeTypeLoader> m_loader;
		std::shared_ptr<TextBatch> m_text;
		std::string m_textCombined;

		std::shared_ptr<Renderer> m_renderer;

	};

}; // namespace Tmpl