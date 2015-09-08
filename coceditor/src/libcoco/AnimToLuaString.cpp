#include "AnimToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

void AnimToLuaString::Pack(const PackAnimation* anim, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + anim->GetFilepath());

	lua::assign_with_end(gen, "type", "\"animation\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(anim->GetID()));
	if (!anim->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + anim->export_name + "\"");
	}

	// component
	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = anim->components.size(); i < n; ++i) {
			const PackAnimation::Component& comp = anim->components[i];

			std::string id_str = lua::assign("id", d2d::StringTools::ToString(comp.node->GetID()));
			if (comp.name.empty()) {
				lua::tableassign(gen, "", 1, id_str.c_str());
			} else {
				std::string name_str = lua::assign("name", "\""+comp.name+"\"");
				lua::tableassign(gen, "", 2, id_str.c_str(), name_str.c_str());
			}
		}
	}

	// actions
	int s_frame = 0;
	for (int i = 0, n = anim->actions.size(); i < n; ++i)
	{
		const PackAnimation::Action& action = anim->actions[i];

		lua::TableAssign ta(gen, "", true);

		if (!action.name.empty()) {
			gen.line(lua::assign("action", "\"" + action.name + "\"") + ",");
		}

		// frames
		int e_frame = s_frame + action.size;
		for (int j = s_frame; j < e_frame; ++j) {
			const PackAnimation::Frame& frame = anim->frames[j];
			lua::TableAssign ta(gen, "", true);
			PackFrame(frame, gen);
		}
	}

	gen.detab();
	gen.line("},");
}

void AnimToLuaString::PackFrame(const PackAnimation::Frame& frame, ebuilder::CodeGenerator& gen)
{
	for (int i = 0, n = frame.parts.size(); i < n; ++i) 
	{
		const PackAnimation::Part& part = frame.parts[i];
		const PackAnimation::SpriteTrans& t = part.t;

		std::vector<std::string> params;

		// index
		std::string idx_str = d2d::StringTools::ToString(part.comp_idx);
		std::string idx_assign = lua::assign("index", idx_str);
		params.push_back(idx_assign);

		// mat
		if (!PackAnimation::IsMatrixIdentity(t.mat)) {
			std::string m[6];
			for (int i = 0; i < 6; ++i) {
				m[i] = d2d::StringTools::ToString(t.mat[i]);
			}
			std::string mat_str = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
				m[3].c_str(), m[4].c_str(), m[5].c_str());
			params.push_back(lua::assign("mat", mat_str));
		}

		// color
		if (t.color != 0xffffffff) {
			params.push_back(lua::assign("color", d2d::StringTools::ToString(t.color)));
		}
		if (t.additive != 0) {
			params.push_back(lua::assign("add", d2d::StringTools::ToString(t.additive)));
		}
		if (t.rmap != 0xffff0000 || t.gmap != 0xff00ff00 || t.bmap != 0xff0000ff) {
			params.push_back(lua::assign("rmap", d2d::StringTools::ToString(t.rmap)));
			params.push_back(lua::assign("gmap", d2d::StringTools::ToString(t.gmap)));
			params.push_back(lua::assign("bmap", d2d::StringTools::ToString(t.bmap)));
		}

		if (params.size() > 1) {
			lua::tableassign(gen, "", params);
		} else {
			gen.line(idx_str + ",");
		}
	}
}

}