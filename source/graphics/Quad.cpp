#include "graphics/Quad.hpp"

namespace Tmpl {

	Quad::Quad()
		: _texture(new Texture(GL_TEXTURE_2D, GL_RGBA8, 256, 256))
		, _vertices(new Buffer(GL_ARRAY_BUFFER))
		, _elements(new Buffer(GL_ELEMENT_ARRAY_BUFFER))
		, _program(new Program())
		, _attributes(new VertexArrays())
		, _uniforms(new Buffer(GL_UNIFORM_BUFFER))
	{
		// Texture

		_texture->bind(0);

			_texture->setParameter(GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
			_texture->setParameter(GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
			_texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			_texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			_texture->image2D(0, (const GLbyte*)nullptr, GL_BGRA);

		_texture->unbind();

		// Vertices

		_vertices->bind();

			Vertex vertex_data[4] = {
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(0.0f, 1.0f)
				},
				{
					glm::vec2(1.0f, 0.0f),
					glm::vec2(1.0f, 1.0f)
				},
				{
					glm::vec2(0.0f, 1.0f),
					glm::vec2(0.0f, 0.0f)
				},
				{
					glm::vec2(1.0f, 1.0f),
					glm::vec2(1.0f, 0.0f)
				},
			};
			_vertices->setData(vertex_data, 4, GL_STATIC_DRAW);

		_vertices->unbind();

		// Elements

		_elements->bind();

			GLuint element_data[6] = {
				0, 1, 2,
				1, 3, 2
			};
			_elements->setData(element_data, 6, GL_STATIC_DRAW);

		_elements->unbind();

		// Program

		bool validated = true;
		validated &= _program->loadShaderFromFile(Shader::Type::Vertex, "media/shaders/texture-2d.vert");
		validated &= _program->loadShaderFromFile(Shader::Type::Fragment, "media/shaders/texture-2d.frag");
		validated &= _program->link();

		_program->bindFragmentDataLocation("Color", Program::FragmentDataLocation::Color);

		_uniformTransform = _program->getUniformBlockIndex("vertexUniforms");
		_uniformDiffuse = _program->getUniformLocation("texDiffuse");

		// Attributes

		_attributes->bind();

			_vertices->bind();
			_attributes->setAttribute(_program->getAttributeLocation("attrPosition"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)Vertex::Offset::Position);
			_attributes->setAttribute(_program->getAttributeLocation("attrTextureCoordinate"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)Vertex::Offset::TextureCoordinate);

			_elements->bind();

		_attributes->unbind();

		// Uniforms

		GLint uniform_buffer_offset = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniform_buffer_offset);
		size_t uniform_buffer_size = glm::max(uniform_buffer_offset, (GLint)sizeof(Uniforms));

		_uniforms->bind();

			_uniforms->setData(nullptr, uniform_buffer_size, GL_DYNAMIC_DRAW);

		_uniforms->unbind();
	}

	Quad::~Quad()
	{
	}

	void Quad::setTextureData(const GLuint* pixels, GLsizei width, GLsizei height)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		_texture->bind(0);

		GLsizei dst_pitch = _texture->getWidth() * sizeof(GLuint);

		const GLbyte* src = (const GLbyte*)pixels;
		GLsizei src_pitch = width * sizeof(GLuint);
		GLsizei src_size = src_pitch * height;

		GLsizei texture_size = src_pitch * height;

		GLuint pixel_buffer = 0;
		glGenBuffers(1, &pixel_buffer);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel_buffer);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, src_size, nullptr, GL_STREAM_DRAW);
		GLbyte* mapped = (GLbyte*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, src_size, GL_MAP_WRITE_BIT);

		GLbyte* dst = mapped;

		for (GLsizei y = 0; y < height; ++y)
		{
			memcpy(dst, src, src_pitch);
			dst += dst_pitch;
			src += src_pitch;
		}

		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		_texture->subImage2D(0, (const GLbyte*)nullptr, GL_BGRA);

		glDeleteBuffers(1, &pixel_buffer);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		_texture->unbind();

		GLenum errors = glGetError();
	}

	void Quad::render(const glm::mat4x4& camera)
	{
		glm::mat4x4 model = glm::mat4(1.0f);

		_uniforms->bind();
			Uniforms* transform = _uniforms->mapRange<Uniforms>(0, 1, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				transform->modelViewProjection = camera * model;
			_uniforms->unmap();
		_uniforms->unbind();

		_program->bind();

		_texture->bind(0);
		_program->setUniform(_uniformDiffuse, 0);

		_uniforms->bindBase(0);
		_program->setUniformBlockBinding(_uniformTransform, 0);

		_attributes->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);

		_attributes->unbind();
		_uniforms->unbind();
		_texture->unbind();
		_program->unbind();
	}

}; // namespace Tmpl