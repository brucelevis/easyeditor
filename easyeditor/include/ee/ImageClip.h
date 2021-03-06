#ifndef _EASYEDITOR_IMAGE_CLIP_H_
#define _EASYEDITOR_IMAGE_CLIP_H_

#include <SM_Rect.h>

#include <stdint.h>

namespace ee
{

class ImageData;

class ImageClip
{
public:
	ImageClip(const ImageData& img_data, bool check_duplicate = false);
	~ImageClip();

	const uint8_t* Clip(const sm::rect& r) const;
	const uint8_t* Clip(int xmin, int xmax, int ymin, int ymax) const;

private:
	const ImageData& m_img_data;

	bool* m_check;

}; // ImageClip

}

#endif // _EASYEDITOR_IMAGE_CLIP_H_