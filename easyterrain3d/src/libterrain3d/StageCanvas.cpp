#include "StageCanvas.h"

#include "DemoTriNetwork.h"
#include "DemoBruteForce.h"
#include "DemoBruteForceTex.h"
#include "DemoOcean.h"

namespace libterrain3d
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: e3d::StageCanvas(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);

	setBgColor(d2d::BLACK);

//	m_demo = new DemoTriNetwork(GetCamera3());
//	m_demo = new DemoBruteForce(GetCamera3());
//	m_demo = new DemoBruteForceTex(GetCamera3());
	m_demo = new DemoOcean(GetCamera3());
}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();

	m_demo->Load();

	// for Camera reset when init
	Refresh();
}

void StageCanvas::onDraw()
{
//	e3d::DrawCross(vec3(0, 0, 0), vec3(100, 100, 100), d2d::LIGHT_RED);

	m_demo->Draw();

//	e3d::LogViewer::Instance()->Draw();
}

void StageCanvas::OnTimer(wxTimerEvent& event)
{
	Refresh();
}

}