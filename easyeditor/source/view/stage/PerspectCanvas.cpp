#include "PerspectCanvas.h"

#include <gl/glu.h>

namespace ee
{

static const GLfloat CAM_POS[] = {0, -400.0f, 400.0f, 1.0f};

PerspectCanvas::PerspectCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							   wxGLContext* glctx)
	: OnePassCanvas(stage_wnd, stage, glctx)
{
}

void PerspectCanvas::OnSize(int w, int h)
{
//	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

 	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
 	gluPerspective(60.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(CAM_POS[0], CAM_POS[1], CAM_POS[2], 
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

}