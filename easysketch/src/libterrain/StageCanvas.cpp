#include "StageCanvas.h"

#include "SimpleTriNetwork.h"

namespace libterrain
{

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: e3d::StageCanvas(stage)
{
	m_demo = new SimpleTriNetwork(GetCamera3());
}

void StageCanvas::onDraw()
{
//	e3d::DrawCross(vec3(0, 0, 0), vec3(100, 100, 100), d2d::LIGHT_RED);

	m_demo->Draw();

//	e3d::LogViewer::Instance()->Draw();
}

}