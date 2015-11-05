#ifndef _DRAG2D_VIEWLIST_PANEL_H_
#define _DRAG2D_VIEWLIST_PANEL_H_

#include <wx/wx.h>

namespace d2d
{

class ISprite;
class ViewlistList;
class ViewlistListImpl;

class ViewlistPanel : public wxPanel
{
public:
	ViewlistPanel(wxWindow* parent);

	void Insert(ISprite* sprite, int idx = -1);

	void SetListImpl(ViewlistListImpl* impl);

private:
	void InitLayout();

private:
	ViewlistList* m_list;

}; // ViewlistPanel

}

#endif // _DRAG2D_VIEWLIST_PANEL_H_