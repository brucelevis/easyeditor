#ifndef _EASYUI_WINDOW_PREVIEW_CANVAS_H_
#define _EASYUI_WINDOW_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <SM_Matrix.h>

namespace ee { class PlayControl; class Sprite; }

namespace eui
{
namespace window
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control, 
		const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	sm::mat4 m_scale_mt;

	ee::PlayControl& m_control;

	std::vector<const ee::Sprite*> m_sprites;

}; // PreviewCanvas

}
}

#endif // _EASYUI_WINDOW_PREVIEW_CANVAS_H_