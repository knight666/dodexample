#include "text/TextBatch.hpp"

#include "graphics/Quad.hpp"

#include "text/FreeTypeLoader.hpp"

namespace Tmpl {

	TextBatch::TextBatch(std::shared_ptr<FreeTypeLoader> loader, GLsizei width, GLsizei height)
		: m_loader(loader)
		, m_width(width)
		, m_height(height)
		, m_quad(new Quad())
	{
		m_pixels = new GLuint[m_width * m_height];

		memset(m_pixels, 0, m_width * m_height * sizeof(GLuint));
	}

	TextBatch::~TextBatch()
	{
		delete m_pixels;
		m_pixels = nullptr;
	}

	void TextBatch::setText(const char* text)
	{
		int32_t errors = 0;

		size_t size_in_bytes = utf8toutf32(text, strlen(text), nullptr, 0, &errors);
		if (size_in_bytes == 0 ||
			errors != 0)
		{
			return;
		}

		clearPixels();

		std::vector<unicode_t> codepoints;
		codepoints.resize(size_in_bytes / sizeof(unicode_t));
		utf8toutf32(text, strlen(text), &codepoints[0], size_in_bytes, nullptr);

		for (unicode_t codepoint : codepoints)
		{
			addCodepoint(codepoint);
		}

		m_quad->setTextureData(m_pixels, m_width, m_height);

		float offset = m_loader->getBaseLineOffset();

		m_model = glm::mat4(1.0f);
		m_model = glm::scale(m_model, glm::vec3(
			(float)m_width, (float)m_height, 1.0f));
		m_model = glm::translate(m_model, glm::vec3(
			0.0f, -offset / (float)m_height, 0.0f));
	}

	void TextBatch::render(const glm::mat4x4& projection, const glm::vec2& position)
	{
		glm::mat4x4 viewProjection =
			glm::translate(projection, glm::vec3(position, 0.0f));

		m_quad->render(viewProjection * m_model);
	}

	void TextBatch::clearPixels()
	{
		memset(m_pixels, 0, m_width * m_height * sizeof(GLuint));

		m_cursor.x = 0.0f;
		m_cursor.y = m_loader->getLineHeight();
	}

	void TextBatch::addCodepoint(unicode_t codepoint)
	{
		if (codepoint == '\n')
		{
			m_cursor.x = 0.0f;
			m_cursor.y += m_loader->getLineHeight();
		}
		else
		{
			std::shared_ptr<Glyph> glyph = m_loader->createGlyph(codepoint);
			if (glyph->bitmapData != nullptr)
			{
				renderBitmap(glyph);
			}

			m_cursor.x += glyph->advance;
		}
	}

	void TextBatch::renderBitmap(std::shared_ptr<Glyph> glyph)
	{
		float from_x = m_cursor.x + glyph->offsetX;
		float to_x = glm::clamp(from_x + glyph->bitmapWidth, 0.0f, (float)m_width);

		float from_y = m_cursor.y + glyph->offsetY;
		float to_y = glm::clamp(from_y + (float)glyph->bitmapHeight, 0.0f, (float)m_height);

		if (from_x > m_width || to_x <= 0.0f ||
			from_y > m_height || to_y <= 0.0f)
		{
			return;
		}

		GLuint* dst = m_pixels + ((GLuint)from_y * m_width) + (GLuint)from_x;
		GLuint* dst_end = m_pixels + (m_width * m_height);

		uint32_t* src = glyph->bitmapData;
		uint32_t src_pitch = glyph->bitmapWidth;

		uint32_t render_width = (uint32_t)(to_x - from_x);
		uint32_t render_height = (uint32_t)(to_y - from_y);

		for (uint32_t y = 0; y < render_height; ++y)
		{
			if (dst >= m_pixels &&
				dst + src_pitch < dst_end)
			{
				uint32_t* dst_line = dst;
				uint32_t* src_line = src;

				for (uint32_t x = 0; x < render_width; ++x)
				{
					uint32_t src_a = (*src_line & 0x000000FF);
					uint32_t src_r = (*src_line & 0x0000FF00) >> 8;
					uint32_t src_g = (*src_line & 0x00FF0000) >> 16;
					uint32_t src_b = (*src_line & 0xFF000000) >> 24;

					uint32_t dst_a = (*dst_line & 0x000000FF);
					uint32_t dst_r = (*dst_line & 0x0000FF00) >> 8;
					uint32_t dst_g = (*dst_line & 0x00FF0000) >> 16;
					uint32_t dst_b = (*dst_line & 0xFF000000) >> 24;

					GLuint rgba =
						((((src_a * src_a) + (dst_a * (255 - src_a))) >> 8) & 0x000000FF) |
						((((src_r * src_a) + (dst_r * (255 - src_a))) >> 8) & 0x000000FF) << 8 |
						((((src_g * src_a) + (dst_g * (255 - src_a))) >> 8) & 0x000000FF) << 16 |
						((((src_b * src_a) + (dst_b * (255 - src_a))) >> 8) & 0x000000FF) << 24;

					*dst_line = rgba;

					++dst_line;
					++src_line;
				}
			}

			dst += m_width;
			src += src_pitch;
		}
	}

}; // namespace Tmpl