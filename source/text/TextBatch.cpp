#include "text/TextBatch.hpp"

#include "graphics/Quad.hpp"

#include "text/FreeTypeLoader.hpp"

namespace Tmpl {

	TextBatch::TextBatch(std::shared_ptr<FreeTypeLoader> loader, GLsizei width, GLsizei height)
		: _loader(loader)
		, _width(width)
		, _height(height)
		, _quad(new Quad())
	{
		_pixels = new GLuint[_width * _height];

		memset(_pixels, 0, _width * _height * sizeof(GLuint));
	}

	TextBatch::~TextBatch()
	{
		delete _pixels;
		_pixels = nullptr;
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

		_quad->setTextureData(_pixels, _width, _height);
	}

	void TextBatch::render(const glm::mat4x4& camera)
	{
		_quad->render(camera);
	}

	void TextBatch::clearPixels()
	{
		memset(_pixels, 0, _width * _height * sizeof(GLuint));
	}

	void TextBatch::addCodepoint(unicode_t codepoint)
	{
		std::shared_ptr<Glyph> glyph = _loader->createGlyph(codepoint);
		if (glyph == nullptr)
		{
			glyph = _loader->createGlyph(0xFFFD);
			if (glyph == nullptr)
			{
				return;
			}
		}

		if (glyph->bitmapData != nullptr)
		{
			renderBitmap(glyph);
		}

		_cursor.x += glyph->advance;
	}

	void TextBatch::renderBitmap(std::shared_ptr<Glyph> glyph)
	{
		uint32_t from_x = (uint32_t)(_cursor.x + glyph->boundingBoxMinimum.x);
		uint32_t to_x = std::max(std::min(from_x + glyph->bitmapWidth, (uint32_t)_width), (uint32_t)0);

		uint32_t from_y = (uint32_t)(_cursor.y + glyph->boundingBoxMinimum.y);
		uint32_t to_y = std::max(std::min(from_y + glyph->bitmapHeight, (uint32_t)_height), (uint32_t)0);

		if (from_x > _width || to_x == 0 ||
			from_y > _height || to_y == 0)
		{
			return;
		}

		GLuint* dst = _pixels + (from_y * _width) + from_x;
		GLuint* dst_end = _pixels + (_width * _height);

		uint32_t* src = glyph->bitmapData;
		uint32_t src_pitch = glyph->bitmapWidth;

		for (uint32_t y = from_y; y < to_y; ++y)
		{
			if (dst >= _pixels &&
				dst + src_pitch < dst_end)
			{
				uint32_t* dst_line = dst;
				uint32_t* src_line = src;

				for (uint32_t x = from_x; x < to_x; ++x)
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

			dst += _width;
			src += src_pitch;
		}
	}

}; // namespace Tmpl