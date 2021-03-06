#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"
#include "PSNode.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <shaderlab.h>
#include <sprite2/RenderParams.h>

#include <fstream>

namespace eparticle3d
{

Symbol::Symbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
	, m_loop(true)
	, m_local(true)
{
	PSNode::Instance();
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
	, m_et_cfg(s.m_et_cfg)
	, m_et(NULL)
{
	PSNode::Instance();
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (!spr) {
		return;
	}

	s2::RenderParams p = params;
	p.color = spr->GetColor() * params.color;

	Sprite* p3d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p3d_spr->SetOuterMatrix(p.mt);
	if (p3d_spr->IsAlone()) {
		p3d_sprite* p3d = p3d_spr->GetP3D();
		if (!p3d) {
			return;
		}
		RenderParams* rp = static_cast<RenderParams*>(p3d->draw_params);
		rp->mat = p.mt;
		rp->ct = p.color;
		sm::mat4 mt = spr->GetTransMatrix() * p.mt;
		p3d->mat[0] = mt.x[0];
		p3d->mat[1] = mt.x[1];
		p3d->mat[2] = mt.x[4];
		p3d->mat[3] = mt.x[5];
		p3d->mat[4] = mt.x[12];
		p3d->mat[5] = mt.x[13];	
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	if (p3d_spr->IsLocalModeDraw()) {
		p.mt = spr->GetTransMatrix() * p.mt;
	}
	p3d_spr->Draw(p);
}

void Symbol::ReloadTexture() const
{
	for (int i = 0; i < m_et_cfg->symbol_count; ++i) {
		ee::Symbol* symbol = static_cast<ee::Symbol*>(m_et_cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void Symbol::Traverse(ee::Visitor& visitor)
{
	if (!m_et_cfg) {
		return;
	}
	for (int i = 0; i < m_et_cfg->symbol_count; ++i)
	{
		const p3d_symbol& p_symbol = m_et_cfg->symbols[i];
		ee::Symbol* symbol = static_cast<ee::Symbol*>(p_symbol.ud);
		symbol->Traverse(visitor);
	}
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = p3d_emitter_create(m_et_cfg);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	m_loop = value["loop"].asBool();
	m_local = value["local"].asBool();
}

}