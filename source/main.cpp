#include "base/Main.hpp"

#include "Application.hpp"

int main(int argc, const char** argv)
{
	Tmpl::Logger::initialize();

	Tmpl::Application application;
	int result = application.run(argc, argv);

	Tmpl::Logger::destroy();

	return result;
}