#ifndef _EASYEDITOR_MULTI_SPRITES_PROPERTY_IMPL_H_
#define _EASYEDITOR_MULTI_SPRITES_PROPERTY_IMPL_H_

#include "Vector.h"
#include "Color.h"

#include <wx/chartype.h>

#include <vector>

namespace ee
{

class Sprite;

class MultiSpritesPropertyImpl
{
public:
	MultiSpritesPropertyImpl(const std::vector<Sprite*>& sprites);
	~MultiSpritesPropertyImpl();

public:
	static const wxChar* BOOL_3TYPE_LABELS[];
	static const wxChar* ALIGN_LABELS[];
	static const wxChar* CENTER_LABELS[];

public:
	std::string GetTag() const;
	const wxChar* GetClip() const;
	Vector GetPosition() const;
	Colorf GetMultiColor() const;
	Colorf GetAddColor() const;
	Colorf GetTransColorR() const;
	Colorf GetTransColorG() const;
	Colorf GetTransColorB() const;
	float GetAngle() const;
	Vector GetScale() const;
	bool GetMirrorX() const;
	bool GetMirrorY() const;
	bool GetVisible() const;
	bool GetEditable() const;

	void SetTag(const std::string& tag);
	void SetClip(int clip);
	void SetPos(float x, float y);
	void SetColorMul(const Colorf& col);
	void SetColorAdd(const Colorf& col);
	void SetColorAlpha(float alpha);
	void SetAngle(bool overall, float angle);
	void SetScale(bool overall, float sx, float sy);
	void SetMirrorX(bool overall, bool mirror);
	void SetMirrorY(bool overall, bool mirror);
	void SetVisible(bool visible);
	void SetEditable(bool editable);

	void OnAlign(int align);
	void OnCenter(int center);

	void OnPosChange(float dx, float dy);
	void OnAngleChange(float angle);
	void OnScaleChange(float dx, float dy);

	Vector GetOverallCenter() const;

private:
	enum Bool3Type
	{
		e_bool_different = 0,
		e_bool_true,
		e_bool_false
	};

	enum AlignType
	{
		e_align_none = 0,
		e_align_left,
		e_align_right,
		e_align_up,
		e_align_down,
		e_align_center_left,
		e_align_center_right,
		e_align_center_up,
		e_align_center_down
	};

	enum CenterType
	{
		e_center_none = 0,
		e_center_horizontal,
		e_center_vertical,
		e_center_relative
	};

private:
	std::vector<Sprite*> m_sprites;

}; // MultiSpritesPropertyImpl

}

#endif // _EASYEDITOR_MULTI_SPRITES_PROPERTY_IMPL_H_