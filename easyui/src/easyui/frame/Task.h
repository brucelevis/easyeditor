#ifndef _EASYUI_TASK_H_
#define _EASYUI_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eui
{

class AnchorMgr;
class StagePanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* getEditPanel() const;

	AnchorMgr* GetAnchorMgr();

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYUI_TASK_H_
