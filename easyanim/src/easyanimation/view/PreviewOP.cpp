#include "PreviewOP.h"
#include "PreviewSettings.h"

#include <easyanim.h>

namespace eanim
{

PreviewOP::PreviewOP(d2d::EditPanel* editPanel, PlaySettings& settings,
					 d2d::PlayControl& control)
	: d2d::ZoomViewOP(editPanel, true, true, false)
	, m_settings(settings)
	, m_control(control)
{
}

bool PreviewOP::OnKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::OnKeyDown(keyCode))
		return true;

	if (keyCode == WXK_SPACE)
	{
		m_settings.isStop = !m_settings.isStop;
		m_control.reset();
	}

	return false;
}

}