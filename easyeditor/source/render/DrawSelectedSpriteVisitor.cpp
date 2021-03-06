#include "DrawSelectedSpriteVisitor.h"
#include "Sprite.h"
#include "BoundingBox.h"
#include "EE_RVG.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageSprite.h"
#include "Image.h"
#include "color_config.h"
#include "Math2D.h"

#include <vector>

namespace ee
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const s2::Color& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::Visit(Object* object, bool& next) 
{
	std::vector<sm::vec2> bound;
	Sprite* sprite = static_cast<Sprite*>(object);
	sprite->GetBounding()->GetBoundPos(bound);

	RVG::Color(m_color);
	RVG::Polyline(bound, true);

	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(sprite))
		{
			RVG::Color(LIGHT_GREY);
			ee::RVG::LineWidth(1);

			Image* img = s->GetSymbol().GetImage();
			float hw = img->GetOriginWidth() * 0.5f,
				hh = img->GetOriginHeight() * 0.5f;

			sm::mat4 mt = s->GetTransMatrix();
			sm::vec2 min(-hw, -hh), max(hw, hh);
			min = ee::Math2D::TransVector(min, mt);
			max = ee::Math2D::TransVector(max, mt);

			RVG::Rect(min, max, false);

			ee::RVG::LineWidth(2);
		}
	}

	next = true;
}

}