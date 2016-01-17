#include "Texture.h"
#include "PngLoader.h"

#include "render/RenderContext.h"
#include "utility/StringHelper.h"

namespace glue
{

Texture::Texture()
	: m_width(0)
	, m_height(0)
	, m_id(0)
{
}

Texture::~Texture()
{
	RenderContext::Instance()->ReleaseTexture(m_id);
}

void Texture::Load(const std::string& filepath)
{
	if (filepath.find(".png") != std::string::npos) {
		const uint8_t* data = PngLoader::Read(filepath, m_width, m_height, m_format);
		Load(data, m_width, m_height, m_format);
		delete[] data;
	}
}

void Texture::Load(const uint8_t* data, int width, int height, TEXTURE_FORMAT format)
{
	m_id = RenderContext::Instance()->CreateTexture(data, width, height, format);
}

}