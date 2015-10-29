#ifndef _EASYUI_LIST_UI_LIST_H_
#define _EASYUI_LIST_UI_LIST_H_

#include <drag2d.h>

#include <json/json.h>

namespace eui
{
namespace list
{

class UIList
{
public:
	UIList();

	bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	bool ClearAllSprite();
	void TraverseSprites(d2d::IVisitor& visitor) const;

	void StoreToFile(const char* filename) const;
	void LoadFromFile(const char* filename);

	bool ReFilling();
	bool Arrange(const d2d::ISprite* spr);

	const d2d::Rect& GetClipbox() const { return m_clipbox; }
	void SetClipbox(const d2d::Rect& cb) { m_clipbox = cb; }

	bool IsHoriEnable() const { return m_horizontal; }
	bool IsVertEnable() const { return m_vertical; }
	void EnableHori(bool enable);
	void EnableVert(bool enable);

private:
	bool Filling();
	bool Arrange(float hori_space, float vert_space);

private:
	d2d::Rect m_clipbox;

	std::vector<d2d::ISprite*> m_items;
//	d2d::ISprite *m_base_spr, *m_hori_spr, *m_vert_spr;
	d2d::ISprite* m_item_spr;

	bool m_horizontal;
	bool m_vertical;

	int m_hori_count;
	int m_vert_count;

	float m_hori_space;
	float m_vert_space;	

}; // UIList

}
}

#endif // _EASYUI_LIST_UI_LIST_H_