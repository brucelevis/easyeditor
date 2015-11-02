#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackAnchor.h"
#include "PackClipbox.h"
#include "Utility.h"
#include "typedef.h"

#include "AnimToLuaString.h"
#include "AnimFromLua.h"

#include "AnimToBin.h"
#include "AnimFromBin.h"

namespace librespacker
{

PackAnimation::PackAnimation(int id)
	: IPackNode(id)
{
}

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, 
									const d2d::TexturePacker& tp,
									float scale) const
{
	AnimToLuaString::Pack(this, gen);
}

void PackAnimation::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	AnimFromLua::Unpack(L, this);
}

int PackAnimation::SizeOfPackToBin() const
{
	return AnimToBin::Size(this);
}

void PackAnimation::PackToBin(uint8_t** ptr, 
							  const d2d::TexturePacker& tp,
							  float scale) const
{
	AnimToBin::Pack(this, ptr);
}

int PackAnimation::SizeOfUnpackFromBin() const
{
	return AnimFromBin::Size(this);
}

void PackAnimation::UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images)
{
	AnimFromBin::Unpack(ptr, this);
}

void PackAnimation::CreateFramePart(const d2d::ISprite* spr, Frame& frame)
{	
	const IPackNode* node = PackNodeFactory::Instance()->Create(spr);

	PackAnimation::Part part;
	std::string name = "";
	if (Utility::IsNameValid(spr->name)) {
		name = spr->name;
	}

	bool force_mat = AddComponent(node, name, part.comp_idx);
	PackAnimation::LoadSprTrans(spr, part.t, force_mat);

	frame.parts.push_back(part);
}

void PackAnimation::CreateClipboxFramePart(const PackClipbox* cb, Frame& frame)
{
	PackAnimation::Part part;

	AddComponent(cb, "", part.comp_idx);

	part.t.mat[0] = part.t.mat[3] = 1024;
	part.t.mat[1] = part.t.mat[2] = 0;
	part.t.mat[4] = cb->x * SCALE;
	part.t.mat[5] = - cb->y * SCALE;

	frame.parts.push_back(part);
}

void PackAnimation::Clear()
{
	export_name.clear();
	components.clear();
	actions.clear();
	frames.clear();
}

bool PackAnimation::AddComponent(const IPackNode* node, const std::string& name, int& comp_idx)
{
	if (const PackAnchor* anchor = dynamic_cast<const PackAnchor*>(node)) {
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (dynamic_cast<const PackAnchor*>(components[i].node) && components[i].name == name) {
				comp_idx = i;
				return true;
			}
		}
	} else {
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node == node && components[i].name == name) {
				comp_idx = i;
				return true;
			}
		}

		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node->GetFilepath() == node->GetFilepath() 
				&& components[i].name == name
				&& !name.empty()) 
			{
				d2d::FileNameParser::Type type = d2d::FileNameParser::getFileType(node->GetFilepath());
				if (type == d2d::FileNameParser::e_image ||
					type == d2d::FileNameParser::e_complex ||
					type == d2d::FileNameParser::e_anim) {
					comp_idx = i;
					return true;
				}
			}
		}
	}

	Component comp;
	comp.node = node;
	comp.name = name;
	components.push_back(comp);
	comp_idx = components.size() - 1;

	if (d2d::FileNameParser::isType(node->GetFilepath(), d2d::FileNameParser::e_image)) {
		return false;
	} else {
		return !name.empty();
	}
}

void PackAnimation::LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans, bool force_mat)
{
	LoadSprMat(spr, trans, force_mat);
	LoadSprColor(spr, trans);
}

void PackAnimation::LoadSprMat(const d2d::ISprite* spr, SpriteTrans& trans, bool force)
{
	if (!force && dynamic_cast<const d2d::ImageSprite*>(spr)) {
		return;
	}

	float mat[6];

	// | 1  ky    | | sx       | |  c  s    | | 1       |
	// | kx  1    | |    sy    | | -s  c    | |    1    |
	// |        1 | |        1 | |        1 | | x  y  1 |
	//     skew        scale        rotate        move

	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	d2d::Vector center = spr->GetCenter();

	bool xmir, ymir;
	spr->GetMirror(xmir, ymir);

	float sx = spr->GetScale().x,
		sy = spr->GetScale().y;
	if (xmir) {
		sx = -sx;
	}
	if (ymir) {
		sy = -sy;
	}

	float c = cos(-spr->GetAngle()),
		s = sin(-spr->GetAngle());
	float kx = -spr->GetShear().x,
		ky = -spr->GetShear().y;
	mat[0] = sx*c - ky*sy*s;
	mat[1] = sx*s + ky*sy*c;
	mat[2] = kx*sx*c - sy*s;
	mat[3] = kx*sx*s + sy*c;
	mat[4] = center.x/* * m_scale*/;
	mat[5] = center.y/* * m_scale*/;

	for (size_t i = 0; i < 4; ++i)
		trans.mat[i] = floor(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		trans.mat[i] = floor(mat[i] * 16 + 0.5f);
	// flip y
	trans.mat[5] = -trans.mat[5];
}

void PackAnimation::LoadSprColor(const d2d::ISprite* spr, SpriteTrans& trans)
{
	trans.color = d2d::trans_color2int(spr->multiCol, d2d::PT_ARGB);
	trans.additive = d2d::trans_color2int(spr->addCol, d2d::PT_ARGB);

	trans.rmap = d2d::trans_color2int(spr->r_trans, d2d::PT_RGBA);
	trans.gmap = d2d::trans_color2int(spr->g_trans, d2d::PT_RGBA);
	trans.bmap = d2d::trans_color2int(spr->b_trans, d2d::PT_RGBA);	
}

bool PackAnimation::IsMatrixIdentity(const int* mat)
{
	return mat[0] == 1024 && mat[3] == 1024 
		&& mat[1] == 0 && mat[2] == 0 && mat[4] == 0 && mat[5] == 0;
}

}