#ifndef _DRAG2D_GROUP_TREE_CTRL_H_
#define _DRAG2D_GROUP_TREE_CTRL_H_

#include "GroupTreeVisitor.h"
#include "GroupTreeItem.h"

#include <wx/treectrl.h>
#include <json/json.h>

namespace d2d
{

class GroupTreePanel;
class ISprite;
class Group;
class MultiSpritesImpl;
class ViewPanelMgr;

class GroupTreeCtrl : public wxTreeCtrl
{
public:
	GroupTreeCtrl(GroupTreePanel* parent, MultiSpritesImpl* sprite_impl,
		ViewPanelMgr* view_panel_mgr);

	void Traverse(IGroupTreeVisitor& visitor) const;

	wxTreeItemId AddNode();
	wxTreeItemId AddNode(const std::string& name);
	wxTreeItemId AddNode(const std::string& name, wxTreeItemId parent);
	void DelNode();

	wxTreeItemId AddSprite(wxTreeItemId parent, d2d::ISprite* spr);

	void Clear();

	void Remove(ISprite* sprite);

	wxTreeItemId GetRootID() const { return m_root; }

	void Visible(wxTreeItemId id, bool visible);
	void Editable(wxTreeItemId id, bool editable);

private:
	wxTreeItemId AddNode(wxTreeItemId parent, const std::string& name, GroupTreeItem* data);

private:
	void OnItemRClick(wxTreeEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnSelChanged(wxTreeEvent& event);

	// menu
	void OnMenuAddSprites(wxCommandEvent& event);
	void OnMenuClear(wxCommandEvent& event);
	void OnMenuVisible(wxCommandEvent& event);
	void OnMenuEditable(wxCommandEvent& event);

	// select
	void SelectUp();
	void SelectDown();
	void SelectLeft();
	void SelectRight();

	void ReorderSelected(bool up);

	void ReorderSprites();

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	void Traverse(wxTreeItemId id, IGroupTreeVisitor& visitor) const;

private:
	enum
	{
		ID_MENU_ADD = 100,
		ID_MENU_CLEAR,
		ID_MENU_VISIBLE,
		ID_MENU_EDITABLE,

		ID_GROUP_TREE_CTRL = 1000
	};

private:
	GroupTreePanel* m_parent_panel;

	MultiSpritesImpl* m_sprite_impl;

	ViewPanelMgr* m_view_panel_mgr;

	wxTreeItemId m_root;
	wxTreeItemId m_on_menu_id;
	wxTreeItemId m_dragged_item;
	wxTreeItemId m_selected_item;

	bool m_remove_open;

	DECLARE_EVENT_TABLE()

}; // GroupTreeCtrl

}

#endif // _DRAG2D_GROUP_TREE_CTRL_H_