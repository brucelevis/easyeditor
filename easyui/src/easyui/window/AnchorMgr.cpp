#include "AnchorMgr.h"

#include "window/QueryWindowViewSizeSJ.h"

namespace eui
{
namespace window
{

AnchorMgr::AnchorMgr()
{
	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);
	OnViewChanged(width, height);

	m_subjects.push_back(d2d::InsertSpriteSJ::Instance());
	m_subjects.push_back(d2d::RemoveSpriteSJ::Instance());
	m_subjects.push_back(d2d::ClearSpriteSJ::Instance());
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->Register(this);
	}
}

AnchorMgr::~AnchorMgr()
{
	Clear();

	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void AnchorMgr::Notify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		Insert(((d2d::InsertSpriteSJ::Params*)ud)->spr);
		break;
	case d2d::MSG_REMOVE_SPRITE:
		Remove((d2d::ISprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void AnchorMgr::OnViewChanged(int width, int height)
{
	float hw = width * 0.5f,
		  hh = height * 0.5f;

	ChangeAnchorPos(m_anchors[0], d2d::Vector(-hw, hh));
	ChangeAnchorPos(m_anchors[1], d2d::Vector(0.0f, hh));
	ChangeAnchorPos(m_anchors[2], d2d::Vector(hw, hh));

	ChangeAnchorPos(m_anchors[3], d2d::Vector(-hw, 0.0f));
	ChangeAnchorPos(m_anchors[4], d2d::Vector(0.0f, 0.0f));
	ChangeAnchorPos(m_anchors[5], d2d::Vector(hw, 0.0f));

	ChangeAnchorPos(m_anchors[6], d2d::Vector(-hw, -hh));
	ChangeAnchorPos(m_anchors[7], d2d::Vector(0.0f, -hh));
	ChangeAnchorPos(m_anchors[8], d2d::Vector(hw, -hh));
}

void AnchorMgr::OnSprPosChanged(d2d::ISprite* spr)
{
	Remove(spr);
	Insert(spr);
}

void AnchorMgr::Draw() const
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		const Anchor& anchor = m_anchors[i];
		d2d::PrimitiveDraw::drawCircle(anchor.pos, RADIUS, true, 2, d2d::LIGHT_GREY);
	}
}

void AnchorMgr::LoadFromFile(const Json::Value& value,
							 const std::vector<d2d::ISprite*>& sprites)
{
	if (value["anchor"].size() == 6) {
		for (int i = 0; i < 6; ++i) {
			int dst = i < 3 ? i : i + 3;
			LoadAnchorData(sprites, value["anchor"][i], m_anchors[dst]);
		}
	} else {
		for (int i = 0; i < ANCHOR_COUNT; ++i) {
			LoadAnchorData(sprites, value["anchor"][i], m_anchors[i]);
		}
	}
}

void AnchorMgr::StoreToFile(Json::Value& value) const
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		Json::Value a_val;
		for (int j = 0; j < m_anchors[i].sprites.size(); ++j) {
			d2d::ISprite* spr = m_anchors[i].sprites[j];
			Json::Value spr_val;
			spr_val["name"] = spr->name;
			spr_val["filepath"] = spr->GetSymbol().GetFilepath();
			a_val[j] = spr_val;
		}
		value["anchor"][i] = a_val;
	}
}

void AnchorMgr::Insert(d2d::ISprite* spr)
{
	for (int i = 0; i < ANCHOR_COUNT; ++i)
	{
		Anchor& anchor = m_anchors[i];
		if (d2d::Math::getDistance(anchor.pos, spr->GetPosition()) < RADIUS) {
			spr->Retain();
			anchor.sprites.push_back(spr);
			spr->SetTransform(anchor.pos, 0);
			break;
		}
	}
}

void AnchorMgr::Remove(d2d::ISprite* spr)
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) 
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

void AnchorMgr::Clear()
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		Anchor& anchor = m_anchors[i];
		for_each(anchor.sprites.begin(), anchor.sprites.end(), 
			d2d::ReleaseObjectFunctor<d2d::ISprite>());
		anchor.sprites.clear();
	}
}

void AnchorMgr::ChangeAnchorPos(Anchor& anchor, const d2d::Vector& pos)
{
	anchor.pos = pos;
	for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
		anchor.sprites[i]->SetTransform(pos, 0);
	}
}

void AnchorMgr::LoadAnchorData(const std::vector<d2d::ISprite*>& sprites,
							   const Json::Value& value, Anchor& anchor)
{
	int idx = 0;
	Json::Value val = value[idx++];
	while (!val.isNull()) {
		std::string name = val.asString();
		d2d::ISprite* spr = NULL;
		for (int j = 0, m = sprites.size(); j < m; ++j) {
			if (sprites[j]->name == name) {
				spr = sprites[j];
				break;
			}
		}
		if (spr) {
			anchor.sprites.push_back(spr);
			spr->SetTransform(anchor.pos, 0);
		}

		val = value[idx++];
	}
}

}
}