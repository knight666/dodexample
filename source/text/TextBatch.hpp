#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class FreeTypeLoader;
	class Quad;
	struct Glyph;

	class TextBatch
	{

	public:

		TextBatch(std::shared_ptr<FreeTypeLoader> loader, GLsizei width, GLsizei height);
		~TextBatch();

		void setText(const char* text);

		void render(const glm::mat4x4& projection, const glm::vec2& position);

	private:

		void clearPixels();
		void addCodepoint(unicode_t codepoint);
		void renderBitmap(std::shared_ptr<Glyph> glyph);

	private:

		GLsizei m_width;
		GLsizei m_height;
		GLuint* m_pixels;
		glm::vec2 m_cursor;
		std::shared_ptr<FreeTypeLoader> m_loader;
		std::shared_ptr<Quad> m_quad;
		glm::mat4x4 m_model;

	};

}; // namespace Tmpl