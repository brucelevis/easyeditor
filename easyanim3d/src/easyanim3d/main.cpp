#include "main.h"
#include "Task.h"

#include <easyanim3d.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(eanim3d::FILE_TAG, &eanim3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim3d::FILE_TAG, &eanim3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new d2d::Frame("EasyAnim3D", eanim3d::FILE_TAG);
	eanim3d::Task* task = new eanim3d::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}