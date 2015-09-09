#include "text/FreeTypeLoader.hpp"

namespace Tmpl {

	Glyph::Glyph()
		: offsetX(0.0f)
		, offsetY(0.0f)
		, advance(0.0f)
		, bitmapWidth(0)
		, bitmapHeight(0)
		, bitmapData(nullptr)
	{
	}

	Glyph::~Glyph()
	{
		if (bitmapData != nullptr)
		{
			delete [] bitmapData;
			bitmapData = nullptr;
		}
	}

	FreeTypeLoader::FreeTypeLoader()
		: m_library(nullptr)
		, m_face(nullptr)
	{
		FT_Error errors = FT_Init_FreeType(&m_library);
	}

	FreeTypeLoader::~FreeTypeLoader()
	{
		FT_Error errors = FT_Done_FreeType(m_library);
	}

	bool FreeTypeLoader::loadFace(const char* path, float size)
	{
		std::fstream font_stream(path, std::ios::in | std::ios::binary);
		if (!font_stream.is_open())
		{
			return false;
		}

		font_stream.seekg(0, std::ios_base::end);
		size_t font_file_size = (size_t)font_stream.tellg();
		font_stream.seekg(0, std::ios_base::beg);

		if (font_file_size == 0)
		{
			return false;
		}

		m_faceData.resize(font_file_size);
		font_stream.read((char*)&m_faceData[0], font_file_size);

		FT_Error errors = 0;
		errors = FT_New_Memory_Face(m_library, &m_faceData[0], (FT_Long)font_file_size, 0, &m_face);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		errors = FT_Set_Char_Size(m_face, 0, (FT_F26Dot6)(size * 64.0f), 0, 96);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		m_glyphReplacement = std::make_shared<Glyph>();

		FT_UInt replacementCharacter = FT_Get_Char_Index(m_face, (FT_ULong)0xFFFD);
		if (!renderGlyphBitmap(m_glyphReplacement, replacementCharacter))
		{
			replacementCharacter = FT_Get_Char_Index(m_face, (FT_ULong)'?');
			renderGlyphBitmap(m_glyphReplacement, replacementCharacter);
		}

		return true;
	}

	void FreeTypeLoader::loadGlyphRange(unicode_t codePointFrom, unicode_t codePointTo)
	{
		if (m_face == nullptr)
		{
			return;
		}

		for (unicode_t i = codePointFrom; i <= codePointTo; ++i)
		{
			createGlyph(i);
		}
	}

	float FreeTypeLoader::getBaseLineOffset() const
	{
		FT_Size_Metrics face_metrics = m_face->size->metrics;
		return (float)(face_metrics.ascender >> 6);
	}

	float FreeTypeLoader::getLineHeight() const
	{
		FT_Size_Metrics face_metrics = m_face->size->metrics;
		return (float)(face_metrics.height >> 6);
	}

	std::shared_ptr<Glyph> FreeTypeLoader::createGlyph(unicode_t codepoint)
	{
		if (m_face == nullptr)
		{
			return nullptr;
		}

		auto found = m_glyphs.find(codepoint);
		if (found != m_glyphs.end())
		{
			return found->second;
		}

		std::shared_ptr<Glyph> glyph;

		FT_UInt index = FT_Get_Char_Index(m_face, (FT_ULong)codepoint);
		if (index > 0)
		{
			glyph = std::make_shared<Glyph>();

			if (!renderGlyphBitmap(glyph, index))
			{
				glyph = m_glyphReplacement;
			}
		}
		else
		{
			glyph = m_glyphReplacement;
		}

		m_glyphs.insert(std::make_pair(codepoint, glyph));

		return glyph;
	}

	bool FreeTypeLoader::renderGlyphBitmap(std::shared_ptr<Glyph> glyph, FT_UInt index)
	{
		FT_Error errors = 0;

		errors = FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		errors = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		FT_GlyphSlot slot = m_face->glyph;
		FT_Glyph_Metrics glyph_metrics = slot->metrics;
		FT_Size_Metrics face_metrics = m_face->size->metrics;

		glyph->offsetX = (float)(glyph_metrics.horiBearingX >> 6);
		glyph->offsetY = (float)((face_metrics.height - glyph_metrics.horiBearingY - face_metrics.ascender - face_metrics.descender) >> 6);
		glyph->advance = (float)(glyph_metrics.horiAdvance >> 6);

		FT_Bitmap& glyph_bitmap = m_face->glyph->bitmap;

		FT_BBox bounding_box;
		errors = FT_Outline_Get_BBox(&slot->outline, &bounding_box);
		if (errors == FT_Err_Ok)
		{
			glyph->boundingBoxMinimum.x = (float)(bounding_box.xMin >> 6);
			glyph->boundingBoxMinimum.y = (float)((face_metrics.height - bounding_box.yMin) >> 6);

			glyph->boundingBoxMaximum.x = (float)(bounding_box.xMax >> 6);
			glyph->boundingBoxMinimum.y = (float)((face_metrics.height - bounding_box.yMax) >> 6);
		}

		uint32_t bitmap_size = glyph_bitmap.width * glyph_bitmap.rows;
		if (bitmap_size > 0)
		{
			glyph->bitmapWidth = glyph_bitmap.width;
			glyph->bitmapHeight = glyph_bitmap.rows;
			glyph->bitmapData = new uint32_t[bitmap_size];

			uint8_t* src_line = (uint8_t*)glyph_bitmap.buffer;
			uint32_t src_pitch = glyph_bitmap.pitch;

			uint8_t* dst_line = (uint8_t*)glyph->bitmapData;
			uint32_t dst_pitch = glyph_bitmap.width * sizeof(uint32_t);

			for (int y = 0; y < glyph_bitmap.rows; ++y)
			{
				unsigned char* src = src_line;
				unsigned char* dst = dst_line;

				for (int x = 0; x < glyph_bitmap.width; ++x)
				{
					char value = *src;
					dst[0] = value;
					dst[1] = value;
					dst[2] = value;
					dst[3] = value;

					dst += 4;
					src++;
				}

				src_line += src_pitch;
				dst_line += dst_pitch;
			}
		}

		return true;
	}

}; // namespace Tmpl