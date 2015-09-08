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
		: _library(nullptr)
		, _face(nullptr)
	{
		FT_Error errors = FT_Init_FreeType(&_library);
	}

	FreeTypeLoader::~FreeTypeLoader()
	{
		FT_Error errors = FT_Done_FreeType(_library);
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

		_faceData.resize(font_file_size);
		font_stream.read((char*)&_faceData[0], font_file_size);

		FT_Error errors = 0;
		errors = FT_New_Memory_Face(_library, &_faceData[0], (FT_Long)font_file_size, 0, &_face);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		errors = FT_Set_Char_Size(_face, 0, (FT_F26Dot6)(size * 64.0f), 0, 96);
		if (errors != FT_Err_Ok)
		{
			return false;
		}

		return true;
	}

	float FreeTypeLoader::getLineHeight() const
	{
		FT_Size_Metrics face_metrics = _face->size->metrics;
		return (float)(face_metrics.height >> 6);
	}

	std::shared_ptr<Glyph> FreeTypeLoader::createGlyph(unicode_t codepoint)
	{
		if (_face == nullptr)
		{
			return nullptr;
		}

		FT_UInt index = FT_Get_Char_Index(_face, (FT_ULong)codepoint);
		if (index == 0)
		{
			return nullptr;
		}

		auto found = _glyphs.find(codepoint);
		if (found != _glyphs.end())
		{
			return found->second;
		}

		FT_Error errors = 0;

		errors = FT_Load_Glyph(_face, index, FT_LOAD_DEFAULT);
		if (errors != FT_Err_Ok)
		{
			return nullptr;
		}

		errors = FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
		if (errors != FT_Err_Ok)
		{
			return nullptr;
		}

		std::shared_ptr<Glyph> glyph = std::make_shared<Glyph>();

		FT_GlyphSlot slot = _face->glyph;
		FT_Glyph_Metrics glyph_metrics = slot->metrics;
		FT_Size_Metrics face_metrics = _face->size->metrics;

		glyph->offsetX = (float)(glyph_metrics.horiBearingX >> 6);
		glyph->offsetY = (float)((face_metrics.height - glyph_metrics.horiBearingY - face_metrics.ascender + face_metrics.descender) >> 6);
		glyph->advance = (float)(glyph_metrics.horiAdvance >> 6);

		FT_Bitmap& glyph_bitmap = _face->glyph->bitmap;

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

		_glyphs.insert(std::make_pair(codepoint, glyph));

		return glyph;
	}

}; // namespace Tmpl