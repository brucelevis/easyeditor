#ifndef _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_
#define _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_

#include <ee/Image.h>

#include <vector>

namespace eimage
{

class OutlineToTriStrip
{
public:
	OutlineToTriStrip(const std::vector<sm::vec2>& outline);

	void TransToStrip();

	const std::vector<std::vector<sm::vec2> >& GetStrip() const {
		return m_strips;
	}

private:
	std::vector<sm::vec2> m_outline;

	std::vector<std::vector<sm::vec2> > m_strips;

}; // OutlineToTriStrip

}

#endif // _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_