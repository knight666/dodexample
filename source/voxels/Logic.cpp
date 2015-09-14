#include "voxels/Logic.hpp"

namespace Tmpl {

	Logic::Logic()
		: m_program(new Program())
		, m_vertices(new Buffer(GL_ARRAY_BUFFER))
		, m_attributes(new VertexArrays())
		, m_uniforms(new Buffer(GL_UNIFORM_BUFFER))
	{
	}

	Logic::~Logic()
	{
	}

}; // namespace Tmpl