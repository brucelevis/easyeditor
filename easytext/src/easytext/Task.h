#ifndef _EASYTEMPLATE_TASK_H_
#define _EASYTEMPLATE_TASK_H_


#include <wx/splitter.h>

namespace etemplate
{

class Task : public ee::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::ISprite*>& sprites) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
 	ee::EditPanel* m_stage;

}; // Task

}

#endif // _EASYTEMPLATE_TASK_H_