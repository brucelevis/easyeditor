#ifndef _EASYEDITOR_TEXTURE_IMG_DATA_H_
#define _EASYEDITOR_TEXTURE_IMG_DATA_H_

#include "Texture.h"

namespace ee
{

class ImageData;

class TextureImgData : public Texture
{
public:
	TextureImgData();
	~TextureImgData();
	
	virtual unsigned int GetTexID() const { return m_texid; }

	virtual std::string GetFilepath() const;

	virtual const uint8_t* GetPixelData() const;
	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetChannels() const;

	virtual void LoadFromFile(const std::string& filepath);
	virtual void LoadFromMemory(ImageData* img_data);
	virtual void Reload();

	virtual const ImageData* GetImageData() const { return m_img_data; }

private:
	unsigned int m_texid;

	ImageData* m_img_data;

}; // TextureImgData

}

#endif // _EASYEDITOR_TEXTURE_IMG_DATA_H_