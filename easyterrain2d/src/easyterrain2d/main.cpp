#include "main.h"
#include "Task.h"

#include <easyterrain2d.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyTerrain2D", eterrain2d::FILE_TAG);
	eterrain2d::Task* task = new eterrain2d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}