#ifndef _EASYPARTICLE3D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE3D_TOOLBAR_PANEL_H_

#include "LoadAdapter.h"

#include <ee/ToolbarPanel.h>
#include <ee/UICallback.h>
#include <ee/Observer.h>

#include <wx/dnd.h>

namespace ee { class LibraryPanel; class Symbol; class Image; class SliderCtrl; }

namespace eparticle3d
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::UICallback, public ee::Observer
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Load(const Json::Value& val, int version);
	void Store(Json::Value& val) const;

	void Add(const LoadAdapter::Component& comp, ee::LibraryPanel* library);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* InitLayout();

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateComponentLayout();

	void Clear();

	void OnAddChild(wxCommandEvent& event, ee::Symbol* symbol);
	void OnDelAllChild(wxCommandEvent& event);

	void OnSetLoop(wxCommandEvent& event);
	void OnSetLocalModeDraw(wxCommandEvent& event);
	void OnSet(wxCommandEvent& event);
	void OnSetHori(wxSpinEvent& event);
	void OnSetVert(wxSpinEvent& event);
	void OnSetGround(wxCommandEvent& event);
	void OnSetOrientToMovement(wxCommandEvent& event);

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(ee::LibraryPanel* library, StagePanel* stage,
			ToolbarPanel* toolbar);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		ee::LibraryPanel* m_library;
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // DropTarget

private:
	StagePanel* m_stage;

	ee::Image* m_image;

	std::vector<ee::SliderCtrl*> m_sliders;

	// move to property
//	wxTextCtrl* m_name;
// 	wxCheckBox* m_loop;
// 	wxCheckBox* m_local_mode_draw;

	wxSpinCtrl *m_min_hori, *m_max_hori;
	wxSpinCtrl *m_min_vert, *m_max_vert;
	wxChoice* m_ground;
	wxCheckBox* m_orient_to_movement;
//	wxCheckBox* m_orient_to_parent;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE3D_TOOLBAR_PANEL_H_