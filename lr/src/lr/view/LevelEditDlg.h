#ifndef _LR_LEVEL_EDIT_DIALOG_H_
#define _LR_LEVEL_EDIT_DIALOG_H_

#include "CfgEditDlg.h"

namespace lr
{

class LevelEditDlg : public CfgEditDlg
{
public:
	LevelEditDlg(wxWindow* parent, ee::Sprite* spr);

	std::string ToString();

private:
	void InitData(ee::Sprite* spr);
	void InitLayout();

private:
	std::string m_type;

	std::vector<DynamicWidget*> m_widgets;

}; // LevelEditDlg

}

#endif // _LR_LEVEL_EDIT_DIALOG_H_