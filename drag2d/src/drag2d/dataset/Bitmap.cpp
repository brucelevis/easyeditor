#include "Bitmap.h"
#include "Image.h"
#include "ISymbol.h"
#include "SymbolMgr.h"

#include "common/FileNameParser.h"
#include "common/Exception.h"
#include "common/TodoConfig.h"
#include "render/Snapshoot.h"

#include <gl/gl.h>
#include <wx/filename.h>

namespace d2d
{

Bitmap::Bitmap()
	: m_bitmap(NULL)
{
}

Bitmap::~Bitmap()
{
	BitmapMgr::Instance()->RemoveItem(m_filename);
	delete m_bitmap;
}

bool Bitmap::LoadFromFile(const std::string& filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw Exception("File: %s don't exist!", filepath.c_str());
	}

#ifndef NOT_LOAD_IMAGE
	m_filename = filepath;
	const GLubyte* test = glGetString(GL_VERSION);
	if (!test) {
		return true;
	}

	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
	}

	if (FileNameParser::isType(filepath, FileNameParser::e_image))
	{
		wxImage image;
		GetImage(filepath, image);

		m_bitmap = GetBitmap(image);
	}
	else if (FileNameParser::isType(filepath, FileNameParser::e_terrain2d))
	{
		;
	}
	else
	{
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::Rect rect = symbol->GetSize();
		int w = std::max(1.0f, rect.xLength()),
			h = std::max(1.0f, rect.yLength());

 		if (w > 2048 || h > 2048) {
 			return true;
 		}

		d2d::Snapshoot ss(w, h);
		unsigned char* rgba = ss.OutputToMemory(symbol, true);
		unsigned char* rgb = TransRGBA2RGB(rgba, w, h);
		delete[] rgba;

		wxImage image(w, h, rgb, true);
		if (m_bitmap) {
			delete m_bitmap;
			m_bitmap = NULL;
		}
		m_bitmap = GetBitmap(image);
		delete[] rgb;
		symbol->Release();
	}
#endif

	return true;
}

unsigned char* Bitmap::TransRGBA2RGB(unsigned char* rgba, int width, int height)
{
	unsigned char* rgb = new unsigned char[width*height*3];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int src = (i*width+j)*4;
			int dst = (i*width+j)*3;
			memcpy(&rgb[dst], &rgba[src], sizeof(unsigned char) * 3);
		}
	}
	return rgb;
}

void Bitmap::GetImage(const std::string& filepath, wxImage& image)
{
	wxImage totimg;
	totimg.LoadFile(filepath);

	Image* pImage = ImageMgr::Instance()->GetItem(filepath);
	Rect rect = pImage->GetClippedRegion();
	// not cache
	pImage->Release();

	float w = totimg.GetWidth();
	float h = totimg.GetHeight();
	// invert y
	float offset = rect.yCenter() * 2;
	rect.translate(Vector(w*0.5f, h*0.5f));

	wxRect wx_rect;
	wx_rect.SetLeft(rect.xMin);
	wx_rect.SetRight(rect.xMax - 1);
	wx_rect.SetTop(rect.yMin - offset);
	wx_rect.SetBottom(rect.yMax - offset - 1);

	image = totimg.GetSubImage(wx_rect);
}

wxBitmap* Bitmap::GetBitmap(const wxImage& image)
{
	float scale = ComputeScale(image.GetWidth());
	int w = std::max(1.0f, image.GetWidth() * scale);
	int h = std::max(1.0f, image.GetHeight() * scale);
	return new wxBitmap(image.Scale(w, h));
}

float Bitmap::ComputeScale(float width)
{
	return width > 300 ? 150.0f / width : 0.5f;
}

} // d2d