#ifndef _DRAG2D_COLOR_SLIDER_H_
#define _DRAG2D_COLOR_SLIDER_H_

#include "common/Color.h"

#include <wx/wx.h>

namespace d2d
{

class RGBColorPanel;
class IColorMonitor;
class ITweenColorPanel;

class ColorSlider : public wxPanel
{
public:
	ColorSlider(wxWindow* parent, IColorMonitor* color_monitor, 
		const wxString& title, bool is_rgb);

	int GetColorValue() const;
	void SetColorValue(int col);

	void SetColorRegion(const Colorf& begin, const Colorf& end);

private:
	void InitLayout(const wxString& title, bool is_rgb);

	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxSpinEvent& event);

private:
	IColorMonitor* m_color_monitor;

	wxStaticText* m_title;

	ITweenColorPanel* m_color_bg;
	
	wxSlider* m_slider;
	wxSpinCtrl* m_spin;

}; // wxPanel

}

#endif // _DRAG2D_COLOR_SLIDER_H_