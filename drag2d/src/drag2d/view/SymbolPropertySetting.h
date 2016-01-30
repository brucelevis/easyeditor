#ifndef _DRAG2D_SYMBOL_PROPERTY_SETTING_H_
#define _DRAG2D_SYMBOL_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

namespace d2d
{

class Symbol;

class SymbolPropertySetting : public IPropertySetting
{
public:
	SymbolPropertySetting(Symbol* symbol);
	SymbolPropertySetting(std::string* name, std::string* tag);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

protected:
	Symbol* m_symbol;

	std::string* m_name;
	std::string* m_tag;

}; // SymbolPropertySetting

}

#endif // _DRAG2D_SYMBOL_PROPERTY_SETTING_H_