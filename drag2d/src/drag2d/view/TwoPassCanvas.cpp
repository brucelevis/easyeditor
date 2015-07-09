#include "TwoPassCanvas.h"
#include "EditPanel.h"

#include "render/ShaderMgr.h"
#include "render/ScreenFBO.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

TwoPassCanvas::TwoPassCanvas(EditPanel* stage)
	: OnePassCanvas(stage)
{
}

void TwoPassCanvas::OnSize(int w, int h)
{
	ScreenFBO::Instance()->GetFBO().ChangeSize(w, h);
}

void TwoPassCanvas::OnDrawWhole() const
{
	const FBO& fbo = ScreenFBO::Instance()->GetFBO();
	ShaderMgr* mgr = ShaderMgr::Instance();

	SpriteRenderer::Instance()->SetCamera(GetCamera());

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
	if (IsDirty()) {
 		mgr->SetFBO(fbo.GetFboID());
		OnePassCanvas::OnDrawWhole();
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	mgr->SetFBO(0);
	mgr->SetTexture(0);

	mgr->Screen();
	mgr->DrawScreen(fbo.GetTexID());
}

}