#include "base/Main.hpp"

#include "Application.hpp"

int main(int argc, const char** argv)
{
	Tmpl::Logger::initialize();

	Tmpl::Application* application = new Tmpl::Application();
	int result = application->run(argc, argv);
	delete application;

	Tmpl::Logger::destroy();

	return result;
}