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

		GLsizei _width;
		GLsizei _height;
		GLuint* _pixels;
		glm::vec2 _cursor;
		std::shared_ptr<FreeTypeLoader> _loader;
		std::shared_ptr<Quad> _quad;
		glm::mat4x4 _model;

	};

}; // namespace Tmpl