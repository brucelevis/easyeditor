#include "Particle3DToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace librespacker
{

void Particle3DToLuaString::Pack(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + p3d->GetFilepath());

	lua::assign_with_end(gen, "type", "\"particle3d\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(p3d->GetID()));
	if (!p3d->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + p3d->export_name + "\"");
	}

	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = p3d->components.size(); i < n; ++i) {
			PackComponent(p3d->components[i], gen);
		}
	}
	PackPS(p3d, gen);

	gen.detab();
	gen.line("},");
}

void Particle3DToLuaString::PackPS(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen)
{
	lua::connect(gen, 1, 
		lua::assign("capacity", p3d->capacity));

	lua::connect(gen, 2, 
		lua::assign("emission_time", TransTime(p3d->emission_time)), 
		lua::assign("count", p3d->count));

	lua::connect(gen, 2, 
		lua::assign("life", TransTime(p3d->life)), 
		lua::assign("life_var", TransTime(p3d->life_var)));

	lua::connect(gen, 4, 
		lua::assign("hori", TransRadian(p3d->hori)), 
		lua::assign("hori_var", TransRadian(p3d->hori_var)),
		lua::assign("vert", TransRadian(p3d->vert)), 
		lua::assign("vert_var", TransRadian(p3d->vert_var)));

	lua::connect(gen, 4, 
		lua::assign("spd", TransFloat(p3d->spd)), 
		lua::assign("spd_var", TransFloat(p3d->spd_var)),
		lua::assign("angular_spd", TransRadian(p3d->angular_spd)), 
		lua::assign("angular_spd_var", TransRadian(p3d->angular_spd_var)));

	lua::connect(gen, 4, 
		lua::assign("dis_region", TransFloat(p3d->dis_region)), 
		lua::assign("dis_region_var", TransFloat(p3d->dis_region_var)),
		lua::assign("dis_spd", TransFloat(p3d->dis_spd)), 
		lua::assign("dis_spd_var", TransFloat(p3d->dis_spd_var)));

	lua::connect(gen, 1, 
		lua::assign("gravity", TransFloat(p3d->gravity)));

	lua::connect(gen, 2, 
		lua::assign("linear_acc", TransFloat(p3d->linear_acc)), 
		lua::assign("linear_acc_var", TransFloat(p3d->linear_acc_var)));

	lua::connect(gen, 1, 
		lua::assign("fadeout_time", TransTime(p3d->fadeout_time)));

	lua::connect(gen, 1, 
		lua::assign("bounce", TransBool(p3d->bounce)));

	lua::connect(gen, 2, 
		lua::assign("start_radius", TransFloat(p3d->start_radius)), 
		lua::assign("is_start_radius_3d", TransBool(p3d->is_start_radius_3d)));

	lua::connect(gen, 1, 
		lua::assign("orient_to_movement", TransBool(p3d->orient_to_movement)));
}

void Particle3DToLuaString::PackComponent(const PackParticle3D::Component& comp, 
										  ebuilder::CodeGenerator& gen)
{
	lua::TableAssign ta(gen, "", true);

	lua::connect(gen, 1, 
		lua::assign("id", comp.node->GetID()));

	lua::connect(gen, 2, 
		lua::assign("scale_start", TransFloatX100(comp.scale_start)), 
		lua::assign("scale_end", TransFloatX100(comp.scale_end)));

	lua::connect(gen, 2, 
		lua::assign("angle", TransFloat(comp.angle)), 
		lua::assign("angle_var", TransFloat(comp.angle_var)));

	lua::connect(gen, 4, 
		lua::assign("col_mul", comp.col_mul), 
		lua::assign("col_add", comp.col_add),
		lua::assign("alpha_start", TransFloat(comp.alpha_start)), 
		lua::assign("alpha_end", TransFloat(comp.alpha_end)));
}

int Particle3DToLuaString::TransTime(float time)
{
	return floor(time * 1000 + 0.5f);
}

int Particle3DToLuaString::TransRadian(float r)
{
	return floor(r * d2d::TRANS_RAD_TO_DEG + 0.5f);
}

int Particle3DToLuaString::TransFloat(float f)
{
	return floor(f + 0.5f);
}

int Particle3DToLuaString::TransFloatX100(float f)
{
	return TransFloat(100 * f);
}

std::string Particle3DToLuaString::TransBool(bool b)
{
	return b ? "true" : "false";
}

}