#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class FreeTypeLoader;
	class Quad;
	class TextBatch;

	class Application
	{

	public:

		Application(GLFWwindow* window);
		~Application();

		bool isRunning() const { return _running; }

		bool initialize();
		void update(uint32_t milliSeconds);
		void render();

		void onKeyPressed(int key, int modifierKeys);
		void onKeyReleased(int key, int modifierKeys);

	private:

		bool _running;
		GLFWwindow* _window;
		std::shared_ptr<FreeTypeLoader> _loader;
		std::shared_ptr<TextBatch> _text;

		glm::vec2 m_camera;

	};

}; // namespace Tmpl