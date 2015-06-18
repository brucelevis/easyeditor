#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "WorldPropertySetting.h"
#include "BodyPropertySetting.h"

namespace emodeling
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: d2d::PropertySettingPanel(parent)
{
}

void PropertySettingPanel::SelectSprite(d2d::ISprite* spr)
{
	assert(m_stage);
	if (spr) {
		SetPropertySetting(new BodyPropertySetting(m_stage, spr));
	} else {
		SetPropertySetting(new WorldPropertySetting(m_stage));
	}
}

}