#include "graphics/Sphere.hpp"

namespace Tmpl {

	Sphere::Sphere()
		: m_vertices(new Buffer(GL_ARRAY_BUFFER))
		, m_elements(new Buffer(GL_ELEMENT_ARRAY_BUFFER))
		, m_program(new Program())
		, m_attributes(new VertexArrays(m_program))
		, m_uniforms(new Buffer(GL_UNIFORM_BUFFER))
	{
		// Program

		bool validated = true;
		validated &= m_program->loadShaderFromFile(
			Shader::Type::Vertex,
			"media/shaders/sphere.vert");
		validated &= m_program->loadShaderFromFile(
			Shader::Type::Fragment,
			"media/shaders/sphere.frag");
		validated &= m_program->link();

		// Attributes

		m_attributes->bind();
			m_vertices->bind();
				m_attributes->setAttribute<glm::vec3>(
					"attrPosition",
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Position);
			m_vertices->unbind();
		m_attributes->unbind();

		// Uniforms

		m_uniformTransform = m_program->getUniformBlockIndex("vertexUniforms");
		m_uniformColor = m_program->getUniformLocation("color");

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

	void Sphere::setup(size_t subdivisionsZ, size_t subdivisionsY)
	{
		m_vertexData.clear();
		m_elementData.clear();

		int position_index = 0;

		float radians_y = 0.0f;
		float radians_y_delta = glm::radians(90.0f) / (float)subdivisionsZ;
		float radians_y_next = radians_y_delta;

		for (size_t y = 0; y < subdivisionsY; y++)
		{
			float radius_y      = glm::sin(radians_y);
			float radius_y_next = glm::sin(radians_y_next);

			float pos_top_y         = glm::cos(radians_y);
			float pos_top_y_next    = glm::cos(radians_y_next);
			float pos_bottom_y      = glm::cos(radians_y) * -1.0f;
			float pos_bottom_y_next = glm::cos(radians_y_next) * -1.0f;

			float radians_z = 0.0f;
			float radians_z_delta = glm::radians(360.0f) / (float)subdivisionsZ;
			float radians_z_next = radians_z_delta;

			for (size_t x = 0; x < subdivisionsZ; x++)
			{
				float pos_x_up_from   = glm::cos(radians_z) * radius_y;
				float pos_x_up_to     = glm::cos(radians_z_next) * radius_y;
				float pos_x_down_from = glm::cos(radians_z) * radius_y_next;
				float pos_x_down_to   = glm::cos(radians_z_next) * radius_y_next;

				float pos_z_up_from   = glm::sin(radians_z) * radius_y;
				float pos_z_up_to     = glm::sin(radians_z_next) * radius_y;
				float pos_z_down_from = glm::sin(radians_z) * radius_y_next;
				float pos_z_down_to   = glm::sin(radians_z_next) * radius_y_next;

				glm::vec3 pos_top_a(pos_x_up_from  , pos_top_y     , pos_z_up_from);
				glm::vec3 pos_top_b(pos_x_up_to    , pos_top_y     , pos_z_up_to);
				glm::vec3 pos_top_c(pos_x_down_from, pos_top_y_next, pos_z_down_from);
				glm::vec3 pos_top_d(pos_x_down_to  , pos_top_y_next, pos_z_down_to);

				glm::vec3 pos_bottom_a(pos_x_up_from  , pos_bottom_y     , pos_z_up_from);
				glm::vec3 pos_bottom_b(pos_x_up_to    , pos_bottom_y     , pos_z_up_to);
				glm::vec3 pos_bottom_c(pos_x_down_from, pos_bottom_y_next, pos_z_down_from);
				glm::vec3 pos_bottom_d(pos_x_down_to  , pos_bottom_y_next, pos_z_down_to);

				if (pos_top_y != 1.0f)
				{
					m_vertexData.push_back(Vertex(pos_top_a));
					m_vertexData.push_back(Vertex(pos_top_b));
					m_vertexData.push_back(Vertex(pos_top_c));
					m_vertexData.push_back(Vertex(pos_top_d));

					m_elementData.push_back(position_index + 0);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 2);

					m_elementData.push_back(position_index + 2);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 3);

					position_index += 4;

					m_vertexData.push_back(Vertex(pos_bottom_a));
					m_vertexData.push_back(Vertex(pos_bottom_b));
					m_vertexData.push_back(Vertex(pos_bottom_c));
					m_vertexData.push_back(Vertex(pos_bottom_d));

					m_elementData.push_back(position_index + 0);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 2);

					m_elementData.push_back(position_index + 2);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 3);

					position_index += 4;
				}
				else
				{
					m_vertexData.push_back(Vertex(pos_top_c));
					m_vertexData.push_back(Vertex(pos_top_b));
					m_vertexData.push_back(Vertex(pos_top_d));

					m_elementData.push_back(position_index + 0);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 2);

					position_index += 3;

					m_vertexData.push_back(Vertex(pos_bottom_c));
					m_vertexData.push_back(Vertex(pos_bottom_b));
					m_vertexData.push_back(Vertex(pos_bottom_d));

					m_elementData.push_back(position_index + 0);
					m_elementData.push_back(position_index + 1);
					m_elementData.push_back(position_index + 2);

					position_index += 3;
				}

				radians_z += radians_z_delta;
				radians_z_next += radians_z_delta;
			}

			radians_y += radians_y_delta;
			radians_y_next += radians_y_delta;
		}

		m_vertices->bind();

			m_vertices->setData<Vertex>(
				&m_vertexData[0],
				m_vertexData.size(),
				GL_STATIC_DRAW);

		m_vertices->unbind();

		m_elements->bind();

			m_elements->setData<GLuint>(
				&m_elementData[0],
				m_elementData.size(),
				GL_STATIC_DRAW);

		m_elements->unbind();
	}

	void Sphere::render(
		const glm::mat4x4& viewProjection,
		const glm::vec3& position,
		float scale /*= 1.0f*/,
		const glm::vec3& color /*= glm::vec3(1.0f, 1.0f, 1.0f)*/)
	{
		glm::mat4x4 model(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));

		m_uniforms->bind();
			Uniforms* transform = m_uniforms->mapRange<Uniforms>(0, 1, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				transform->modelViewProjection = viewProjection * model;
			m_uniforms->unmap();
		m_uniforms->unbind();

		m_program->bind();

		m_program->setUniform(m_uniformColor, color);

		m_uniforms->bindBase(0);
		m_program->setUniformBlockBinding(m_uniformTransform, 0);

		m_attributes->bind();

		m_elements->bind();

			glDrawElements(
				GL_TRIANGLES,
				m_elementData.size(),
				GL_UNSIGNED_INT,
				nullptr);

		m_elements->unbind();
		m_attributes->unbind();
		m_uniforms->unbind();
		m_program->unbind();
	}

}; // namespace Tmpl