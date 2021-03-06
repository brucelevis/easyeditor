#include "StagePanel.h"
#include "StageCanvas.h"

#include "TestLoad.h"
#include "TestC3.h"
#include "TestC2.h"
// #include "TestC1.h"
#include "TestC1New.h"

// #include "TestAsync.h"
#include "TestAsyncLoad.h"
#include "TestAsyncC3.h"
#include "TestAsyncC2.h"
#include "TestAsyncC1.h"
#include "TestAsyncC3Load.h"
#include "TestAsyncC3C2.h"

// #include "TestSprite.h"
// #include "TestPVR.h"
// #include "TestRRP_C2C3.h"
// #include "TestRRP_C2C3_PVR.h"
// #include "TestRRR_C3.h"
// #include "TestB4R_C3.h"
// #include "TestETC1.h"
// #include "TestEPP.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, m_test(NULL)
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{
	delete m_test;
}

void StagePanel::Load()
{
	eejoy2d::EJScreen::Create();

	// normal
//	m_test = new TestLoad();
//	m_test = new TestC3(true);
//	m_test = new TestC2(false);
//	m_test = new TestC1();

	// async
//	m_test = new TestAsyncLoad();
//	m_test = new TestAsyncC3();
//	m_test = new TestAsyncC2();
//	m_test = new TestAsyncC1();
//	m_test = new TestAsyncC3Load();
	m_test = new TestAsyncC3C2();

//	m_test = new TestSprite();

//	m_test = new TestPVR();

//	m_test = new TestRRP_C2C3();
//	m_test = new TestRRP_C2C3_PVR();

//	m_test = new TestRRR_C3();
//	m_test = new TestB4R_C3();

//	m_test = new TestETC1();

//	m_test = new TestEPP();

	m_test->Load();
}

void StagePanel::UnLoad()
{
	assert(eejoy2d::EJScreen::Instance());
	eejoy2d::EJScreen::Instance()->Unload();
}

void StagePanel::Update()
{
	if (m_test) {
		m_test->Update();
	}

	PrintLog();
}

void StagePanel::Draw() const
{
	if (m_test) {
		m_test->Draw();
	}
}

void StagePanel::PrintLog() const
{
	static int COUNT = 0;

	if (COUNT < 60 * 1) {
		++COUNT;
		return;
	}

	COUNT = 0;

	wxLogDebug("drawcall: %d", dtex_stat_get_drawcall());

	int tex_count;
	struct stat_texture* tex_list;
	dtex_stat_get_texture(&tex_count, &tex_list);
	for (int i = 0; i < tex_count; ++i) {
		struct stat_texture* tex = &tex_list[i];
		wxLogDebug("tex %d: id[%d], width[%d], height[%d]", i, tex->id, tex->w, tex->h);
	}
}

}