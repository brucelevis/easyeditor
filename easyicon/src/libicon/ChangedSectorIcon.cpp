#include "ChangedSectorIcon.h"

#include <ee/Math2D.h>
#include <ee/Image.h>

#include <assert.h>

namespace eicon
{

ChangedSectorIcon::ChangedSectorIcon()
	: m_min(SM_PI * 0.5f)
	, m_max(0)
{
}

void ChangedSectorIcon::LoadFromFile(const Json::Value& value)
{
}

void ChangedSectorIcon::StoreToFile(Json::Value& value) const
{
}

void ChangedSectorIcon::GetBound(float process, sm::vec2 bound[4]) const
{
	bound[0].Set(0, 0);
	bound[1].Set(0, 1);

	float angle = m_min + (m_max - m_min) * process;
	float tan_val = tan(angle);

	float w = static_cast<float>(m_img->GetClippedWidth()),
		  h = static_cast<float>(m_img->GetClippedHeight());
	float x = h / tan_val;
	if (x < w) {
		bound[2].Set(x / w, 1.0f);
		bound[3] = bound[2];
	} else {
		float y = w * tan_val;
		assert(y <= h);
		bound[2].Set(1, 1);
		bound[3].Set(1, y / h);
	}
}

}