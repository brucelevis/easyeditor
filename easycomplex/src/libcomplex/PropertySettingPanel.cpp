#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "SymbolPropertySetting.h"

namespace ecomplex
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: d2d::PropertySettingPanel(parent)
{
}

d2d::IPropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	return new SymbolPropertySetting(m_stage, 
		static_cast<StagePanel*>(m_stage)->getSymbol());
}

}