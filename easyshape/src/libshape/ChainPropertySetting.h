#ifndef _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_
#define _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape;

class ChainPropertySetting : public d2d::IPropertySetting
{
public:
	ChainPropertySetting(d2d::EditPanel* editPanel, ChainShape* chain);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	ChainShape* m_chain;

}; // ChainPropertySetting

}

#endif // _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_