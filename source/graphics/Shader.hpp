#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Program;

	class Shader
	{

		friend class Program;

	public:

		enum struct Type
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Geometry = GL_GEOMETRY_SHADER,
		};

		Shader(Program& program, Type type);
		~Shader();

		const std::string& getLog() const { return _log; }

		bool compile(const std::string& source);

	private:

		Program& _program;
		GLuint _handle;
		Type _type;
		std::string _log;

	};

}; // namespace Tmpl