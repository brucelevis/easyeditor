#ifndef _EASYICON_SPRITE_PROPERTY_SETTING_H_
#define _EASYICON_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace eicon
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* sprite);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYICON_SPRITE_PROPERTY_SETTING_H_