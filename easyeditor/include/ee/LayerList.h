#ifndef _EASYEDITOR_LAYER_LIST_H_
#define _EASYEDITOR_LAYER_LIST_H_

#include "Observer.h"

#include <wx/listctrl.h>

namespace ee
{

class Layer;
class LayerMgr;

class LayerList : public wxListCtrl, public Observer
{
public:
	LayerList(wxWindow* parent);	

	void Insert();
	void Remove();

	void Insert(Layer* layer);
	void Remove(Layer* layer);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

	void LoadFromLayerMgr(LayerMgr* layer_mgr);

	void ClearLayer(Layer* layer);

private:
	void OnEndLabelEdit(wxListEvent& event);
	void OnSelected(wxListEvent& event);
	void OnColClick(wxListEvent& event);
	void OnListKeyDown(wxListEvent& event);
	void OnActivated(wxListEvent& event);

private:
	LayerMgr* m_layer_mgr;

	int m_selected;

	wxDECLARE_EVENT_TABLE();

}; // LayerList

}

#endif // _EASYEDITOR_LAYER_LIST_H_