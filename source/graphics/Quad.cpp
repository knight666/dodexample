#include "graphics/Quad.hpp"

namespace Tmpl {

	Quad::Quad()
		: m_texture(new Texture(GL_TEXTURE_2D, GL_RGBA8, 256, 256))
		, m_vertices(new Buffer(GL_ARRAY_BUFFER))
		, m_elements(new Buffer(GL_ELEMENT_ARRAY_BUFFER))
		, m_program(new Program())
		, m_attributes(new VertexArrays(m_program))
		, m_uniforms(new Buffer(GL_UNIFORM_BUFFER))
	{
		// Texture

		m_texture->bind(0);
			m_texture->setParameter(GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
			m_texture->setParameter(GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
			m_texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			m_texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			m_texture->image2D<GLbyte>(0, nullptr, GL_BGRA);
		m_texture->unbind();

		// Vertices

		m_vertices->bind();
			Vertex vertex_data[4] = {
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(0.0f, 0.0f)
				},
				{
					glm::vec2(1.0f, 0.0f),
					glm::vec2(1.0f, 0.0f)
				},
				{
					glm::vec2(0.0f, 1.0f),
					glm::vec2(0.0f, 1.0f)
				},
				{
					glm::vec2(1.0f, 1.0f),
					glm::vec2(1.0f, 1.0f)
				},
			};
			m_vertices->setData(vertex_data, 4, GL_STATIC_DRAW);
		m_vertices->unbind();

		// Elements

		m_elements->bind();
			GLuint element_data[6] = {
				1, 0, 2,
				3, 1, 2
			};
			m_elements->setData(element_data, 6, GL_STATIC_DRAW);
		m_elements->unbind();

		// Program

		bool validated = true;
		validated &= m_program->loadShaderFromFile(
			Shader::Type::Vertex,
			"media/shaders/texture-2d.vert");
		validated &= m_program->loadShaderFromFile(
			Shader::Type::Fragment,
			"media/shaders/texture-2d.frag");
		validated &= m_program->link();

		m_program->bindFragmentDataLocation(
			"Color",
			Program::FragmentDataLocation::Color);

		m_uniformTransform = m_program->getUniformBlockIndex("vertexUniforms");
		m_uniformDiffuse = m_program->getUniformLocation("texDiffuse");

		// Attributes

		m_attributes->bind();
			m_vertices->bind();
				m_attributes->setAttribute<glm::vec2>(
					"attrPosition",
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Position);
				m_attributes->setAttribute<glm::vec2>(
					"attrTextureCoordinate",
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::TextureCoordinate);
			m_vertices->unbind();
		m_attributes->unbind();

		// Uniforms

		GLint uniform_buffer_offset = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniform_buffer_offset);
		size_t uniform_buffer_size = glm::max(uniform_buffer_offset, (GLint)sizeof(Uniforms));

		m_uniforms->bind();
			m_uniforms->setData<Uniforms>(
				nullptr,
				uniform_buffer_size,
				GL_DYNAMIC_DRAW);
		m_uniforms->unbind();
	}

	Quad::~Quad()
	{
	}

	void Quad::setTextureData(const GLuint* pixels, GLsizei width, GLsizei height)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		m_texture->bind(0);
		m_texture->resize(width, height);

		GLsizei dst_pitch = m_texture->getWidth() * sizeof(GLuint);

		const GLbyte* src = (const GLbyte*)pixels;
		GLsizei src_pitch = width * sizeof(GLuint);
		GLsizei src_size = src_pitch * height;

		GLsizei texture_size = src_pitch * height;

		std::shared_ptr<Buffer> pixel_buffer = m_texture->getPixels();
		pixel_buffer->bind();

			pixel_buffer->setData<GLbyte>(nullptr, src_size, GL_STREAM_DRAW);

			GLbyte* mapped = pixel_buffer->mapRange<GLbyte>(
				0, src_size,
				GL_MAP_WRITE_BIT);
			{
				GLbyte* dst = mapped;

				for (GLsizei y = 0; y < height; ++y)
				{
					memcpy(dst, src, src_pitch);
					dst += dst_pitch;
					src += src_pitch;
				}
			}
			pixel_buffer->unmap();

			m_texture->subImage2D(0, (const GLbyte*)nullptr, GL_BGRA);

		pixel_buffer->unbind();
		m_texture->unbind();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	void Quad::render(const glm::mat4x4& modelViewProjection)
	{
		m_uniforms->bind();
			Uniforms* transform = m_uniforms->mapRange<Uniforms>(
				0, 1,
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				transform->modelViewProjection = modelViewProjection;
			m_uniforms->unmap();
		m_uniforms->unbind();

		m_program->bind();

		m_texture->bind(0);
		m_program->setUniform<GLint>(m_uniformDiffuse, 0);

		m_uniforms->bindBase(0);
		m_program->setUniformBlockBinding(m_uniformTransform, 0);

		m_attributes->bind();

		m_elements->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDrawElements(
				GL_TRIANGLES,
				6,
				GL_UNSIGNED_INT,
				nullptr);

		glDisable(GL_BLEND);

		m_elements->unbind();
		m_attributes->unbind();
		m_uniforms->unbind();
		m_texture->unbind();
		m_program->unbind();
	}

}; // namespace Tmpl