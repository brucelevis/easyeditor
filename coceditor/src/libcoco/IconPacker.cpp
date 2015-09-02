#include "IconPacker.h"
#include "PackAnimation.h"

#include "Scale9Packer.h"
#include "ImagePacker.h"
#include "IPackNode.h"
#include "PackPicture.h"

namespace libcoco
{

IconPacker::IconPacker()
{
}

IconPacker::~IconPacker()
{
	std::multimap<const eicon::Symbol*, Value>::iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second.node;
	}
}

void IconPacker::PackToLuaString(ebuilder::CodeGenerator& gen,
						   const TexturePacker& tp) const
{
	std::vector<const IPackNode*> nodes;
	nodes.reserve(m_map_data.size());

	std::multimap<const eicon::Symbol*, Value>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		nodes.push_back(itr->second.node);
	}

	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToLuaString(gen, tp);
	}
}

const IPackNode* IconPacker::Create(const eicon::Sprite* spr)
{
	if (const IPackNode* node = Query(spr)) {
		return node;
	}

	PackPicture* node = new PackPicture;
	Load(spr, node);

	Value val;
	val.proc = spr->GetProcess();
	val.node = node;
	m_map_data.insert(std::make_pair(&spr->GetSymbol(), val));
	return node;
}

const IPackNode* IconPacker::Query(const eicon::Sprite* spr) const
{
	float proc = spr->GetProcess();
	const eicon::Symbol* key = &spr->GetSymbol();
	std::multimap<const eicon::Symbol*, Value>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		const Value& val = itr->second;
		if (val.proc == proc) {
			return val.node;
		}
	}

	return NULL;
}

void IconPacker::Load(const eicon::Sprite* spr, PackPicture* pic)
{
	float proc = spr->GetProcess();

	PackPicture::Quad quad;

	const eicon::Icon* icon = spr->GetSymbol().GetIcon();
	icon->GetTexCoords(proc, quad.texture_coord);
	icon->GetScreenCoords(proc, quad.texture_coord, quad.screen_coord);

	pic->quads.push_back(quad);
}

}