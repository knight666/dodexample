#pragma once

#include "base/Main.hpp"
#include "voxels/Options.hpp"

namespace Tmpl {

	class Buffer;
	class FreeTypeLoader;
	class Logic;
	class Quad;
	class Program;
	class Sphere;
	class TextBatch;
	class VertexArrays;

	class Application
	{

		struct Uniforms
		{
			glm::mat4x4 modelViewProjection;
		};

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

		bool m_running;
		GLFWwindow* m_window;
		std::shared_ptr<FreeTypeLoader> m_loader;

		std::chrono::microseconds m_timeUpdate;
		std::chrono::microseconds m_timeRender;

		Options m_options;
		std::shared_ptr<Logic> m_logicOOP;
		std::shared_ptr<Logic> m_logicDOD;
		std::shared_ptr<Logic> m_logic;
		size_t m_voxelsActive;
		size_t m_voxelsCulled;
		float m_voxelHalfSize;

		glm::vec3 m_targetPosition;
		float m_targetAngle;
		float m_targetDistance;
		std::shared_ptr<Sphere> m_targetSphere;

		glm::vec3 m_eyePosition;

		float m_cameraAngle;
		float m_cameraDistance;

		bool m_keysPressed[GLFW_KEY_LAST];

		std::shared_ptr<TextBatch> m_text;
		std::string m_textCombined;

		std::shared_ptr<Program> m_program;
		std::shared_ptr<Buffer> m_vertices;
		std::shared_ptr<VertexArrays> m_attributes;
		std::shared_ptr<Buffer> m_uniforms;
		GLint m_uniformTransform;
		GLint m_uniformHalfSize;

	};

}; // namespace Tmpl