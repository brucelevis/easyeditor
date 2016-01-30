#ifndef _LIBSKETCH_STAGE_PANEL_H_
#define _LIBSKETCH_STAGE_PANEL_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::LibraryPanel* library);

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(d2d::Visitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;
	vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Insert(d2d::Sprite* spr);
	void Remove(d2d::Sprite* spr);
	void Clear();

private:
	std::vector<d2d::Sprite*> m_sprites;

}; // StagePanel

}

#endif // _LIBSKETCH_STAGE_PANEL_H_
