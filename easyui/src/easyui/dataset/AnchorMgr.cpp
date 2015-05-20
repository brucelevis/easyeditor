#include "AnchorMgr.h"

#include "frame/SettingCfg.h"

namespace eui
{

AnchorMgr::AnchorMgr()
{
	SettingCfg* cfg = SettingCfg::Instance();
	OnViewChanged(cfg->m_view_width, cfg->m_view_height);
}

AnchorMgr::~AnchorMgr()
{
	for (int i = 0; i < 6; ++i) {
		Anchor& anchor = m_anchors[i];
		for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
			anchor.sprites[i]->Release();
		}
	}
}

void AnchorMgr::Insert(d2d::ISprite* spr)
{
	for (int i = 0; i < 6; ++i)
	{
		Anchor& anchor = m_anchors[i];
		if (d2d::Math::getDistance(anchor.pos, spr->getPosition()) < RADIUS) {
			anchor.sprites.push_back(spr);
			spr->setTransform(anchor.pos, 0);
			break;
		}
	}
}

void AnchorMgr::Remove(d2d::ISprite* spr)
{
	for (int i = 0; i < 6; ++i) 
	{
		Anchor& anchor = m_anchors[i];
		if (anchor.sprites.empty()) {
			continue;
		}

		std::vector<d2d::ISprite*>::iterator itr = anchor.sprites.begin();
		for ( ; itr != anchor.sprites.end(); ++itr) {
			if (*itr == spr) {
				spr->Release();
				anchor.sprites.erase(itr);
				break;
			}
		}
	}
}

void AnchorMgr::OnViewChanged(int width, int height)
{
	float hw = width * 0.5f,
		hh = height * 0.5f;
	ChangeAnchorPos(m_anchors[0], d2d::Vector(-hw, hh));
	ChangeAnchorPos(m_anchors[1], d2d::Vector(0.0f, hh));
	ChangeAnchorPos(m_anchors[2], d2d::Vector(hw, hh));
	ChangeAnchorPos(m_anchors[3], d2d::Vector(-hw, -hh));
	ChangeAnchorPos(m_anchors[4], d2d::Vector(0.0f, -hh));
	ChangeAnchorPos(m_anchors[5], d2d::Vector(hw, -hh));
}

void AnchorMgr::Draw() const
{
	for (int i = 0; i < 6; ++i) {
		const Anchor& anchor = m_anchors[i];
		d2d::PrimitiveDraw::drawCircle(anchor.pos, RADIUS, true, 2, d2d::LIGHT_GREY);
	}
}

void AnchorMgr::ChangeAnchorPos(Anchor& anchor, const d2d::Vector& pos)
{
	anchor.pos = pos;
	for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
		anchor.sprites[i]->setTransform(pos, 0);
	}
}

}