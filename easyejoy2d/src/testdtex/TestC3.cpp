#include "TestC3.h"
#include "Utility.h"

#include <easyejoy2d.h>

#include <dtex.h>

namespace tdtex
{

TestC3::TestC3()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestC3::~TestC3()
{
	dtexf_release();
}

void TestC3::Load()
{
	{
		dtex_package* pkg = Utility::LoadPackage("c00001");
		dtexf_c3_load(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00001_attack_1"));
	}
	{
		dtex_package* pkg = Utility::LoadPackage("c00002");
		dtexf_c3_load(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00002_attack01_1"));
	}
	{
		dtex_package* pkg = Utility::LoadPackage("c00003");
		dtexf_c3_load(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00003_attack01_1"));
	}

	dtexf_c3_load_end();
}

}