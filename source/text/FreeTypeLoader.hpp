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
		void loadGlyphRange(unicode_t codePointFrom, unicode_t codePointTo);

		float getBaseLineOffset() const;
		float getLineHeight() const;

		std::shared_ptr<Glyph> createGlyph(unicode_t codepoint);

	private:

		bool renderGlyphBitmap(std::shared_ptr<Glyph> glyph, FT_UInt index);

		FT_Library m_library;
		std::vector<FT_Byte> m_faceData;
		FT_Face m_face;
		std::map<FT_UInt, std::shared_ptr<Glyph>> m_glyphs;
		std::shared_ptr<Glyph> m_glyphReplacement;

	};

}; // namespace Tmpl