#pragma once

#include <wx/wx.h>
#include <vector>

namespace d2d
{
	class EditPanel;
	class MultiSpritesImpl;
	class PropertySettingPanel;
	class VerticalImageList;
	class ISprite;
	class ViewlistList;

	class ViewlistPanel : public wxPanel
	{
	public:
		ViewlistPanel(wxWindow* parent, EditPanel* editPanel,
			MultiSpritesImpl* stage = NULL, 
			PropertySettingPanel* property = NULL);

		void remove(ISprite* sprite);
		void insert(ISprite* sprite);
		void reorder(const ISprite* sprite, bool up);

		void onSelected(int index);

		void clear();

	private:
		void initLayout();

		void swap(int i0, int i1);

	private:
		EditPanel* m_editPanel;
		MultiSpritesImpl* m_stage;
		PropertySettingPanel* m_property;

		ViewlistList* m_list;

		std::vector<ISprite*> m_sprites;

	}; // ViewlistPanel
}

