#include "Particle3DBuilder.h"
#include "PackParticle3D.h"
#include "PackNodeFactory.h"

#include <easyparticle3d.h>
#include <ps/particle3d.h>

namespace libcoco
{

Particle3DBuilder::Particle3DBuilder()
{
}

Particle3DBuilder::~Particle3DBuilder()
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void Particle3DBuilder::Traverse(d2d::IVisitor& visitor) const
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle3D*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Particle3DBuilder::Create(const eparticle3d::Symbol* symbol)
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackParticle3D* node = new PackParticle3D;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void Particle3DBuilder::Load(const eparticle3d::Symbol* symbol, PackParticle3D* ps)
{
	const ps_cfg_3d* cfg = symbol->GetPS()->GetConfig();

	ps->emission_time = cfg->emission_time;
	ps->count = cfg->count;

	ps->life = cfg->life;
	ps->life_var = cfg->life_var;

	ps->hori = cfg->hori;
	ps->hori_var = cfg->hori_var;
	ps->vert = cfg->vert;
	ps->vert_var = cfg->vert_var;

	ps->spd = cfg->spd;
	ps->spd_var = cfg->spd_var;
	ps->angular_spd = cfg->angular_spd;
	ps->angular_spd_var = cfg->angular_spd_var;

	ps->dis_region = cfg->dis_region;
	ps->dis_region_var = cfg->dis_region_var;
	ps->dis_spd = cfg->dis_spd;
	ps->dis_spd_var = cfg->dis_spd_var;

	ps->gravity = cfg->gravity;

	ps->linear_acc = cfg->linear_acc;
	ps->linear_acc_var = cfg->linear_acc_var;

	ps->fadeout_time = cfg->fadeout_time;

	ps->bounce = cfg->bounce;

	ps->start_radius = cfg->start_radius;
	ps->is_start_radius_3d = cfg->is_start_radius_3d;

	ps->orient_to_movement = cfg->orient_to_movement;

	ps->components.reserve(cfg->symbol_count);
	for (int i = 0; i < cfg->symbol_count; ++i) {
		const particle_symbol& p_symbol = cfg->symbols[i];

		PackParticle3D::Component comp;

		comp.scale_start = p_symbol.scale_start;
		comp.scale_end = p_symbol.scale_end;

		comp.angle = p_symbol.angle;
		comp.angle_var = p_symbol.angle_var;

		d2d::Colorf d2d_mul(p_symbol.col_mul.r, p_symbol.col_mul.g, p_symbol.col_mul.b, p_symbol.col_mul.a);
		comp.col_mul = d2d::trans_color2int(d2d_mul, d2d::PT_BGRA);
		d2d::Colorf d2d_add(p_symbol.col_add.r, p_symbol.col_add.g, p_symbol.col_add.b, p_symbol.col_add.a);
		comp.col_add = d2d::trans_color2int(d2d_add, d2d::PT_BGRA);
		comp.alpha_start = p_symbol.alpha_start;
		comp.alpha_end = p_symbol.alpha_end;

		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(p_symbol.ud);
		comp.node = PackNodeFactory::Instance()->Create(symbol);

		ps->components.push_back(comp);
	}
}

}