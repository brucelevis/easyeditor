#include "FileIO.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ParticleSystem.h"
#include "config.h"

namespace eparticle3d
{

void FileIO::store(const char* filepath, ToolbarPanel* toolbar)
{
	Json::Value value;

	value["name"] = toolbar->m_name->GetValue().ToStdString();
	value["package"] = toolbar->m_package->GetValue().ToStdString();
	value["count"] = toolbar->m_count->GetValue();
	value["layer"] = toolbar->m_layer->GetValue();
	value["emission_time"] = toolbar->m_emission_time->GetValue();
	value["min_life"] = toolbar->m_min_life->GetValue();
	value["max_life"] = toolbar->m_max_life->GetValue();
	value["min_hori"] = toolbar->m_min_hori->GetValue();
	value["max_hori"] = toolbar->m_max_hori->GetValue();
	value["min_vert"] = toolbar->m_min_vert->GetValue();
	value["max_vert"] = toolbar->m_max_vert->GetValue();
	value["min_spd"] = toolbar->m_min_spd->GetValue();
	value["max_spd"] = toolbar->m_max_spd->GetValue();
	value["gravity"] = toolbar->m_gravity->GetValue();
	value["inertia"] = toolbar->m_inertia->GetValue();
	value["fadeout_time"] = toolbar->m_fadeout_time->GetValue();
	value["bounce"] = toolbar->m_bounce->GetValue();
	value["additive_blend"] = toolbar->m_additiveBlend->GetValue();
	value["start_radius"] = toolbar->m_start_radius->GetValue();
	value["orient_to_movement"] = toolbar->m_orient_to_movement->GetValue();
	value["orient_to_parent"] = toolbar->m_orient_to_parent->GetValue();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	for (size_t i = 0, n = toolbar->m_children.size(); i < n; ++i)
	{
		ToolbarPanel::ChildPanel* cp = toolbar->m_children[i];
		ParticleChild* pc = cp->m_pc;
		value["components"][i]["filepath"] = 
			d2d::FilenameTools::getRelativePath(dir, pc->symbol->getFilepath()).ToStdString();
		if (pc->bind_ps) {
			value["components"][i]["bind ps filepath"] = 
				d2d::FilenameTools::getRelativePath(dir, pc->bind_ps->filepath).ToStdString();
		}
		value["components"][i]["name"] = cp->m_name->GetValue().ToStdString();
		value["components"][i]["start_scale"] = cp->m_start_scale->GetValue();
		value["components"][i]["end_scale"] = cp->m_end_scale->GetValue();
		value["components"][i]["min_rotate"] = cp->m_min_rotate->GetValue();
		value["components"][i]["max_rotate"] = cp->m_max_rotate->GetValue();
		value["components"][i]["start_z"] = cp->m_startz->GetValue();
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::load(const char* filepath, ParticleSystem* ps,
				  ToolbarPanel* toolbar)
{
	FileAdapter adapter;
	adapter.load(filepath);

	toolbar->m_name->SetValue(adapter.name);
	toolbar->m_package->SetValue(adapter.package);
	toolbar->m_count->SetValue(adapter.count);
	ps->setCount(adapter.count);
	toolbar->m_layer->SetValue(adapter.layer);
	toolbar->m_emission_time->SetValue(adapter.emission_time);
	ps->setEmissionTime(adapter.emission_time);
	toolbar->m_min_life->SetValue(adapter.min_life);
	toolbar->m_max_life->SetValue(adapter.max_life);
	ps->setLife(adapter.min_life, adapter.max_life);
	toolbar->m_min_hori->SetValue(adapter.min_hori);
	toolbar->m_max_hori->SetValue(adapter.max_hori);
	ps->setHori(adapter.min_hori, adapter.max_hori);
	toolbar->m_min_vert->SetValue(adapter.min_vert);
	toolbar->m_max_vert->SetValue(adapter.max_vert);
	ps->setVert(adapter.min_vert, adapter.max_vert);
	toolbar->m_min_spd->SetValue(adapter.min_spd);
	toolbar->m_max_spd->SetValue(adapter.max_spd);
	ps->setSpeed(adapter.min_spd, adapter.max_spd);
	toolbar->m_gravity->SetValue(adapter.gravity);
	ps->setGravity(adapter.gravity);
	toolbar->m_inertia->SetValue(adapter.inertia);
	toolbar->m_fadeout_time->SetValue(adapter.fadeout_time);
	ps->setFadeoutTime(adapter.fadeout_time);
	toolbar->m_bounce->SetValue(adapter.bounce);
	ps->setBounce(adapter.bounce);
	toolbar->m_additiveBlend->SetValue(adapter.additive_blend);
	toolbar->m_start_radius->SetValue(adapter.start_radius);
	ps->setStartRadius(adapter.start_radius);
	toolbar->m_orient_to_movement->SetValue(adapter.orient_to_movement);
	toolbar->m_orient_to_parent->SetValue(adapter.orient_to_parent);
	for (size_t i = 0, n = adapter.children.size(); i < n; ++i) {
		toolbar->add(adapter.children[i]);
	}
}

ParticleSystem* FileIO::LoadPS(const char* filepath)
{
	FileAdapter adapter;
	adapter.load(filepath);

	ParticleSystem* ps = new ParticleSystem(PARTICLE_CAP);

	ps->filepath = filepath;
	ps->setCount(adapter.count);
	ps->setEmissionTime(adapter.emission_time);
	ps->setLife(adapter.min_life, adapter.max_life);
	ps->setHori(adapter.min_hori, adapter.max_hori);
	ps->setVert(adapter.min_vert, adapter.max_vert);
	ps->setSpeed(adapter.min_spd, adapter.max_spd);
	ps->setGravity(adapter.gravity);
	ps->setFadeoutTime(adapter.fadeout_time);
	ps->setBounce(adapter.bounce);
	ps->setStartRadius(adapter.start_radius);
	for (size_t i = 0, n = adapter.children.size(); i < n; ++i)
	{
		const FileAdapter::Child& child = adapter.children[i];

		ParticleChild* pc = new ParticleChild;

		// todo Release symbol
		pc->symbol = d2d::SymbolMgr::Instance()->fetchSymbol(child.filepath);

		pc->start_scale = child.start_scale * 0.01f;
		pc->end_scale = child.end_scale * 0.01f;

		pc->min_rotate = child.min_rotate * d2d::TRANS_DEG_TO_RAD;
		pc->max_rotate = child.max_rotate * d2d::TRANS_DEG_TO_RAD;

		ps->addChild(pc);
	}

	return ps;
}

}