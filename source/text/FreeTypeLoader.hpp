#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	struct Glyph
	{
		Glyph();
		~Glyph();

		float offsetX;
		float offsetY;
		float advance;
		glm::vec2 boundingBoxMinimum;
		glm::vec2 boundingBoxMaximum;
		uint32_t bitmapWidth;
		uint32_t bitmapHeight;
		uint32_t* bitmapData;
	};

	class FreeTypeLoader
	{

	public:

		FreeTypeLoader();
		~FreeTypeLoader();

		bool loadFace(const char* path, float size);

		std::shared_ptr<Glyph> createGlyph(unicode_t codepoint);

	private:

		FT_Library _library;
		std::vector<FT_Byte> _faceData;
		FT_Face _face;
		std::map<FT_UInt, std::shared_ptr<Glyph>> _glyphs;

	};

}; // namespace Tmpl