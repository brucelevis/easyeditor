#include "InvertRecord.h"
#include "ParticleSystem.h"

namespace eparticle3d
{

InvertRecord::InvertRecord()
	: m_gravity(0)
{
}

InvertRecord::~InvertRecord()
{
	Clear();
}

void InvertRecord::AddItem(Particle* p)
{
	m_items.push_back(new Item(p));
}

void InvertRecord::StoreToFile(const std::string& filepath) const
{
	Json::Value value;

	value["ps"]["gravity"] = m_gravity;

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i]->StoreToFile(value["sprite"][i]);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

void InvertRecord::Clear()
{
	for_each(m_items.begin(), m_items.end(), DeletePointerFunctor<Item>());
	m_items.clear();
}

//////////////////////////////////////////////////////////////////////////
//	class InvertRecord::Item
//////////////////////////////////////////////////////////////////////////

InvertRecord::Item::
Item(Particle* p)
{
	dead_time = clock();

	life = p->lifetime;
	for (int i = 0; i < 2; ++i) {
		direction[i] = p->direction[i];
	}
	for (int i = 0; i < 3; ++i) {
		position[i] = p->position[i];
	}
	for (int i = 0; i < 3; ++i) {
		speed[i] = p->speed[i];
	}	
	linear_acc = p->linear_acc;
	rotate = p->rotate;
	angle = p->angle;

	symbol = p->pc->symbol->GetFilepath();

	start_scale = p->pc->start_scale;
	end_scale = p->pc->end_scale;

	start_alpha = p->pc->start_alpha;
	end_alpha = p->pc->end_alpha;
}

void InvertRecord::Item::
StoreToFile(Json::Value& val) const
{
	val["dead time"] = dead_time;

	val["life"] = life;
	for (int i = 0; i < 2; ++i) {
		val["direction"][i] = direction[i];
	}
	for (int i = 0; i < 3; ++i) {
		val["position"][i] = position[i];
	}
	for (int i = 0; i < 3; ++i) {
		val["speed"][i] = speed[i];
	}
	val["linear_acc"] = linear_acc;
	val["rotate"] = rotate;
	val["angle"] = angle;

	val["symbol"] = symbol;
	val["start_scale"] = start_scale;
	val["end_scale"] = end_scale;
	val["start_alpha"] = start_alpha;
	val["end_alpha"] = end_alpha;
}

}