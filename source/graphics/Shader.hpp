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
		static std::string TypeToString(Type type);

		Shader(Program& program, Type type);
		~Shader();

		const std::string& getLog() const { return m_log; }

		bool compile(const std::string& source);

	private:

		Program& m_program;
		GLuint m_handle;
		Type m_type;
		std::string m_log;

	};

}; // namespace Tmpl